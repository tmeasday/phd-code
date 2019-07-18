#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int get_rand (int range)
{
  return (int)((double)rand() / ((double)RAND_MAX +1) * range);
}

int main (int argc, char *argv[])
{
  srand (time (NULL));

  int i, j;
  int n = atoi (argv [1]); // length of sequence
  int m = atoi (argv [2]); // # of runs

  printf ("n = %d, m = %d\n", n, m);

  int *results = malloc (sizeof (int) * n+1);
  for (i = 0; i < n+1; i++)
  {
    results [i] = 0;
  }

  for (i = 0; i < m; i++)
  {
    int sum = 0;
    for (j = 0; j < n; j++) 
    {
      if ( get_rand (2) ) {
        sum++;
      }
    }
    results [sum] += 1;
  }

  for (i = 0; i < n+1; i++)
  {
    printf ("%2d: %d\n", i, results [i]);
  }
  return 0;
}


