/*
*  PA1 List.c
*  Implementation file for List ADT
*  Author: Mia Santos (miesanto)
*  Date: 10/7/2023
*  CSE 101 Fall 2023, Tantalo
*/

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>
#include "List.h"

// Structs -------------------------------------------------------------------------------------------------------------------------------

// private NodeObj type
typedef struct NodeObj {
    int data;
    struct NodeObj* previous;
    struct NodeObj* next;
} NodeObj;

// private Node type (pointer to NodeObj struct)
typedef NodeObj* Node;

// private ListObj type
typedef struct ListObj {
    Node front;
    Node back;
    Node cursor;
    int length;
    int index;
} ListObj;


// Constructors-Destructors --------------------------------------------------------------------------------------------------------------

// newNode()
// Returns reference to new Node object. Initializes previous, next, and data fields
Node newNode(ListElement data) {
    Node N = malloc(sizeof(NodeObj));
    assert( N!=NULL );
    N->data = data;
    N->previous = NULL;
    N->next = NULL;
    return(N);
}

//freeNode()
// Frees heap memory pointed to by *pN, sets *pN to NULL
void freeNode(Node* pN) {
    if( pN!=NULL && *pN!=NULL ) {
        free(*pN);
        *pN = NULL;
    }
}

// newList()
// Creates and returns a new empty List
List newList() {
    List L;
    L = malloc(sizeof(ListObj));            // allocate memory
    assert( L!=NULL );                      // confirm that allocation was successful
    
    // initialize the List fields
    L->front = NULL;
    L->back = NULL;
    L->cursor = NULL;
    L->length = 0;                          // empty list
    L->index = -1;                          // cursor is undefined by default
    
    return(L);
}

// freeList()
// Frees all heap memory assoc. with *pL and sets *pL to NULL
void freeList(List* pL) {
    // check if pL and *pL are not NULL to ensure we have a valid pointer to a List
    if( pL!=NULL && *pL!=NULL ) {
        clear(*pL);                         // remove all elements from the list and frees memory assoc. w/ nodes
        free(*pL);                          // free the memory assoc. w/ the List structure itself
        *pL = NULL;                         // pointer is no longer pointing to the deallocated memory
    }
}

// Access functions ----------------------------------------------------------------------------------------------------------------------

