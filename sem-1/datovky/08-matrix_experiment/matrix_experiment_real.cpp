#include <functional>
#include <string>
#include <vector>
#include <cstdio>
#include <cmath>
#include <iostream>

#include <time.h>   // FIXME

using namespace std;

// If the condition is not true, report an error and halt.
#define EXPECT(condition, message) do { if (!(condition)) expect_failed(message); } while (0)

void expect_failed(const string& message) {
    cerr << "Test error: " << message << endl;
    exit(1);
}

class Matrix {
    vector<unsigned> items;
    unsigned &item(unsigned i, unsigned j) { return items[i*N + j]; }
  public:
    unsigned N;
    Matrix(unsigned N) { this->N = N; items.resize(N*N, 0); }

    void swap(unsigned i1, unsigned j1, unsigned i2, unsigned j2)
    {
        // EXPECT(i1 < N && j1 < N && i2 < N && j2 < N, "Swap out of range: " + coord_string(i1, j1) + " with " + coord_string(i2, j2) + ".");
        std::swap(item(i1, j1), item(i2, j2));
    }

    void naive_transpose()
    {
        for (unsigned i=0; i<N; i++)
            for (unsigned j=0; j<i; j++)
                swap(i, j, j, i);
    }

// #include "matrix_transpose.h" todo

// split A and B to A1 A2 A3 A3 A4 and B1 B2 B3 B4
// transpose and swap A1/B1   A4/B4   A2/B3   A3/B2
    void transposeAndSwap(unsigned Ai, unsigned Aj, unsigned Bi, unsigned Bj, unsigned size, bool skip_right_bottom){
        // new dimensions
        unsigned size_1_4 = (size + 1) / 2; // a1 and a3 can be overlapping ...
        unsigned size_2_3 = size - size_1_4;

        // stopping criterion
        if(size == 1 && ! skip_right_bottom ) swap(Ai, Aj, Bi, Bj);  // prevent duplicate swapping
        if(size == 1) return;

        // for the overlapping case ... matrix A4 have +1 in both dimensions
        unsigned shift = size%2 == 0 ? 0 : 1;
        // for the overlapping case ...  we want to swap middle value only once !
        bool skip_corner = size % 2 != 0;


        // A1 B1 -- right bottom corner of A1 is not swapped (when matrices A1(B1) and A4(B4) are overlapping)
        transposeAndSwap(Ai, Aj, Bi, Bj, size_1_4, skip_corner);
        // A4 B4 -- right corner of A4(B4) skipped (in case when this matrix A(B) was at some point right bottom sub-matrix of A*1(B*1) matrix which was overlapped with A*4(B*4) in upper level of recursion
        transposeAndSwap(Ai + size_1_4 - shift, Aj + size_1_4 - shift, Bi + size_1_4 - shift, Bj + size_1_4 - shift, size_1_4, skip_right_bottom);
        // A2 B3
        transposeAndSwap(Ai, Aj + size_1_4, Bi + size_1_4, Bj, size_2_3, false);
        // A3 B2
        transposeAndSwap(Ai + size_1_4, Aj, Bi, Bj + size_1_4, size_2_3, false);
    }

// split A to A1 A2 A3 A3 A4
// transpose A1, A4
// transpose and swap A2 with A3
    void transposeRec(unsigned Ai, unsigned Aj, unsigned size){
        // new dimensions
        unsigned size_1_4 = (size + 1) / 2; // A1 and A3 can be overlapping ...
        unsigned size_2_3 = size - size_1_4;

        // stopping criterion
        if(size == 1) return;

        // for the overlapping case ... matrix A4 have +1 in both dimensions
        unsigned shift = size%2 == 0 ? 0 : 1;

        // A1
        transposeRec(Ai, Aj, size_1_4);
        // A4
        transposeRec(Ai + size_1_4 - shift, Aj + size_1_4 - shift, size_1_4);
        // A2, A3
        transposeAndSwap(Ai, Aj + size_1_4, Ai + size_1_4, Aj, size_2_3, false);
    }

// initialize recursive transposition
    void transpose(){
        transposeRec(0,0,N);
    }
};

void real_test(bool naive)
{
    printf("matrix_size avg_time");

    for (int e=40; e<=112; e++) {
        unsigned N = (unsigned) pow(2, e/8.);
        Matrix m(N);

        clock_t start_time, stop_time;
        unsigned tries = 1;
        do {
            start_time = clock();
            for (unsigned t=0; t < tries; t++) {
                if (naive)
                    m.naive_transpose();
                else
                    m.transpose();
            }
            stop_time = clock();
            tries *= 2;
        } while (stop_time - start_time < CLOCKS_PER_SEC/10);
        // It is guaranteed that the total number of tries is odd :)

        double ns_per_item = (double)(stop_time - start_time) / CLOCKS_PER_SEC / (N*(N-1)) / tries * 1e9;
        printf("%d %.6f\n", N, ns_per_item);
    }
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s (smart|naive)\n", argv[0]);
        return 1;
    }

    std::string mode = argv[1];

    bool naive;
    if (mode == "smart")
      naive = false;
    else if (mode == "naive")
      naive = true;
    else {
        fprintf(stderr, "The argument must be either 'smart' or 'naive'\n");
        return 1;
    }

    real_test(naive);
    return 0;
}
