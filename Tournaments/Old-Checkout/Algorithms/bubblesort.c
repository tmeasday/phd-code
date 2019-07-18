#include <assert.h>
#include "order.h"
#include "digraph.h"
#include "rand.h"
#include "util.h"


void bubblesort (digraph dg, int nodes[], int n)
{
	int i, changed = 1;
	
	while (changed) {
		changed = 0;
		for (i = 0; i < n-1; i++) {
			float cmp = dg_weight (dg, nodes[i], nodes[i+1]);
			
			// if cmp == 0.5 should it be random? (could run forever...)

			// if i+1 is pointing to i, they should be swapped
			if (cmp < 0.5) {
				int_swap (&nodes[i], &nodes[i+1]);
				changed = 1;
			}
		}
	}
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
    
    bubblesort (dg, o.nodes, dg.size);

    print_order (stdout, o);
  }
  return 0;
}
