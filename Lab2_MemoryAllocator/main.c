#include <stdio.h>
#include <malloc.h>
#include <crtdbg.h>
#include "memallocator.h"
#define DATA_SIZE 60
int main()
{
  void *p = 0, *p1 = 0, *p2 = 0, *p3 = 0, *p4 = 0, *p5 = 0;
  void *ptr = malloc(memgetminimumsize() + DATA_SIZE);
  
  if (ptr != NULL)
  {
    meminit(ptr, memgetminimumsize() + DATA_SIZE);
    p = memalloc(10);
    p1 = memalloc(5);
    p2 = memalloc(15);
    memfree(p1);
    //p2 = memalloc(10);
    p3 = memalloc(8); //! not enough memory
    p4 = memalloc(5); //all right
    memfree(p);
    memfree(p2);
    p5 = memalloc(19); 
    memfree(p3);
    memfree(p4);
    memfree(p5);
    free(ptr);
  }
  memdone();
  return 0;
}