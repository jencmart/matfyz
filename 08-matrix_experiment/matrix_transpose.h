
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