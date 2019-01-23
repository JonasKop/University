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

#include "graph.h"

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
graph *graph_empty(compareFunc *comparef) {
    graph *myGraph = calloc(1, sizeof(graph));
    myGraph->nodes = dlist_empty();
    myGraph->compareVertex = comparef;
    dlist_setMemHandler(myGraph->nodes, free);
    if (graph_isEmpty(myGraph))
        return myGraph;
    else{
        fprintf(stderr, "Could not create graph.");
        exit(0);
    }
}

/*
 * Function:    graph_isEmpty
 *
 * Description: Checks if a graph is empty
 *
 * Input:       *myGraph: Pointer to a compare function.
 *
 * Output:      Returns true if empty, else false;
 */
bool graph_isEmpty(graph *myGraph) {
    return dlist_isEmpty(myGraph->nodes);
}

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
void graph_setVertexMemHandler(graph *myGraph, freeVertexFunc *func) {
    myGraph->freeVertex = func;
}

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
void graph_setEdgeMemHandler(graph *myGraph, freeEdgeFunc *func) {
    myGraph->freeEdge = func;
}

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
void graph_freeEdge(graph *myGraph, edge e){
    if (myGraph->freeEdge != 0){
       // myGraph->freeEdge(e.v1);
       // myGraph->freeEdge(e.v2);
    }
}

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
bool graph_insertNode(graph *myGraph, vertex vert) {

    if (graph_nodeExist(myGraph, vert)){
        return true;
    }

    node *newNode = calloc(1, sizeof(node));
    newNode->v = vert;
    newNode->neighbours = dlist_empty();
    dlist_setMemHandler(newNode->neighbours, free);
    dlist_insert(myGraph->nodes, dlist_first(myGraph->nodes), newNode);
    return false;
}

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
void graph_insertEdge(graph *myGraph, edge e) {

    if (myGraph->compareVertex(e.v1, e.v2)){
        graph_freeEdge(myGraph, e);
        return;
    }

    node *n1, *n2;
    bool n1Found = false;
    bool n2Found = false;

    //Finds and gets both nodes, n and n2
    for (dlist_position p = dlist_first(myGraph->nodes);
    !dlist_isEnd(myGraph->nodes, p); p = dlist_next( myGraph->nodes, p)) {

        if (!n1Found) {
            n1 = dlist_inspect(myGraph->nodes, p);
            if (myGraph->compareVertex(e.v1, n1->v))
                n1Found = true;
        }
        if (!n2Found) {
            n2 = dlist_inspect(myGraph->nodes, p);
            if (myGraph->compareVertex(e.v2, n2->v))
                n2Found = true;
        }
        if (n1Found && n2Found)
            break;
    }

    //If found neighbour return
    for (dlist_position p2 = dlist_first(n1->neighbours); !dlist_isEnd(
    n1->neighbours, p2); p2 = dlist_next(n1->neighbours, p2)) {

        if (myGraph->compareVertex(dlist_inspect(n1->neighbours, p2), e.v2)) {
            graph_freeEdge(myGraph, e);
            return;
        }
    }

    dlist_insert(n1->neighbours, dlist_first(n1->neighbours), n2);
    graph_freeEdge(myGraph, e);
}

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
bool graph_nodeExist(graph *myGraph, vertex vert1){

    if(myGraph->compareVertex("", vert1))
        return false;
    for(dlist_position p=dlist_first(myGraph->nodes);
    !dlist_isEnd(myGraph->nodes, p); p = dlist_next(myGraph->nodes, p)) {

        node *n = dlist_inspect(myGraph->nodes, p);
        if(myGraph->compareVertex(vert1, n->v)){
            return true;
        }
    }
    return false;
}

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
bool graph_traverseBTF(graph *myGraph, vertex vert1, vertex vert2) {

    graph_setVisitedFalse(myGraph);

    node *n;
    for (dlist_position p = dlist_first(myGraph->nodes); !dlist_isEnd(myGraph->nodes, p); p = dlist_next( myGraph->nodes, p)) {
        n = dlist_inspect(myGraph->nodes, p);
        if (myGraph->compareVertex(vert1, n->v)) {
            break;
        }
    }

    queue *visitedNodes = queue_empty();

    n->visited = true;

    queue_enqueue(visitedNodes, n);

    while (!queue_isEmpty(visitedNodes)) {
        node *n2 = queue_front(visitedNodes);

        if (myGraph->compareVertex(vert2, n2->v)) {
            queue_free(visitedNodes);
            return true;
        }
        queue_dequeue(visitedNodes);

        for (dlist_position p = dlist_first(n2->neighbours); !dlist_isEnd(n2->neighbours, p); p = dlist_next(n2->neighbours, p)) {
            node *n3 = dlist_inspect(n2->neighbours, p);

            if (!n3->visited) {
                n3->visited = true;
                queue_enqueue(visitedNodes, n3);
            }
        }

    }
    queue_free(visitedNodes);
    return false;
}

/*
 * Function:    graph_setVisitedFalse
 *
 * Description: Sets all the graphs nodes bools visited as false.
 *
 * Input:       *myGraph: pointer to a graph.
 *
 * Output:      All bools visited are now false.
 */
void graph_setVisitedFalse(graph *myGraph){
    for (dlist_position p = dlist_first(myGraph->nodes); !dlist_isEnd(myGraph->nodes, p); p = dlist_next(myGraph->nodes, p)) {
        node *n = dlist_inspect(myGraph->nodes, p);
        n->visited = false;
    }
}

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
void graph_deleteNode(graph *myGraph, node *n){
    dlist_setMemHandler(n->neighbours, NULL);
    while (!dlist_isEmpty(n->neighbours)){
        dlist_remove(n->neighbours, dlist_first(n->neighbours));
    }
    dlist_setMemHandler(n->neighbours, free);

    dlist_free(n->neighbours);
    if(myGraph->freeVertex != 0)
        myGraph->freeVertex(n->v);

    dlist_remove(myGraph->nodes, dlist_first(myGraph->nodes));
}

/*
 * Function:    graph_free
 *
 * Description: Frees the graph.
 *
 * Input:       *myGraph: pointer to a graph.
 *
 * Output:      No more memory is allocated by the graph.
 */
void graph_free(graph *myGraph) {
    while (!dlist_isEmpty(myGraph->nodes)){
        node *n = dlist_inspect(myGraph->nodes, dlist_first(myGraph->nodes));
        graph_deleteNode(myGraph, n);
    }
    dlist_free(myGraph->nodes);
    free(myGraph);
}