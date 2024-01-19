/*
*  PA1 Lex.c
*  Author: Mia Santos (miesanto)
*  Date: 10/9/2023
*  CSE 101 Fall 2023, Tantalo
*/

#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include "List.h"

#define MAX_LEN 300

int main(int argc, char *argv[]) {

    int line_count = 0;     // initialize line_count to zero to fix "conditional jump error"
    char c;
    FILE *in, *out;
    char line[MAX_LEN];

    // check that there are two command line arguments (other than the program name Lex)
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

    // count the number of lines in the input file
    bool end_of_sentence = false;
    for(c = getc(in); c != EOF; c = getc(in)) {
        // remember: not c=="n" because "" is a string
        if( c=='\n' ) {
            line_count++;
            end_of_sentence = true;
        }
        else {
            end_of_sentence = false;
        }
    }

    if( end_of_sentence==false ) {
        line_count++;
    }

    // go back to start of file
    fseek(in, 0, SEEK_SET);

    // allocate memory for an array of strings

    // declare a pointer to a pointer to 'char' (lines_array)
    // malloc(...) allocates a block of memory on the heap of the size of our array of pointers
    // assigns the address of the allocated memory to lines_array variable
    char **lines_array = (char **)malloc(line_count * sizeof(char *));
    if( lines_array==NULL ) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // create a string array of length and read in the lines of the file as strings, placing them into the array
    // allocate this array from heap memory using calloc() or malloc()
    // note: do not use a variable length array
    int i = 0;
    while(fgets(line, sizeof(line), in) != NULL) {
        lines_array[i] = strdup(line);                          // allocate memory for the line and copy
        if( lines_array[i]==NULL ) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        i++;
    }

    // create a List whose elements are the indices of the above string array
    // these indices should be arranged in an order that indirectly sorts the array
    // begin with an initially empty List, then insert the indices of the array one by one into the appropriate positions of the List
    List index_list = newList();
    append(index_list, 0);                                      // insert index of the first line

    for(i = 1; i < line_count; i++) {                           // iterate through the remaining lines in the string array
        moveFront(index_list);                                  // move the cursor to the front of the list
        char *current_line = lines_array[i];

        // compare the current line w/ lines at the indices in the list
        // strcmp(s1, s2)<0 is true if and only if s1 comes before s2
        // strcmp(s1, s2)>0 is true if and only if s1 comes after s2
        // strcmp(s1, s2)==0 is true if and only if s1 is identical to s2
        while(true) {
            int current_index = get(index_list);
            char *compare_line = lines_array[current_index];

            // 
            if( strcmp(current_line, compare_line) < 0 ) {
                insertBefore(index_list, i);
                break;
            }
            else {
                if( (index(index_list) + 1) == length(index_list)) {
                    insertAfter(index_list, i);
                    break;
                }
                else {
                    moveNext(index_list);
                }
            }
        }

        /* SCRAPPED
        while(index(index_list) >= 0) {
            int current_index = get(index_list);
            char *compare_line = lines_array[current_index];

            // check if the current_line is <= the compare_line lexicographically
            if( strcmp(current_line, compare_line) <= 0 ) {
                insertBefore(index_list, i);                    // insert the index before the cursor
                break;                                          // exit loop (we've inserted the index)
            }

            moveNext(index_list);                               // move to next index
        }

        // if we reach the end of the list, insert the index at the back
        if( index(index_list)==-1 ) {
            append(index_list, i);
        }
        */
    }

    // print the array in alphabetical order to the output file
    moveFront(index_list);
    while(index(index_list) >= 0) {
        int current_index = get(index_list);
        fprintf(out, "%s", lines_array[current_index]);
        moveNext(index_list);
    }

    // free allocated memory
    freeList(&index_list);
    for(i = 0; i < line_count; i++) {
        free(lines_array[i]);
    }
    free(lines_array);
    
    // close files 
    fclose(in);
    fclose(out);

    return(0);
}