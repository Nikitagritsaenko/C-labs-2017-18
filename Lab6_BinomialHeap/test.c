#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>
#include "BinomialHeap.h"

int main(void)
{
  int i = 0;
  int keys[10] = {45,4,5,2,7,9,10,3,66,74};
  bin_heap_t* H = InitBinomialHeap();
  //1. test ADD
  for (i = 0; i <= 3; i++)
    H = Add(H,keys[i]);
  //2. test GetMin
  i = GetMinimum(H);
  //3. test Delete
  for (i = 3; i >= 0; i--)
    H = Delete(H,keys[i]);
  //4. test Free
  FreeHeap(H);
  _CrtDumpMemoryLeaks();
  return 0;
}