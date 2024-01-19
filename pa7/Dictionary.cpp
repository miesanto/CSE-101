//-----------------------------------------------------------------------------
// Mia Santos (miesanto)
// CSE 101 Fall 2023 pa7
// Dictionary.h
// Implementation file for Dictionary ADT based on a Binary Search Tree. Includes a
// built in iterator called current that can perform a smallest-to-largest
// (forward) or a largest-to-smallest (reverse) iteration over all keys.
//-----------------------------------------------------------------------------

#include<iostream>
#include<string>
#include"Dictionary.h"

// Node constructor
Dictionary::Node::Node(keyType k, valType v) {
    key = k;
    val = v;

    parent = nullptr;
    left = nullptr;
    right = nullptr;
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
        s += R->key + "\n";

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


// Class Constructors & Destructors ----------------------------------------
   
// Creates new Dictionary in the empty state.
Dictionary::Dictionary() {
    nil = new Node("", 0);                                      // sentinel node ('""' is the key, '0' is the value)
    
    // set the root and current pointers to the sentinel node
    root = nil;
    current = nil;

    // init the number of key-value pairs to 0
    num_pairs = 0;
}

// Copy constructor.
Dictionary::Dictionary(const Dictionary& D) {
    nil = new Node("", 0);                                      // sentinel node ('""' is the key, '0' is the value)
    
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
}

// remove()
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

    // toRemove has no children
    if( toRemove->left==nil && toRemove->right==nil ) {
        if( toRemove==root ) {
            root = nil;
        }
        // adjust parent's left or right pointer to nil
        else {
            if( toRemove->parent->left==toRemove ) {
                toRemove->parent->left = nil;
            }
            else {
                toRemove->parent->right = nil;
            }
        }
    }
    // toRemove has both left and right children
    else if( toRemove->left!=nil && toRemove->right!=nil ) {
        // find the minimum node in the right subtree
        Node* S = findMin(toRemove->right);

        // if S is not in the right child of the node to be removed, update pointers
        if( S->parent!=toRemove ) {
            S->parent->left = S->right;
            if( S->right!=nil ) {
                S->right->parent = S->parent;
            }
            S->right = toRemove->right;
            S->right->parent = S;
        }
        
        // adjust parent's left or right pointer to S
        if( toRemove==root ) {
            root = S;
        } 
        else {
            if( toRemove->parent->left==toRemove ) {
                toRemove->parent->left = S;
            }
            else {
                toRemove->parent->right = S;
            }
        }

        // update S' pointers
        S->parent = toRemove->parent;
        S->left = toRemove->left;
        S->left->parent = S;
    }
    // toRemove has either left or right child
    else {
        // left child
        if( toRemove->right==nil ) {
            if( toRemove==root ) {
            root = toRemove->left;
            } 
            // adjust parent's left or right pointer to the left child
            else {
                if( toRemove->parent->left==toRemove ) {
                    toRemove->parent->left = toRemove->left;
                }
                else {
                    toRemove->parent->right = toRemove->left;
                }
            }

            // update left child's parent pointer
            toRemove->left->parent = toRemove->parent;
        }

        // right child
        if( toRemove->left==nil ) {
            if( toRemove==root ) {
                root = toRemove->right;
            } 
            // adjust parent's left or right pointer to the right child
            else {
                if( toRemove->parent->left==toRemove ) {
                    toRemove->parent->left = toRemove->right;
                }
                else {
                    toRemove->parent->right = toRemove->right;
                }
            }

            // update right child's parent pointer
            toRemove->right->parent = toRemove->parent;
        }
    }

    // delete the node to be removed
    delete toRemove;

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