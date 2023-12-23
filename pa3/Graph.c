/*
*   PA3 Graph.c
*   Implementation file for Graph ADT
*   Author: Mia Santos (miesanto)
*   Date: 10/25/2023
*   CSE 101 Fall 2023, Tantalo
*/

#include "Graph.h"
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>

/*** Exported Types ***/
typedef struct GraphObj {
    List* adjacency;            // an array of Lists (adjacency lists)
    int* color;                 // an array of ints whose ith element is the color (white, gray, black) of vertex i
    int* parent;                // an array of ints whose ith element is the parent of vertex i
    int* discover;              // an array of ints whose ith element is the discover time
    int* finish;                // an array of ints whose ith element is the finish time

    int order;                  // number of vertices
    int size;                   // number of edges
} GraphObj;

/*** Constructors-Destructors ***/

// newGraph()
// returns a Graph pointing to a newly created GraphObj
// GraphObj represents a graph with n vertices and no edges
Graph newGraph(int n) {
    Graph G;
    G = malloc(sizeof(GraphObj));                       // allocate memory
    assert( G!=NULL );                                  // confirm that allocation was successful

    // allocate memory for the arrays

    // note to self: I shouldn't cast the result as "void * is automatically and safely promoted to any other pointer type"
    // ^ taken from https://stackoverflow.com/questions/605845/should-i-cast-the-result-of-malloc
    G->adjacency = malloc((n+1) * sizeof(List));
    G->color = malloc((n+1) * sizeof(int));
    G->parent = malloc((n+1) * sizeof(int));
    G->discover = malloc((n+1) * sizeof(int));
    G->finish = malloc((n+1) * sizeof(int));

    // initialize the Graph fields
    G->order = n;
    G->size = 0;

    // initialize the arrays and lists
    for(int i = 1; i <= n; i++) {
        G->adjacency[i] = newList();                    // create new empty adjacency list
        G->color[i] = WHITE;
        G->parent[i] = NIL;
        G->discover[i] = UNDEF;
        G->finish[i] = UNDEF;
    }

    return(G);
}

// freeGraph()
// frees all heap memory associated with the Graph *pG
// then sets *pG to NULL
void freeGraph(Graph* pG) {

    // check if the graph has been allocated and not already freed (set to NULL)
    if( pG!=NULL && *pG!=NULL ) {
        Graph G = *pG;

        // free the adjacency lists (neighbors) of each vertex (from 1 to the order of the graph)
        for(int i = 1; i <= G->order; i++) {
            freeList(&(G->adjacency[i]));
        }

        // free the arrays
        free(G->adjacency);
        free(G->color);
        free(G->parent);
        free(G->discover);
        free(G->finish);

        // free Graph
        free(G);
        *pG = NULL;
    }
}


/*** Access functions ***/

// getOrder() and getSize() return the corresponding field values

