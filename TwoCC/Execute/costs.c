#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "cost.h"

int
main (int argc, char* argv[])
{
  assert (argc == 3);
  
  int nprob = atoi (argv [1]);
  int nalgs = atoi (argv [2]);
  int i, j;

	int costs[nprob][nalgs];
  for (i = 0; i < nprob; i++) {
    sigraph sg = read_sigraph (stdin);
	  for (j = 0; j < nalgs; j++) {
		    labelling l = read_labelling (stdin);
				costs[i][j] = cost (sg, l);
				free_labelling (l);
		}
		free_sigraph (sg);
	}

	for (i = 0; i < nprob; i++) {
		for (j = 0; j < nalgs; j++) {
			if (j > 0) {
				printf (", ");
			}
	    printf ("%d", costs[i][j]);		
		}
		printf ("\n");
	}

  return 0;
}
