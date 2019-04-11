#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <time.h>
#include <windows.h>
#include <assert.h>
#include "ImplicitTree.h"
#define TREE_SIZE_MIN 100000
#define TREE_SIZE_MAX 1000000
#define SIZE_STEP 100000

typedef struct
{
  LARGE_INTEGER t0, t1;
  LARGE_INTEGER frequency;
} timer_t;


void FillRandom(int arr[], int size, int a, int b)
{
  int i = 0;
  for (i = 0; i < size; i++)
    arr[i] = Random(a, b);
}

void PrintResult(double const arr[], int size)
{
  int i = 0, step = TREE_SIZE_MIN; 
  for (i = 0; i < size; i++)
  {
    //printf("(n = %i; dt = %lf) \n", step, arr[i]);
     printf("%lf\n", arr[i]);
    step += SIZE_STEP;
  }
  printf("\n");
}

void TimerCreate(timer_t* self)
{
  assert(self);
  QueryPerformanceFrequency(&self->frequency);
  QueryPerformanceCounter(&self->t1);
  self->t0 = self->t1;
}

double TimerTick(timer_t* self)
{
  assert(self);
  self->t0 = self->t1;
  QueryPerformanceCounter(&self->t1);
  return 1000.0 * (double)(self->t1.QuadPart - self->t0.QuadPart) / (double)self->frequency.QuadPart;
}


double AddResearch(treap_t** tree, int data[], int size)
{
  timer_t timer;
  int i = 0, j = 0;
  double dt = 0.0;
  for (i = 1; i < SIZE_STEP; i++)
    *tree = Add(*tree, Random(1,i), data[i]);
  TimerCreate(&timer);
  dt = TimerTick(&timer);
  for (j = 1; j <= 10000; j++)
    *tree = Add(*tree, Random(1, size), data[j]);
  dt = TimerTick(&timer);
  return (dt / 10000);
}

double EraseResearch(treap_t** tree, int size)
{
  timer_t timer;
  int i = 0, j = 0;
  double dt = 0.0;
  TimerCreate(&timer);
  dt = TimerTick(&timer);
  for (j = 1; j <= 10000; j++)
    *tree = Erase(*tree, Random(1, size-j));
  dt = TimerTick(&timer);
  return (dt / 10000);
}

void SplitMergeResearch(int data[], int size, double* resMerge, double* resSplit)
{
  treap_t *tree = NULL, *left = NULL, *right = NULL;
  timer_t timer;
  int i = 0, j = 0, r = 0;
  double dt1 = 0.0, dt2 = 0.0;
  //InitProcess();
  TimerCreate(&timer);
  for (i = 1; i < size; i++)
    tree = Add(tree, Random(1,i), data[i]);

  TimerTick(&timer);
  for (j = 1; j <= 100; j++)
  {
    r = Random(1, size); 

    TimerTick(&timer);
    Split(tree, r, &left, &right);
    dt1 += TimerTick(&timer);

    TimerTick(&timer);
    tree = Merge(left, right);  
    dt2 += TimerTick(&timer);
  }  
  *resSplit = dt1 / 100;
  *resMerge = dt2 / 100;
  FreeTree(tree);
  //StopProcess();
}

void AsymptoticsResearch()
{
  int i = 0, size = 0;
  int *data = (int*) malloc(sizeof(int) * (size_t)TREE_SIZE_MAX);
  int stepCount = (TREE_SIZE_MAX - TREE_SIZE_MIN) / SIZE_STEP + 1;
  double *resAdd = (double*) malloc(sizeof(double) * stepCount); 
  double *resErase = (double*) malloc(sizeof(double) * stepCount);  
  double *resSplit = (double*) malloc(sizeof(double) * stepCount);  
  double *resMerge = (double*) malloc(sizeof(double) * stepCount); 
  treap_t* treap = NULL;
  srand((unsigned)time(NULL));
  FillRandom(data, (int)TREE_SIZE_MAX, 1, (int)TREE_SIZE_MAX);

  for (size = TREE_SIZE_MIN; size < TREE_SIZE_MAX; size += SIZE_STEP)
  {
    resAdd[i] = AddResearch(&treap, data, size);
    //resErase[i] = EraseResearch(&treap, size);
    //SplitMergeResearch(data, size, &resMerge[i], &resSplit[i]);
    i++;
  }
  printf("Add\n");
  PrintResult(resAdd, i);
  printf("Erase\n");
  PrintResult(resErase, i);  
  printf("Split\n");
  PrintResult(resSplit, i);
  printf("Merge\n");
  PrintResult(resMerge, i);
  FreeTree(treap);
  free(data);
  free(resAdd);
  free(resSplit);
  free(resMerge);
  free(resErase);
}

int main(void)
{
  InitProcess();
  AsymptoticsResearch();
  _CrtDumpMemoryLeaks();
  StopProcess();
  return 0;
}