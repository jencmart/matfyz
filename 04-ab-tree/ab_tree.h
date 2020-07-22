#include <limits>
#include <vector>
#include <iostream>

using namespace std;

// If the condition is not true, report an error and halt.
#define EXPECT(condition, message) do { if (!(condition)) expect_failed(message); } while (0)

void expect_failed(const string& message);

/*** One node ***/

class ab_node {
  public:
    // Keys stored in this node and the corresponding children
    // The vectors are large enough to accomodate one extra entry
    // in overflowing nodes.
    vector<ab_node *> children;
    vector<int> keys;

    // If this node contains the given key, return true and set i to key's position.
    // Otherwise return false and set i to the first key greater than the given one.
    bool find_branch(int key, int &keyPosition)
    {
        keyPosition = 0;
        while (keyPosition < (int) keys.size() && keys[keyPosition] <= key) {
            if (keys[keyPosition] == key)
                return true;
            keyPosition++;
        }
        return false;
    }

    // Insert a new key at position i and add a new child between keys i and i+1.
    void insert_branch(int position, int key, ab_node *child)
    {
        keys.insert(keys.begin() + position, key);
        children.insert(children.begin() + position + 1, child);
    }

    // An auxiliary function for displaying a sub-tree under this node.
    void show(int indent);
};

/*** Tree ***/

class ab_tree {
  public:
    int a;          // Minimum allowed number of children
    int b;          // Maximum allowed number of children
    ab_node *root;  // Root node (even a tree with no keys has a root)
    int num_nodes;  // We keep track of how many nodes the tree has

    // Create a new node and return a pointer to it.
    ab_node *new_node()
    {
        ab_node *n = new ab_node;
        n->keys.reserve(b); // malloc
        n->children.reserve(b+1); // malloc
        num_nodes++;
        return n;
    }

    // Constructor: initialize an empty tree with just the root.
    ab_tree(int a, int b)
    {
        EXPECT(a >= 2 && b >= 2*a - 1, "Invalid values of a,b");
        this->a = a;
        this->b = b;
        num_nodes = 0;
        // The root has no keys and one null child pointer.
        root = new_node();
        root->children.push_back(nullptr);
    }

    ~ab_tree()
    {
        delete_tree(root);
        EXPECT(num_nodes == 0, "Memory leak detected: some nodes were not deleted");
    }

    void delete_tree(ab_node *n)
    {
        for (auto & i : n->children)
            if (i)
                delete_tree(i);

        // Delete a given node, assuming that its children have been already unlinked.
        num_nodes--;
        delete n;
    }

    void splitRecursively(ab_node * node){
        // split to three parts

        // insert middle part to the parent

        // call split on parent
    }

    void insertToNode(ab_node *n, int key, int i) {
        // one key == two children (root)
        if(n->keys.empty())
            n->children.insert(n->children.begin(), nullptr);

        // insert value + children
        n->insert_branch(i,key, nullptr);

        // overflow
        if(n->children.size() > b){
            splitRecursively(n);
        }
    }

    /**
     *
     * @param subtreeRoot -- root of the subtree where we want to insert the key
     * @param key  -- key to insert
     * @param left -- in case we split node, this is the left node
     * @param right -- this is the right node
     * @param middleValue -- this is the key from the middle
     */
    void insertRec(ab_node * subtreeRoot, int key, ab_node *& left, ab_node *& right, int *& middleValue){

        /// ------------ stopping criterion  -------------------------------------------

        // we are in a leaf
        if(subtreeRoot == nullptr){
            left = right = nullptr; // no split of the node
            *middleValue = key; // but this key goes to the parent
            return;
        }

        // key already present
        int pos;
        if(subtreeRoot->find_branch(key,pos)){
            left = right = nullptr; // no split
            middleValue = nullptr; // no send to parent
            return;
        }

        // otherwise go deeper
        ab_node * child = subtreeRoot->children[pos];
        insertRec(child, key, left, right, middleValue);

        /// ------------ now we have returned and we modify current node ---------------

        // no value was returned from lower level == all done
        if(middleValue == nullptr)
            return;

        // child was the (nullptr) leaf - we insert the value here
        else if(left == nullptr && right == nullptr)
            subtreeRoot->insert_branch(pos, key, nullptr);

        // child was split
        // left part remains on it's position ( we just modified the child)
        // right part is the new node we need to insert ( as right child of the value)
        else
            subtreeRoot->insert_branch(pos, *middleValue, right);

        /// ------------  finally we have to deal with possible overflow ---------------
        if(subtreeRoot->keys.size() == b){
            // middle key
            int splitIdx = ((b-1)/2);
            *middleValue = subtreeRoot->keys[splitIdx];

            // -- Right part --
            // for the right part we crate new node and copy appropriate values and child nodes
            // move keys     - form indexes (splitIdx+1, ..., b-1)
            // move children - form indexes (splitIdx+1, ..., b)
            right = new_node();
            right->keys.insert(right->keys.begin(), make_move_iterator(subtreeRoot->keys.begin() + splitIdx + 1), make_move_iterator(subtreeRoot->keys.end()));
            right->children.insert(right->children.begin(), make_move_iterator(subtreeRoot->children.begin() + splitIdx + 1), make_move_iterator(subtreeRoot->children.end()));

            // -- Left part --
            // just modification of current node
            // keys     - remain indexes (0, ..., splitIdx - 1)
            // children - remain indexes (0, ..., splitIdx)
            subtreeRoot->keys.erase(subtreeRoot->keys.begin() + splitIdx, subtreeRoot->keys.end() );
            subtreeRoot->children.erase(subtreeRoot->children.begin() + splitIdx + 1,  (subtreeRoot->children.end()));
            left = subtreeRoot;
        }
        // all set - no further work needed
        else {
            left = right = nullptr;
            middleValue = nullptr;
        }
    }

    // Insert: add key to the tree (unless it was already present).
    void insert(int key)
    {
        // prepare some nice pointers
        ab_node * left = nullptr;
        ab_node * right = nullptr;
        int *val = new int;

        // insert recursively
        insertRec(root, key, left, right, val);

        // if root was split, create new root
       if(right != nullptr){
            auto * newRoot = new_node();
            newRoot->children.push_back(left);
            newRoot->children.push_back(right);
            newRoot->keys.emplace_back(*val);
            root = newRoot;
       }
       delete(val);
    }

    // Find a key: returns true if it is present in the tree.
    bool find(int key)
    {
        ab_node *n = root;
        while (n) {
            int i;
            if (n->find_branch(key, i))
                return true;
            n = n->children[i];
        }
        return false;
    }

    // Display the tree on standard output in human-readable form.
    void show();

    // Check that the data structure satisfies all invariants.
    void audit();
};
