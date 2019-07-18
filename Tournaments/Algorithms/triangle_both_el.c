#include <assert.h>
#include <math.h>
#include "order.h"
#include "digraph.h"
#include "rand.h"
#include "edge_weights.h"
#include "util.h"
#include "triangles.h"
#include "upsets.h"

#define ALPHA 1.65
#define MIN_IMP 0.001

#define mult(x) (x*x*x + 0.047)
//#define mult(x) (x*x + 0.15)
//#define mult(x) (x*x*x*x + 0.015)



int
main (int argc, char* argv[])
{
	register_triangle_user (1, 1);
	
	digraph dg;
  while ((dg = read_digraph (stdin)).size != -1) {
		digraph orient = round_digraph (dg, 0);
		init_triangle_scores (dg, orient);
		
		
//		printf ("here\n");
		
		int iterations = 0;
		// ok, now destroy triangles
		while (1) {
			
			iterations++;
			
			edge best = get_best_tc_with_delta ();
						
			// TESTING --
//			float diff = fabs (dg_weight (dg, i, j) - dg_weight (orient, i, j));
//			new_count *= mult (diff);

			if (best.from == 0 && best.to == 0) { // there aren't any triangles left
				break;
			}
			
			// let's reverse it
			set_dg_weight (orient, best.from, best.to, 0);
			
//			fprintf (stderr, "reversing the edge from %d to %d\n", best.from, best.to);
			update_triangle_scores (orient, best);
		}
		
		order o = digraph_to_order (orient);
		print_order (stdout, o);
		
/*		printf ("%0.5f,", upsets (dg, o));
		printf ("in %d iterations\n", iterations);
*/  }
  return 0;
}
