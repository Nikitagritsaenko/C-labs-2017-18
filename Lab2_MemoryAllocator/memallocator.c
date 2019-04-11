#include <crtdbg.h>
#include <malloc.h>
#include <stdio.h>
#include "memallocator.h"

typedef struct mem_block_t
{
  int size;
  struct mem_block_t *next;
} mem_block_t;

static mem_block_t* S;
static mem_block_t* Curr;
static mem_block_t* Start;
static mem_block_t* End;

int memgetblocksize()
{
  return (sizeof(mem_block_t) + sizeof(int));
}

int memgetminimumsize()
{
  return 2 * (sizeof(mem_block_t) + sizeof(int));
}

int meminit(void *pMemory, int size) 
{
  if (pMemory == NULL) 
  {
    printf("ERR: NOT_ENOUGH_MEMORY.");
    return 0;
  }
  if (size < memgetminimumsize())
  {
    printf("ERR: NOT ENOUGH SIZE TO INIT MEMSYSTEM");
  }
  Start = (mem_block_t*) pMemory;
  Start->size = size;
  Start->next = NULL;
  End = (mem_block_t*)((char*)Start + size);
  S = Start;
  Curr = Start;
  return 1;
}

void BlockCreate(int size, void* p)
{
  int *last = NULL;
  mem_block_t* start = (mem_block_t*) ((char*) p - sizeof(mem_block_t));
  //"size" > 0 at the end of allocated block
  last = (int*)((char*) p + size);
  *last = size + memgetblocksize();
  start->next = NULL;
  start->size = size + memgetblocksize();
  return;
}


mem_block_t* Prev(mem_block_t* L)
{
  mem_block_t *P = S; 
  while(P->next != L)
    P = P->next;
  return P;
}

// Allocate memory block of size 'size'.
// Returns pointer to memory block if success, 0 otherwise
void *memalloc(int size)
{
  void *p = NULL;
  int *last = NULL;
  mem_block_t *L = Curr;
  mem_block_t *tCurr = Curr;
  mem_block_t *pointStart = Curr;
  if (size <= 0)
    return NULL;
    
  //searching for appropriate block 
  while (L->size < size + memgetblocksize()) 
  {
    L = L->next;
    if (L == NULL) 
    {
      L = Start;
    }
    if (L == pointStart->next) ////NEEDS TO CHECK
    {
      Curr = tCurr; // we don't change the current pointer in this case
      printf("ERR: Appropriate block didn't find. We have only %i bytes instead of %i.\n", Curr->size - memgetblocksize(), size);
      return NULL;
    }
  }

  p = (char*)L + L->size - (size + sizeof(int)); //"p" at the start of data
  L->size = L->size - (size + memgetblocksize()); //remaining space? best fit == -->0 this num?

  if (L->size <= memgetblocksize()) //if we have a little piece remain, we just give it to user
  {
    p = (char*)p - L->size;
    if (L == S) //??????????
    {
      Curr = S;
      S = S->next;
      
    }
    else //??????????
    {  
      Prev(L)->next = L->next; 
      Curr =  Prev(L);
    }
    size += L->size;
    
  }
  else 
  {
    last = (int*)((char*)L + L->size - sizeof(int));
    *last = -(L->size);
    Curr = L;
  }

  BlockCreate(size, p);
  //L->size ("size") < 0 at the end of free block
  //Curr = L;
  return p;
}

int CheckLeftMerge(void *p, int size)
{
  int* temp = NULL;
  int tempSize;
  mem_block_t *node = NULL;
  temp = (int*) ((char*)p - sizeof(mem_block_t) - sizeof(int));
  if (temp < (int*)Start)
    return 0;

  tempSize = *temp;
  if (tempSize < 0)
  {
    node = (mem_block_t*) ((char*)temp + tempSize + sizeof(int));
    node->size += size;
    p = (char*)node + sizeof(mem_block_t); //replace p to the start of new block's data
    temp = (int*)((char*)node + node->size - sizeof(int));
    *temp = -node->size;
    return 1;
  }
  else 
    return 0;
}

int CheckRightMerge(void* p, int size, int check)
{
  int* lastInt = NULL;
  int tempSize;
  mem_block_t *node = NULL, *buff = NULL;
  node = (mem_block_t*) ((char*)p - sizeof(mem_block_t) + size); //right block //??????????????
  //need to check, if this block exist or not 
  if (node >= End)
    return 0;
  lastInt = (int*) ((char*)node + node->size - sizeof(int));
  tempSize = *lastInt; //size of right block(end)
  if (tempSize < 0)
  {
    buff = (mem_block_t*) ((char*) p - sizeof(mem_block_t)); //start of block p
    buff->size += node->size;
    *lastInt = -buff->size;
    if (check)
    {
      //delete node from list and insert buff
      if (node != S) //node != S
        Prev(node)->next = node->next;
      else  //node == S
        S = node->next;
      return 1;
    }
    else  //if block buff is not in list  
    {
      if (node != S)
        Prev(node)->next = buff;
      else //if node == S
        S = buff;
      buff->next = node->next;
      return 1;
    }
  }
  else 
    return 0;
}
// Free memory previously allocated by memalloc
void memfree(void *p)
{
  mem_block_t* temp;
  int check = 0;
  int* last = NULL;
  if (p == NULL)
    return;
  temp = (mem_block_t*) ((char*)p - sizeof(mem_block_t));
  last = (int*)((char*)temp + temp->size - sizeof(int));
  *last = -temp->size;
  check += CheckLeftMerge(p, temp->size);
  check += CheckRightMerge(p, temp->size, check);
  if (!check)
  { 
    temp->next = S;
    S = temp;
    //Curr = temp;
  }
  return;
}


void memdone()
{
  _CrtCheckMemory();
  _CrtDumpMemoryLeaks();
}
