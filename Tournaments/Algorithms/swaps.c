#include <assert.h>
#include <strings.h>
#include "order.h"
#include "digraph.h"
#include "rand.h"
#include "util.h"

order
search (digraph dg, order o)
{
	int i, j;
	
	// first up calculate move costs
	float move_cost[dg.size][dg.size];
	// this could perhaps be done more efficiently
	for (i = 0; i < dg.size; i += 1) {
		move_cost[i][i] = 0;
		// fill in left moves
		for (j = i-1; j >= 0; j -= 1) {
			move_cost[i][j] = move_cost[i][j+1] 
				+ (2 * dg_weight (dg, o.nodes[i], o.nodes[j]) - 1);
		}
		// now the right moves
		for (j = i+1; j < dg.size; j += 1) {
			move_cost[i][j] = move_cost[i][j-1]
				+ (2 * dg_weight (dg, o.nodes[j], o.nodes[i]) - 1);
		}
	}
	
	// search for the best moves
	float best = 0;
	int best_i[dg.size], best_j[dg.size], n_best = 0;
	for (i = 0; i < dg.size; i += 1) {
		for (j = i+1; j < dg.size; j += 1) {
			float cost = move_cost[i][j] + move_cost[j][i];
			
			if (cost > best) { // new best
				best = cost;
				best_i[0] = i; best_j[0] = j; n_best = 0;
			
			} else if (cost == best) { // new equal best
				best_i[n_best] = i; best_j[n_best++] = j;
			}
		}
	}
	
	if (best == 0) { // can't be improved
		return o;
	} else {
		int choice = get_rand (n_best);
		int_swap (&o.nodes[best_i[choice]], &o.nodes[best_j[choice]]);
	}
	// doesn't actually have to be recursive, but hey? why the hell not
	return search (dg, o);
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
