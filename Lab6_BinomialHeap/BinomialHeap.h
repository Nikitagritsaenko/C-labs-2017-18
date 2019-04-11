#ifndef __BINOMIAL_HEAP_H__34AF8EC9
#define __BINOMIAL_HEAP_H__34AF8EC9

/* сделать 3 структуры */

typedef struct Node
{
  int key;
  struct Node *parent;
  struct Node *child;
  struct Node *sibling;
} node_t;

typedef struct Root
{
  int key;
  int degree;
  struct Root *next;
  struct Node *child;
} root_t;

typedef union BinomialHeap
{
   root_t* head;
   node_t* node;
} bin_heap_t;

bin_heap_t* InitBinomialHeap(void);

int GetMinimum(bin_heap_t* H); 

bin_heap_t* Merge(bin_heap_t* H1, bin_heap_t* H2);

bin_heap_t* Add(bin_heap_t* heap, int x);

bin_heap_t* ExtractMin(int *min, bin_heap_t* H);

bin_heap_t* Delete(bin_heap_t* H, int x);

void FreeHeap(bin_heap_t* t);

#endif
