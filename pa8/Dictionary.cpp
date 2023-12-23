//-----------------------------------------------------------------------------
// Mia Santos (miesanto)
// CSE 101 Fall 2023 pa8
// Dictionary.cpp
// Implementation file for Dictionary ADT based on a Red-Black Tree. Includes a
// built in iterator called current that can perform a smallest-to-largest
// (forward) or a largest-to-smallest (reverse) iteration over all keys.
//-----------------------------------------------------------------------------

#include<iostream>
#include<string>
#include"Dictionary.h"

#define BLACK -2
#define RED -1

// Node constructor
Dictionary::Node::Node(keyType k, valType v) {
    key = k;
    val = v;

    parent = nullptr;
    left = nullptr;
    right = nullptr;

    color = RED;
}


// Helper Functions (Optional) ---------------------------------------------

// inOrderString()
// Appends a string representation of the tree rooted at R to string s. The
// string appended consists of: "key : value \n" for each key-value pair in
// tree R, arranged in order by keys.
void Dictionary::inOrderString(std::string& s, Node* R) const {
    // if the current node is not null
    if( R!=nil ) {
        // in-order traversal
        inOrderString(s, R->left);                                  // left subtree
        s += R->key + " : " + std::to_string(R->val) + "\n";        // append key-val pair to the string
        inOrderString(s, R->right);                                 // right subtree
    }
}

// preOrderString()
// Appends a string representation of the tree rooted at R to s. The appended
// string consists of keys only, separated by "\n", with the order determined
// by a pre-order tree walk.
void Dictionary::preOrderString(std::string& s, Node* R) const {
    // if the current node is not null
    if( R!=nil ) {
        // append the key of the current node to the string
        s += R->key + (R->color == RED ? " (RED)" : "") + "\n";

        // pre-order traversal
        preOrderString(s, R->left);                                 // left subtree
        preOrderString(s, R->right);                                // right subtree
    }
}

// preOrderCopy()
// Recursively inserts a deep copy of the subtree rooted at R into this 
// Dictionary. Recursion terminates at N.
void Dictionary::preOrderCopy(Node* R, Node* N) {
    // if the current node is not at the node N
    if( R!=N ) {
        // deep copy of current node
        setValue(R->key, R->val);

        // recursively copy left and right subtrees
        preOrderCopy(R->left, N);
        preOrderCopy(R->right, N);
    }
}

// postOrderDelete()
// Deletes all Nodes in the subtree rooted at R.
void Dictionary::postOrderDelete(Node* R) {
    // if the current node is not null
    if( R!=nil ) {
        // recursively delete left and right subtrees
        postOrderDelete(R->left);
        postOrderDelete(R->right);

        // delete the current node
        delete R;

        // decrement num_pairs
        // num_pairs--;
    }
}

// search()
// Searches the subtree rooted at R for a Node with key==k. Returns
// the address of the Node if it exists, returns nil otherwise.
Dictionary::Node* Dictionary::search(Node* R, keyType k) const {
    // until k is found or the end of the tree is reached
    while( R!=nil && k!=R->key ) {
        // if k is less than the current node's key
        if( k < R->key ) {
            R = R->left;                                            // move to left subtree
        }
        // else k is greater than the current node's key
        else {
            R = R->right;                                           // move to right subtree
        }
    }

    // return the address of the Node with key==k or nil otherwise
    return R;
}

// findMin()
// If the subtree rooted at R is not empty, returns a pointer to the 
// leftmost Node in that subtree, otherwise returns nil.
Dictionary::Node* Dictionary::findMin(Node* R) {
    // find the leftmost node
    while( R->left!=nil ) {
        R = R->left;
    }

    return R;
}

// findMax()
// If the subtree rooted at R is not empty, returns a pointer to the 
// rightmost Node in that subtree, otherwise returns nil.
Dictionary::Node* Dictionary::findMax(Node* R) {
    // find the rightmost node
    while( R->right!=nil ) {
        R = R->right;
    }

    return R;
}

