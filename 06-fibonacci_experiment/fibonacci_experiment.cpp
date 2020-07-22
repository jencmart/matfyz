#include <algorithm>
#include <functional>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <cmath>
#include <tuple>
#include "fibonacci.h"
#include "random.h"
//#include "fibonacci_test.cpp"
std::string text = "n avg_changes\n";
// std::string text = "n op_num op_name tree_cnt colored_cnt elem_cnt step_per_this_op\n";

void expect_failed(const std::string& message) {
    fprintf(stderr, "Test error: %s\n", message.c_str());
    exit(1);
}


/*
 *  A modified Fibonacci heap for benchmarking.
 *
 *  We inherit the implementation of operations from the FibonacciHeap class
 *  and extend it by keeping statistics on the number of operations
 *  and the total number of structural changes. Also, if naive is turned on,
 *  the decrease does not mark parent which just lost a child.
 */

class HeapTests : public FibonacciHeap {


    class NodeIterator {
        Node *root;
        Node *node;
        bool recursive;

    public:
        NodeIterator begin() { return *this; }
        NodeIterator end() { return NodeIterator(); }
        bool operator !=(const NodeIterator& b) { return node != b.node; }

        Node *operator*() { return node; }
        NodeIterator &operator++() {
            if (recursive && node->rank > 0) {
                node = node->first_child;
            } else {
                while (node->next_sibling == node->parent->first_child) {
                    node = node->parent;
                    if (node == root) {
                        node = nullptr;
                        return *this;
                    }
                }
                node = node->next_sibling;
            }
            return *this;
        }

        NodeIterator(Node *root = nullptr, bool recursive = true) : root(root),
                                                                    node(root ? root->first_child : nullptr), recursive(recursive) {}
    };

    // Helper of show_heap.
    static void show_node(Node *node, int indent) {
        for (int i = 0; i < indent; i++) printf("  ");
        {
            std::string s = "";
            if (node->marked)
                s = "M";
            printf("- %i  %s\n", node->priority(),  s.c_str());
        }
        for (Node* child : NodeIterator(node, false))
            show_node(child, indent + 1);
    }


    int ops = 0; // N
    int steps = 0;
    bool naive;
    size_t max_size = 0;

  public:
    int num_del_ops = 0;
    int num_consolidations = 0;
    std::vector<std::tuple<int, int, int, int, int, int>> heapPotential;
    int num_of_trees;
    int num_of_colored;
    int num_of_elements;
    int num_current_cnt_steps;

    HeapTests(bool naive = false) : naive(naive) {
        num_of_elements = 0;
        num_of_trees = 0;
        num_of_colored = 0;
    }

    void stats(int N) {

         // was before
         printf("%i", N); // toto je actually ok
         printf(" %.3lf\n", (double)(steps * 1.0 / std::max(1, ops)));

//        for( auto & x : heapPotential){
//            int op_num =  std::get<0>(x);
//            int op_name =  std::get<1>(x);
//            int tree_cnt =  std::get<2>(x);
//            int colored_cnt =  std::get<3>(x);
//            int elem_cnt =  std::get<4>(x);
//            int step_per_this_op =  std::get<5>(x);
//
//            printf("%i %i %i %i %i %i %i\n", N, op_num, op_name, tree_cnt, colored_cnt, elem_cnt, step_per_this_op);
//        }
    }

    Node *insert(priority_t prio, payload_t payload = payload_t()) {
        ops++;

        // 1/4
        num_current_cnt_steps = 0;
        // 2/4
        num_of_elements += 1;

        Node * x = FibonacciHeap::insert(prio, payload);

        // 3/4
      //  num_of_trees += 1;
   //     EXPECT(num_of_trees == FibonacciHeap::calculate_trees(), "TREE COUNT IF FUCKED UP!.");
        // 4/4 - same
        // num_of_colored = same

//        auto c = std::tuple<int, int, int, int, int, int>{ops, 1, num_of_trees, num_of_colored, num_of_elements, num_current_cnt_steps};
      //  heapPotential.emplace_back(std::make_tuple(ops, 1, num_of_trees, num_of_colored, num_of_elements, num_current_cnt_steps));
        return x;
    }

