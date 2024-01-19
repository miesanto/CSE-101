//-----------------------------------------------------------------------------
// Mia Santos (miesanto)
// CSE 101 Fall 2023
// List.cpp
// Implementation file for List ADT. List is a double ended queue with a vertical
// cursor standing between elements. The cursor position is always defined
// to be an int in the range 0 (at front) to length of List (at back).
// An empty list consists of the vertical cursor only, with no elements.
//-----------------------------------------------------------------------------
#include<iostream>
#include<string>
#include<stdexcept>
#include "List.h"


// Private constructor --------------------------------------------------------

List::Node::Node(ListElement x) {
    data = x;
    next = nullptr;
    prev = nullptr;
}


// Class Constructors & Destructors -------------------------------------------

// Creates new List in the empty state.
// cursor position and number of elements are set to 0 to represent an empty list
// when the list is empty, both 'frontDummy' and 'backDummy' point to each other
// ^and the 'beforeCursor' and 'afterCursor' pointes are set to these dummy nodes
// when elements are added, they are inserted in between 'frontDummy' and 'backDummy'
List::List() {
    frontDummy = new Node(0);
    backDummy = new Node(0);

    beforeCursor = frontDummy;
    afterCursor = backDummy;

    pos_cursor = 0;
    num_elements = 0;

    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
}

// Copy constructor.
List::List(const List& L) {
    // make this an empty List
    frontDummy = new Node(0);
    backDummy = new Node(0);

    beforeCursor = frontDummy;
    afterCursor = backDummy;

    pos_cursor = 0;
    num_elements = 0;

    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;

    Node* curr = L.frontDummy->next;

    // load elements of L into this List
    while(curr != L.backDummy) {
        this->insertBefore(curr->data);
        curr = curr->next;
    }
}

// Destructor
List::~List() {
    // clear the list
    clear();
    delete frontDummy;
    delete backDummy;
}


// Access functions -----------------------------------------------------------

// length()
// Returns the length of this List.
int List::length() const {
    return num_elements;
}

// front()
// Returns the front element in this List.
// pre: length()>0
ListElement List::front() const {
    // error message
    if( num_elements<=0 ) {
        throw std::runtime_error("List: front(): empty List\n");
    }
    
    return frontDummy->next->data;
}

// back()
// Returns the back element in this List.
// pre: length()>0
ListElement List::back() const {
    // error message
    if( num_elements<=0 ) {
        throw std::runtime_error("List: back(): empty List\n");
    }

    return backDummy->prev->data;
}

// position()
// Returns the position of cursor in this List: 0 <= position() <= length().
int List::position() const {
    return pos_cursor;
}

// peekNext()
// Returns the element after the cursor.
// pre: position()<length()
ListElement List::peekNext() const {
    if( pos_cursor < num_elements ) {
        return afterCursor->data;
    }
    // handle the case where the cursor is at the back of the list
    else {
        throw std::runtime_error("List: peekNext(): cursor is at the back\n");
    }
}

// peekPrev()
// Returns the element before the cursor.
// pre: position()>0
ListElement List::peekPrev() const {
    if( pos_cursor > 0 ) {
        return beforeCursor->data;
    }
    // handle the case where the cursor is at the front of the list
    else {
        throw std::runtime_error("List: peekPrev(): cursor is at the front\n");
    }
}


// Manipulation procedures ----------------------------------------------------

// clear()
// Deletes all elements in this List, setting it to the empty state.
void List::clear() {
    moveFront();
    while(frontDummy->next != backDummy) {
        eraseAfter();
        moveFront();
    }

    pos_cursor = 0;
}

// moveFront()
// Moves cursor to position 0 in this List.
void List::moveFront() {
    pos_cursor = 0;
    afterCursor = frontDummy->next;
    beforeCursor = frontDummy;
}

// moveBack()
// Moves cursor to position length() in this List.
void List::moveBack() {
    pos_cursor = num_elements;
    beforeCursor = backDummy->prev;
    afterCursor = backDummy;
}

// moveNext()
// Advances cursor to the next higher position. Returns the List element that
// was passed over.
// Pre: position() < length()
ListElement List::moveNext() {
    if( pos_cursor < num_elements ) {
        afterCursor = afterCursor->next;
        beforeCursor = beforeCursor->next;
        
        pos_cursor++;

        return beforeCursor->data;
    }
    // handle the case where the cursor is at the back of the list
    else {
        throw std::runtime_error("List: moveNext(): cursor is at the back\n");
    }
}