// findNext()
// If N does not point to the rightmost Node, returns a pointer to the
// Node after N in an in-order tree walk.  If N points to the rightmost 
// Node, or is nil, returns nil. 
Dictionary::Node* Dictionary::findNext(Node* N) {
    // if N has a right child
    if( N->right!=nil ) {
        return findMin(N->right);                                   // find leftmost node in right subtree
    }

    // otherwise, move up the tree until a parent is found (whose left child is the current node)
    Node* P = N->parent;

    // continue moving up while N is the right child of its parent
    while( P!=nil && N==P->right ) {
        N = P;
        P = P->parent;
    }

    return P;
}

// findPrev()
// If N does not point to the leftmost Node, returns a pointer to the
// Node before N in an in-order tree walk.  If N points to the leftmost 
// Node, or is nil, returns nil.
Dictionary::Node* Dictionary::findPrev(Node* N) {
    // if N has a left child
    if( N->left!=nil ) {
        return findMax(N->left);                                   // find rightmost node in left subtree
    }

    // otherwise, move up the tree until a parent is found (whose right child is the current node)
    Node* P = N->parent;
    
    // continue moving up while N is the left child of its parent
    while( P!=nil && N==P->left ) {
        N = P;
        P = P->parent;
    }

    return P;
}


// RBT Helper Functions (Optional) -----------------------------------------
   
// LeftRotate()
void Dictionary::LeftRotate(Node* N) {
    // save the right child of N as R
    Node* R = N->right;

    // update the right child of N to be the left child of R
    N->right = R->left;
    
    // update the parent pointer of R's left child
    if( R->left!=nil ) {
        R->left->parent = N;
    }

    // update R's parent to be N's parent
    R->parent = N->parent;

    // update the parent's child pointer to R
    if( N->parent==nil ) {
        root = R;                                                  // R becomes the new root
    }
    else if( N==N->parent->left ) {
        N->parent->left = R;                                       // R becomes the left child of N's parent
    }
    else {
        N->parent->right = R;                                      // R becomes the right child of N's parent
    }

    // update R's left child to be N
    R->left = N;

    // update N's parent to be R
    N->parent = R;
}

// RightRotate()
// symmetric to LeftRotate()
void Dictionary::RightRotate(Node* N) {
    Node* L = N->left;
    N->left = L->right;
    
    if( L->right!=nil ) {
        L->right->parent = N;
    }

    L->parent = N->parent;

    if( N->parent==nil ) {
        root = L;
    }
    else if( N==N->parent->right ) {
        N->parent->right = L;
    }
    else {
        N->parent->left = L;
    }

    L->right = N;
    N->parent = L;
}

// RB_InsertFixUP()
void Dictionary::RB_InsertFixUp(Node* N) {
    while( N->parent->color==RED ) {
        // check if N's parent is a left child
        if( N->parent==N->parent->parent->left ) {
            // N's uncle
            Node* U = N->parent->parent->right;

            // N's uncle is red
            if( U->color==RED ) {
                N->parent->color = BLACK;
                U->color = BLACK;
                N->parent->parent->color = RED;
                N = N->parent->parent;                             // move N up to its grandparent
            }
            // N's uncle is black
            else {
                if( N==N->parent->right ) {
                    N = N->parent;
                    LeftRotate(N);
                }

                // recolor nodes and perform right rotation
                N->parent->color = BLACK;
                N->parent->parent->color = RED;

                RightRotate(N->parent->parent);
            }
        }
        // N's parent is a right child
        else {
            Node* U = N->parent->parent->left;

            // N's uncle is red
            if( U->color==RED ) {
                N->parent->color = BLACK;
                U->color = BLACK;
                N->parent->parent->color = RED;
                N = N->parent->parent;                              // move N up to its grandparent
            }
            // N's uncle is black
            else {
                if( N==N->parent->left ) {
                    N = N->parent;
                    RightRotate(N);
                }

                // recolor nodes and perform left rotation
                N->parent->color = BLACK;
                N->parent->parent->color = RED;

                LeftRotate(N->parent->parent);
            }
        }
    }

    // ensure the root is black
    root->color = BLACK;
}

