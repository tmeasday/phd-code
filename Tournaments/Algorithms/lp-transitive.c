#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "digraph.h"
#include "order.h"
#include "rand.h"
#include "util.h"

#include "lp-solve.c"

void
transitize (digraph orient, double** solution)
{
	float min = 1;
	int n_edges = (orient.size * (orient.size - 1)) / 2;
	int min_i[n_edges], min_j[n_edges], n_min = 0;
	int i, j;
	for (i = 0; i < orient.size; i += 1) {
		for (j = 0; j < orient.size; j++) {
			if (i == j) continue;
		
			// this edge is alreadly oriented.
			if (dg_weight (orient, i, j) != 0.5) continue;
			
			float value = solution[i][j];
			if (value < min) {
				min = value;
				min_i[0] = i; min_j[0] = j; n_min = 1;
			} else if (value == min) {
				min_i[n_min] = i; min_j[n_min++] = j;
			}
		}
	}
	
	if (n_min == 0) {
		// we are done
		return;
	}
	
	// choose the minimum
	int selected = get_rand (n_min);
	int sel_i = min_i[selected], sel_j = min_j[selected];
	
	// now set the orientation
	//printf ("adding edge %d -> %d\n", sel_i, sel_j);
	set_dg_weight_transitive (orient, sel_i, sel_j);
	// more recursion where maybe it isn't necessary
	transitize (orient, solution);
}

int 
main (int argc, char const* argv[])
{
	digraph dg;
	while ((dg = read_digraph (stdin)).size != -1) {
		double** solution = solve_digraph (dg);

		digraph orient = new_digraph (dg.size);
		transitize (orient, solution);
		order o = digraph_to_order (orient);

		print_order (stdout, o);
	}

	return 0;
}