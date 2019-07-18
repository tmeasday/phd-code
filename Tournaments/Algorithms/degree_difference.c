#include <assert.h>
#include <math.h>
#include "order.h"
#include "digraph.h"
#include "rand.h"
#include "edge_weights.h"
#include "util.h"
#include "degree_diff.h"
#include "upsets.h"

int debug = 1;

int
main (int argc, char* argv[])
{
	digraph dg;
  while ((dg = read_digraph (stdin)).size != -1) {
		digraph orient = round_digraph (dg, 0);
	
		degree_diff* dd = new_degree_diff(&dg, &orient);
		
		int iterations = 0;
		// ok, now destroy triangles
		while (1) {
			iterations++;
			
			edge best = get_best_dd_edge (dd);
						
			if (best.from == 0 && best.to == 0) { // there aren't any triangles left
				break;
			}
			
			// let's reverse it
			set_dg_weight (orient, best.from, best.to, 0);
			
//			fprintf (stderr, "reversing the edge from %d to %d\n", best.from, best.to);
			update_degree_diff (dd, best);
		}
		
		order o = digraph_to_order (orient);
		print_order (stdout, o);
		
#ifdef DEBUG
    printf ("%0.1f upsets ", upsets (dg, o));
		printf ("in %d iterations\n", iterations);
#endif
  }
  return 0;
}