// movePrev()
// Advances cursor to the next lower position. Returns the List element that
// was passed over.
// Pre: position() > 0
ListElement List::movePrev() {
    if( pos_cursor > 0 ) {
        afterCursor = afterCursor->prev;
        beforeCursor = beforeCursor->prev;
        
        pos_cursor--;

        return afterCursor->data;
    }
    // handle the case where the cursor is at the front of the list
    else {
        throw std::runtime_error("List: movePrev(): cursor is at the front\n");
    }
}

// insertAfter()
// Inserts x after the cursor.
void List::insertAfter(ListElement x) {
    Node* new_node = new Node(x);               // create a new node with x
    
    // adjust pointers to link the new node after the cursor
    new_node->next = afterCursor;
    new_node->prev = beforeCursor;
    
    afterCursor->prev = new_node;               // update the previous node's next pointer to the new node
    beforeCursor->next = new_node;              // update the node before the cursor's next pointer to point to the new node
    afterCursor = new_node;                     // move the cursor forward to the new node
    
    num_elements++;                             // increment num_elements
}

// insertBefore()
// Inserts x before the cursor.
void List::insertBefore(ListElement x) {
    Node* new_node = new Node(x);               // create a new node with x

    // adjust pointers to link the new node before the cursor
    new_node->next = afterCursor;
    new_node->prev = beforeCursor;

    afterCursor->prev = new_node;               // update the next node's previous pointer to point to the new node
    beforeCursor->next = new_node;              // update the node before the cursor's next pointer to point to the new node
    beforeCursor = new_node;                    // move the cursor backward to the new node

    num_elements++;                             // increment num_elements
    pos_cursor++;                               // increment pos_cursor
}

// setAfter()
// Overwrites the List element after the cursor with x.
// Pre: position() < length()
void List::setAfter(ListElement x) {
    if( pos_cursor < num_elements ) {
        afterCursor->data = x;
    }
    // handle the case where the cursor is at the back of the list
    else {
        throw std::runtime_error("List: setAfter(): cursor is at the back\n");
    }
}

// setBefore()
// Overwrites the List element before the cursor with x.
// Pre: position() > 0
void List::setBefore(ListElement x) {
    if( pos_cursor > 0 ) {
        beforeCursor->data = x;
    }
    // handle the case where the cursor is at the front of the list
    else {
        throw std::runtime_error("List: setBefore(): cursor is at the front\n");
    }
}

// eraseAfter()
// Deletes the element after the cursor.
// Pre: position() < length()
void List::eraseAfter() {
    if( pos_cursor < num_elements ) {
        // check if the node after the cursor exists
        if( afterCursor!=nullptr ) {
			Node* to_delete = afterCursor;      // save a pointer to the node to be deleted

			afterCursor = afterCursor->next;    // update the cursor to point to the next node
			
            // adjust pointers to remove to_delete
            beforeCursor->next = afterCursor;
			afterCursor->prev = beforeCursor;

			delete to_delete;                   // delete the node

			num_elements--;                     // decrement num_elements
		}
    }
    // handle the case where the cursor is at the back of the list
    else {
        throw std::runtime_error("List: eraseAfter(): cursor is at the back\n");
    }
}

// eraseBefore()
// Deletes the element before the cursor.
// Pre: position() > 0
void List::eraseBefore() {
    if( pos_cursor > 0 ) {
        // check if the node before the cursor exists
        if( beforeCursor!=nullptr ) {
			Node* to_delete = beforeCursor;     // save a pointer to the node to be deleted

			beforeCursor = beforeCursor->prev;  // update the cursor to point to the previous node
			
            // adjust pointers to remove to_delete
            afterCursor->prev = beforeCursor;
			beforeCursor->next = afterCursor;

			delete to_delete;                   // delete the node

			pos_cursor--;                       // decrement pos_cursor
			num_elements--;                     // decrement num_elements
		}
    }
    // handle the case where the cursor is at the front of the list
    else {
        throw std::runtime_error("List: eraseBefore(): cursor is at the front\n");
    }
}


// Other Functions ------------------------------------------------------------

// findNext()
// Starting from the current cursor position, performs a linear search
// (in the direction front-to-back) for the first occurrence of element x.
// If x is found, places the cursor immediately after the found element,
// then returns the final cursor position. If x is not found, places
// the cursor at position length(), and returns -1.
int List::findNext(ListElement x) {
    while(pos_cursor < num_elements) {
		moveNext();
		
        // if x is found
        if( beforeCursor->data==x ) {
			return pos_cursor;
		}	
	}

    // element not found, place cursor at the end
    // afterCursor = backDummy;
    // pos_cursor = num_elements;
    moveBack();

    return -1;
}