// RB_Transplant()
void Dictionary::RB_Transplant(Node* u, Node* v) {
    // if u is the root
    if( u->parent==nil ) {
        root = v;                                                   // set v as the new root
    }
    else if( u==u->parent->left ) {
        u->parent->left = v;                                        // set v as the left child of u's parent
    }
    else {
        u->parent->right = v;                                       // set v as the right child of u's parent
    }

    // update the parent pointer of v to be the parent of u
    v->parent = u->parent;
}

// RB_DeleteFixUp()
void Dictionary::RB_DeleteFixUp(Node* N) {
    while( N!=root && N->color==BLACK ) {
        if( N==N->parent->left ) {
            Node* S = N->parent->right;

            // sibling is red
            if( S->color==RED ) {
                S->color = BLACK;                                   // set sibling to black
                N->parent->color = RED;                             // set parent to red

                LeftRotate(N->parent);                              // perform left rotation on parent

                S = N->parent->right;                               // update sibling
            }
            // both of the sibling's children are black
            if( S->left->color==BLACK && S->right->color==BLACK ) {
                S->color = RED;                                     // set sibling to red
                N = N->parent;                                      // move up to the parent
            }
            else {
                // sibling's right child is black
                if( S->right->color==BLACK ) {
                    S->left->color = BLACK;                         // set sibling's left child to black
                    S->color = RED;                                 // set sibling to red

                    RightRotate(S);                                 // perform right rotation on sibling

                    S = N->parent->right;                           // update sibling
                }

                // sibling's right child is red
                S->color = N->parent->color;                        // set sibling's color to parent's color
                N->parent->color = BLACK;                           // set parent to black
                S->right->color = BLACK;                            // set sibling's right child to black

                LeftRotate(N->parent);                              // perform left rotation on parent

                N = root;                                           // move to the root to exit the loop
            }
        }
        else {
            // same cases for right child
            Node* S = N->parent->left;

            // sibling is red
            if( S->color==RED ) {
                S->color = BLACK;                                   // set sibling to black
                N->parent->color = RED;                             // set parent to red

                RightRotate(N->parent);                             // perform right rotation on parent

                S = N->parent->left;                                // update sibling
            }
            // both of the sibling's children are black
            if( S->right->color==BLACK && S->left->color==BLACK ) {
                S->color = RED;                                     // set sibling to red
                N = N->parent;                                      // move up to the parent
            }
            else {
                // sibling's left child is black
                if( S->left->color==BLACK ) {
                    S->right->color = BLACK;                        // set sibling's right child to black
                    S->color = RED;                                 // set sibling to red

                    LeftRotate(S);                                  // perform left rotation on sibling

                    S = N->parent->left;                            // update sibling
                }

                // sibling's left child is red
                S->color = N->parent->color;                        // set sibling's color to parent's color
                N->parent->color = BLACK;                           // set parent to black
                S->left->color = BLACK;                             // set sibling's left child to black

                RightRotate(N->parent);                             // perform right rotation on parent

                N = root;                                           // move to the root to exit the loop
            }
        }
    }

    N->color = BLACK;
}

// RB_Delete()
void Dictionary::RB_Delete(Node* N) {
    Node* Y = N;                                                    // node to be spliced out
    Node* X = nil;                                                  // node to replace Y

    int og_color = Y->color;

    // N has no left child
    if( N->left==nil ) {
        X = N->right;
        RB_Transplant(N, N->right);
    }
    // N has no right child
    else if( N->right==nil ) {
        X = N->left;
        RB_Transplant(N, N->left);
    }
    // N has both left and right children
    else {
        Y = findMin(N->right);                                      // find the smallest node in right subtree
        og_color = Y->color;                                        // save the original color
        X = Y->right;

        if( Y->parent==N ) {
            X->parent = Y;                                          // update X's parent
        }
        else {
            RB_Transplant(Y, Y->right);
            Y->right = N->right;
            Y->right->parent = Y;
        }

        RB_Transplant(N, Y);
        Y->left = N->left;
        Y->left->parent = Y;
        Y->color = N->color;
    }

    if( og_color==BLACK ) {
        RB_DeleteFixUp(X);
    }

    delete N;
}


