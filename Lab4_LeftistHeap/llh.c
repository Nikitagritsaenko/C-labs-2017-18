#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>
#include "llh.h"
#define INF 10e8
#define MIN(a,b) ((a < b) ? (a) : (b))

void Swap(left_list_heap_t** x, left_list_heap_t** y)
{
  left_list_heap_t* tmp = *x;
  *x = *y;
  *y = tmp;
}

left_list_heap_t* Merge(left_list_heap_t* tree1, left_list_heap_t* tree2)
{
  if (tree1 == NULL)
    return tree2;
  if (tree2 == NULL)
    return tree1;
  if (tree1->key > tree2->key)
    Swap(&tree1,&tree2);
  if (tree1->left == NULL)
    tree1->left = tree2;
  else
  {
    tree1->right = Merge(tree1->right, tree2);
    if (tree1->left->dist < tree1->right->dist)
      Swap(&(tree1->left), &(tree1->right));
    tree1->dist = MIN(tree1->left->dist, tree1->right->dist) + 1;
  }
  return tree1;
}

left_list_heap_t* InitLeftListHeap(int x)
{
  left_list_heap_t* tree = (left_list_heap_t*) malloc(sizeof( left_list_heap_t));
  tree->key = x;
  tree->dist = 0;
  tree->left = NULL;
  tree->right = NULL;
  return tree;
}

void Push(left_list_heap_t* tree, int x)
{
  left_list_heap_t* elem = InitLeftListHeap(x);
  tree = Merge(elem, tree);
}

void FreeTree(left_list_heap_t* t)
{
  if (t != NULL)
  {
    FreeTree(t->left);
    FreeTree(t->right);
    free(t);
  }
}

left_list_heap_t* ExtractMin(int *min, left_list_heap_t* tree)
{
  left_list_heap_t* x = NULL;
  left_list_heap_t* y = NULL;
  if (tree == NULL)
    return NULL;
  *min = tree->key;
  x = tree->left;
  y = tree->right;
  tree->left = NULL;
  tree->right = NULL;
  free(tree);
  return Merge(x,y);
}

left_list_heap_t* Search(left_list_heap_t* x, int k, int* found)
{
  static left_list_heap_t* result = NULL;
  if (x != NULL)
  {
    if (((x->left != NULL) && (k == x->left->key)) || ((x->right != NULL) && (k == x->right->key)))
    {
      *found = 1;
      result = x;
    }
    if (!(*found))
      Search(x->left, k, found);
    if (!(*found))
      Search(x->right, k, found);
  }
  return result;
}

left_list_heap_t* SearchParent(left_list_heap_t* tree, int k)
{
  left_list_heap_t* result = NULL;
  int found = 0;
  if (tree == NULL)
    return NULL;
  result = Search(tree, k, &found);
  if (found == 0)
    result = Search(tree->right, k, &found);
  return result;
}

left_list_heap_t* DecreaseKey(left_list_heap_t* tree, int x, int key)
{
  left_list_heap_t* subtree = NULL;
  left_list_heap_t* parent = NULL;
  int dist = 0;
  if (key > tree->key)
    return NULL;
  parent = SearchParent(tree, x); 
  if (parent->left->key == x)
  {
    subtree = parent->left;
    parent->left = NULL;
  }
  else
  {
    subtree = parent->right;
    parent->right = NULL;
  }
  parent->dist = dist;
  subtree->key = key;
  while (parent != tree)
  {
    dist++;
    parent = SearchParent(tree, parent->key);
    if (parent->dist > dist)
      parent->dist = dist;
    if ((parent->left) && (parent->right) && (parent->left->dist < parent->right->dist))
      Swap(&(tree->left), &(tree->right));
  }

  return Merge(tree, subtree);
}

void Delete(left_list_heap_t* tree, int x)
{
  int min = 0;
  left_list_heap_t* y  = NULL;
  if ((tree->left == NULL) && (tree->right == NULL))
  {
    tree->key = 0;
    tree = NULL;
    return;
  }
  tree = DecreaseKey(tree,x,(int)(-INF));
  tree = ExtractMin(&min, tree);
  y = tree->right;
  tree->right = NULL;
  tree = Merge(tree, y);
}