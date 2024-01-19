/*
*   PA2 Graph.h
*   Header file for Graph ADT
*   Author: Mia Santos (miesanto)
*   Date: 10/17/2023
*   CSE 101 Fall 2023, Tantalo
*/

#ifndef GRAPH_H_INCLUDE_
#define GRAPH_H_INCLUDE_
#include "List.h"
#include<stdbool.h>
#include<stdio.h>

#define INF -1
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

// getSource()
// returns the source vertex most recently used in BFS()
// or NIL if BFS() has not yet been called
int getSource(Graph G);

// getParent()
// return the parent of vertex u in the bFS tree created by BFS()
// or NIL if BFS() has not yet been called
// pre: 1<=u<=getOrder(G)
int getParent(Graph G, int u);

// getDist()
// returns the distance from the most recent bFS source to vertex u
// or INF if BFS() has not yet been called
// pre: 1<=u<=getOrder(G)
int getDist(Graph G, int u);

// getPath()
// appends to the List L the vertices of a shortest path in G from source to u
// or appends to L the value NIL if no such path exists
// pre: getSource(G)!=NIL (basically BFS() must be called first)
// pre: 1<=u<=getOrder(G)
void getPath(List L, Graph G, int u);


/*** Manipulation procedures ***/

// makeNull()
// deletes all edges of G, restoring it to its original (no edge) state
void makeNull(Graph G);

// addEdge()
// inserts a new edge joining u to v
// i.e., u is added to v's adjacency List, and v to u's adjacency List
// !!! NOTE !!! "your program is requried to maintain these lists in sorted order by increasing labels"
// pre: 1<=u<=getOrder(G) and 1<=v<=getOrder(G)
void addEdge(Graph G, int u, int v);

// addArc()
// inserts a new directed edge from u to v
// i.e., v is added to u's adjacency List (BUT NOT VICE VERSA)
// pre: 1<=u<=getOrder(G) and 1<=v<=getOrder(G)
void addArc(Graph G, int u, int v);

// BFS()
// runs the BFS algorithm on the Graph G with source s
// sets the color distance, parent, and source fields of G accordingly
void BFS(Graph G, int s);


/*** Other operations ***/

// printGraph() prints the adjacency list representation of G to the file pointed to by out
void printGraph(FILE* out, Graph G);

#endif