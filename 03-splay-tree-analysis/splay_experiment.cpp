#include <algorithm>
#include <functional>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <cmath>

#include "splay_operation.h"
#include "random.h"

using namespace std;

/*
 *  A modified Splay tree for benchmarking.
 *
 *  We inherit the implementation of operations from the Tree class
 *  and extend it by keeping statistics on the number of splay operations
 *  and the total number of rotations. Also, if naive is turned on,
 *  splay uses only single rotations.
 *
 *  Please make sure that your Tree class defines the rotate() and splay()
 *  methods as virtual.
 */

class BenchmarkingTree : public Tree {
public:
    int num_operations;
    int num_rotations;
    bool do_naive;
    std::vector<int> potential;
    std::vector<int> elements;
    float current_potential;
    int current_elements;


    BenchmarkingTree(bool naive=false)
    {
        do_naive = naive;
        reset();
        elements.emplace_back(0);
        potential.emplace_back(0);
        current_elements = 0;
        current_potential = 0.0f;
    }

    void insert(int key) override {
        elements.emplace_back(++current_elements);
        Tree::insert(key);
        current_potential = 0;
        current_potential = Tree::calcPotential();
        potential.emplace_back(current_potential);
    }


        void reset()
    {
        num_operations = 0;
        num_rotations = 0;
    }


    void doubleRotate(Node * node) override {
        num_rotations++;
        num_rotations++;
        Tree::doubleRotate(node);
    }

    Node* lookup(int key) override {
        Node * node =  Tree::lookup(key);
        elements.emplace_back(current_elements); // same cnt
        current_potential = Tree::calcPotential();
        potential.emplace_back(current_potential);
        return node;
    }

        void rotate(Node *node) override
    {
        num_rotations++;
        Tree::rotate(node);
    }

    void splay(Node *node) override
    {
        num_operations++;
        if (do_naive) {
            while (node->parent)
                rotate(node);
        } else {
            Tree::splay(node);
        }
    }

    // Return the average number of rotations per operation.
    double rot_per_op()
    {
        if (num_operations > 0)
            return (double) num_rotations / num_operations;
        else
            return 0;
    }

    void printPotential(int n) {
        for(int i = 0 ; i < (int) elements.size() ; i++){
            cout << n << "," << elements[i] << ','  << potential[i] << endl;
        }
//        cout << endl;
//        for(int i = 0 ; i < (int) potential.size() ; i++){
//            cout << potential[i] << ',';
//        }
//        cout << endl;

    }
};

bool naive;             // Use of naive rotations requested
RandomGen *rng;         // Random generator object

void test_sequential()
{
    cout << "set_size,average_num_rot" << endl;
//    for (int n=100; n<=3000; n+=100) {
    for (int n=100; n<=100; n+=100) {
        BenchmarkingTree tree = BenchmarkingTree(naive);

        for (int x=0; x<n; x++)
            tree.insert(x);

        for (int i=0; i<5; i++) // five times
            for (int x=0; x<n; x++) // look up all values in sequence
                tree.lookup(x);

//        cout << n << "," << tree.rot_per_op() << endl;
        tree.printPotential(n);
        break;
    }
}

// An auxiliary function for generating a random permutation.
vector<int> random_permutation(int n, bool mix = true)
{
    vector<int> perm;
    for (int i=0; i<n; i++)
        perm.push_back(i);

    if(!mix)
        return perm;

    for (int i=0; i<n-1; i++)
        swap(perm[i], perm[i + rng->next_range(n-i)]);  /// prohod nahdone 2 prvky
    return perm;
}

void test_random()
{
    exit(1);
    cout << "set_size,average_num_rot" << endl;
    for (int e=32; e<=64; e++) {
        int n = (int) pow(2, e/4.);
        BenchmarkingTree tree = BenchmarkingTree(naive);

        vector<int> perm = random_permutation(n);
        for (int x : perm)
            tree.insert(x);

        for (int i=0; i<5*n; i++)
            tree.lookup(rng->next_range(n));

        cout << n << "," << tree.rot_per_op() << endl;
        tree.printPotential(n);
        break;
    }
}