// length()
// Returns the number of elements in L
int length(List L) {
    // error message
    if( L==NULL ) {
        printf("List Error: calling length() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    // return number of elements
    return(L->length);
}

// index()
// Returns index of cursor element if defined, -1 otherwise
int index(List L) {
    // error message
    if( L==NULL ) {
        printf("List Error: calling index() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    // return index of cursor
    return(L->index);
}

// front()
// Returns front element of L
// Pre: length() > 0
int front(List L) {
    // error messages
    if( L==NULL ) {
        printf("List Error: calling front() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if( L->length <= 0 ) {
        printf("List Error: calling front() on an empty List\n");
        exit(EXIT_FAILURE);
    }

    // return front element
    return(L->front->data);
}

// back()
// Returns back element of L
// Pre: length() > 0
int back(List L) {
    // error messages
    if( L==NULL ) {
        printf("List Error: calling back() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if( L->length <= 0 ) {
        printf("List Error: calling back() on an empty List\n");
        exit(EXIT_FAILURE);
    }

    // return back element
    return(L->back->data);
}

// get()
// Returns cursor element of L
// Pre: length() > 0, index() >= 0
int get(List L) {
    // error messages
    if( L==NULL ) {
        printf("List Error: calling get() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if( L->length <= 0 ) {
        printf("List Error: calling get() on an empty List\n");
        exit(EXIT_FAILURE);
    }

    if( index(L) < 0 ) {
        printf("List Error: calling get() with an undefined cursor\n");
        exit(EXIT_FAILURE);
    }

    // return cursor element
    return(L->cursor->data);
}

// equals()
// Returns true iff Lists A and B are in same state, false otherwise
bool equals(List A, List B) {
    // error message
    if( A==NULL || B==NULL ) {
        printf("List Error: calling equals() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    // if lists are of different lengths, they can't be in same state
    if( A->length != B->length ) {
        return(false);
    }

    Node nodeA = A->front;
    Node nodeB = B->front;

    // go through every element in A and B and compare
    while( nodeA!=NULL && nodeB!=NULL ) {
        if( nodeA->data != nodeB->data ) {          // elements are different
            return(false);                          // ...therefore lists are not the same
        }
        nodeA = nodeA->next;
        nodeB = nodeB->next;
    }

    return true;                                    // A and B have the same elements in the same order
}


// Manipulation procedures ---------------------------------------------------------------------------------------------------------------

// clear()
// Resets L to its original empty state
void clear(List L) {
    // error message
    if( L==NULL ) {
        printf("List Error: calling clear() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    // free each node in the list
    Node curr = L->front;
    while( curr!=NULL ) {
        Node temp = curr;
        curr = curr->next;
        freeNode(&temp);
    }

    // reset the list state
    L->front = NULL;
    L->back = NULL;
    L->cursor = NULL;
    L->length = 0;
    L->index = -1;
}

// set()
// Overwrites the cursor element's data with x
// Pre: length() > 0, index() >= 0
void set(List L, int x) {
    // error messages
    if( L==NULL ) {
        printf("List Error: calling set() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if( L->length <= 0 ) {
        printf("List Error: calling set() on an empty List\n");
        exit(EXIT_FAILURE);
    }

    if( index(L) < 0 ) {
        printf("List Error: calling set() with an undefined cursor\n");
        exit(EXIT_FAILURE);
    }

    // update the data of the cursor element
    L->cursor->data = x;
}

// moveFront()
// If L is non-empty, sets cursor under the front element
// Otherwise, does nothing
void moveFront(List L) {
    if( L!=NULL ) {
        if( L->length > 0 ) {
            L->cursor = L->front;
            L->index = 0;
        }
    }
}

// moveBack()
// If L is non-empty, sets cursor under the back element
// Otherwise, does nothing
void moveBack(List L) {
    if( L!=NULL ) {
        if( L->length > 0 ) {
            L->cursor = L->back;
            L->index = L->length - 1;
        }
    }
}

// movePrev()
// If cursor is defined and not at the front, move cursor one step toward the front of L
// If cursor is defined and at the front, cursor becomes undefined
// If cursor is undefined, do nothing
void movePrev(List L) {
    // error message
    if( L==NULL ) {
        printf("List Error: calling movePrev() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    // check if cursor is defined
    if( L->cursor != NULL ) {
        // if cursor is not at the front, move cursor one step toward front
        if( L->cursor != L->front ) {
            L->cursor = L->cursor->previous;
            L->index--;
        }
        // cursor becomes undefined if it's already at the front
        else {
            L->cursor = NULL;
            L->index = -1;
        }
    }
}


// moveNext()
// If cursor is defined and not at the back, move cursor one step toward the back of L
// If cursor is defined and at the back, cursor becomes undefined
// If cursor is undefined, do nothing
void moveNext(List L) {
    // error message
    if( L==NULL ) {
        printf("List Error: calling moveNext() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    // check if cursor is defined
    if( L->cursor != NULL ) {
        // if cursor is not at the back, move cursor one step toward back
        if( L->cursor != L->back ) {
            L->cursor = L->cursor->next;
            L->index++;
        }
        // cursor becomes undefined if it's already at the back
        else {
            L->cursor = NULL;
            L->index = -1;
        }
    }
}


// prepend()
// Insert new element into L
// If L is non-empty, insertion takes place before front element
void prepend(List L, int x) {
    // error message
    if( L==NULL ) {
        printf("List Error: calling prepend() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    // create a new node with the given data
    Node createNewNode = newNode(x);

    // if the list is empty, the new node becomes both front and back
    if( L->front == NULL ) {
        L->front = createNewNode;
        L->back = createNewNode;
    }
    // else, insert the new node before the current front
    else {
        createNewNode->next = L->front;
        L->front->previous = createNewNode;
        L->front = createNewNode;
    }

    // update length
    L->length++;
    
    // if the cursor is defined, update its index accordingly
    if( L->index != -1 ) {
        L->index++;
    }
}

// append()
// Insert new element into L
// If L is non-empty, insertion takes place after back element
void append(List L, int x) {
    // error message
    if( L==NULL ) {
        printf("List Error: calling append() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    // create a new node with the given data
    Node createNewNode = newNode(x);

    // if the list is empty, the new node becomes both front and back
    if( L->back == NULL ) {
        L->front = createNewNode;
        L->back = createNewNode;
    }
    // else, insert the new node after the current back
    else {
        createNewNode->previous = L->back;
        L->back->next = createNewNode;
        L->back = createNewNode;
    }

    // update length
    L->length++;
}

// insertBefore()
// Insert new element before cursor
// Pre: length() > 0, index() >= 0
void insertBefore(List L, int x) {
    // error messages
    if( L==NULL ) {
        printf("List Error: calling insertBefore() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if( L->length <= 0 ) {
        printf("List Error: calling insertBefore() on an empty List\n");
        exit(EXIT_FAILURE);
    }

    if( index(L) < 0 ) {
        printf("List Error: calling insertBefore() with an undefined cursor\n");
    }

    // create a new node with the given data
    Node createNewNode = newNode(x);

    // if the cursor is at the front, the new node becomes the new front
    if( index(L) == 0 ) {
        createNewNode->next = L->front;
        L->front->previous = createNewNode;
        L->front = createNewNode;
    }
    // else, insert the new node before the current cursor node
    else {
        createNewNode->next = L->cursor;
        createNewNode->previous = L->cursor->previous;
        L->cursor->previous->next = createNewNode;
        L->cursor->previous = createNewNode;
    }

    L->length++;
    // increment the index to reflect the cursor's new position
    L->index++;
}

// insertAfter()
// Insert new element after cursor
// Pre: length() > 0, index() >= 0
void insertAfter(List L, int x) {
    // error messages
    if( L==NULL ) {
        printf("List Error: calling insertAfter() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if( L->length <= 0 ) {
        printf("List Error: calling insertAfter() on an empty List\n");
    }

    if( index(L) < 0 ) {
        printf("List Error: calling insertAfter with an undefined cursor\n");
    }

    // create a new node with the given data
    Node createNewNode = newNode(x);

    // if the cursor is at the back, the new node becomes the new back
    if( index(L) == L->length - 1 ) {
        createNewNode->previous = L->back;
        L->back->next = createNewNode;
        L->back = createNewNode;
    }
    // else, insert the new node after the current cursor node
    else {
        createNewNode->previous = L->cursor;
        createNewNode->next = L->cursor->next;
        L->cursor->next->previous = createNewNode;
        L->cursor->next = createNewNode;
    }

    // update length
    L->length++;
}

// deleteFront()
// Delete the front element
// Pre: length() > 0
void deleteFront(List L) {
    // error messages
    if( L==NULL ) {
        printf("List Error: calling deleteFront() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if( L->length <= 0 ) {
        printf("List Error: calling deleteFront() on an empty List\n");
        exit(EXIT_FAILURE);
    }

    // save the front node to be deleted
    Node temp = L->front;

    // if the list has only one element, set front and back to NULL
    if( L->length == 1 ) {
        L->front = NULL;
        L->back = NULL;
        L->cursor = NULL;
        L->index = -1;
    }
    // else, update front and its previous node
    else {
        L->front = L->front->next;
        L->front->previous = NULL;
        if( index(L) == 0 ) {
            L->cursor = NULL;           // cursor becomes undefined if it was at the front
            L->index = -1;
        }
        else {
            L->index--;                 // decrement the index if cursor was ahead of the deleted element
        }
    }

    // free the memory of the deleted node
    freeNode(&temp);
    // update length
    L->length--;
}

// deleteBack()
// Delete the back element
// Pre: length() > 0
void deleteBack(List L) {
    if( L==NULL ) {
        printf("List Error: calling deleteBack() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if( L->length <= 0 ) {
        printf("List Error: calling deleteBack() on an empty List\n");
        exit(EXIT_FAILURE);
    }

    // save the back node to be deleted
    Node temp = L->back;

    // if the list has only one element, set front and back to NULL
    if( L->length == 1 ) {
        L->front = NULL;
        L->back = NULL;
        L->cursor = NULL;
        L->index = -1;
    }
    // else, update back and its next node
    else {
        L->back = L->back->previous;
        L->back->next = NULL;
        if( index(L) == L->length - 1 ) {
            L->cursor = NULL;           // cursor becomes undefined if it was at the back
            L->index = -1;
        }
    }

    // free the memory of the deleted node
    freeNode(&temp);
    // update length
    L->length--;
}

// delete()
// Delete cursor element, making cursor undefined
// Pre: length() > 0, index() >= 0
void delete(List L) {
    // error messages
    if( L==NULL ) {
        printf("List Error: calling delete() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if( L->length <= 0 ) {
        printf("List Error: calling delete() on an empty List\n");
        exit(EXIT_FAILURE);
    }

    if( index(L) < 0 ) {
        printf("List Error: calling delete() with an undefined cursor\n");
        exit(EXIT_FAILURE);
    }

    // save the cursor node to be deleted
    Node temp = L->cursor;

    // if list has only one element, set front and back to NULL
    if( L->length == 1 ) {
        L->front = NULL;
        L->back = NULL;
        L->cursor = NULL;
        L->index = -1;
    }
    // if cursor is at the front, update front and its next node
    else if( index(L) == 0 ) {
        L->front = L->front->next;
        L->front->previous = NULL;
        L->cursor = NULL;
        L->index = -1;
    }
    // if cursor is at the back, update back and its previous node
    else if( index(L) == L->length - 1 ) {
        L->back = L->back->previous;
        L->back->next = NULL;
        L->cursor = NULL;
        L->index = -1;
    }
    // else, update the previous and next nodes of cursor
    else {
        L->cursor->previous->next = L->cursor->next;
        L->cursor->next->previous = L->cursor->previous;
        L->cursor = NULL;
        L->index = -1;
    }

    // free the memory of the deleted node
    freeNode(&temp);
    // update length
    L->length--;
}


// Other operations ----------------------------------------------------------------------------------------------------------------------

// printList()
// Prints to the file pointed to by out, a string representation of L consisting of a space separated sequence of integers, with front on left
void printList(FILE* out, List L) {
    Node N = NULL;

    // error message
    if( L==NULL ) {
        printf("List Error: calling printList() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    // modify to accept an output file pointer
    for (N = L->front; N != NULL; N = N->next) {
        fprintf(out, "%d ", N->data);               // print to the provided file pointer
    }
    fprintf(out, "\n");

    /*
    for(N = L->front; N != NULL; N = N->next) {
        printf(FORMAT" ", N->data);
    }
    printf("\n");
    */
}

// copyList()
// Returns a new List representing the same integer sequence as L
// The cursor in the new list is undefined, regardless of the state of the cursor in L
// The state of L is unchanged
List copyList(List L) {
    if( L==NULL ) {
        printf("List Error: calling copyList on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    // create a new empty list to copy elements into
    List copiedList = newList();
    // start at the front of the original list
    Node curr = L->front;

    // append each element from the original list to the copied list
    while( curr!=NULL ) {
        append(copiedList, curr->data);
        curr = curr->next;
    }

    return(copiedList);
}