// getOrder()
int getOrder(Graph G) {
    // error message
    if( G==NULL ) {
        printf("Graph Error: calling getOrder() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }

    return G->order;
}

// getSize()
int getSize(Graph G) {
    // error message
    if( G==NULL ) {
        printf("Graph Error: calling getSize() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }

    return G->size;
}

// getParent()
// return the parent of vertex u
// may be NIL
// pre: 1<=u<=n=getOrder(G)
int getParent(Graph G, int u) {
    // error messages
    if( G==NULL ) {
        printf("Graph Error: calling getParent() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }

    if( u < 1 || u > G->order ) {
        printf("Graph Error: calling getParent() with an invalid vertex\n");
        exit(EXIT_FAILURE);
    }
    
    return G->parent[u];
}

// getDiscover()
// pre: 1<=u<=n=getOrder(G)
int getDiscover(Graph G, int u) {
    // error messages
    if( G==NULL ) {
        printf("Graph Error: calling getDiscover() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }

    if( u < 1 || u > G->order ) {
        printf("Graph Error: calling getDiscover() with an invalid vertex\n");
        exit(EXIT_FAILURE);
    }

    return G->discover[u];
}

// getFinish()
// pre: 1<=u<=n=getOrder(G)
int getFinish(Graph G, int u) {
    // error messages
    if( G==NULL ) {
        printf("Graph Error: calling getFinish() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }

    if( u < 1 || u > G->order ) {
        printf("Graph Error: calling getFinish() with an invalid vertex\n");
        exit(EXIT_FAILURE);
    }

    return G->finish[u];
}


/*** Manipulation procedures ***/

// addArc()
// inserts a new directed edge from u to v
// i.e., v is added to u's adjacency List (BUT NOT VICE VERSA)
// pre: 1<=u<=getOrder(G) and 1<=v<=getOrder(G)
void addArc(Graph G, int u, int v) {
    // error messages
    if( G==NULL ) {
        printf("Graph Error: calling addArc() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if( u < 1 || u > getOrder(G) || v < 1 || v > getOrder(G) ) {
        printf("Graph Error: calling addArc() with invalid vertex indices\n");
        exit(EXIT_FAILURE);
    }

    // add v to u's adjacency list (directed edge)
    List u_adj = G->adjacency[u];

    // if u's adjacency list is empty or v is smaller than the first element
    if( length(u_adj)==0 || v < front(u_adj) ) {
        prepend(u_adj, v);
    }

    // else, traverse the list
    else {
        moveFront(u_adj);
        while(index(u_adj) >= 0) {
            int x = get(u_adj);

            // if v is smaller than the current element, insert v before it
            if( v < x ) {
                insertBefore(u_adj, v);
                break;
            }

            moveNext(u_adj);
        }

        // if we reach the end of the list, append v
        if( index(u_adj) == -1 ) {
            append(u_adj, v);
        }
    }

    // increment the number of edges in the graph
    G->size++;    
}

// addEdge()
// inserts a new edge joining u to v
// i.e., u is added to v's adjacency List, and v to u's adjacency List
// !!! NOTE !!! "your program is requried to maintain these lists in sorted order by increasing labels"
// pre: 1<=u<=getOrder(G) and 1<=v<=getOrder(G)
void addEdge(Graph G, int u, int v) {
    // error messages
    if( G==NULL ) {
        printf("Graph Error: calling addEdge() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }

    if( u < 1 || u > getOrder(G) || v < 1 || v > getOrder(G) ) {
        printf("Graph Error: calling addEdge() with invalid vertex indices\n");
        exit(EXIT_FAILURE);
    }

    // add u to v's adjacency list and vice versa while maintaining the sorted order
    List v_adj = G->adjacency[v];

    // if v's adjacency list is empty or u is smaller than the first element
    if( length(v_adj)==0 || u < front(v_adj) ) {
        prepend(v_adj, u);
    }

    // else, traverse the list
    else {
        moveFront(v_adj);
        while(index(v_adj) >= 0) {
            int w = get(v_adj);

            // if u is smaller than the current element, insert u before it
            if( u < w ) {
                insertBefore(v_adj, u);
                break;
            }

            moveNext(v_adj);
        }

        // if we reach the end of the list, append u
        if( index(v_adj) == -1 ) {
            append(v_adj, u);
        }
    }

    // add v to u's adjacency list and vice versa while maintaining the sorted order
    List u_adj = G->adjacency[u];

    // similar process as above
    if( length(u_adj)==0 || v < front(u_adj) ) {
        prepend(u_adj, v);
    }
    else {
        moveFront(u_adj);
        while(index(u_adj) >= 0) {
            int x = get(u_adj);
            if( v < x ) {
                insertBefore(u_adj, v);
                break;
            }

            moveNext(u_adj);
        }

        if( index(u_adj) == -1 ) {
            append(u_adj, v);
        }
    }

    // increment the number of edges in the graph
    G->size++;
}

// Visit()
// private helper function
/* PSEUDOCODE
Visit(x)
   d[x] = (++time)          // discover x
   color[x] = gray            
   for all y in adj[x]
      if color[y] == white 
         p[y] = x
         Visit(y)
   color[x] = black 
   f[x] = (++time)          // finish x
*/
void Visit(Graph G, List S, int x, int* time) {
    G->discover[x] = ++(*time);                     // discover x
    G->color[x] = GRAY;                             // mark x as gray
    List adj_list = G->adjacency[x];                // get the adjacency list of x
    
    for(moveFront(adj_list); index(adj_list) >= 0; moveNext(adj_list)) {
        int y = get(adj_list);                      // get the next adjacent vertex
        
        // if unvisited, set the parent of y to x
        if( G->color[y] == WHITE ) {
            G->parent[y] = x;
            Visit(G, S, y, time);                   // recursively visit y
        }
    }

    G->color[x] = BLACK;
    G->finish[x] = ++(*time);                       // finish x and increment time
    prepend(S, x);                                  // stack
}

// DFS()
// runs the DFS algorithm on the Graph G with List S
// S has two purposes:
// 1. defines the order in which vertices are to be processed in the main loop of DFS
// 2. when DFS is complete, it will store the vertices by decreasing finish times
// hence, S is considered to be a stack
// pre: length(S) == n
// pre: S contains some permutations of the integers {1,2,...,n} where n = getOrder(G)
/* PSEUDOCODE
DFS(G)
   for all x in V(G)
      color[x] = white 
      p[x] = nil 
   time = 0
   for all x in V(G)  // main loop of DFS
      if color[x] == white 
         Visit(x)
*/
void DFS(Graph G, List S) {
    // error message
    if( length(S)!=getOrder(G) ) {
        printf("Graph Error: calling DFS() on invalid list length\n");
        exit(EXIT_FAILURE);
    }

    // initialize vertices
    for(int x = 1; x <= getOrder(G); x++) {
        G->color[x] = WHITE;                        // mark all vertices as unvisited
        G->parent[x] = NIL;                         // set parents of all vertices to NIL
        /*
        G->discover[x] = UNDEF;                     // set discover time to UNDEF
        G->finish[x] = UNDEF;                       // set finish time to UNDEF
        */
    }

    // initialize time
    int time = 0;

    // main loop of DFS
    for(moveFront(S); index(S) >= 0; moveNext(S)) {
        int x = get(S);
        // if unvisited, perform Visit()
        if( G->color[x] == WHITE ) {
            Visit(G, S, x, &time);
        }
    }

    // delete elements from the back of S such that all is left is the reverse order
    for(int i = 0; i < getOrder(G); i++) {
        deleteBack(S);
    }
}


/*** Other operations ***/

// transpose()
// returns a reference to a new graph object representing the transpose of G
Graph transpose(Graph G) {
    // error message
    if( G==NULL ) {
        printf("Graph Error: calling Transpose() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }

    int n = getOrder(G);
    Graph T = newGraph(n);                          // new graph with the same number of vertices

    for(int u = 1; u <= n; u++) {
        List adj_list = G->adjacency[u];            // get the list adjacency of u
        moveFront(adj_list);

        while(index(adj_list) >= 0) {
            int v = get(adj_list);                  // get the next adjacent vertex
            addArc(T, v, u);                        // reverse the direction of the edge, i.e., (u,v) becomes (v,u)
            moveNext(adj_list);
        }
    }

    // return the transposed graph
    return T;
}

// copyGraph()
// returns a reference to a new graph that is a copy of G
Graph copyGraph(Graph G) {
    // error message
    if( G==NULL ) {
        printf("Graph Error: calling copyGraph() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }

    int n = getOrder(G);
    Graph my_copy = newGraph(n);

    for(int u = 1; u <= n; u++) {
        List adj_list = G->adjacency[u];
        moveFront(adj_list);

        while(index(adj_list) >= 0) {
            int v = get(adj_list);
            addArc(my_copy, u, v);
            moveNext(adj_list);
        }
    }

    return(my_copy);
}

// printGraph() prints the adjacency list representation of G to the file pointed to by out
void printGraph(FILE* out, Graph G) {
    if( G==NULL ) {
        // error message
        printf("Graph Error: calling printGraph() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 1; i <= getOrder(G); i++) {
        fprintf(out, "%d:", i);

        // iterate through adjacency list
        List list_to_print = G->adjacency[i];

        for(moveFront(list_to_print); index(list_to_print) >= 0; moveNext(list_to_print)) {
            int adj = get(list_to_print);
            fprintf(out, " %d", adj);
        }

        fprintf(out, "\n");
    }
}