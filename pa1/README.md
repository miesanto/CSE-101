# Grade
100/100 🎉

# Files
**(1) List.h**

The List.h header file provides the **__`interface`__** for an abstract data type (ADT) called **__`'List'`__**. This ADT represents a dynamic list of integer elements with various operations for manipulation.

	EXPORTED TYPES:
	'ListElement' is an integer type, and 'List' is defined as a pointer to a structure named 'ListObj'.

	CONSTRUCTOR AND DESTRUCTOR FUNCTIONS:
	'newList' creates a new empty list, while 'freeList' deallocates memory associated with a List and sets the List pointer to NULL.

	ACCESS FUNCTIONS:
	'length', 'index', 'front', 'back', 'get', and 'equals'.

	MANIPULATION PROCEDURES:
	'clear', 'set', 'moveFront', 'moveBack', 'movePrev', 'moveNext', 'prepend', 'append', 'insertBefore', 'insertAfter', 'deleteFront', 'deleteBack', and 'delete'.

	OTHER OPERATIONS
	'printList', 'copyList'


**(2) List.c**

List.c is the **__`implementation`__** file for a List ADT. It includes functions and structures necessary for managing a doubly-linked list of integers. Notes about its contents:

	- 'NodeObj' and 'ListObj' are defined as private structs because their implementation details should not be exposed to users of the ADT.

	- 'newNode' creates a new node with the given data.
	- 'freeNode' frees the memory associated with a node.
	- 'newList' creates and returns a new empty list.
	- 'freeList' frees the memory associated with a list.

	- 'length' returns the number of elements in the list.
	- 'index' returns the index of the cursor element (or -1 if undefined).
	- 'front' returns the front element of the list.
	- 'back' returns the back element of the list.
	- 'get' returns the cursor element of the list.
	- 'equals' compares two lists to check if they are the same.

  	- 'clear' clears the list and resets it to an empty state.
  	- 'set' overwrites the cursor element's data with a new value.
  	- 'moveFront' moves the cursor to the front of the list.
  	- 'moveBack' moves the cursor to the back of the list.
  	- 'movePrev' moves the cursor one step toward the front (or makes it undefined).
  	- 'moveNext' moves the cursor one step toward the back (or makes it undefined).
  	- 'prepend' inserts a new element at the front of the list.
  	- 'append' inserts a new element at the back of the list.
  	- 'insertBefore' inserts a new element before the cursor.
  	- 'insertAfter' inserts a new element after the cursor.
  	- 'deleteFront' deletes the front element of the list.
  	- 'deleteBack' deletes the back element of the list.
  	- 'delete' deletes the cursor element (making the cursor undefined).

  	- 'printList' prints the elements of the list to a file in space-separated format.
  	- 'copyList' creates a new list that is a copy of the original list, with an undefined cursor.


**(3) Lex.c**

Lex.c is designed to **__`perform lexicographic sorting`__** of lines from an input text file and write the sorted lines to an output file. It uses the following procedures:

	COMMAND LINE ARGUMENT HANDLING:
	The program starts by checking if it has received exactly two command-line arguments (other than the program name itself).

	FILE HANDLING:
	The program opens the input file for reading and the output file for writing.
	
	LINE COUNTING:
	The program counts the number of lines in the input file by reading through it using 'fgets()' in a while loop.

	MEMORY ALLOCATION:
	After determining the number of lines, the program allocates memory for an array of strings named 'lines_array'. This array will hold the lines from the input file.

	LIST CREATION:
	The program creates a List data structure named 'index_list'. This list will store the indices of the lines in the 'lines_array' in a way that indirectly sorts the array. It starts as an empty list and iterates through the lines, inserting indices into the list based on lexicographical comparisons with the lines already in the list.

	SORTING:
	For each line in 'lines_array', the program compares it lexicographically with the lines corresponding to the indices stored in 'index_list'. It does this by moving through the list, comparing each line, and inserting the current index before the cursor position when it finds a lexicographically equal or smaller line. If it reaches the end of the list, it appends the index.

	OUTPUT:
	After sorting is complete, the program writes the lines from 'lines_array' to the output file in the order specified by 'index_list'.

-------

**SPECIAL NOTES TO PROF. AND GRADER:**

In List.h and List.c, I recieved the following warnings:
	
	In file included from List.c:13:
	List.h:40:5: warning: conflicting types for built-in function ‘index’; expected ‘char *(const char *, int)’ [-Wbuiltin-declaration-mismatch]
   		40 | int index(List L);
      		   |     ^~~~~
	
	In file included from ListTest.c:12:
	List.h:40:5: warning: conflicting types for built-in function ‘index’; expected ‘char *(const char *, int)’ [-Wbuiltin-declaration-mismatch]
   		40 | int index(List L);
      		   |     ^~~~~

This did not affect the functionality of my List.c, as my ListTest.c ran without errors. However, I believe that this is connected to my error in Lex.c:

	In file included from Lex.c:12:
	List.h:40:5: error: conflicting types for ‘index’; have ‘int(struct ListObj *)’
   		40 | int index(List L);
      	           |     ^~~~~
	
	In file included from /usr/include/string.h:462,
                 from Lex.c:11:
	/usr/include/strings.h:68:14: note: previous declaration of ‘index’ with type ‘char *(const char *, int)’
   		68 | extern char *index (const char *__s, int __c)
      	           |              ^~~~~


I do not know how to resolve this conflict without renaming the 'index' function in my List.h file, which we are not allowed to do. I am compiling on ubuntu with gcc and make.
	

**EDIT:** After debugging, I found that I didn't initialize **__`'int line_count = 0;'`__** for example, which lead to a 'Conditional jump or move depends on uninitialised value(s)' error. Now, **__`make checkLex`__** runs with 0 leaks and 0 errors!