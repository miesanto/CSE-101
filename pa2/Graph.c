/*
*   PA2 Graph.c
*   Implementation file for Graph ADT
*   Author: Mia Santos (miesanto)
*   Date: 10/17/2023
*   CSE 101 Fall 2023, Tantalo
*/

#include "Graph.h"
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>

/*** Exported Types ***/
typedef struct GraphObj {
    List* neighbors;            // an array of Lists whose ith element contains the neighbors of vertex i
    int* color;                 // an array of ints whose ith element is the color (white, gray, black) of vertex i
    int* parent;                // an array of ints whose ith element is the parent of vertex i
    int* distance;              // an array of ints whose ith element is the distance from the most recent source to vertex i

    int order;                  // number of vertices
    int size;                   // number of edges
    int source;                 // label of the vertex tat was most recently used as source for BFS
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
    G->neighbors = malloc((n+1) * sizeof(List));
    G->color = malloc((n+1) * sizeof(int));
    G->parent = malloc((n+1) * sizeof(int));
    G->distance = malloc((n+1) * sizeof(int));

    // initialize the Graph fields
    G->order = n;
    G->size = 0;
    G->source = NIL;

    // initialize the arrays and lists
    for(int i = 1; i <= n; i++) {
        G->neighbors[i] = newList();                    // create new empty adjacency list
        G->color[i] = WHITE;
        G->parent[i] = NIL;
        G->distance[i] = INF;
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
            freeList(&(G->neighbors[i]));
        }

        // free the arrays
        free(G->neighbors);
        free(G->color);
        free(G->parent);
        free(G->distance);

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

// getSource()
// returns the source vertex most recently used in BFS()
// or NIL if BFS() has not yet been called
int getSource(Graph G) {
    // error message
    if( G==NULL ) {
        printf("Graph Error: calling getSource() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }

    return G->source;
}

// getParent()
// return the parent of vertex u in the bFS tree created by BFS()
// or NIL if BFS() has not yet been called
// pre: 1<=u<=getOrder(G)
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

// getDist()
// returns the distance from the most recent bFS source to vertex u
// or INF if BFS() has not yet been called
// pre: 1<=u<=getOrder(G)
int getDist(Graph G, int u) {
    // error messages
    if( G==NULL ) {
        printf("Graph Error: calling getDist() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }

    if( u < 1 || u > G->order ) {
        printf("Graph Error: calling getDist() with an invalid vertex\n");
        exit(EXIT_FAILURE);
    }

    return G->distance[u];
}

// getPath()
// appends to the List L the vertices of a shortest path in G from source to u
// or appends to L the value NIL if no such path exists
// pre: getSource(G)!=NIL (basically BFS() must be called first)
// pre: 1<=u<=getOrder(G)
void getPath(List L, Graph G, int u) {
    if( G==NULL ) {
        printf("Graph Error: calling getPath() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }

    if( u < 1 || u > G->order ) {
        printf("Graph Error: calling getPath() with an invalid vertex\n");
        exit(EXIT_FAILURE);
    }

    if( getSource(G)==NIL ) {
        printf("Graph Error: calling getPath() before BFS() is called\n");
        exit(EXIT_FAILURE);
    }

    // check if u is the same as the source vertex
    // if so, the shortest path from the source to itself is just u
    if( u==getSource(G) ) {
        append(L, u);
    }

    // check if the parent of vertex u is NIL
    // if so, there is no path from the source to u
    else if( G->parent[u]==NIL ) {
        append(L, NIL);
    }

    // else, there is a path from the source to u
    else {
        // recursive call to getPath()
        // repeatedly look for the path to the parent of u and then extending the path to include u itself
        // continues until source vertex = u

        // example:
        // source vertex S to vertex U
        // S -> A -> B -> C -> U

        // 1. getPath(L, G, U)
        // 2. check if U is the source vertex; if not, go to else
        // 3. call getPath(L, G, C)
        // 4. step 2
        // 5. call getPath(L, G, B)
        // 6. step 2
        // 7. call getPath(L, G, A)
        // 8. step 2
        // 9. call getPat(L, G, S)
        // 10. S is the source, so final path (appending vertices in reverse order) is S -> A -> B -> C -> U

        getPath(L, G, G->parent[u]);
        append(L, u);
    }
}


/*** Manipulation procedures ***/

// makeNull()
// deletes all edges of G, restoring it to its original (no edge) state
void makeNull(Graph G) {
    // error message
    if( G==NULL ) {
        printf("Graph Error: calling makeNull() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 1; i <= G->order; i++) {
        // clear adjacency lists for all vertices
        clear(G->neighbors[i]);
    }

    // reset the number of edges to 0
    G->size = 0;
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
    List v_adj = G->neighbors[v];

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
    List u_adj = G->neighbors[u];

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
    List u_adj = G->neighbors[u];

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

// BFS()
// runs the BFS algorithm on the Graph G with source s
// sets the color distance, parent, and source fields of G accordingly
void BFS(Graph G, int s) {
    // error messages
    if( G==NULL ) {
        printf("Graph Error: calling BFS() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if( s < 1 || s > getOrder(G) ) {
        printf("Graph Error: calling BFS() with an invalid source vertex\n");
        exit(EXIT_FAILURE);
    }

    // initialize all vertices
    for(int u = 1; u <= getOrder(G); u++) {
        G->color[u] = WHITE;                            // unvisited
        G->distance[u] = INF;                           // undefined distance
        G->parent[u] = NIL;                             // no parent
    }

    // set source vertex attributes
    G->source = s;                                      // set source vertex to s
    G->color[s] = GRAY;                                 // indicate s as starting point
    G->distance[s] = 0;                                 // since we start at s, there is no distance from itself

    // create a queue for BFS
    List Q = newList();                                 // keep track of vertices to visit in the BFS
    append(Q, s);                                       // append source vertex

    // perform BFS
    // loop continues until the queue is empty and the BFS visits all vertices reachable from source vertex
    while(length(Q) > 0) {
        
        // remove and retrieve the front vertex from the queue
        int u = front(Q);
        deleteFront(Q);

        // explore neighbors of u
        List adj_list = G->neighbors[u];

        for(moveFront(adj_list); index(adj_list) >= 0; moveNext(adj_list) ) {
            int v = get(G->neighbors[u]);
            // check if neighbor v is unvisited
            if( G->color[v] == WHITE ) {
                G->color[v] = GRAY;
                G->distance[v] = G->distance[u] + 1;    // update distance to one more than that of u
                G->parent[v] = u;                       // set u as v's parent
                append(Q, v);                           // append v to queue
            }
        }

        G->color[u] = BLACK;                            // once all neighbors of u are visited, u is marked as black
    }

    // free the queue
    freeList(&Q);
}


/*** Other operations ***/

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
        List list_to_print = G->neighbors[i];

        for(moveFront(list_to_print); index(list_to_print) >= 0; moveNext(list_to_print)) {
            int neighbor = get(list_to_print);
            fprintf(out, " %d", neighbor);
        }

        fprintf(out, "\n");
    }
}