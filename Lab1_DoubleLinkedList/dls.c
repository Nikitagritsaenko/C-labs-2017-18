#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <string.h>
#pragma warning(disable:4996)
#define STR_SIZE 256
#define BUCKET_NUM 256

typedef struct List
{
  char *str; 
  struct List *next; 
  struct List *prev; 
} List;

List* InitList()  
{
  List *l;
  l = (List*) malloc(sizeof(List));
  l->str = 0;
  l->next = NULL; 
  l->prev = NULL; 
  return l;
}
//-----------------------------------------------------
List* Push(List* l, char* line)
{
  List *tmp, *p;
  char* str = NULL;
  int i = 0, k = 0;

  k  = strlen(line);
  str = (char*) malloc(sizeof(char) * (k + 1));
  strcpy(str,line);

  tmp = (List*) malloc(sizeof(List));
  p = l->next; 
  l->next = tmp; 
  tmp->str = str; 
  tmp->next = p; 
  tmp->prev = l; 
  if (p != NULL)
    p->prev = tmp;
  return(tmp);
}

//-----------------------------------------------------
void PrintList(List* l)
{
  List *p = l->next;
  do 
  {
    printf(" %s", p->str); 
    p = p->next; 
  } while (p != NULL); 
}
//-----------------------------------------------------
List* FreeList(List* s)
{
  List *curr = s->next;
  while (curr)
  {
    List *tmp = curr;
    curr = curr->next;
    free(tmp->str);
    free(tmp);
  }
  s->next = NULL;
  s->prev = NULL;
  return s;
}
//-----------------------------------------------------
List* SortedByIndex(List* S, int index)
{
  List* pList = S;
  int i = 0, j = 0, key = 0;
  List* front[BUCKET_NUM]; 
  List** ppNextItem[BUCKET_NUM];
  List** ppNext;

  for (j = 0; j < BUCKET_NUM; j++)
    front[j] = 0;
  for (j = 0; j < BUCKET_NUM; j++)
    ppNextItem[j] = &front[j];

  while (pList != NULL)
  {
    List* tmp = pList;
    pList = pList->next;

    tmp->next = NULL; 
    if ((tmp->str == NULL) || (strlen(tmp->str) < (unsigned int)index))
    {
      *ppNextItem[0] = tmp;  
      ppNextItem[0] = &(tmp->next);
    }
    else
    {
      key = (unsigned char) tmp->str[index];
      *ppNextItem[key] = tmp;  
      ppNextItem[key] = &(tmp->next);
    }
  }

  pList = NULL;
  ppNext = &pList;

  for (j = 0; j < BUCKET_NUM; j++)
  {
    if (front[j] == 0)
      continue;
    else 
      *ppNext = front[j];

    while (*ppNext != NULL)
      ppNext = &((*ppNext)->next);
  }
  return pList;
}

void RadixSort(List* S, int max)
{
  List* p;
  List *previous;
  int index = 0;
  for (index = max; index >= 0; index--)
    S = SortedByIndex(S, index);

  p = S;
  previous = S;
  p = p->next;
  while (p != NULL)
  {
    p->prev = previous;
    p = p->next;
    previous = previous->next; 
  }
}

//-----------------------------------------------------------------
int main(int argc, char const* argv[])
{
  FILE* in = stdin;
  List *s = InitList();
  char* line = (char*) malloc (STR_SIZE * sizeof(char));
  unsigned int max = 0;

  if (argc == 2) 
  {
    if (freopen_s(&in, argv[1], "r", stdin) != 0)
    {
      printf("ERROR: Can't open input file '%s'", argv[1]);
      return -1;
    }
  }

  while ((fgets(line, STR_SIZE, in)) != NULL)
  {
    Push(s, line); 
    if (strlen(line) > max)
      max = strlen(line);
  }

  printf("\nUnsorted:\n");
  PrintList(s);
  RadixSort(s, max);
  printf("\nSorted:\n");
  PrintList(s);

  //тест на сохранение двусвязности
  /*s=s->next->next->next->next;
  printf("[%s]",s->str);
  s=s->next;
  printf("[%s]",s->prev->str);*/

  if (in != stdin)
    fclose(in);
  FreeList(s);
  free(s);
  free(line);
  _CrtDumpMemoryLeaks();
  return 0;
}