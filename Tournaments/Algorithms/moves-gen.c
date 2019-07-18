#include <assert.h>
#include <string.h>
#include "order.h"
#include "digraph.h"
#include "rand.h"
#include "util.h"

#define MIN_IMP 0.001

typedef struct {
	int to;
	float cost;
} move;

move 
move_left (digraph dg, order o, int from)
{
	// current best improvement
	float best = 0;
	int best_to[dg.size], n_best = 0;

	float cost = 0;
	int to; for (to = from - 1; to >= 0; to -= 1) {
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
	
	if (best != 0) {
		return (move) {best_to[get_rand (n_best)], best};
	} else {
		return (move) {from, 0};
	}
}

move
move_right (digraph dg, order o, int from)
{
	// current best improvement
	float best = 0;
	int best_to[dg.size], n_best = 0;

	float cost = 0;
	int to; for (to = from + 1; to < dg.size; to += 1) {
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
	
	if (best != 0) {
		return (move) {best_to[get_rand (n_best)], best};
	} else {
		return (move) {from, 0};
	}
}


order
search (digraph dg, order o)
{
	int from, to, i;

	int improved = 1;
	while (improved) {
		improved = 0;
		
		int improved_l = 1;
		while (improved_l) {
			improved_l = 0;
			
			for (from = 0; from < dg.size; from += 1) {
				move left = move_left (dg, o, from);
				// next choice of from
				if (left.cost > -MIN_IMP) { // can't be improved
					continue;
				} 

				improved = improved_l = 1;

				to = left.to;
				// move best_from to best_to
				for (i = from; i > to; i -= 1) {
					int_swap (&o.nodes[i], &o.nodes[i-1]);
				}
			}
		}


		int improved_r = 1;
		while (improved_r) {
		
			improved_r = 0;
			for (from = dg.size-1; from >= 0; from -= 1) {
				move right = move_right (dg, o, from);
				// next choice of from
				if (right.cost > -MIN_IMP) { // can't be improved
					continue;
				} 
				improved = improved_r = 1;
			
				to = right.to;
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
