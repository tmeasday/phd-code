#include <assert.h>
#include <string.h>
#include <math.h>
#include "order.h"
#include "digraph.h"
#include "rand.h"
#include "util.h"

order
search (digraph dg, order o)
{
	order best_o = copy_order (o);
	// note that these two are relative to the starting cost
	float best = 0, cost, cost_so_far = 0;
	float temp;
	
	
	int i, j;
	int iterations = dg.size * dg.size * sqrt(dg.size); // ? what should this be?
//	printf ("%d\n", iterations);
	for (i = 0; i < iterations; i++) {
		temp = iterations - i; // FIXME
	
		// ok this is the move we're gonna try
		int from = get_rand (o.size);
		int to   = get_rand (o.size);
		if (from == to) continue;
		
		int dir = (from > to) ? -1 : +1;
	
		cost = 0;
		for (j = from; j != to; j += dir) {
			cost += 2 * dg_weight (dg, o.nodes[from], o.nodes[j+dir]) - 1;
		}
		cost *= dir;
	
		// ok.. do we pass the test?
		if (cost < 0 || 
				get_rand_frac () > exp (-1 * cost / temp)) {
			cost_so_far += cost; // update the cost
			
			// move the node
			for (j = from; j != to; j += dir) {
				int_swap (&o.nodes[j], &o.nodes[j+dir]);
			}

			if (cost_so_far < best) {
				best = cost_so_far;
				free_order (best_o);
				best_o = copy_order (o);
			}
		}
	}
	
	free_order (o);
	return best_o;
}

int
main (int argc, char* argv[])
{
	int initial = FALSE;
	
	if (argc > 1) {
		if (strcmp (argv[1], "-i") == 0) {
			initial = TRUE;
		}
	}

	digraph dg; order o;
	while ((dg = read_digraph (stdin)).size != -1) {
		if (initial) {
			o = read_order (stdin);
			assert (o.size == dg.size);
		} else {
			o = random_order (dg.size);
		}
		
		o = search (dg, o);
		
    print_order (stdout, o);
  }
  return 0;
}