    std::pair<payload_t, priority_t> extract_min() {
       // num_del_ops +=1;
        ops++;

        // 1/4
        num_current_cnt_steps = 0;
        // 2/4
        num_of_elements -= 1;

        std::pair<payload_t, priority_t> x =  FibonacciHeap::extract_min();


        // 3/4
    //    num_of_trees = FibonacciHeap::calculate_trees();
        // 4/4
  //      num_of_colored = FibonacciHeap::get_colored_counter(); // for naive it stays 0


   //     if (naive)
  //          EXPECT(num_of_colored == 0, "COLORED COUNT IF FUCKED UP!.");

     //   heapPotential.emplace_back(std::make_tuple(ops, 2, num_of_trees, num_of_colored, num_of_elements, num_current_cnt_steps));
        return x;
    }

    void decrease(Node *node, priority_t new_prio) {
        ops++;

        // 1/4
        num_current_cnt_steps = 0;
        // 2/4
        // num_of_elements

        if (naive) {
            EXPECT(node, "Cannot decrease null pointer.");
            EXPECT(node->prio >= new_prio, "Decrase: new priority larger than old one.");

            node->prio = new_prio;
            if (is_root(node) || new_prio >= node->parent->prio)
                ; // byl tu return
            else
                add_child(&meta_root, remove(node));
        }

        else{
            FibonacciHeap::decrease(node, new_prio);
        }


        // 3/4
  //      num_of_trees = FibonacciHeap::calculate_trees();
        // 4/4
   //     num_of_colored = FibonacciHeap::get_colored_counter(); // for naive it stays 0
    //    if (naive)
    //        EXPECT(num_of_colored == 0, "COLORED COUNT IF FUCKED UP!.");

    //    heapPotential.emplace_back(std::make_tuple(ops, 3, num_of_trees, num_of_colored, num_of_elements, num_current_cnt_steps));
    }

    // Show heap in human-readable form.
    static void show_heap(FibonacciHeap* H) {
        printf("Heap of size %i\n", (int)H->size);
        for (Node* root : NodeIterator(&H->meta_root, false))
            show_node(root, 0);
        printf("\n");
    }

protected:
    size_t max_rank() override {
        max_size = std::max(max_size, size);
        if (naive) return sqrt(2*max_size) + 2;
        else return FibonacciHeap::max_rank();
    }

    void add_child(Node* parent, Node *node) override {
        steps++;
        num_current_cnt_steps++;
        FibonacciHeap::add_child(parent, node);
    }

    Node *remove(Node *n) override {
        steps++;
        num_current_cnt_steps++;
        return FibonacciHeap::remove(n);
    }
};



using Heap = HeapTests;

RandomGen *rng;         // Random generator object

std::vector<int> log_range(int b, int e, int n) {
    std::vector<int> ret;
    for (int i = 0; i < n; i++)
        ret.push_back(b * exp(log(e / b) / (n - 1) * i));
    return ret;
}

// An auxiliary function for generating a random permutation.
template < typename Vector >
void random_permutation(Vector& v)
{
    for (int i = 0; i < (int) v.size(); i++)
        std::swap(v[i], v[i + rng->next_range(v.size() - i)]);
}

// Remove given node from heap.
void remove(Heap* H, Node *n) {
    H->decrease(n, -1000*1000*1000); /// decrease !!!!!!! ?? we want to ensure that this is the min????
    H->extract_min();
    H->num_del_ops +=1;
}

// Force consolidation of the heap.
void consolidate(Heap* H) {
    remove(H, H->insert(0)); /// insert + decrease + extract min ....
    H->num_del_ops -=1;
    H->num_consolidations +=1;
}

// Construct a star with n nodes and root index r.
void star(Heap *H, Node **node_map, int n, int r, bool consolidate_) {

    if (n == 1) {
        // inset one node
        EXPECT(!node_map[r], "");
        node_map[r] = H->insert(r);

    //    HeapTests::show_heap(H);

        if (consolidate_){
          //  std::cout << "consolidation" << std::endl;
            consolidate(H);
        //    HeapTests::show_heap(H);
        }
     //   std::cout << "";
    }

    else {
        star(H, node_map, n - 1, r, false);

        star(H, node_map, n - 1, r + n - 1, true);


        int from = r + n;
        int to =   r + 2*n - 2;

        for (int i = from; i < to; i++) {
            remove(H, node_map[i]);
       //     std::cout << "removing" << std::endl;
        //    HeapTests::show_heap(H);

            node_map[i] = nullptr;
        }
    }
}

/* Generates a sequence on which non-cascading heaps need lots of time.
 * Source: "Replacing Mark Bits with Randomness in Fibonacci Heaps" Jerry Li and John Peebles, MIT
 * -> modified so that only a quadratic number of elements are needed in the star construction.
 */
