/*
*  PA4 List.h
*  Header file for List ADT
*  Author: Mia Santos (miesanto)
*  Date: 11/1/2023
*  CSE 101 Fall 2023, Tantalo
*/

#ifndef LIST_H_INCLUDE_
#define LIST_H_INCLUDE_
#include<stdbool.h>
#include<stdio.h>

#define FORMAT "%p"         // format string for ListElement


// Exported types ------------------------------------------------------------------------------------------------------------------------
typedef void* ListElement;
typedef struct ListObj* List;


// Constructors-Destructors --------------------------------------------------------------------------------------------------------------

// newList()
// Creates and returns a new empty List
List newList(void);

// freeList()
// Frees all heap memory assoc. with *pL, sets *pL to NULL
void freeList(List* pL);

// Access functions ----------------------------------------------------------------------------------------------------------------------

// length()
// Returns the number of elements in L
int length(List L);

// index()
// Returns index of cursor element if defined, -1 otherwise
int index(List L);

// front()
// Returns front element of L
// Pre: length() > 0
void* front(List L);

// back()
// Returns back element of L
// Pre: length() > 0
void* back(List L);

// get()
// Returns cursor element of L
// Pre: length() > 0, index() >= 0
void* get(List L);


// Manipulation procedures ---------------------------------------------------------------------------------------------------------------

// clear()
// Resets L to its original empty state
void clear(List L);

// set()
// Overwrites the cursor element's data with x
// Pre: length() > 0, index() >= 0
void set(List L, void* x);

// moveFront()
// If L is non-empty, sets cursor under the front element
// Otherwise, does nothing
void moveFront(List L);

// moveBack()
// If L is non-empty, sets cursor under the back element
// Otherwise, does nothing
void moveBack(List L);

// movePrev()
// If cursor is defined and not at front, move cursor one step toward the front of L
// ^ If cursor is defined and at front, cursor becomes undefined
// ^^ If cursor is undefined, do nothing
void movePrev(List L);

// moveNext()
// If cursor is defined and not at back, move cursor one step toward the back of L
// ^ If cursor is defined and at back, cursor becomes undefined
// ^^ If cursor is undefined, do nothing
void moveNext (List L);

// prepend()
// Insert new elemnt into L
// If L is non-empty, insertion takes place before front element
void prepend(List L, void* x);

// append()
// Insert new element into L
// If L is non-empty, insertion takes place after back element
void append(List L, void* x);

// insertBefore()
// Insert new element before cursor
// Pre: length() > 0, index() >= 0
void insertBefore(List L, void* x);

// insertAfter()
// Insert new element after cursor
// Pre: length() > 0, index() >= 0
void insertAfter(List L, void* x);

// deleteFront()
// Delete the front element
// Pre: length() > 0
void deleteFront(List L);

// deleteBack()
// Delete the back element
// Pre: length() > 0
void deleteBack(List L);

// delete()
// Delete cursor element, making cursor undefined
// Pre: length() > 0, index() >= 0
void delete(List L);

#endif