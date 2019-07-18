#include <stdio.h>
#include <assert.h>
#include <strings.h>
#include "order.h"
#include "digraph.h"
#include "rand.h"
#include "util.h"

/*
   branch_and_bound.c
   Tournaments
   
   Created by Tom Coleman on 2007-07-22.
*/

// not a huge fan of global variables, but the best method I can think of here..
order best;

float
branch_and_bound (digraph dg, order o, float cost, int len, float bound)
{
	// ok so far we've added len nodes for a cost of cost. Best possible is bound.
	if (len == dg.size) {
		// we made it! yay!
		best = copy_order (o);
		return min (cost, bound);
	}
	
	int i, j;
	for (i = 0; i < dg.size; i += 1) {
		// what is the cost of adding this guy
		float new_cost = cost;
		
		for (j = 0; j < len; j += 1) {
			// make sure we haven't already met this node
			if (o.nodes[j] == i) break;

			new_cost += dg_weight (dg, i, o.nodes[j]);
		}
		// we didn't make it to the end, so i was already in there
		if (j != len) continue;
		
		// it isn't worth investigating this avenue
		if (new_cost > bound) continue;
		
		// if we get here, we should probably investigate
		o.nodes[len] = i;
		float new_bound = branch_and_bound (dg, o, new_cost, len+1, bound);
		if (new_bound < bound) {
			bound = new_bound;
		}
	}
	
	return bound;
}


int
main (int argc, char* argv[])
{
	int bounds_given = 0;
	FILE* bounds;
	if (argc == 3) {
		assert (strcmp (argv[1], "-b") == 0);
		bounds_given = 1;
		bounds = fopen (argv[2], "r");
		assert (bounds != NULL);
	}
	
	digraph dg;
	while ((dg = read_digraph (stdin)).size != -1) {
    order o = new_order (dg.size);

		float bound;
		if (bounds_given) {
			assert (fscanf (bounds, "%f", &bound) == 1);
			
			// deal with reading error
			// we are printing 5 decimal places in calc_upsets.c
			bound += 0.00001;
		} else {
			// something like the worst possible cost
			bound = dg.size * (dg.size - 1) / 4;
    }
		branch_and_bound (dg, o, 0, 0, bound);
    
    print_order (stdout, best);
  }
  return 0;
}
