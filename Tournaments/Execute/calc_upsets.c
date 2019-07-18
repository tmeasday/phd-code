#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "upsets.h"

int
main (int argc, char* argv[])
{
  assert (argc == 3);
  
  int ntourn = atoi (argv [1]);
  int nalgs = atoi (argv [2]);
  int i, j;

  for (i = 0; i < ntourn; i++) {
    digraph dg = read_digraph (stdin);

    for (j = 0; j < nalgs; j++) {
      order o = read_order (stdin);
			// NOTE: change this and change branch_and_bound.c
      printf ("%0.5f,", upsets (dg, o));
    }
    printf ("\n");
  }

  return 0;
}
