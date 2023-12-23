/*
*   PA3 FindComponents.c
*   Author: Mia Santos (miesanto)
*   Date: 10/25/2023
*   CSE 101 Fall 2023, Tantalo
*
*   FindComponents.c will do the following:
*
*   Read the input file.
*   Assemble a graph object G using newGraph() and addArc().
*   Print the adjacency list representation of G to the output file.
*   Run DFS on G and G^T, processing the vertices in the second call by decreasing finish times from
the first call.
*   Determine the strong components of G.
*   Print the strong components of G to the output file in topologically sorted order.
*/

#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include "Graph.h"

int main(int argc, char *argv[]) {
    // ----------------------------------------------------------------------------------------------------------------------------------
    // step 1: read the input file
    // ----------------------------------------------------------------------------------------------------------------------------------
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

    // ----------------------------------------------------------------------------------------------------------------------------------
    // step 2: assemble a graph object G using newGraph() and addArc()
    // ----------------------------------------------------------------------------------------------------------------------------------

    // read an int value from in and store it in variable n
    int n = 0;
    fscanf(in, "%d", &n);

    // create a new graph
    Graph G = newGraph(n);
    int u = 0;
    int v = 0;

    // read and add arcs to the graph
    while(fscanf(in, "%d %d", &u, &v)==2 && u!=0 && v!=0 ) {
        addArc(G, u, v);
    }

    // ----------------------------------------------------------------------------------------------------------------------------------
    // step 3: print the adjacency list representation of G to the output file
    // ----------------------------------------------------------------------------------------------------------------------------------

    fprintf(out, "Adjacency list representation of G:\n");
    printGraph(out, G);

    // ----------------------------------------------------------------------------------------------------------------------------------
    // step 4: run DFS on G and G^T, processing the vertices in the second call by decreasing finish times from the first call
    // ----------------------------------------------------------------------------------------------------------------------------------

    List S = newList();

    // append each integer to S (ascending order)
    for(int x = 1; x <= getOrder(G); x++) {
        append(S, x);
    }

    DFS(G, S);

    // create a transposed graph G^T
    Graph T = transpose(G);

    // ----------------------------------------------------------------------------------------------------------------------------------
    // step 5: determine the strong components of G
    // ----------------------------------------------------------------------------------------------------------------------------------

    // run DFS on G^T
    DFS(T, S);

    // find the number of strongly connected components
    int scc_count = 0;                                        // counter for SCC's
    moveFront(S);

    int y = 1;
    while(y <= getOrder(T)) {
        int v = get(S);                                       // current element
        
        // if the parent is NIL, there is an SCC
        if( getParent(T, v)==NIL ) {
            scc_count++;                                      // increment number of SCC
        }
        
        moveNext(S);
        y++;
    }

    // ----------------------------------------------------------------------------------------------------------------------------------
    // step 6: print the strong components of G to the output file in topologically sorted order
    // ----------------------------------------------------------------------------------------------------------------------------------

    fprintf(out, "\n");
    fprintf(out, "G contains %d strongly connected components:\n", scc_count);

    // create a list to store SCCs
    List scc_list = newList();
    int scc_index = 1;

    // move through adj_list in reverse order
    for(moveBack(S); index(S) >= 0; movePrev(S)) {
        prepend(scc_list, get(S));

        // if the parent is NIL, there is an SCC
        if( getParent(T, get(S))==NIL ) {
            fprintf(out, "Component %d: ", scc_index++);
            moveFront(scc_list);

            // print each vertex in scc_list
            while(index(scc_list) >= 0) {
                fprintf(out, "%d ", get(scc_list));
                moveNext(scc_list);
            }

        clear(scc_list);
        fprintf(out, "\n");
        }
    }

    // clean and close files
    freeList(&S);
    freeList(&scc_list);
    freeGraph(&G);
    freeGraph(&T);
    fclose(in);
    fclose(out);

    return 0;
}