#include <assert.h>
#include <string.h>
#include "order.h"
#include "digraph.h"
#include "upsets.h"
#include "rand.h"
#include "util.h"
#include "insertionsort.h"


int
main (int argc, char* argv[])
{
	int initial = FALSE;
	
	if (argc > 1) {
		if (strcmp (argv[1], "-i") == 0) {
			initial = TRUE;
		}
	}

	FILE* out = fopen ("Temp/insertionsort_starting_orders_reps", "w");
	
	digraph dg; order o;
	while ((dg = read_digraph (stdin)).size != -1) {
		float cost, cost2, cost3;

		if (initial) {
			o = read_order (stdin);
			assert (o.size == dg.size);
			cost = cost2 = cost3 = upsets (dg, o);
		} else {
			o = random_order (dg.size);
			// very big
			cost = cost2 = cost3 = dg.size * dg.size;
			print_order (out, o);
		}
		
		
		int calls = 0;
		do {
			cost = cost2;
			reverse_order (o);

			insertionsort (dg, o.nodes, dg.size);
			calls += 1;
			do {
				cost2 = cost3;
				
				insertionsort (dg, o.nodes, dg.size);
				calls += 1;
				
				cost3 = upsets (dg, o);
				//printf ("A)cost2: %.2f -> cost3: %.2f\n", cost2, cost3);
			} while (cost2 - cost3 > 0.001);	
			//printf ("B)cost: %.2f -> cost2: %.2f\n", cost, cost2);
		} while (cost - cost2 > 0.001);


    print_order (stdout, o);

		//printf ("calls: %d\n", calls);
  }

	fclose (out);
  return 0;
}
