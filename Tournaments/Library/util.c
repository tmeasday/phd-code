#include <stdlib.h>
#include <assert.h>

void int_swap (int* a, int* b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

void float_swap (float* a, float* b) {
  float temp = *a;
  *a = *b;
  *b = temp;
}

void pointer_swap (void** a, void** b) {
  void* temp = *a;
  *a = *b;
  *b = temp;
}

void first_n (int array[], int n)
{
  int i;
  for (i = 0; i < n; i++)
  {
    array[i] = i;
  }
}

int count_ones (int xs[], int length)
{
  int i, count = 0;
  for (i = 0; i < length; i++) {
    if (xs[i]) {
      count++;
    }
  }
  return count;
}

int in_array (int needle, int array[], int n)
{
  int i;
  for (i = 0; i < n; i++) {
    if (array[i] == needle) {
      return 1;
    }
  }
  return 0;
}

void 
array_copy (int A[], int B[], int n)
{
  int i;
  for (i = 0; i < n; i++) {
    B[i] = A[i];
  }
}

int*
array_duplicate (int old[], int n)
{
  int* new = malloc (sizeof(int) * n);
  assert (new != NULL);

  array_copy (old, new, n);

  return new;
}

void
fill (int array[], int size, int fill)
{
  int i;
  for (i = 0; i < size; i++)
  {
    array[i] = fill;
  }	
}

void
zeroes (int array[], int size)
{
	fill (array, size, 0);
}
