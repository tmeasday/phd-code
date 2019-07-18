#include <assert.h>
#include "order.h"
#include "digraph.h"
#include "upsets.h"
#include "rand.h"
#include "util.h"


void insertionsort (digraph dg, int nodes[], int n)
{
	int i, j;
	float cost, min_cost;
	int min_pos;
	
	for (i = 0; i < n; i++)
	{
		// insert node at pos i into list from 0 to i-1
		cost = 0;
		min_cost = 0;
		min_pos  = 0;
		for (j = 0; j < i; j++)
		{
			// if there is an edge from i to j, we would prefer to put i on the left
			cost += (dg_weight (dg, nodes[i], nodes[j]) - 0.5);
			
			if (cost < min_cost) {
				min_cost = cost;
				min_pos = j+1;
			}
		}
		
		// ok we have the best position. Now we have to move nodes[i] there
		for (j = i; j > min_pos; j--) {
			int_swap (&nodes[j], &nodes[j-1]);
		}
	}
}


int
main (int argc, char* argv[])
{
	digraph dg;
	while ((dg = read_digraph (stdin)).size != -1) {
    order o = random_order (dg.size);

		float cost, cost2, cost3;
		
		// very big
		cost = cost2 = cost3 = dg.size * dg.size;
		
		do {
			cost = cost2;
			reverse_order (o);
			do {
				cost2 = cost3;
				insertionsort (dg, o.nodes, dg.size);
				cost3 = upsets (dg, o);
				//printf ("A)cost2: %.2f -> cost3: %.2f\n", cost2, cost3);
			} while (cost2 - cost3 > 0.001);	
			//printf ("B)cost: %.2f -> cost2: %.2f\n", cost, cost2);
		} while (cost - cost2 > 0.001);


    print_order (stdout, o);
  }
  return 0;
}
