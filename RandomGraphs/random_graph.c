#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int n_cliques (int ** adj, int k);

int get_rand (int range)
{
  return (int)((double)rand() / ((double)RAND_MAX +1) * range);
}

int main (int argc, char *argv [])
{
  srand (time (NULL));

  int n = atoi (argv [1]);
  int m = atoi (argv [2]);
  float p = 0.5; // ok we wont actually use this right now
  int i, j;
  
  int ** adj = malloc (sizeof (int *) * (n-1));

  for (i = 0; i < n-1; i ++) {
    adj [i] = malloc (sizeof (int) * (n-i-1));
  }

  for (i = 0; i < n-1; i ++) {
    for (j = 0; j < n - i - 1; j ++) {
      adj [i] [j] = get_rand (2)
      printf ("%d\t", adj [i] [j]);
    }
    printf("\n");
  }

  // ok now lets do some kind of back-tracking gww style search

  return 0;
}

// man i should have written this in haskell.. oh well
int n_cliques (

