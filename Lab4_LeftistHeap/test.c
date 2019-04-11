#include <stdio.h>
#include <crtdbg.h>
#include "llh.h"
#pragma warning(disable : 4996)

int main(void)
{
  left_list_heap_t* tree = InitLeftListHeap(1);
  int a[50] = {25, 45, 32, 20, 57, 28, 60, 27, 17, 73, 18, 33, 6, 5, 76, 42, 77, 36, 3, 70, 37, 47, 14, 16,
    66, 71, 11, 9, 52, 46, 34, 38, 2, 64, 44, 67, 30, 26, 63, 75, 65, 15, 17, 4, 39, 55, 68, 13, 74, 23};
  int x = 0;
  int i = 0;
  for (i = 0; i < 47; i++)
  {
    Push(tree, a[i]);
    Push(tree, a[i+1]);
    Push(tree, a[i+2]);
    Delete(tree, a[i+1]);
    Push(tree, a[i+3]);
    Delete(tree,a[i]);
    Delete(tree,a[i+3]);
    Delete(tree,a[i+2]);

    Push(tree, a[i+3]);
    Push(tree, a[i+2]);
    Delete(tree,a[i+3]);
    Push(tree, a[i+1]);
    Delete(tree,a[i+2]);
    Push(tree, a[i]);
    Delete(tree,a[i+1]);
    Delete(tree,a[i]);
  }

  FreeTree(tree);
  _CrtDumpMemoryLeaks();
  return 0;
}