/*
 *  An auxiliary function for constructing arithmetic progressions.
 *  The vector
 *  seq will be modified to contain an arithmetic progression
 *  of elements in
 *  interval [A,B] starting
 *  from position s with
 *  step inc.
 */
void make_progression(vector<int> &seq, int A, int B, int s, int inc)
{
    for (int i=0; i<seq.size(); i++){
        while (seq[i] >= A && seq[i] <= B && s + inc*(seq[i]-A) != i) // mame hodnotu z intervalu
        {
            swap(seq[i], seq[s + inc*(seq[i] - A)]); // prvni  A ( index prvni. )   // seq[i] = seq[s+inc(seq[i] - A]
        }
    }

//    for(int i = 0 ; i < seq.size() ; i++)
//        std::cout << seq[i] << "," ;
//
//    std::cout << std::endl;
}

void test_subset_s(int sub) /// subset size sub -- each subset is basically different test
{
    for (int e=48; e<=64; e++) {   /// for different N ( 64 different n )  preskoc pokud spatne
        int n = (int) pow(2, e/4.);  // n = 2^8 (256)  ... 512  ... 1024 ...  2^8.25, ... , 2^16 (65536)
        if (n < sub) // pro sub == 1000 preskoc prvnch 8 clenu
          continue;


//      n = 100;

        // We will insert elements in order, which contain several
        // arithmetic progressions interspersed with random elements.
        vector<int> seq = random_permutation(n);  /// 0, 1, 2 ... n randomly permutated

//        for (int i=0; i<seq.size(); i++) {
//            std::cout << seq[i] << " ";
//        }
     //   std::cout<< std::endl;
        /// values from index 25 - 30% map to 10-15%
        make_progression(seq, n/4, n/4 + n/20, n/10, 1); // [A,B] from_pos, increment   A = 5 , 5+1 , 2  ; 6 jina ; 2 jina

        /// values from index 50 - 55% map to 10-5%
        make_progression(seq, n/2, n/2 + n/20, n/10, -1);

        /// values from index 75 - 80% map 50% - 30%
        make_progression(seq, 3*n/4, 3*n/4 + n/20, n/2, -4);

        /// values from index 85 - 90% map 40% - 65%
        make_progression(seq, 17*n/20, 17*n/20 + n/20, 2*n/5, 5);
      //  exit(1);

        BenchmarkingTree tree = BenchmarkingTree(naive);
        for (int x : seq)
            tree.insert(x);

        /// reset operation count
        tree.reset();

        /// >>>> ONLY THIS SPLAYS ARE COUNTED  <<<
        for (int i=0; i<10000; i++)
            tree.lookup(seq[rng->next_range(sub)]); // na indexu 0 ... sub-1 // 0..9  , 0 ..99 , 0 .. 999

//        cout << sub << "," << n << "," << tree.rot_per_op() << endl;
        tree.printPotential(n);
        break;
    }
}

void test_subset()
{
    cout << "subset_size,set_size,potential" << endl;
//    test_subset_s(10);
//    test_subset_s(100);
    test_subset_s(1000);
}

vector<pair<string, function<void()>>> tests = {
    { "sequential", test_sequential },
    { "random",     test_random },
    { "subset",     test_subset },
};

int main(int argc, char **argv)
{
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <test> <student-id> (std|naive)" << endl;
        return 1;
    }

    string which_test = argv[1];
    string id_str = argv[2];
    string mode = argv[3];

    try {
        rng = new RandomGen(stoi(id_str));
    } catch (...) {
        cerr << "Invalid student ID" << endl;
        return 1;
    }

    if (mode == "std")
      naive = false;
    else if (mode == "naive")
      naive = true;
    else
      {
        cerr << "Last argument must be either 'std' or 'naive'" << endl;
        return 1;
      }

    for (const auto& test : tests) {
        if (test.first == which_test)
          {
            cout.precision(12);
            test.second();
            return 0;
          }
    }
    cerr << "Unknown test " << which_test << endl;
    return 1;
}