// Class Constructors & Destructors ----------------------------------------
   
// Creates new Dictionary in the empty state.
Dictionary::Dictionary() {
    nil = new Node("", 0);                                      // sentinel node ('""' is the key, '0' is the value)
    
    nil->color = BLACK;

    // set the root and current pointers to the sentinel node
    root = nil;
    current = nil;

    // init the number of key-value pairs to 0
    num_pairs = 0;
}

// Copy constructor.
Dictionary::Dictionary(const Dictionary& D) {
    nil = new Node("", 0);                                      // sentinel node ('""' is the key, '0' is the value)
    
    nil->color = BLACK;

    // set the root and current pointers to the sentinel node
    root = nil;
    current = nil;

    // init the number of key-value pairs to 0
    num_pairs = 0;

    // copy the pairs from the source dictionary
    preOrderCopy(D.root, D.nil);
}

// Destructor
Dictionary::~Dictionary() {
    postOrderDelete(root);
    delete nil;
}


// Access functions --------------------------------------------------------

// size()
// Returns the size of this Dictionary.
int Dictionary::size() const {
    return num_pairs;
}

// contains()
// Returns true if there exists a pair such that key==k, and returns false
// otherwise.
bool Dictionary::contains(keyType k) const {
    return search(root, k) != nil;
}

// getValue()
// Returns a reference to the value corresponding to key k.
// Pre: contains(k)
valType& Dictionary::getValue(keyType k) const {
    // error message
    if( !contains(k) ) {
        throw std::logic_error("getValue(): pre-condition contains(k) not met");
    }
    
    Node* N = search(root, k);

    return N->val;
}

// hasCurrent()
// Returns true if the current iterator is defined, and returns false 
// otherwise.
bool Dictionary::hasCurrent() const {
    return current!=nil;
}

// currentKey()
// Returns the current key.
// Pre: hasCurrent() 
keyType Dictionary::currentKey() const {
    // error message
    if( !hasCurrent() ) {
        throw std::logic_error("currentKey(): pre-condition hasCurrent() not met");
    }

    return current->key;
}

// currentVal()
// Returns a reference to the current value.
// Pre: hasCurrent()
valType& Dictionary::currentVal() const {
    // error message
    if( !hasCurrent() ) {
        throw std::logic_error("currentKey(): pre-condition hasCurrent() not met");
    }

    return current->val;
}


// Manipulation procedures -------------------------------------------------

// clear()
// Resets this Dictionary to the empty state, containing no pairs.
void Dictionary::clear() {
    // delete all nodes in the tree
    postOrderDelete(root);
    
    // reset member variables
    root = nil;
    current = nil;
    num_pairs = 0;
}

// setValue()
// If a pair with key==k exists, overwrites the corresponding value with v, 
// otherwise inserts the new pair (k, v).
// Manipulation procedure: setValue
void Dictionary::setValue(keyType k, valType v) {
    // init parent and current pointers
    Node* P = nil;
    Node* C = root;
    
    // new node with the key, value pair
    Node* newNode = new Node(k, v);
    
    // set the left and right child pointers of the new node to nil
    newNode->left = nil;
    newNode->right = nil;

    newNode->color = RED;

    // traverse the tree to find the position for the new node
    while( C!=nil ) {
        P = C;
        if( k < C->key ) {
            C = C->left;
        }
        else if( k > C->key ) {
            C = C->right;
        }
        // a pair with key==k exists
        else {
            C->val = v;                                         // overwrite the corresponding value with v
            delete newNode;
            return;
        }
    }

    // set the parent pointer of the new node
    newNode->parent = P;
    
    // if the tree is empty
    if( P==nil ) {
        root = newNode;                                         // new node becomes the root
    }
    else if( k < P->key ) {
        P->left = newNode;
    }
    else {
        P->right = newNode;
    }
    
    // increment num_pairs
    num_pairs++;

    RB_InsertFixUp(newNode);
}

