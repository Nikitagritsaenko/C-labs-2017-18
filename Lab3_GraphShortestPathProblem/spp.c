#include "spp.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#pragma warning(disable:4996)
//-----------------------------------------------------------------
List* InitList(void)
{
  List* p = (List*) malloc(sizeof(List));
  p->field = 0;
  p->next = NULL;
  return p;
}
//-----------------------------------------------------------------
List* AddToList(List* g, int x)
{
  List *tmp = NULL, *p = NULL;
  tmp = (List*) malloc(sizeof(List));
  p = g->next; 
  g->next = tmp; 
  tmp->field = x; 
  tmp->next = p;
  return(tmp);
}
//-----------------------------------------------------------------
List* ListReverse(List* list)
{
  List* rev = NULL;
  List* tmp = list;
  List* tmp_next = NULL;
  for (tmp = list; tmp != NULL; tmp = tmp_next)
  {
    tmp_next = tmp->next;
    tmp->next = rev;
    rev = tmp;
  }
  return rev;
}
//-----------------------------------------------------------------
void FreeList(List* g) 
{ 
  List* tmp = NULL; 
  if (g == NULL)
    return;
  while (g->next != NULL) 
  { 
    tmp = g->next; 
    g->next = tmp->next; 
    free(tmp); 
  } 
  free(g); 
}
//-----------------------------------------------------------------
int ListSize(List* g)
{
  List* t = g;
  int k = 0;
  while (t->field != 0)
  {
    t = t->next;
    k++;
  }
  return k;
}
//-----------------------------------------------------------------
Queue* InitQueue(void) 
{
  Queue* q = (Queue*) malloc(sizeof(Queue));
  q->head = NULL;
  q->tail = NULL;
  return q;
}
//-----------------------------------------------------------------
int IsEmptyQueue(Queue *q) 
{
  return q->head == NULL;
}
//-----------------------------------------------------------------
void Enqueue(Queue *q, int x) 
{
  if ((q->tail == 0) && (q->head == 0)) 
  {
    q->tail = InitList();
    q->head = q->tail;
  }
  q->tail = AddToList(q->tail, x);
}
//-----------------------------------------------------------------
int Dequeue(Queue *q) 
{
  List *temp;
  int x;
  if (IsEmptyQueue(q)) 
    return 0;
  x = q->head->field;
  temp = q->head;
  q->head = q->head->next;
  free(temp);
  return x;
}
//-----------------------------------------------------------------
int ReadFile(FILE* in, int *start, int *end)
{
  int n = 0;
  char* line = (char*) malloc (STR_SIZE * sizeof(char));
  do
  {
    fgets(line, STR_SIZE, in);
    n++;
  } while (*line != '\n');
  n--;

  fscanf(in, "%d", start);
  fscanf(in, "%d", end);
  rewind(in);
  free(line);
  return n;
}
//-----------------------------------------------------------------
List* FindPath(int start, int end, int p[])
{ 
  int curr = end;
  List* path = NULL;
  if ((p[end] == 0) && (start != end))
    return NULL;
  path = InitList();
  for (curr = end; curr != start; curr = p[curr])
    AddToList(path,curr);
  AddToList(path,start);
  return path;
}
//-----------------------------------------------------------------
Graph* ReadGraph(FILE* in, int V) 
{
  List* g = NULL;
  int i = 0, num = 0;
  char c = 0;
  Graph* graph = (Graph*) malloc(sizeof(Graph));
  graph->n = V;
  graph->vertexList = (List**) malloc((V + 1) * sizeof(List*));
  for (i = 1; i <= V; i++)
  {
    g = InitList();
    while (fscanf(in, "%d", &num) > 0) 
    {
      AddToList(g,num);
      do
      {
        c = (char) fgetc(in);
        if (c == '\n')
          break;
      } while(!isalnum(c));  
      if (c == '\n')        
        break;
      fseek(in, -1, SEEK_CUR);  
    }
    g = ListReverse(g);
    graph->vertexList[i] = g;
  }
  return graph;
}
//-----------------------------------------------------------------
void PrintPath(List* g)
{
  List *p;
  if (g == NULL)
  {
    printf("Error: Path is not exist.");
    return;
  }
  p = g->next; 
  do 
  {
    printf("%d ", p->field); 
    p = p->next; 
  } while (p != NULL); 
}
//-----------------------------------------------------------------
List* ShortestPathSearch(int start, int end, int n, Graph* g)
{
  int i = 0, size = 0, v = 0, rel = 0;
  List* t = NULL;
  List* path = NULL;
  Queue* q = InitQueue();
  int* pred = (int*) calloc(n + 1, sizeof(int)); 
  int* was = (int*) calloc(n + 1, sizeof(int)); 
  Enqueue(q, start);
  pred[start] = -1;
  was[start] = 1;
  q->head->field = start;
  Dequeue(q);
  while (!IsEmptyQueue(q))
  {
    v = q->head->field;
    size = ListSize(g->vertexList[v]);
    t = g->vertexList[v];
    t = t->next;
    for (i = 0; i < size - 1; i++)
    {
      rel = t->field;
      if (was[rel] == 0)
      {
        Enqueue(q,rel);
        was[rel] = 1;
        pred[rel] = v;
      }
      t = t->next;
    }
    Dequeue(q);
  }
  path = FindPath(start,end,pred);
  free(was);
  free(q);
  free(pred);
  return path;
}