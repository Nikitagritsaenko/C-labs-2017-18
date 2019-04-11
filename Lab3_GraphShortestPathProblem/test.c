#include <stdlib.h>
#include <crtdbg.h>
#include <ctype.h>
#include "spp.h"
#pragma warning(disable:4996)
//-----------------------------------------------------------------
int main(int argc, char const* argv[])
{
  FILE* in = stdin;
  List* path = NULL;
  Graph* gr = NULL;

  int start = 0, end = 0, n = 0, i = 0;

  if (argc == 2) 
  {
    if (freopen_s(&in, argv[1], "r", stdin) != 0)
    {
      printf("ERROR: Can't open input file '%s'", argv[1]);
      return -1;
    }
  }

  n = ReadFile(in, &start, &end);
  gr = ReadGraph(in,n); 
  path = ShortestPathSearch(start, end, n, gr);

  printf("\nPath (%i --> %i): ", start, end); 
  PrintPath(path);

  if (in != stdin)
    fclose(in);
  
  for (i = 1; i <= n; i++)
    FreeList(gr->vertexList[i]);
  free(gr->vertexList);
  free(gr);
  FreeList(path);
  _CrtDumpMemoryLeaks();
  return 0;
}