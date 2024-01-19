/*
*  PA4 Matrix.c
*  Implementation file for Matrix ADT
*  Author: Mia Santos (miesanto)
*  Date: 11/1/2023
*  CSE 101 Fall 2023, Tantalo
*/

#include "Matrix.h"
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>

// Structs -------------------------------------------------------------------------------------------------------------------------------

// private EntryObj type
typedef struct EntryObj {
    int column;                             // column index
    double value;                           // column value
} EntryObj;

// private Entry type (pointer to EntryObj struct)
typedef EntryObj* Entry;

// private MatrixObj type
typedef struct MatrixObj {
    int size;                                // number of rows and columns
    // int nnz;                              // number of non-zero entries
    List* rows;                              // an array of lists
} MatrixObj;


// Constructors-Destructors --------------------------------------------------------------------------------------------------------------

// newEntry()
// returns a reference to a new Entry object
Entry newEntry(int col, double val) {
    Entry E = malloc(sizeof(EntryObj));
    if( E!=NULL ) {
        E->column = col;
        E->value = val;
    }

    return E;
}

// freeEntry()
// frees heap memory associated with *pE, sets *pE to NULL
void freeEntry(Entry* pE) {
    // check if the entry has been allocated and not already freed (set to NULL)
    if( pE!=NULL && *pE!=NULL ) {
        // free entry
        free(*pE);
        *pE = NULL;
    }
}

// newMatrix()
// returns a reference to a new nXn Matrix object in the zero state
Matrix newMatrix(int n) {
    Matrix M;
    M = malloc(sizeof(MatrixObj));          // allocate memory for the MatrixObj
    assert( M!=NULL );                      // confirm that allocation was successful

    // initialize the fields of MatrixObj
    M->size = n;                            // set size to n
    // M->nnz = 0;                          // set nnz to 0
    M->rows = malloc((n+1) * sizeof(List)); // allocate memory for an array of lists of rows

    // memory allocation error message
    if( M->rows==NULL ) {
        printf("Matrix Error: Memory allocation failed for row lists\n");
        exit(EXIT_FAILURE);
    }

    // for each row, create an empty list
    for(int i = 0; i <= n; i++) {
        M->rows[i] = newList();
    }

    return M;
}

// freeMatrix()
// frees heap memory associated with *pM, sets *pM to NULL
void freeMatrix(Matrix* pM) {
    
    // check if the matrix has been allocated and not already freed (set to NULL)
    if( pM!=NULL && *pM!=NULL ) {

        // free the memory for each list in the rows array
        for(int i = 0; i <= (*pM)->size; i++) {
            List R = (*pM)->rows[i];
            moveFront(R);
            while( index(R)>= 0) {
                Entry entry = get(R);
                freeEntry(&entry);
                moveNext(R);
            }

            freeList(&((*pM)->rows[i]));

        }

        // free the array of lists
        free((*pM)->rows);

        // free Matrix
        free(*pM);
        *pM = NULL;
    }
}


// Access functions ----------------------------------------------------------------------------------------------------------------------

