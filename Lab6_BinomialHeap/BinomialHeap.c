#include <stdio.h>
#include <stdlib.h>
#include "BinomialHeap.h"
#define INF 10e6

bin_heap_t* InitBinomialHeap(void) 
{
  bin_heap_t* heap = (bin_heap_t*) malloc(sizeof(bin_heap_t));
  heap->head = (root_t*) malloc(sizeof(root_t));
  heap->head->degree = heap->head->key = 0;
  heap->head->next = NULL;
  heap->head->child = NULL;
  return heap;
}

int GetMinimum(bin_heap_t* H) 
{
  root_t *curH = NULL, *minH = NULL;
  int min = 0;
  if (H == NULL)
    return 0;
  curH = H->head->next;
  min = H->head->key; 
  minH = H->head;
  while (curH)
  {
    if (min > curH->key)
    {
      min = curH->key;
      minH = curH;
    }
    curH = curH->next;
  }
  return minH->key;
}

void BinomialLink(bin_heap_t** y, bin_heap_t** z)
{
  int keyY = (*y)->head->key;
  node_t* childY = (*y)->head->child;
  (*y)->node->parent = (node_t*)((*z)->head);
  (*y)->node->sibling = (*z)->head->child;
  (*y)->node->key = keyY;
  (*y)->node->child = childY;
  (*z)->head->child = (node_t*)((*y)->node);
  (*z)->head->degree += 1;
}

root_t* MergeRootLists(root_t* H1, root_t* H2) 
{
  root_t *curH1 = H1, *curH2 = H2, *H = NULL, *curH = NULL, *res = NULL;
  curH = (root_t*) malloc(sizeof(root_t));
  H = curH;
  while ((curH1 != NULL) && (curH2 != NULL))
  {
    if (curH1->degree < curH2->degree)
    {
      curH->next = curH1;
      curH = curH->next;
      curH1 = curH1->next;
    }
    else
    {
      curH->next = curH2;
      curH = curH->next;
      curH2 = curH2->next;
    }
  }
  if (curH1 == NULL)
    curH->next = curH2;
  else
    curH->next = curH1;
  res = H->next;
  free(H);
  return res;
}

root_t* Merge1(root_t* H1, root_t* H2)
{
  root_t *x = NULL, *prev_x = NULL, *next_x = NULL, *H = NULL;
  bin_heap_t* t1 = InitBinomialHeap();
  bin_heap_t* t2 = InitBinomialHeap();
  if (H1 == NULL)
    return H2;
  if (H2 == NULL)
    return H1;
  H = MergeRootLists(H1, H2);
  if (H == NULL)
    return H;
  x = H;
  next_x = x->next;
  while (next_x != NULL)
  {
    if ((x->degree != next_x->degree) || ((next_x->next != NULL) && (next_x->next->degree == x->degree)))
    {
      prev_x = x;
      x = next_x;
    }
    else if (x->key <= next_x->key)
    {
      x->next = next_x->next;
      t1->head = next_x;
      t2->head = x;
      BinomialLink(&t1, &t2);
    }
    else
    {
      if (prev_x == NULL)
        H = next_x;
      else
        prev_x->next = next_x;
      t1->head = x;
      t2->head = next_x;
      BinomialLink(&t1, &t2);
      x = next_x;   
    }
    next_x = x->next;
  }


  free(t1);
  free(t2);
  return H;
}

int GetDegree(node_t* node)
{
  int k = 0;
  while (node->child)
  {
    node = node->child;
    k++;
  }
  return k;
}

bin_heap_t* HeapOfDeletedNode(node_t* list)
{
  node_t* rev = NULL;
  node_t* tmp = list;
  node_t* tmp_next = NULL;
  node_t *tmp_child = NULL, *tmp_sibling = NULL;
  root_t *root = NULL;
  int deg = 0, key = 0;
  bin_heap_t* H1 = InitBinomialHeap();
  for (tmp = list; tmp != NULL; tmp = tmp_next)
  {
    tmp_next = tmp->sibling;
    tmp->sibling = rev;
    rev = tmp;
  }
  H1->head = (root_t*)rev;
  while (rev)
  {
    tmp = rev->sibling;
    tmp_child = rev->child;
    tmp_sibling = rev->sibling;
    key = rev->key;

    root = (root_t*) rev;
    root->degree = GetDegree(rev);
    root->child = tmp_child;
    root->next = (root_t*)tmp_sibling;
    root->key = key;
    
    rev = tmp;
  }
  return H1;
}

