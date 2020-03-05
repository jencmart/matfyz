//
// Created by jencmart on 11/10/19.
//

#ifndef UKOL5_TMP_HPP
#define UKOL5_TMP_HPP


class tmp {

};






#endif //UKOL5_TMP_HPP


#include <vector>
#include <iostream>

// If the condition is not true, report an error and halt.
#define EXPECT(condition, message) do { if (!(condition)) expect_failed(message); } while (0)
void expect_failed(const std::string& message);

/*
 * payload_t: Type used to allow storing user data in each heap node.
 *
 * priority_t: Type used to represent priority of the node. It must
 *             support comparison.
 *
 * In proper C++, the heap should be a template parametrized by these
 * types, but we decided to keep the code as simple as possible.
 */
typedef int payload_t;
typedef int priority_t;

struct FibonacciHeap;

// Node of FibonacciHeap
struct Node {
    payload_t payload; // custom data
    priority_t priority() { return prio; }

private:
    priority_t prio; // priority for comparison
    Node *parent = nullptr; // parent
    Node *first_child = nullptr; // first child ???
    Node *prev_sibling, *next_sibling; // Pointers in a cyclic list of children of the parent.
    int rank = 0; // 'depth'
    bool marked = false;

    Node(priority_t p, payload_t payload) : payload(payload), prio(p) {
        next_sibling = prev_sibling = this;
    }

    ~Node() {}

    friend class FibonacciHeap;
    friend class HeapTests;
};

/* FibonacciHeap
 *
 * Implemented using a meta root to avoid special handling of the
 * linked list of heap trees.
 */
struct FibonacciHeap {
public:
    FibonacciHeap() : size(0), meta_root(priority_t(), payload_t()) , min(nullptr){} // meta_root(0,0)

    ~FibonacciHeap() {
        if (size == 0) return;
        Node *next;
        Node *n = meta_root.first_child;
        do {
            while (n->first_child) n = n->first_child;
            next = (n->next_sibling != n) ? n->next_sibling : n->parent;
            delete remove(n); // destructor
        } while ((n = next) != &meta_root);
    }

    // Check whether the heap is empty
    bool is_empty() { return size == 0; }

    /* Insert a new node with given priority and payload and return it.
     *
     * `payload` can be used to store any data in the node.
     * TODO - I guess we are good to go
     */
    Node *insert(priority_t prio, payload_t payload = payload_t()) {
        // create new node
        Node *n = new Node(prio, payload);

        // update min pointer
        if(!min || min->prio > n->prio)
            min = n;

        add_child(&meta_root, n); // insert node - as the child of the meta root
        size++;
        return n;
    }

    /* Extract node with minimum priority.
     *
     * Must not be called when the heap is empty.
     * Returns pair (payload, priority) of the removed node.
     * TODO --  I guess we are good to go
     */
    std::pair<payload_t, priority_t> extract_min() {
        EXPECT(meta_root.rank > 0, "Cannot extract minimum of an empty heap."); // make sense

        // Find the tree whose root is minimal.
        Node *min = meta_root.first_child;
        Node *node = min->next_sibling;
        while (node != meta_root.first_child) {
            if (node->prio < min->prio) min = node;
            node = node->next_sibling;
        }

        // Add all its subtrees to the heap. -- yes
        while (min->rank > 0){
            add_child(&meta_root, remove(min->first_child)); // remove subtree of min and connect as child of meta root

        }

        // Remove the root. -- yes
        std::pair<payload_t, priority_t> ret = { min->payload, min->prio };
        delete remove(min); // remove the the min
        min = nullptr;
        size--;


        // Finally, consolidate the heap. -- yes
        consolidate();

        return ret;
    }



    /* Decrease priority of node to new_prio.
     *
     * new_prio must not be higher than node.prio.
     */
    void decrease(Node *node, priority_t new_prio) {
        EXPECT(node, "Cannot decrase null pointer.");
        EXPECT(node->prio >= new_prio, "Decrase: new priority larget than old one.");
        if(node == &meta_root) // in case of enemy attack
            return;
        node->prio = new_prio;  // update the priority
        if(is_root(node))
            return;
        cut(node); // cut the node and connect to the meta root list
    }


    void cut(Node * v){
        // first, we save the parent
        Node *p = v->parent;

        // next we cut the node (unmark it) and put it to the root list
        v->marked = false;
        add_child(&meta_root, remove(v)); // CUT

        // finally we deal with the parent
        if(is_root(p))
            return;

        if(p->marked)
            cut(p);
        else
            p->marked = true;
    }

protected:
    size_t size;
    Node meta_root;
    Node * min;

    // Add node as a child of a given parent.
    virtual void add_child(Node* parent, Node *node) {
        EXPECT(parent, "add_child: Parent cannot be nullptr.");
        EXPECT(node, "add_child: Node cannot be nullptr.");
        EXPECT(!node->parent, "add_child: Node already has a parent.");
        EXPECT(parent == &meta_root || node->rank == parent->rank,"add_child: Ranks of node and parent are different."); // maji mit stejny rank
        EXPECT(node->prev_sibling == node && node->next_sibling == node,"add_child: Node has a sibling."); // node ma byt "verstv" next==prev==node
        EXPECT(parent == &meta_root || parent->prio <= node->prio,"add_child: Parent has bigger priority than node."); // parent (meta root) ma nejsimensi prioritu == 0

        if (parent->rank == 0)  // no siblings - no double linked list
            parent->first_child = node;
        else { // 1+ siblings -> add to double linked list
            join(parent->first_child->prev_sibling, node);
            join(node, parent->first_child);
        }

        node->parent = parent;
        parent->rank++; // cnt of children
    }

    // Consolidate heap during extract_min.
    void consolidate() {
        std::vector<Node*> buckets(max_rank(), nullptr); // ok .... we have the buckets

        while (meta_root.first_child) { // till we have stuff to pair
            Node *node = remove(meta_root.first_child); // remove all subtrees of meta root -- add them to buckets
            while (Node *&b = buckets.at(node->rank)) { // par it till possible
                node = pair_trees(b, node); // merge two trees
                b = nullptr;
            }
            buckets.at(node->rank) = node; // store in bucket
        }

        for (Node *node : buckets)
            if (node)
                add_child(&meta_root, node); // set meta parents from buckets
    }

    // Join two trees of the same rank.
    Node *pair_trees(Node *a, Node *b) {
        if (a->prio > b->prio)
            std::swap(a, b);
        add_child(a, b); // merging two trees of same rank
        return a;
    }

    // Return maximum possible rank of a tree in a heap of the current size.
    virtual size_t max_rank() {
        size_t ret = 1;
        while ((1 << ret) <= size) ret++;
        return ret;
    }

    // Check whether a node is the root.
    bool is_root(Node *n) {
        return n->parent == &meta_root;
    }

    // Link together two elements of linked list -- a and b.
    void join(Node *a, Node *b) {
        a->next_sibling = b;
        b->prev_sibling = a;
    }

    // Disconnect a node from its parent.
    virtual Node *remove(Node *n) {
        EXPECT(n->parent, "remove: Cannot disconnect node without parent.");
        EXPECT(n != &meta_root, "remove: Cannot remove meta root.");

        n->parent->rank--;
        if (n->parent->first_child == n) n->parent->first_child = n->next_sibling;
        if (n->parent->rank == 0) n->parent->first_child = nullptr;

        n->parent = nullptr;
        join(n->prev_sibling, n->next_sibling);
        join(n, n);

        return n;
    }

    friend class HeapTests;
};
