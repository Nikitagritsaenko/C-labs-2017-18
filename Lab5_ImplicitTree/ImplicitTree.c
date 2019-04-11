#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ImplicitTree.h"

int Random(int a, int b)
{
  double x, xd;
  x = rand();
  xd = (x * (b - a)) / (RAND_MAX + 1) + a;
  return (int)xd;
}

double RandomD(double a, double b)
{
  double x, xd;
  x = rand();
  xd = (x * (b - a)) / (RAND_MAX + 1) + a;
  return xd;
}

FILE *in;

void InitProcess()
{
  in = fopen("rand.bin","rb");
}

void StopProcess()
{
  fclose(in);
}

treap_t* InitImplicitTree(int data)
{
  treap_t* tree = (treap_t*) malloc(sizeof(treap_t));
  fread(&(tree->priority), sizeof(double), 1, in);
  tree->data = data;
  tree->size = 1;
  tree->left = tree->right = NULL; 
  fseek(in, -Random(1,8), SEEK_CUR);
  return tree;
}

int SizeOf(treap_t* treap)
{
  return (treap == NULL) ? 0 : treap->size;
}

void RecalcSize(treap_t* treap)
{
  if (treap == NULL)
    return;
  treap->size = SizeOf(treap->left) + SizeOf(treap->right) + 1;
} 

treap_t* Merge(treap_t* left, treap_t* right)
{
  if (left == NULL)
    return right;
  if (right == NULL)
    return left;

  if (left->priority >= right->priority)
  {
    left->right = Merge(left->right, right);
    RecalcSize(left);
    return left;
  }
  else
  {
    right->left = Merge(left, right->left);
    RecalcSize(right);
    return right;
  }
}

void Split(treap_t* treap, int x, treap_t **outLeft, treap_t **outRight)
{
  int currIndex = 0;
  if (treap == NULL)
  {
    *outLeft = *outRight = NULL;
    return;
  }
  currIndex = SizeOf(treap->left) + 1;
  if (currIndex <= x)
  {
    //split index (x) has changed! (left-tree and root include left-size + 1 elements))
    if (treap->right == NULL)
      *outRight = NULL;
    else
      Split(treap->right, x - currIndex, &(treap->right), outRight);
    *outLeft = treap;
  }
  else
  {
    //split index is still (x)
    if (treap->left == NULL)
      *outLeft = NULL;
    else
      Split(treap->left, x, outLeft, &(treap->left));
    *outRight = treap;
  }
  RecalcSize(treap);
}

treap_t* Add(treap_t* tree, int pos, int value)
{
  treap_t *left = NULL, *right = NULL;
  treap_t* newTree = InitImplicitTree(value);
  Split(tree, pos, &left, &right);
  return Merge(Merge(left, newTree), right);
}

void FreeTree(treap_t* tree)
{
  if (tree != NULL)
  {
    FreeTree(tree->left);
    FreeTree(tree->right);
    free(tree);
  }
}

treap_t* Erase(treap_t* tree, int pos)
{
  treap_t *left = NULL, *right = NULL, *elem = NULL;
  Split(tree, pos, &left, &right);
  Split(right, 1, &elem, &right);
  FreeTree(elem);
  return Merge(left, right);
}

