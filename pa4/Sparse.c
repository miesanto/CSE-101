/*
*  PA4 Sparse.c
*  Implementation file for Matrix ADT
*  Author: Mia Santos (miesanto)
*  Date: 11/4/2023
*  CSE 101 Fall 2023, Tantalo
*/

#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include "Matrix.h"

int main(int argc, char *argv[]) {
    FILE *in, *out;

    // check that there are two command line arguments (other than the program name FindComponents)
    // quit with a usage message to stderr if more than or less than two command line arguments are given
    if( argc!=3 ) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // open input file for reading
    in = fopen(argv[1], "r");
    if( in==NULL ) {
        fprintf(stderr, "Unable to open file %s for reading\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // open output file for writing
    out = fopen(argv[2], "w");
    if( out==NULL ) {
        fprintf(stderr, "Unable to open file %s for writing\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    // the input file will begin with a single line containing three integers n, a, and b separated by spaces
    // the second line will be blank, and the following 'a' lines will specify the non-zero entries of an nXn matrix A
    // each of these lines will contain a space separated list of three numbers: two integers and a double
    // ^giving the row, column, and value of the corresponding matrix entry
    // after another blank line, there will be 'b' lines specifying the non-zero entries of an nXn matrix B
    int n, a, b;
    fscanf(in, "%d %d %d", &n, &a, &b);

    Matrix A = newMatrix(n);
    Matrix B = newMatrix(n);

    // read and initialize A
    for(int i = 0; i < a; i++) {
        int row, col;
        double value;
        fscanf(in, "%d %d %lf", &row, &col, &value);
        changeEntry(A, row, col, value);
    }

    // read and initialize B
    for(int i = 0; i < b; i++) {
        int row, col;
        double value;
        fscanf(in, "%d %d %lf", &row, &col, &value);
        changeEntry(B, row, col, value);
    }

    fprintf(out, "A has %d non-zero entries:\n", NNZ(A));
    printMatrix(out, A);
    fprintf(out, "\n");

    fprintf(out, "B has %d non-zero entries:\n", NNZ(B));
    printMatrix(out, B);
    fprintf(out, "\n");

    Matrix res;

    // (1.5)*A
    res = scalarMult(1.5, A);
    fprintf(out, "(1.5)*A =\n");
    printMatrix(out, res);
    freeMatrix(&res);

    fprintf(out, "\n");

    // A + B
    res = sum(A, B);
    fprintf(out, "A+B =\n");
    printMatrix(out, res);
    freeMatrix(&res);

    fprintf(out, "\n");

    // A + A
    res = sum(A, A);
    fprintf(out, "A+A =\n");
    printMatrix(out, res);
    freeMatrix(&res);

    fprintf(out, "\n");

    // B - A
    res = diff(B, A);
    fprintf(out, "B-A =\n");
    printMatrix(out, res);
    freeMatrix(&res);

    fprintf(out, "\n");

    // A - A
    res = diff(A, A);
    fprintf(out, "A-A =\n");
    printMatrix(out, res);
    freeMatrix(&res);

    fprintf(out, "\n");

    // transpose(A)
    res = transpose(A);
    fprintf(out, "Transpose(A) =\n");
    printMatrix(out, res);
    freeMatrix(&res);

    fprintf(out, "\n");

    // A*B
    res = product(A, B);
    fprintf(out, "A*B =\n");
    printMatrix(out, res);
    freeMatrix(&res);

    fprintf(out, "\n");

    // B*B
    res = product(B, B);
    fprintf(out, "B*B =\n");
    printMatrix(out, res);
    freeMatrix(&res);

    // clean and close files
    freeMatrix(&A);
    freeMatrix(&B);
    fclose(in);
    fclose(out);

    return 0;
}