// size()
// return the size of square Matrix M
int size(Matrix M) {
    // error message
    if( M==NULL ) {
        printf("Matrix Error: calling size() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }

    return M->size;
}

// NNZ()
// return the number of non-zero elements in M
int NNZ(Matrix M) {
    // error message
    if( M==NULL ) {
        printf("Matrix Error: calling NNZ() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }

    int nnz = 0;
    
    for(int i = 0; i <= M->size; i++) {
        List R = M->rows[i];
        moveFront(R);

        while(index(R) != -1) {
            Entry entry = (Entry)get(R);
            if( entry->value!=0.0 ) {
                nnz++;
            }

            moveNext(R);
        }
    }

    return nnz;
}

// equals()
// return true (1) if matrices A and B are equal, false (0) otherwise
int equals(Matrix A, Matrix B) {

    // check if sizes match
    // if not, then they're not equal (false)
    if( A->size!=B->size ) {
        return 0;
    }

    for(int i = 0; i <= A->size; i++) {
        List R_A = A->rows[i];              // get the list representing row in A
        List R_B = B->rows[i];              // get the list representing row in B

        // check if number of entries match
        // if not, then they're not equal (false)
        if ( length(R_A)!=length(R_B) ) {
            return 0;
        }

        moveFront(R_A);
        moveFront(R_B);

        while(index(R_A) >= 0) {
            Entry entry_A = get(R_A);       // get entries from matrix A
            Entry entry_B = get(R_B);       // get entries from matrix B

            // if entries don't match, return false
            if( entry_A->column!=entry_B->column || entry_A->value!=entry_B->value ) {
                return 0;
            }

            moveNext(R_A);
            moveNext(R_B);
        }
    }

    // return true
    return 1;
}


// Manipulation procedures ---------------------------------------------------------------------------------------------------------------

// makeZero()
// re-sets M to the zero Matrix state
void makeZero(Matrix M) {
    for(int i = 0; i <= M->size; i++) {
        clear(M->rows[i]);                // get the list representing a row
    }
}

// changeEntry()
// changes the ith row, jth column of M to the value x
// pre: 1<=i<=size(M), 1<=j<=size(M)
void changeEntry(Matrix M, int i, int j, double x) {
    // error messages
    if( i < 1 || i > M->size ) {
        printf("Matrix Error: calling changeEntry() on an invalid row index\n");
        exit(EXIT_FAILURE);
    }

    if(  j < 1 || j > M->size ) {
        printf("Matrix Error: calling changeEntry() on an invalid column index\n");
        exit(EXIT_FAILURE);
    }

    // get the list of row i
    List R = M->rows[i - 1];

    // iterate to find the correct position
    for(moveFront(R); index(R) >= 0; moveNext(R)) {
        Entry entry = get(R);
        if( entry->column==j ) {
            entry->value = x;                           // update the value
            return;
        }
        else if( entry->column > j ) {
            Entry new_entry = newEntry(j, x);
            insertBefore(R, new_entry);
            return;
        }
    }

    if( x!=0 ) {
        Entry new_entry = newEntry(j, x);
        append(R, new_entry);                           // append a new non-zero entry
    }

}


// Matrix Arithmetic operations -----------------------------------------------------------------------------------------------------------

// copy()
// returns a reference to a new Matrix object having the same entries as A
Matrix copy(Matrix A) {
    Matrix B = newMatrix(A->size);                      // new matrix B the same size as A

    for(int i = 0; i <= A->size; i++) {
        List R_A = A->rows[i];
        // List R_B = B->rows[i];

        moveFront(R_A);

        while(index(R_A) >= 0) {
            Entry entry_A = get(R_A);

            changeEntry(B, i + 1, entry_A->column, entry_A->value);

            moveNext(R_A);
        }
    }

    // return the copy of A
    return B;
}

// transpose()
// returns a reference to a new Matrix object representing the transpose of A
Matrix transpose(Matrix A) {
    Matrix B = newMatrix(A->size);                      // new matrix B the same size as A

    for(int i = 0; i <= A->size; i++) {
        List R_A = A->rows[i];
        moveFront(R_A);

        while(index(R_A) >= 0) {
            Entry entry_A = get(R_A);

            // change the corresponding entry in B with its value transposed
            // note that the index became the row and vice versa
            changeEntry(B, entry_A->column, i + 1, entry_A->value);

            moveNext(R_A);
        }
    }

    // return the transpose of A
    return B;
}

// scalarMult()
// returns a reference to a new Matrix object representing xA
Matrix scalarMult(double x, Matrix A) {
    Matrix B = newMatrix(A->size);                      // new matrix B the same size as A

    for(int i = 0; i <= A->size; i++) {
        List R_A = A->rows[i];

        moveFront(R_A);

        while(index(R_A) >= 0) {
            Entry entry_A = get(R_A);
            double product = x * entry_A->value;

            // change the corresponding entry in B with the product
            changeEntry(B, i + 1, entry_A->column, product);
            moveNext(R_A);
        }
    }

    // return xA
    return B;
}

// sum()
// returns a reference to a new Matrix object representing A+B
// pre: size(A)==size(B)
Matrix sum(Matrix A, Matrix B) {
    // error message
    if( A->size!=B->size ) {
        printf("Matrix Error: calling sum() on matrices of two different sizes\n");
        exit(EXIT_FAILURE);
    }

    // create a new matrix of the same size as A and B
    Matrix C = newMatrix(A->size);

    // loop through each row of A and B
    for(int i = 0; i <= A->size; i++) {
        List R_A = A->rows[i];
        List R_B = B->rows[i];

        moveFront(R_A);
        moveFront(R_B);

        // continue as long as there are valid indices in R_A and R_B
        while(index(R_A) >= 0 && index(R_B) >= 0) {
            Entry entry_A = get(R_A);
            Entry entry_B = get(R_B);

            // check if the columns of the entries in A and B match
            if( entry_A->column==entry_B->column ) {
                double sum = entry_A->value + entry_B->value;                       // sum
                
                // if non-zero, update the corresponding entry in C
                if( sum!=0 ) {
                    changeEntry(C, i + 1, entry_A->column, sum);
                }

                moveNext(R_A);
                moveNext(R_B);
            }
            // if the column's don't match, update C with the non-zero entry
            else if(entry_A->column < entry_B->column) {
                if( entry_A->value!=0 ) {
                    changeEntry(C, i + 1, entry_A->column, entry_A->value);
                }

                moveNext(R_A);
            }
            else {
                if( entry_B->value!=0 ) {
                    changeEntry(C, i + 1, entry_B->column, entry_B->value);
                }
                
                moveNext(R_B);
            }
        }

        // process remaining entries in list R_A
        while(index(R_A) >= 0) {
            Entry entry_A = get(R_A);
            
            if( entry_A->value!=0 ) {
                changeEntry(C, i + 1, entry_A->column, entry_A->value);
            }

            moveNext(R_A);
        }

        // process remaining entries in list R_B
        while(index(R_B) >= 0) {
            Entry entry_B = get(R_B);
            
            if( entry_B->value!=0 ) {
                changeEntry(C, i + 1, entry_B->column, entry_B->value);
            }
            
            moveNext(R_B);
        }
    }

    // return the sum of A and B
    return C;
}

// diff()
// returns a reference to a new Matrix object representing A-B
// pre: size(A)==size(B)
Matrix diff(Matrix A, Matrix B) {
    // error message
    if( A->size!=B->size ) {
        printf("Matrix Error: calling diff() on matrices of two different sizes\n");
        exit(EXIT_FAILURE);
    }

    // create a new matrix of the same size as A and B
    Matrix C = newMatrix(A->size);

    // loop through each row of A and B
    for(int i = 0; i <= A->size; i++) {
        List R_A = A->rows[i];
        List R_B = B->rows[i];

        moveFront(R_A);
        moveFront(R_B);

        // continue as long as there are valid indices in R_A or R_B
        while(index(R_A) >= 0 && index(R_B) >= 0) {
            Entry entry_A = get(R_A);
            Entry entry_B = get(R_B);

            // check if the columns of the entries in A and B match
            if( entry_A->column==entry_B->column ) {
                double diff = entry_A->value - entry_B->value;                      // diff
                // if non-zero, update the corresponding entry in C
                if( diff!=0 ) {
                    changeEntry(C, i + 1, entry_A->column, diff);
                }
                
                moveNext(R_A);
                moveNext(R_B);
            }
            // if the column's don't match, update C with the non-zero entry
            else if(entry_A->column < entry_B->column) {
                if( entry_A->value!=0 ) {
                    changeEntry(C, i + 1, entry_A->column, entry_A->value);
                }

                moveNext(R_A);
            }
            else {
                if( entry_B->value!=0 ) {
                    changeEntry(C, i + 1, entry_B->column, -entry_B->value);
                }
                
                moveNext(R_B);
            }
        }

        // process remaining entries in list R_A
        while(index(R_A) >= 0) {
            Entry entry_A = get(R_A);
            
            if( entry_A->value!=0 ) {
                changeEntry(C, i + 1, entry_A->column, entry_A->value);
            }
            
            moveNext(R_A);
        }

        // process remaining entries in list R_B
        while(index(R_B) >= 0) {
            Entry entry_B = get(R_B);
            
            if( entry_B->value!=0 ) {
                changeEntry(C, i + 1, entry_B->column, -entry_B->value);
            }

            moveNext(R_B);
        }
    }

    // return the diff of A and B
    return C;
}

// vectorDot()
// private function
// computes the vector dot product of two matrix rows represented by Lists P and Q
double vectorDot(List P, List Q) {
    double dot_product = 0.0;

    moveFront(P);
    moveFront(Q);

    // continue as long as there are valid indices in P and Q
    while(index(P) >= 0 && index(Q) >= 0) {
        Entry entry_P = get(P);
        Entry entry_Q = get(Q);

        // check if the columns of the entries in P and Q match
        if( entry_P->column==entry_Q->column ) {
            dot_product += entry_P->value * entry_Q->value;                     // update the dot product
            moveNext(P);
            moveNext(Q);
        }
        // if the columns don't match and the column in P is smaller
        else if(entry_P->column < entry_Q->column) {
            moveNext(P);
        }
        // if the columns don't match and the column in Q is smaller
        else {
            moveNext(Q);
        }
    }

    return dot_product;
}

// product()
// returns a reference to a new Matrix object representing AB
// pre: size(A)==size(B)
Matrix product(Matrix A, Matrix B) {
    // error message
    if( A->size!=B->size ) {
        printf("Matrix Error: calling product() on matrices of two different sizes\n");
        exit(EXIT_FAILURE);
    }

    // create a new matrix C of the same size as matrices A and B
    Matrix C = newMatrix(A->size);

    // transpose matrix B, T_B
    Matrix T_B = transpose(B);

    // iterate through each row of A
    for(int i = 1; i <= A->size; i++) {
        // iterate through each column of A
        for(int j = 1; j <= A->size; j++) {
            // calculate the dot product between row i of A and column j of T_B
            double dot_product = vectorDot(A->rows[i - 1], T_B->rows[j - 1]);
            
            // if non-zero update corresponding entry in C with the dot product
            if( dot_product!=0.0 ) {
                changeEntry(C, i, j, dot_product);
            }
        }
    }

    freeMatrix(&T_B);

    // return the product of A and B
    return C;
}

/*
// sortEntries()
// private function
// sorts a list of entries by column in ascending order
void sortEntries(List L) {
    if (L == NULL) {
        printf("Matrix Error: calling sortEntries() on a NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    moveFront(L);
    moveNext(L);

    while(index(L) >= 0) {
        Entry entry = get(L);

        int current_col = entry->column;
        movePrev(L);

        while(index(L) >= 0) {
            Entry prev_entry = get(L);

            if(prev_entry->column <= current_col) {
                break;
            }

            entry->column = prev_entry->column;
            entry->value = prev_entry->value;
            prev_entry->column = current_col;
            prev_entry->value = entry->value;

            movePrev(L);
        }

        moveNext(L);
        moveNext(L);
    }
}
*/

// printMatrix()
// Prints a string representation of Matrix M to filestream out. Zero rows
// are not printed. Each non-zero row is represented as one line consisting
// of the row number, followed by a colon, a space, then a space separated
// list of pairs "(col, val)" giving the column numbers and non-zero values
// in that row. The double val will be rounded to 1 decimal point.
void printMatrix(FILE* out, Matrix M) {
    if( M==NULL ) {
        // error message
        printf("Matrix Error: calling printMatrix() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i <= M->size; i++) {
        if( length(M->rows[i]) > 0 ) {
            // sortEntries(M->rows[i]);
            fprintf(out, "%d: ", i + 1);

            for(moveFront(M->rows[i]); index(M->rows[i]) != -1; moveNext(M->rows[i])) {
                Entry entry = get(M->rows[i]);
                fprintf(out, "(%d, %.1f) ", entry->column, entry->value);
            }

            fprintf(out, "\n");
        }
    }
}