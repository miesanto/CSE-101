/*
*   PA3 Graph.h
*   Header file for Graph ADT
*   Author: Mia Santos (miesanto)
*   Date: 10/25/2023
*   CSE 101 Fall 2023, Tantalo
*/

#ifndef GRAPH_H_INCLUDE_
#define GRAPH_H_INCLUDE_
#include "List.h"
#include<stdbool.h>
#include<stdio.h>

#define UNDEF -1
#define NIL 0
#define WHITE 0
#define GRAY 1
#define BLACK 2

/*** Exported Types ***/
typedef struct GraphObj* Graph;

/*** Constructors-Destructors ***/

// newGraph()
// returns a Graph pointing to a newly created GraphObj
// GraphObj represents a graph with n vertices and no edges
Graph newGraph(int n);

// freeGraph()
// frees all heap memory associated with the Graph *pG
// then sets *pG to NULL
void freeGraph(Graph* pG);


/*** Access functions ***/

// getOrder() and getSize() return the corresponding field values

// getOrder()
int getOrder(Graph G);

// getSize()
int getSize(Graph G);

// getParent()
// return the parent of vertex u
// may be NIL
// pre: 1<=u<=n=getOrder(G)
int getParent(Graph G, int u);

// discover and finish times of vertices will be undefined before DFS is called

// getDiscover()
// pre: 1<=u<=n=getOrder(G)
int getDiscover(Graph G, int u);

// getFinish()
// pre: 1<=u<=n=getOrder(G)
int getFinish(Graph G, int u);


/*** Manipulation procedures ***/

// decriptions of addEdge() and addArc are exactly as they were in pa2
// it is the responsibility of these two functions to maintain adj. lists in sorted order

// addArc()
// inserts a new directed edge from u to v
// i.e., v is added to u's adjacency List (BUT NOT VICE VERSA)
// pre: 1<=u<=n=getOrder(G) and 1<=v<=n=getOrder(G)
void addArc(Graph G, int u, int v);

// addEdge()
// inserts a new edge joining u to v
// i.e., u is added to v's adjacency List, and v to u's adjacency List
// !!! NOTE !!! "your program is requried to maintain these lists in sorted order by increasing labels"
// pre: 1<=u<=n=getOrder(G) and 1<=v<=n=getOrder(G)
void addEdge(Graph G, int u, int v);

// DFS()
// runs the DFS algorithm on the Graph G with List S
// S has two purposes:
// 1. defines the order in which vertices are to be processed in the main loop of DFS
// 2. when DFS is complete, it will store the vertices by decreasing finish times
// hence, S is considered to be a stack
// pre: length(S) == n
// pre: S contains some permutations of the integers {1,2,...,n} where n = getOrder(G)
void DFS(Graph G, List S);


/*** Other operations ***/

// transpose()
// returns a reference to a new graph object representing the transpose of G
Graph transpose(Graph G);

// copyGraph()
// returns a reference to a new graph that is a copy of G
Graph copyGraph(Graph G);

// printGraph() prints the adjacency list representation of G to the file pointed to by out
void printGraph(FILE* out, Graph G);

#endif