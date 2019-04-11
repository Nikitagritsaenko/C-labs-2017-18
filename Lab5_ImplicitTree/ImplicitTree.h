#ifndef __IMPLICIT_TREE_H__36AF8EC9
#define __IMPLICIT_TREE_H__36AF8EC9

typedef struct ImplicitTreap
{
  double priority;
  int size;
  int data;
  struct ImplicitTreap* left;
  struct ImplicitTreap* right;
} treap_t;


int Random(int a, int b);

treap_t* InitImplicitTree(int data);

treap_t* Merge(treap_t* left, treap_t* right);

void Split(treap_t* treap, int x, treap_t **outLeft, treap_t **outRight);

treap_t* Add(treap_t* tree, int pos, int value);

treap_t* Erase(treap_t* tree, int pos);

void FreeTree(treap_t* tree);

void InitProcess();

void StopProcess();

#endif