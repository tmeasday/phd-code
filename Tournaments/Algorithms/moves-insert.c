#include <assert.h>
#include <string.h>
#include "order.h"
#include "digraph.h"
#include "rand.h"
#include "util.h"

#define MIN_IMP 0.001

order
search (digraph dg, order o)
{
	int improved = 1;
	while (improved) {
		int from, to, i;
		
		improved = 0;
	
		// TODO -- could it be better to pick nodes in some other order?
		for (from = 0; from < dg.size; from += 1) {
			// current best improvement
			float best = 0;
			int best_to[dg.size], n_best = 0;
		
			// ok we are going to move the node in position from. 
			// lets try to be a little smart about this
			// first we'll look at moving it left
			float cost = 0;
			for (to = from - 1; to >= 0; to -= 1) {
				cost += 2 * dg_weight (dg, o.nodes[to], o.nodes[from]) - 1;
			
				if (cost < best) {
					// new best
					best = cost;
					best_to[0] = to; n_best = 1;
				} else if (cost == best) {
					best_to[n_best++] = to;
				} else {
					// there are only to more spots, so we can only get that much better
					if (cost - best > to) {
						break;
					}
				}
			}
		
			// ok now the same thing to the right
			cost = 0;
			for (to = from + 1; to < dg.size; to += 1) {
				cost += 2 * dg_weight (dg, o.nodes[from], o.nodes[to]) - 1;
			
				if (cost < best) {
					// new best
					best = cost;
					best_to[0] = to; n_best = 1;
				} else if (cost == best) {
					best_to[n_best++] = to;
				} else {
					// there are only (dg.size - (to+1)) more spots
					if (cost - best > dg.size - (to+1)) {
						break;
					}
				}
			}
		
			// next choice of from
			if (best > -MIN_IMP) { // can't be improved
				continue;
			} 

			improved = 1;

			int choice = get_rand (n_best);
			to = best_to[choice];

		//printf ("moving node %d from %d to %d\n", o.nodes[from], from, to);

			// move best_from to best_to
			if ( from > to ) {
				for (i = from; i > to; i -= 1) {
					int_swap (&o.nodes[i], &o.nodes[i-1]);
				}
			} else {
				for (i = from; i < to; i += 1) {
					int_swap (&o.nodes[i], &o.nodes[i+1]);
				}
			}
			
		}
	}
	return o;
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
