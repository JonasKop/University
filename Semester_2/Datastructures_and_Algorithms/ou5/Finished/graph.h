/*
 * LP3 Datastrukturer & algoritmer (C)
 * Spring 17
 * Assignment 5
 * File: graph.c
 * Description: Graph implemented as directed list. I have used Johan Eliassons
 * file graph_nav_directed.c as a guideline for creating this file. This graph
 * implementation does not include the complete graph interface since it was not
 * needed for completing this assignment.
 *
 * Author: Jonas Sjödin
 * CS-user: id16jsn
 * Date: 2017-03-16
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "dlist.h"
#include "queue.h"

//Data type for generic datatype.
typedef data vertex;

//Function that handles the memory for a node and frees it.
#ifndef __FREEVERTEXFUNC
#define __FREEVERTEXFUNC
typedef void freeVertexFunc(vertex v);
#endif


//Function that handles the memory for an edge and frees it.
#ifndef __FREEEDGEFUNC
#define __FREEEDGEFUNC
typedef void freeEdgeFunc(data d);
#endif

//Data type for an edge between two nodes.
typedef struct edge {
    vertex v1; //Startnode
    vertex v2; //Endnode
} edge;

//Data type for a node.
typedef struct node {
    vertex v; //Vertex and its name
    dlist *neighbours; //List that contains the node neighbours.
    bool visited; //Bool that gets marked as true if visited when traversing.
} node;

//Function for comparing two nodes. Returns true if equal vertices, else false.
typedef bool compareFunc(vertex v1, vertex v2);

//Data type for a graph.
typedef struct graph {
    dlist *nodes; //List of nodes
    compareFunc *compareVertex; //Function for comparing vertices
    freeVertexFunc *freeVertex; //Function for freeing vertices
    freeEdgeFunc *freeEdge; //Function for freeing edges.
} graph;

/*
 * Function:    graph_empty
 *
 * Description: Creates an empty graph with an empty list of nodes and sets a
 * compare function for it. It also sets a memhandler for the graph.
 *
 * Input:       *comparef: Pointer to a compare function.
 *
 * Output:      Returns a Graph.
 */
bool graph_isEmpty(graph *myGraph);

/*
 * Function:    graph_isEmpty
 *
 * Description: Checks if a graph is empty
 *
 * Input:       *myGraph: Pointer to a compare function.
 *
 * Output:      Returns true if empty, else false;
 */
graph *graph_empty(compareFunc *cf);

/*
 * Function:    graph_setVertexMemHandler
 *
 * Description: Sets a memhandler for a vertex
 *
 * Input:       *myGraph: pointer to a graph.
 *              *func: pointer to a freefunction.
 *
 * Output:      None
 */
void graph_setVertexMemHandler(graph *myGraph, freeVertexFunc *f);

/*
 * Function:    graph_setEdgeMemHandler
 *
 * Description: Sets a memhandler for an edge (e.v1 or e.v2)
 *
 * Input:       *myGraph: pointer to a graph.
 *              *func: pointer to a freefunction.
 *
 * Output:      None
 */
void graph_setEdgeMemHandler(graph *myGraph, freeEdgeFunc *f);

/*
 * Function:    graph_freeEdge
 *
 * Description: If an edge hasn't been freed it frees it.
 *
 * Input:       *myGraph: pointer to a graph.
 *              e: an edge that contains e.v1, startnode and e.v2, endnode.
 *
 * Output:      A freed edge.
 */
void graph_freeEdge(graph *myGraph, edge e);

/*
 * Function:    graph_insertNode
 *
 * Description: Inserts a node in a graph
 *
 * Input:       *myGraph: pointer to a graph.
 *              vert1: a vertex.
 *
 * Output:      If node already exists it returns true else it inserts a node
 * in a graph and returns false
 */
bool graph_insertNode(graph *myGraph, vertex vert);

/*
 * Function:    graph_insertEdge
 *
 * Description: Inserts an edge between two nodes in a graph
 *
 * Input:       *myGraph: pointer to a graph.
 *              e: an edge that contains e.v1, startnode and e.v2, endnode.
 *
 * Output:      A graph with an inserted edge between two nodes..
 */
void graph_insertEdge(graph *myGraph, edge e);

/*
 * Function:    graph_nodeExist
 *
 * Description: Checks if a node exists in a given graph.
 *
 * Input:       *myGraph: pointer to a graph.
 *              v1: a vertex
 *
 * Output:      None
 */
bool graph_nodeExist(graph *myGraph, vertex v1);

/*
 * Function:    graph_traverseBTF
 *
 * Description: uses BTF (Breadth first) to find if two nodes are connected.
 *
 * Input:       *myGraph: pointer to a graph.
 *              v1: a vertex
 *              v2: a vertex
 *
 * Output:      Returns true if to nodes are connected, else false
 */
bool graph_traverseBTF(graph *g, vertex v1, vertex v2);

/*
 * Function:    graph_setVisitedFalse
 *
 * Description: Sets all the graphs nodes bools visited as false.
 *
 * Input:       *myGraph: pointer to a graph.
 *
 * Output:      All bools visited are now false.
 */
void graph_setVisitedFalse(graph *myGraph);

/*
 * Function:    graph_deleteNode
 *
 * Description: Deletes a node in the graph and frees its allocated memory.
 *
 * Input:       *myGraph: pointer to a graph.
 *              *n: pointer to a node
 *
 * Output:      A node has been removed by the graph.
 */
void graph_deleteNode(graph *myGraph, node *n);

/*
 * Function:    graph_free
 *
 * Description: Frees the graph.
 *
 * Input:       *myGraph: pointer to a graph.
 *
 * Output:      No more memory is allocated by the graph.
 */
void graph_free(graph *myGraph);

#endif