// findPrev()
// Starting from the current cursor position, performs a linear search
// (in the direction back-to-front) for the first occurrence of element x.
// If x is found, places the cursor immediately before the found element,
// then returns the final cursor position. If x is not found, places
// the cursor at position 0, and returns -1.
int List::findPrev(ListElement x) {
    while(pos_cursor > 0) {
		movePrev();
		
        // if x is found
        if( afterCursor->data==x) {
			return pos_cursor;
		}
	
	}

    // element not found, place cursor at the front
    // beforeCursor = frontDummy;
    // pos_cursor = 0;
    moveFront();

    return -1;
}

// cleanup()
// Removes any repeated elements in this List, leaving only unique elements.
// The order of the remaining elements is obtained by retaining the frontmost
// occurrence of each element, and removing all other occurrences. The cursor
// is not moved with respect to the retained elements, i.e., it lies between
// the same two retained elements that it did before cleanup() was called.
void List::cleanup() {
    Node* curr = frontDummy->next;

    while(curr != backDummy) {
        Node* my_node = curr->next;

        while(my_node != backDummy) {
            if( curr->data==my_node->data ) {
                // remove repeated element
                my_node->prev->next = my_node->next;
                my_node->next->prev = my_node->prev;

                /*
                // if the cursor is after the removed element, adjust position
                if( pos_cursor > 0 && my_node == afterCursor )
                    pos_cursor--;
                */

                // if the cursor is at or after the removed element
                if( pos_cursor >= num_elements - 1 || my_node == afterCursor ) {
                    pos_cursor--;
                }

                // delete the node and move the my_node to the next node
                Node* to_delete = my_node;
                my_node = my_node->next;

                delete to_delete;

                num_elements--;
            }
            else {
                my_node = my_node->next;
            }
        }

        curr = curr->next;
    }
}

// concat()
// Returns a new List consisting of the elements of this List, followed by
// the elements of L. The cursor in the returned List will be at position 0.
List List::concat(const List& L) const {
    List result = *this;

    Node* curr_node = L.frontDummy->next;

    // traverse the nodes of L and insert each element before the cursor in result
    while(curr_node != L.backDummy) {
        result.insertBefore(curr_node->data);
        curr_node = curr_node->next;
    }

    result.moveFront();

    return result;
}

// to_string()
// Returns a string representation of this List consisting of a comma-separated
// sequence of elements, surrounded by parentheses.
std::string List::to_string() const {
    std::string result = "(";

    Node* curr = frontDummy->next;

    while(curr != backDummy) {
        if( curr!=frontDummy->next ) {
            result += ", ";
        }

        result += std::to_string(curr->data);
        curr = curr->next;
    }

    result += ")";

    return result;
}

// equals()
// Returns true if and only if this List is the same integer sequence as R.
// The cursors in this List and in R are unchanged.
bool List::equals(const List& R) const {
    // check if the number of elements in the current list is equal to that of R
    if( num_elements!=R.num_elements ) {
        return false;
    }

    Node* curr_this = frontDummy->next;
    Node* curr_R = R.frontDummy->next;

    // traverse until the end of either list is reached
    while(curr_this != backDummy && curr_R != R.backDummy) {
        // check if the data in the current nodes of both lists are equal
        if( curr_this->data!=curr_R->data ) {
            return false;
        }

        curr_this = curr_this->next;
        curr_R = curr_R->next;
    }

    // if all elements are checked and equal, return true
    return true;
}


// Overriden Operators --------------------------------------------------------

// operator<<()
// Inserts the string representation of the List into the stream.
std::ostream& operator<<(std::ostream& stream, const List& L) {
    stream << L.List::to_string();            // using the to_string() function to get the string representation
    return stream;
}

// operator==()
// Returns true if and only if A is the same integer sequence as B.
// The cursors in both Lists are unchanged.
bool operator==(const List& A, const List& B) {
    return A.List::equals(B);           // using the equals() function to check if the lists have the same integer sequence
}

// operator=()
// Overwrites the state of this List with the state of L.
List& List::operator=(const List& L) {
    if( this!=&L ) {                    // not self-assignment
        List temp = L;
        std::swap(frontDummy, temp.frontDummy);
        std::swap(backDummy, temp.backDummy);
        std::swap(beforeCursor, temp.beforeCursor);
        std::swap(afterCursor, temp.afterCursor);
        std::swap(pos_cursor, temp.pos_cursor);
        std::swap(num_elements, temp.num_elements);
    }

    return *this;
}