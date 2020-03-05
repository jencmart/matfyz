// A node of the tree
class Node {
  public:
    int key;
    Node* left;
    Node* right;
    Node* parent;

    // Constructor
    Node(int key, Node* parent=nullptr, Node* left=nullptr, Node* right=nullptr) {
        this->key = key;
        this->parent = parent;
        this->left = left;
        this->right = right;
        if (left) left->parent = this;
        if (right) right->parent = this;
    }
};

// Binary tree
class Tree {
  public:
    // Pointer to root of the tree; nullptr if the tree is empty.
    Node* root;

    Tree(Node* root=nullptr) {
        this->root = root;
    }

    // Rotate the given `node` up. Perform a single rotation of the edge
    // between the node and its parent, choosing left or right rotation
    // appropriately.
    virtual void rotate(Node* node) {
        if (node->parent) {
            // ZIG a-x-b---r---c
            if (node->parent->left == node) {
                if (node->right) node->right->parent = node->parent;
                node->parent->left = node->right;
                node->right = node->parent;

            // ZAG
            } else {
                if (node->left) node->left->parent = node->parent;
                node->parent->right = node->left;
                node->left = node->parent;
            }

            // happens to be a root already
            if (node->parent->parent) {
                if (node->parent->parent->left == node->parent)
                    node->parent->parent->left = node;
                else
                    node->parent->parent->right = node;
            } else {
                root = node;
            }

            Node* original_parent = node->parent;
            node->parent = node->parent->parent;
            original_parent->parent = node;
        }
    }


    void doubleRotate(Node * node){
        Node* x = node;
        Node* y = node->parent;
        Node* z = node->parent->parent;
        Node* parentZ = node->parent->parent->parent;

        // same for all
        if(parentZ){
            if(parentZ->left == z)
                parentZ->left = x;
            else
                parentZ->right = x;
        }
        x->parent = parentZ;

        // zig-zig OR zag-zag
        if((y->left == x && z->left == y) || (y->right == x && z->right == y)) {

            // zig-zig
            bool right = false;
            if(y->left == x && z->left == y)
                right = true;

            Node* subB = right ? x->right : x->left;
            Node* subC = right ? y->right : y->left;

            // 2/5
            right ? x->right = y : x->left = y;
            y->parent = x;

            // 3/5
            z->parent = y;
            right ? y->right = z :  y->left = z;

            // 4/5
            right ? z->left = subC : z->right = subC; // ma byt opacne
            if(subC)
                subC->parent = z;

            // 5/5
            right ? y->left = subB : y->right = subB; // ma byt opacne
            if(subB)
                subB->parent = y;
        }

            // zag-zig OR zig-zag
        else {
            bool zag_zig = true;
            if(y->left == x && z->right == y)
                zag_zig = false;

            Node* subB = x->left;
            Node* subC = x->right;

            // 2/5
            zag_zig ? x->left = y : x->right = y;
            y->parent = x;

            // 3/5
            zag_zig ? x->right = z : x->left = z;
            z->parent = x;

            if(zag_zig) {
                if(subB) subB->parent = y;
                y->right = subB;

                if(subC) subC->parent = z;
                z->left = subC;
            }

            else {
                if(subB) subB->parent = z;
                z->right = subB;

                if(subC) subC->parent = y;
                y->left = subC;
            }
        }
        if(node->parent == nullptr)
            this->root = node;
    }

    // Look up the given key in the tree, returning the
    // the node with the requested key or nullptr.
    // splay the found node
    // if not exist splay last existing node
    Node* lookup(int key) {
        Node* node = root;
        while (node) {
            // found
            if (node->key == key) {
                this->splay(node);
                return node;
            }
            // go left
            if (key < node->key){
                if(node->left == nullptr){
                    this->splay(node);
                    return nullptr;
                }
                else
                    node = node->left;
            }
            // go right
            else{
                if (node->right == nullptr){
                    this->splay(node);
                    return nullptr;
                }
                else
                    node = node->right;
            }
        }
        return nullptr;
    }

    // Insert a key into the tree.
    // If the key is already present, nothing happens.
    void insert(int key) {
        // new root - done
        if (!root) {
            root = new Node(key);
            return;
        }

        Node* node = root;
        while (node->key != key) {
            if (key < node->key) {
                if (!node->left){
                    node->left = new Node(key, node);
                    node = node->left;
                    splay(node);
                    return;
                }
                node = node->left;
            }
            else if ( key > node->key){
                if (!node->right){
                    node->right = new Node(key, node);
                    node = node->right;
                    splay(node);
                    return;
                }
                node = node->right;
            }
            else {
                // splay ??
                return;
            }
        }
    }

    // Delete given key from the tree.
    // It the key is not present, do nothing.
    void remove(int key) {
        if(! this->root)
            return;

        Node* node = root;
        while (node && node->key != key) {
            if (key < node->key)
                node = node->left;
            else
                node = node->right;

            // what about some splaying closest value to the root?
        }

        // we have the key
        if (node) {

            // not the leaf - switch with successor == make it a (partial) leaf
            if (node->left && node->right) {
                Node* replacement = node->right;
                while (replacement->left)
                    replacement = replacement->left;
                node->key = replacement->key;
                node = replacement;
            }

            // null or node
            Node* replacement = node->left ? node->left : node->right;

            // if we have parent - connect it to the replacement
            if (node->parent) {
                if (node->parent->left == node) node->parent->left = replacement;
                else node->parent->right = replacement;
            } else {
                root = replacement;
            }

            // same way connect the replacement (if it is not already null)
            if (replacement)
                replacement->parent = node->parent;

            Node * toSplay = node->parent;
            delete node;
            if(toSplay)
                this->splay(toSplay);
        }
    }

    // Splay the given node.
    // If a single rotation needs to be performed, perform it as the last rotation
    // (i.e., to move the splayed node to the root of the tree).
    virtual void splay(Node* node) {
        while(true){
            // you are root, do nothing (root is already marked correctly) end
            if(node == this->root)
                return;
            // yor parent is root, do the simple rotation and end
            else if(node->parent == this->root)
                this->rotate(node);
            // if you have grandparent do double rotation
            else
                this->doubleRotate(node);
        }
    }

    // Destructor to free all allocated memory.
    ~Tree() {
        Node* node = root;
        while (node) {
            Node* next;
            if (node->left) {
                next = node->left;
                node->left = nullptr;
            } else if (node->right) {
                next = node->right;
                node->right = nullptr;
            } else {
                next = node->parent;
                delete node;
            }
            node = next;
        }
    }
};
