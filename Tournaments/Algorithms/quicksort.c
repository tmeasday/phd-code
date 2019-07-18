#include <assert.h>
#include "order.h"
#include "digraph.h"
#include "rand.h"
#include "util.h"

int choose_pivot (digraph, int[], int);
void partition (digraph, int[], int, int, int*, int*);

void quicksort (digraph dg, int nodes[], int n)
{
  int pivot;
  int first_eq, first_gt;

  if (n <= 1) {
    return;
  }

  pivot = choose_pivot (dg, nodes, n);
  //printf ("pivot is %d\n", pivot);
  partition (dg, nodes, n, pivot, &first_eq, &first_gt);

  quicksort (dg, nodes, first_eq);
  quicksort (dg, nodes+first_gt, n-first_gt);
}

// the algorithm I use here is probably a little too general
// (this probably isn't the end of the world..)
void partition (digraph dg, int nodes[], int n,
                int pivot, int* first_eq, int* first_gt)
{
  int i = 0, fe = 0, fg = n;
  float cmp;

  while (i < fg) {
    //printf ("%d %d %d, %d to %d\n", i, fe, fg, nodes[i], pivot);
    if (nodes[i] == pivot) {
      i++;
      continue;
    }
    
    if ((cmp = dg_weight(dg, nodes[i], pivot)) == 0.5) {
      // we need to break ties arbitrarily here
      cmp = get_rand_frac ();
    } 

    // the edge from i to pivot is > 1/2, we should make sure i is left
    if (cmp > 0.5) {
      int_swap (&nodes[fe], &nodes[i]);
      fe++;
      i++;
    } else if (cmp < 0.5) {
      fg--;
      int_swap (&nodes[i], &nodes[fg]);
    } 
  }

  *first_eq = fe;
  *first_gt = fg;
  return;
}

int
choose_pivot (digraph dg, int nodes[], int n)
{
  return nodes[get_rand (n)];
}

int
main (int argc, char* argv[])
{
  int i;

	digraph dg;
	while ((dg = read_digraph (stdin)).size != -1) {
    order o = new_order (dg.size);

    for (i = 0; i < o.size; i++) {
      o.nodes [i] = i;
    }
    
    quicksort (dg, o.nodes, dg.size);

    
    
    print_order (stdout, o);
  }
  return 0;
}
