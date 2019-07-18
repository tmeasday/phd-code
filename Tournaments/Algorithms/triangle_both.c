#include <assert.h>
#include <math.h>
#include "order.h"
#include "digraph.h"
#include "rand.h"
#include "edge_weights.h"
#include "util.h"
#include "triangles.h"

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
			
			// find the maximum triangle count
			float max = 0;
			int max_i = 0, max_j = 0;
			int i, j;
			for (i = 0; i < dg.size; i += 1) {
				for (j = i+1; j < dg.size; j += 1) {
					// FIXME -- strictly speaking this is incorrect
					edge e = {i, j};
					if (get_triangle_delta (e) <= MIN_IMP) {
						// skip this one.
						continue;
					}
					
					float new_count = get_triangle_count (e);
					
					// TESTING --
					float diff = fabs (dg_weight (dg, i, j) - dg_weight (orient, i, j));
//					new_count *= mult (diff);
					
//					if (diff != 0) { fprintf (stderr, "diff != 0!!\n"); }
					
					if (new_count > max) {
						max = new_count;
						max_i = i; max_j = j;
					}	
				}
			}
			
			if (max <= 0.001) { // there aren't any triangles left
				break;
			}
			
			// ok let's kill some triangles (hopefully)
			int l2r = dg_weight (orient, max_i, max_j); // is the edge the way we expect
			// let's reverse it
			set_dg_weight (orient, max_i, max_j, 1-l2r);
			
			//fprintf (stderr, "reversing the edge from %d to %d\n", max_i, max_j);
			
			// ok. Here source is the start of the edge of interest, sink is the end.
			int source, sink;
			if (l2r) {
				source = max_i; sink = max_j;
			} else{
				source = max_j; sink = max_i;
			}

			update_triangle_scores (orient, (edge) {source, sink});
		}
		
		order o = digraph_to_order (orient);
		print_order (stdout, o);
//		printf ("in %d iterations\n", iterations);
  }
  return 0;
}
