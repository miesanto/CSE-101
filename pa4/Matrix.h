/*
*  PA4 Matrix.h
*  Header file for Matrix ADT
*  Author: Mia Santos (miesanto)
*  Date: 11/1/2023
*  CSE 101 Fall 2023, Tantalo
*/

#ifndef MATRIX_H_INCLUDE_
#define MATRIX_H_INCLUDE_
#include "List.h"
#include<stdbool.h>
#include<stdio.h>

typedef struct MatrixObj* Matrix;

// newMatrix()
// returns a reference to a new nXn Matrix object in the zero state
Matrix newMatrix(int n);

// freeMatrix()
// frees heap memory associated with *pM, sets *pM to NULL
void freeMatrix(Matrix* pM);


// Access functions ----------------------------------------------------------------------------------------------------------------------

// size()
// return the size of square Matrix M
int size(Matrix M);

// NNZ()
// return the number of non-zero elements in M
int NNZ(Matrix M);

// equals()
// return true (1) if matrices A and B are equal, false (0) otherwise
int equals(Matrix A, Matrix B);


// Manipulation procedures ---------------------------------------------------------------------------------------------------------------

// makeZero()
// re-sets M to the zero Matrix state
void makeZero(Matrix M);

// changeEntry()
// changes the ith row, jth column of M to the value x
// pre: 1<=i<=size(M), 1<=j<=size(M)
void changeEntry(Matrix M, int i, int j, double x);


// Matrix Arithmetic operations -----------------------------------------------------------------------------------------------------------

// copy()
// returns a reference to a new Matrix object having the same entries as A
Matrix copy(Matrix A);

// transpose()
// returns a reference to a new Matrix object representing the transpose of A
Matrix transpose(Matrix A);

// scalarMult()
// returns a reference to a new Matrix object representing xA
Matrix scalarMult(double x, Matrix A);

// sum()
// returns a reference to a new Matrix object representing A+B
// pre: size(A)==size(B)
Matrix sum(Matrix A, Matrix B);

// diff()
// returns a reference to a new Matrix object representing A-B
// pre: size(A)==size(B)
Matrix diff(Matrix A, Matrix B);

// product()
// returns a reference to a new Matrix object representing AB
// pre: size(A)==size(B)
Matrix product(Matrix A, Matrix B);

// printMatrix()
// Prints a string representation of Matrix M to filestream out. Zero rows
// are not printed. Each non-zero row is represented as one line consisting
// of the row number, followed by a colon, a space, then a space separated
// list of pairs "(col, val)" giving the column numbers and non-zero values
// in that row. The double val will be rounded to 1 decimal point.
void printMatrix(FILE* out, Matrix M);

#endif