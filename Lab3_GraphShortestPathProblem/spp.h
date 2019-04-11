#ifndef __DLS_H__34AF8EC9
#define __DLS_H__34AF8EC9
#define INF 10e7
#define STR_SIZE 1000
#include <stdio.h>
//spp means "shortest path problem"

typedef struct List         
{
  unsigned int field;
  struct List *next;
} List; 

typedef struct Graph
{
   int n;
   struct List** vertexList;
} Graph;

typedef struct Queue 
{
  List *head; 
  List *tail;
} Queue;

void FreeList(List* g);

void PrintPath(List* g);

List* ShortestPathSearch(int start, int end, int n, Graph* g);

Graph* ReadGraph(FILE* in, int V);

int ReadFile(FILE* in, int *start, int *end);

#endif