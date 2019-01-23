/*
 * LP3 Datastrukturer & algoritmer (C)
 * Spring 17
 * Assignment 5
 * File: main.c
 * Description: Checks if nodes are connected in a graph by using myc own graph
 * implementation in graph.c and graph.h.
 *
 * Author: Jonas Sjödin
 * CS-user: id16jsn
 * Date: 2017-03-16
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

bool compareString(void *string1, void *string2);
FILE *fetchFile(char *filePath);
void insertNodesAndEdges(graph *myGraph, FILE *map);
bool nodesExists(graph *myGraph, vertex node1, vertex node2);
void runProgram(graph *myGraph);

/*
 * Function:    main
 *
 * Description: Runs the program and gets user input on which nodes it should
 * check if they are connected.
 *
 * Input:       *string1: Pointer file with nodes, user input about which nodes
 *              to check if they are connected.
 *
 * Output:      Prints if nodes are connected.
 */
int main(int argc, char **argv) {

    graph *myGraph = graph_empty(compareString);
    graph_setEdgeMemHandler(myGraph, free);
    graph_setVertexMemHandler(myGraph, free);
    FILE *mapFile = fetchFile(argv[1]);

    insertNodesAndEdges(myGraph, mapFile);
 //   rewind(mapFile);
  //  insertEdges(myGraph, mapFile);
    fclose(mapFile);

    runProgram(myGraph);
    graph_free(myGraph);
    return 0;
}

/*
 * Function:    compareString
 *
 * Description: Compares two string and returns true if equal, else false.
 *
 * Input:       *string1: Pointer to a string
 *              *string2: Pointer to a string
 *
 * Output:      Returns "true" if equal string, else it returns "false".
 */
bool compareString(void *string1,void *string2){
    int equal = strcmp((char *) string1, (char *) string2);
    if (equal == 0)
        return true;
    return false;
}

/*
 * Function:    fetchFile
 *
 * Description: Fetches the file from the given file path.
 *
 * Input:       *string1: Pointer to a file path
 *
 * Output:      Returns the mapfile from the given filepath.
 */
FILE *fetchFile(char *filePath){
    FILE *mapFile = fopen(filePath, "r");
    if(mapFile == NULL){
        fprintf(stderr, "Can't open: '%s'.\nExiting...", filePath);
        exit(0);
    }
    return mapFile;
}

/*
 * Function:    insertNodesAndEdges
 *
 * Description: Inserts nodes and edges from the given file.
 *
 * Input:       *myGraph: Pointer to a graph
 *              *mapFile: Pointer to a map file
 *
 * Output:      The graph has been filled with nodes and edges.
 */
void insertNodesAndEdges(graph *myGraph, FILE *mapFile){
    int nrOfEdges;

    if(fscanf(mapFile, "%d" , &nrOfEdges) == 1){

        while(!feof(mapFile)) {
            char *vert1 = calloc(1, sizeof(vertex));
            char *vert2 = calloc(1, sizeof(vertex));

            fscanf(mapFile, "%40s %40s", vert1, vert2);
            edge e;

            e.v1 = vert1;
            e.v2 = vert2;

            bool v1Exists = graph_insertNode(myGraph, vert1);
            bool v2Exists = graph_insertNode(myGraph, vert2);

            graph_insertEdge(myGraph, e);
            if (v1Exists)
                if (myGraph->freeVertex != 0)
                    myGraph->freeVertex(vert1);
            if (v2Exists)
                if (myGraph->freeVertex != 0)
                    myGraph->freeVertex(vert2);
        }
    }
}

/*
 * Function:    nodesExists
 *
 * Description: Checks if two nodes exists in the graph, else it prints info.
 *
 * Input:       *myGraph: Pointer to a graph
 *              node1: A vertex
 *              node2: A vertex
 *
 * Output:      Returns true if at least one of the vertices exists in the
 *              graph, else it returns false;
 */
bool nodesExists(graph *myGraph, vertex node1, vertex node2){
    bool Node1Exists = true;
    bool Node2Exists = true;
    if (myGraph->compareVertex(node1, node2)){
        printf("Wrong, entered same node\n");
        return false;
    }

    if (!graph_nodeExist(myGraph, node1)){
        Node1Exists = false;
        printf("'%s' doesn't exists in file\n", (char *) node1);
    }
    if (!graph_nodeExist(myGraph, node2)){
        Node2Exists = false;
        printf("'%s' doesn't exists in file\n", (char *) node2);
    }
    if (Node1Exists == false || Node2Exists == false)
        return false;

    return true;
}

/*
 * Function:    runProgram
 *
 * Description: Runs the program and gets user input on which nodes the function
 * should check if they are connected.
 *
 * Input:       *myGraph: Pointer to a graph
 *
 * Output:      Prints info about if two Nodes are connected or not.
 */
void runProgram(graph *myGraph){

    char buff[81];

    while(true){
        char *node1 = calloc(1, sizeof(vertex));
        char *node2 = calloc(1, sizeof(vertex));

        printf("Enter origin and destination (quit to exit): ");

        if(strncmp(fgets(buff, 81, stdin), "quit", 4) == 0){
            free(node1);
            free(node2);
            break;
        }

        sscanf(buff, "%40s %40s", node1, node2);

        if (nodesExists(myGraph, node1, node2)){
            if (graph_traverseBTF(myGraph, node1, node2))
                printf("%s and %s are connected\n", node1, node2);
            else
                printf("%s and %s are not connected\n", node1, node2);
        }
        free(node1);
        free(node2);
    }
}