void test_star(bool naive) {
  //  return;
    printf("%s", text.c_str());

    for (int i = 5; i < 17; i++) {
//        std::cout << "NOVY TEST>>>>>>>>" << std::endl;

        // 1 ---- jeden cely test ------
        int start = 3;
        int N = start + i * (i+1) / 2;
        Heap H(naive);
        Node **node_map = new Node*[N];
        for (int i = 0; i < N; i++) node_map[i] = nullptr;

        // prvni cast == vloz j stromu tvaru hvezda ?
        // start = 3,  3 + j , 3 + j-1,  .... 3 +1
        for (int j = i; j > 0; j--) {
            star(&H, node_map, j, start, false);
//            break;
            start += j;
        }
        int x = H.num_of_trees;
        int elems = H.num_of_elements;

//        H.stats(N);
//        std::cout << "po hvezde konec TEST>>>>>>>>" << std::endl;

        // insert insert, extract extract
        for (int j = 0; j < (1 << i); j++) {
            H.insert(1);
    //        HeapTests::show_heap(&H);
            H.insert(1);
     //       HeapTests::show_heap(&H);
            H.extract_min();
      //      HeapTests::show_heap(&H);
            H.extract_min();
       //     HeapTests::show_heap(&H);
         //   std::cout << "";
        }


        H.stats(N);

        delete[] node_map;
    }
}

/* A random test.
 *
 * The test does 2N insertions first, and then N random operations insert / decrease /
 * extract_min, each with probability proprotional to its weight ins / dec / rem.
 */
void test_random(int ins, int dec, int rem, bool naive) {

    printf("%s", text.c_str());
    for (int N : log_range(50, 80000, 30)) {
        Heap H(naive);
        std::vector<Node*> node_map;
        int s = ins + dec + rem;

        std::vector<int> ops;
        for (int i = 0; i < (N*ins) / s; i++) ops.push_back(0); // pocet insertu
        for (int i = 0; i < (N*dec) / s; i++) ops.push_back(1); // pocet decrease
        for (int i = 0; i < (N*rem) / s; i++) ops.push_back(2); // pocet remove
        random_permutation(ops);

#       define INSERT() node_map.push_back(H.insert(rng->next_range(5*N), node_map.size())) // cislo mezi 5N a node map size

        for (int i = 0; i < 2*N; i++)
            INSERT(); // 2*N insertu !!!

        for (int op : ops) switch (op) {
          case 0:
            INSERT();
            break;
          case 1: {
            Node *node = node_map[rng->next_range(node_map.size())];
            int p = node->priority() - rng->next_range(N / 5) - 1;
            H.decrease(node, p);
            break;
          }
          default: {
            auto ret = H.extract_min();
            if (ret.first + 1 != node_map.size()) {
                node_map[ret.first] = node_map.back();
                node_map[ret.first]->payload = ret.first;
            }
            node_map.pop_back();
          }
        }

        H.stats(N);

#       undef INSERT
    }
}


std::vector<std::pair<std::string, std::function<void(bool)>>> tests = {
    { "star",       test_star },
    { "random",     [](bool n){ test_random(10, 10, 10, n); } },
    { "biased-10",  [](bool n){ test_random(10, 10, 20, n); } },
    { "biased-40",  [](bool n){ test_random(10, 40, 20, n); } },
    { "biased-70",  [](bool n){ test_random(10, 70, 20, n); } },
    { "biased-100", [](bool n){ test_random(10, 100, 20, n); } },
};


int test_it(std::string which_test, std::string id_str,  std::string mode){
    try {
        rng = new RandomGen(stoi(id_str));
    } catch (...) {
        fprintf(stderr, "Invalid student ID\n");
        return 1;
    }

    bool naive;
    if (mode == "std")
        naive = false;
    else if (mode == "naive")
        naive = true;
    else
    {
        fprintf(stderr, "Last argument must be either 'std' or 'naive'\n");
        return 1;
    }

    for (const auto& test : tests) {
        if (test.first == which_test) {
            test.second(naive);
            return 0;
        }
    }

    fprintf(stderr, "Unknown test %s\n", which_test.c_str());
    return 1;

}
int main(int argc, char **argv)
{
//    if (argc != 4) {
//        fprintf(stderr, "Usage: %s <test> <student-id> (std|naive)\n", argv[0]);
//        return 1;
//    }

    std::string which_test = argv[1]; // "star"; //argv[1];
    std::string id_str = argv[2]; // "18"; // argv[2];
    std::string mode =  argv[3]; // "naive" ; // argv[3];

    test_it(which_test, id_str, mode);


//    mode = "std";
//
//    test_it(which_test, id_str, mode);

}