// remove() VERY DIFF FROM PA7
// Deletes the pair for which key==k. If that pair is current, then current
// becomes undefined.
// Pre: contains(k).
void Dictionary::remove(keyType k) {
    // error message
    if( !contains(k) ) {
        throw std::logic_error("remove(): pre-condition contains(k) not met");
    }

    // find the node to be removed
    Node* toRemove = search(root, k);
    
    // if the node to be removed is the current iterator, set current to nil
    if (toRemove == current) {
        current = nil;
    }

    // perform red-black tree deletion
    RB_Delete(toRemove);

    // decrement num_pairs
    num_pairs--;
}

// begin()
// If non-empty, places current iterator at the first (key, value) pair
// (as defined by the order operator < on keys), otherwise does nothing. 
void Dictionary::begin() {
    if( root!=nil ) {
        current = findMin(root);
    }
}

// end()
// If non-empty, places current iterator at the last (key, value) pair
// (as defined by the order operator < on keys), otherwise does nothing. 
void Dictionary::end() {
    if( root!=nil ) {
        current = findMax(root);
    }
}

// next()
// If the current iterator is not at the last pair, advances current 
// to the next pair (as defined by the order operator < on keys). If 
// the current iterator is at the last pair, makes current undefined.
// Pre: hasCurrent()
void Dictionary::next() {
    // error message
    if( !hasCurrent() ) {
        throw std::logic_error("next(): pre-condition hasCurrent() not met");
    }

    Node* N = findNext(current);

    // update the current iterator to the next node (or nil if at the last pair)
    current = (N != nil) ? N : nil;
}

// prev()
// If the current iterator is not at the first pair, moves current to  
// the previous pair (as defined by the order operator < on keys). If 
// the current iterator is at the first pair, makes current undefined.
// Pre: hasCurrent()
void Dictionary::prev() {
    // error message
    if( !hasCurrent() ) {
        throw std::logic_error("prev(): pre-condition hasCurrent() not met");
    }

    Node* N = findPrev(current);

    // update the current iterator to the next node (or nil if at the last pair)
    current = (N != nil) ? N : nil;
}


// Other Functions ---------------------------------------------------------

// to_string()
// Returns a string representation of this Dictionary. Consecutive (key, value)
// pairs are separated by a newline "\n" character, and the items key and value 
// are separated by the sequence space-colon-space " : ". The pairs are arranged 
// in order, as defined by the order operator <.
std::string Dictionary::to_string() const {
    std::string s = "";

    inOrderString(s, root);
    return s;
}

// pre_string()
// Returns a string consisting of all keys in this Dictionary. Consecutive
// keys are separated by newline "\n" characters. The key order is given
// by a pre-order tree walk.
std::string Dictionary::pre_string() const {
    std::string s = "";

    preOrderString(s, root);
    return s;
}

// equals()
// Returns true if and only if this Dictionary contains the same (key, value)
// pairs as Dictionary D.
bool Dictionary::equals(const Dictionary& D) const {
    return to_string() == D.to_string();
}

// Overloaded Operators ----------------------------------------------------
   
// operator<<()
// Inserts string representation of Dictionary D into stream, as defined by
// member function to_string().
std::ostream& operator<<( std::ostream& stream, Dictionary& D ) {
    return stream << D.to_string();
}

// operator==()
// Returns true if and only if Dictionary A equals Dictionary B, as defined
// by member function equals(). 
bool operator==( const Dictionary& A, const Dictionary& B ) {
    return A.equals(B);
}

// operator=()
// Overwrites the state of this Dictionary with state of D, and returns a
// reference to this Dictionary.
Dictionary& Dictionary::operator=( const Dictionary& D ) {
    if( this!=&D ) {
        clear();
        preOrderCopy(D.root, D.nil);
    }

    return *this;
}