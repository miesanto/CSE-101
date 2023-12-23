/*
*   PA2 FindPath.c
*   Author: Mia Santos (miesanto)
*   Date: 10/17/2023
*   CSE 101 Fall 2023, Tantalo
*/

#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include "Graph.h"

int main(int argc, char *argv[]) {
    // ----------------------------------------------------------------------------------------------------------------------------------
    // step 1: read and store the graph and print out its adjacency list representation
    // ----------------------------------------------------------------------------------------------------------------------------------
    FILE *in, *out;

    // check that there are two command line arguments (other than the program name FindPath)
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

    // read an int value from in and store it in variable n
    int n, u, v;
    fscanf(in, "%d", &n);

    // create a new graph
    Graph my_graph = newGraph(n);

    // read and add edges to the graph
    while(fscanf(in, "%d %d", &u, &v)==2 && u!=0 && v!=0 ) {
        addEdge(my_graph, u, v);
    }

    // print adjacency list to the output file
    printGraph(out, my_graph);

    // ----------------------------------------------------------------------------------------------------------------------------------
    // step 2: enter a loop that processes the second part of the input
    // each iteration of the loop should read in one pair of vertices (source, destination) and print the distance and resulting shortest path
    // ----------------------------------------------------------------------------------------------------------------------------------

    int source_vertex, dest_vertex;

    // blank line after adjacency list representation
    fprintf(out, "\n");
    
    // while two ints are successfully read and neither of them are 0
    while(fscanf(in, "%d %d", &source_vertex, &dest_vertex)==2 && source_vertex!=0 && dest_vertex!=0) {
        BFS(my_graph, source_vertex);

        // get distance from source to destination
        int distance = getDist(my_graph, dest_vertex);

        // check if distance is INF (infinity), meaning there is no path
        if( distance==INF ) {
            fprintf(out, "The distance from %d to %d is infinity\n", source_vertex, dest_vertex);
            fprintf(out, "No %d-%d path exists\n\n", source_vertex, dest_vertex);
        }
        else {
            // else, print the distance
            fprintf(out, "The distance from %d to %d is %d\n", source_vertex, dest_vertex, distance);

            // store the shortest path from source to destination
            List shortest_path = newList();
            getPath(shortest_path, my_graph, dest_vertex);

            // print the shortest path
            fprintf(out, "A shortest %d-%d path is: ", source_vertex, dest_vertex);
            printList(out, shortest_path);
            fprintf(out, "\n");

            // free memory
            freeList(&shortest_path);
        }
    }

    // close files
    fclose(in);
    fclose(out);
    freeGraph(&my_graph);

    return 0;
}