bin_heap_t* Merge(bin_heap_t* H1, bin_heap_t* H2)
{
  bin_heap_t* H = InitBinomialHeap();
  if (H1->head == NULL)
    return H2;
  if (H2->head == NULL)
    return H1;
  H->head = Merge1(H1->head, H2->head);
  free(H1);
  free(H2);
  return H;
}

bin_heap_t* Add(bin_heap_t* heap, int x)
{
  bin_heap_t* elem = InitBinomialHeap();
  elem->head->key = x;
  return Merge(heap, elem);
}

bin_heap_t* ExtractMin(int *min, bin_heap_t* H)
{
  root_t *x = NULL, *x_prev = NULL, *curx = H->head, *curx_prev = NULL;
  bin_heap_t *H1 = NULL;
  node_t *curn = NULL;
  // find min-key elem from root-list
  while (curx != NULL)
  {
    if (curx->key < *min)
    {
      *min = curx->key;
      x = curx;
      x_prev = curx_prev;
    }
    curx_prev = curx;
    curx = curx->next;
  }
  //delete min-key elem from root-list
  if (x_prev == NULL)
    H->head = x->next;
  else
    x_prev->next = x->next;
  //build H1 from x's children
  if (x->child == NULL)
  {
    free(x);
    return H;
  }
  curn = x->child;
  H1 = HeapOfDeletedNode(curn);
  free(x);
  return Merge(H, H1);
}


node_t* SearchInTree(node_t* x, int k, int* found)
{
  static node_t* result = NULL;
  if (x != NULL)
  {
    if ((x != NULL) && (k == x->key))
    {
      *found = 1;
      result = x;
    }
    if (!(*found))
      SearchInTree(x->child, k, found);
    if (!(*found))
      SearchInTree(x->sibling, k, found);
  }
  return result;
}

bin_heap_t* Search(bin_heap_t* H, int x)
{
  root_t* root = H->head;
  bin_heap_t* res = InitBinomialHeap();
  int found = 0;
  while (root)
  {
    if (root->key == x)
    {
      res->head = root;
      return res;
    }
    res->node = SearchInTree(root->child, x, &found);
    if (found)
      return res;
    root = root->next;
  }
  return NULL;
}

int IsRoot(bin_heap_t* H, node_t* z)
{
  root_t *root = H->head;
  if (z == NULL)
    return 1;
  while (root)
  {
    if ((root_t*)z == root)
      return 1;
    root = root->next;
  }
  return 0;
}

void DecreaseKey(bin_heap_t* H, int key, int k)
{
  bin_heap_t *y = InitBinomialHeap();
  bin_heap_t *z = InitBinomialHeap();
  node_t *tmp_parent = NULL, *tmp_child = NULL, *tmp_sibling = NULL;
  node_t *tmp_node = NULL;
  int tmp = 0;
  bin_heap_t *x = Search(H, key);
  if (x == NULL)
    return;
  if (k > x->node->key)
  {
    printf("Error. The key must be less than before.");
    return;
  }
  x->node->key = k;
  y->node = x->node;
  z->node = y->node->parent;
  while ((z->node != NULL) && (y->node->key < z->node->key))
  {
    tmp = y->node->key;
    y->node->key = z->node->key;
    z->node->key = tmp;
    y->node = z->node;
    z->node = y->node->parent;
    if (IsRoot(H,y->node))
    {
      z->node = NULL;
      break;
    }
  }
  free(y);
  free(z);
  free(x);
  
}

bin_heap_t* Delete(bin_heap_t* H, int x)
{
  int min = 0;
  if (H == NULL)
    return H;
  min = H->head->key;
  DecreaseKey(H, x, -(int)INF);
  return ExtractMin(&min, H);
}

void FreeTree(node_t* node)
{
  if (node != NULL)
  {
    FreeTree(node->child);
    FreeTree(node->sibling);
    free(node);
  }
}

void FreeHeap(bin_heap_t* t)
{
  root_t *root = NULL, *tmp = NULL;
  if (t == NULL)
    return;
  root = t->head;
  while (root != NULL)
  {
    FreeTree(root->child);
    tmp = root;
    root = root->next;
    free(tmp);
  }
 
  free(t);
}

