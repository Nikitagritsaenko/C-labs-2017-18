#ifndef __DLS_H__34AF8EC9
#define __DLS_H__34AF8EC9
//llh means "Leftlist heap"

typedef struct LeftListHeap
{
  int key;
  struct LeftListHeap *left;
  struct LeftListHeap *right;
  int dist;
} left_list_heap_t;

left_list_heap_t* InitLeftListHeap(int x);

left_list_heap_t* Merge(left_list_heap_t* tree1, left_list_heap_t* tree2);

void Push(left_list_heap_t* tree, int x);

left_list_heap_t* ExtractMin(int *min, left_list_heap_t* tree);

left_list_heap_t* DecreaseKey(left_list_heap_t* tree, int x, int key);

void Delete(left_list_heap_t* tree, int x);

void FreeTree(left_list_heap_t* t);

#endif
