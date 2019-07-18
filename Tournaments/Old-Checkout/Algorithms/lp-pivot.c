#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "digraph.h"
#include "order.h"
#include "rand.h"
#include "util.h"

#include "lp-solve.c"

int choose_pivot (double**, int[], int);
void partition (double**, int[], int, int, int*, int*);
void quicksort (double** solution, int nodes[], int n);

void quicksort (double** solution, int nodes[], int n)
{
  int pivot;
  int first_eq, first_gt;

  if (n <= 1) {
    return;
  }

  pivot = choose_pivot (solution, nodes, n);
  //printf ("pivot is %d\n", pivot);
  partition (solution, nodes, n, pivot, &first_eq, &first_gt);

  quicksort (solution, nodes, first_eq);
  quicksort (solution, nodes+first_gt, n-first_gt);
}

// the algorithm I use here is probably a little too general
// (this probably isn't the end of the world..)
void partition (double** solution, int nodes[], int n,
                int pivot, int* first_eq, int* first_gt)
{
  int i = 0, fe = 0, fg = n;

  while (i < fg) {
    //printf ("%d %d %d, %d to %d\n", i, fe, fg, nodes[i], pivot);
    if (nodes[i] == pivot) {
      i++;
      continue;
    }
    
		
    if (get_rand_frac () < solution[pivot][nodes[i]]) {
      int_swap (&nodes[fe], &nodes[i]);
      fe++;
      i++;
    } else {
      fg--;
      int_swap (&nodes[i], &nodes[fg]);
    } 
  }

  *first_eq = fe;
  *first_gt = fg;
  return;
}

int
choose_pivot (double** solution, int nodes[], int n)
{
  return nodes[get_rand (n)];
}



int 
main (int argc, char const* argv[])
{
	digraph dg;
	while ((dg = read_digraph (stdin)).size != -1) {
		double** solution = solve_digraph (dg);

		order o = sorted_order (dg.size);
		quicksort (solution, o.nodes, dg.size);

		print_order (stdout, o);
	}

	return 0;
}