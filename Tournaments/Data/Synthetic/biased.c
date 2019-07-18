/* 
 * Generate tournaments that are biased towards some particular ordering 
 * Note: doesn't use tournament library (but should)
 */

#include <stdio.h>
#include <stdlib.h>
#include "rand.h"

typedef int* order;
typedef double** tourn;

int get_rand (int);
order get_rand_order (int);
void print_order (order, int);
tourn get_rand_tourn (order, int, double);
void print_tourn (tourn, int);
double get_rand_frac ();

int main (int argc, char *argv[])
{
  int n = atoi (argv[1]);
  int m = atoi (argv[2]);
  double p = atof (argv[3]);
  int i;
  tourn t;

  for (i = 0; i < m; i++) {
    order o = get_rand_order (n);
    print_order(o, n);

    t = get_rand_tourn (o, n, p);
    print_tourn (t, n);
  }

  return 0;
}


order get_rand_order (int size)
{
  int i, n;
  order o = malloc (sizeof (int) * size);

  for (i = 0; i < size; i++) {
    o[i] = -1;
  }

  // tell me if there is a better way here
  for (i = 0; i < size; i++) 
  {
    do { n = get_rand (size);
    } while (o[n] != -1);

    o[n] = i;
  }

  return o;
}

tourn get_rand_tourn (order o, int size, double p)
{
  int i, j;
  tourn t = malloc (sizeof (double*) * size);
  double weight;

  for (i = 0; i < size; i++) {
    t[i] = malloc (sizeof (double) * (size - i - 1));
    for (j = i+1; j < size; j++) {
      if (o[i] < o[j]) {
        weight = p; 
      } else {
        weight = 1-p;
      }
      
      if (get_rand_frac () < weight) {
        t[i][j-i-1] = 1;
      } else {
        t[i][j-i-1] = 0;
      }
    }
  }

  return t;
}

void print_tourn (tourn t, int size)
{
  int i, j;

  printf ("[");
  for (i = 0; i < size; i++) {
    printf ("[");
    for (j = 0; j < size - i - 1; j++) {
      printf ("%0.2f", t[i][j]);
      if ( j < size - i - 2) {
        printf (", ");
      }
    }
    printf ("]");
    if ( i < size - 1) {
      printf (", ");
    }
  }
  printf ("]\n");
}

// order is stored as a reverse permutation (makes making the tourn easier)
void print_order (order o, int size)
{
  int i, pos;

  printf ("[");
  for (i = 0; i < size; i++) {
    for (pos = 0; o [pos] != i; pos++);
    printf ("%d", pos);

    if (i < size - 1) {
      printf (", ");
    } else {
      printf ("]\t");
    }
  }
}

