#include <assert.h>
#include <math.h>
#include "order.h"
#include "digraph.h"
#include "rand.h"
#include "edge_weights.h"
#include "util.h"

#define ALPHA 1.65
#define MIN_IMP 0.001

#define mult(x) (x*x*x + 0.047)
//#define mult(x) (x*x + 0.15)
//#define mult(x) (x*x*x*x + 0.015)


#define net_weight(i,j) (dg_weight (orient, i, j) * dg_weight (dg, i, j))

// the assumption is that i,j,k is (or could be) a triangle under orient

// V0
//#define triangle_weight(i,j,k) 1

// V1
#define triangle_weight(i,j,k) (dg_weight (dg, i, j) + dg_weight (dg, j, k) \
															  + dg_weight (dg, k, i))

// V2
//#define triangle_weight(i,j,k) (max (max (dg_weight (dg, i, j), \
//                                dg_weight (dg, j, k)), dg_weight(dg, k, i)))



int
main (int argc, char* argv[])
{
	digraph dg;
  while ((dg = read_digraph (stdin)).size != -1) {
		digraph orient = new_digraph (dg.size);
		edge_weights delta = create_edge_weights (dg.size);
		edge_weights count = create_edge_weights (dg.size);
		
		int i, j, k;		
		
		for (i = 0; i < dg.size; i += 1) {
			for (j = i+1; j < dg.size; j += 1) {
				float weight = dg_weight (dg, i, j);
				if (weight == 0.5) {
					weight = get_rand_frac ();
				}
				if (weight > 0.5) {
					set_dg_weight (orient, i, j, 1);
				} else{
					set_dg_weight (orient, j, i, 1);
				}
			}
		}
					
		// count the initial number of triangles
		// a little harder this time
		for (i = 0; i < dg.size; i += 1) {
			for (j = 0; j < dg.size; j += 1) {
				for (k = i+1; k < dg.size; k += 1) {
					if (i == j || i == k || k == j) continue;
					
					int l2r = dg_weight (orient, i, k);
					
					int right = dg_weight (orient, i, j) && dg_weight(orient, j, k);
					int left  = dg_weight (orient, j, i) && dg_weight(orient, k, j);


					float tw;
					if (left) {
						tw = triangle_weight (i, k, j);							
					} else { // if right
						tw = triangle_weight (i, j, k);
					}
										
					if ((l2r && left) || (!l2r && right)) {
						inc_edge_weight (delta, i, k, tw);
						inc_edge_weight (count, i, k, tw);
					} else if ((l2r && right) || (!l2r && left)) {
						inc_edge_weight (delta, i, k, -tw);
					}
				}
			}
		}
		
		// ok, now destroy triangles
		while (1) {
			// find the maximum triangle count
			float max = 0;
			int max_i = 0, max_j = 0;
			for (i = 0; i < dg.size; i += 1) {
				for (j = i+1; j < dg.size; j += 1) {
					// FIXME -- strictly speaking this is incorrect
					if (get_edge_weight (delta, i, j) <= MIN_IMP) {
						// skip this one.
						continue;
					}
					
					float new_count = get_edge_weight (count, i, j);
					
					// TESTING --
					float diff = fabs (dg_weight (dg, i, j) - dg_weight (orient, i, j));
					new_count *= mult (diff);
					
//					if (diff != 0) { fprintf (stderr, "diff != 0!!\n"); }
					
					if (new_count > max) {
						max = new_count;
						max_i = i; max_j = j;
					}	
				}
			}
			
			if (max == 0) { // there aren't any triangles left
				break;
			}
			
			// ok let's kill some triangles (hopefully)
			int l2r = dg_weight (orient, max_i, max_j); // is the edge the way we expect
			// let's reverse it
			set_dg_weight (orient, max_i, max_j, 1-l2r);
			
//			fprintf (stderr, "reversing the edge from %d to %d\n", max_i, max_j);
			
			// ok. Here source is the start of the edge of interest, sink is the end.
			int source, sink;
			if (l2r) {
				source = max_i; sink = max_j;
			} else{
				source = max_j; sink = max_i;
			}
						
			for (k = 0; k < dg.size; k += 1) {
				if (k == source || k == sink) continue;

				int right1 = dg_weight (orient, source, k);
				int right2 = dg_weight (orient, k, sink);

				float tw1 = triangle_weight (source, k, sink);
				float tw2 = triangle_weight (source, sink, k);

				// OK. There are 4 cases to consider
				if (right1 && right2) { // (1)
					inc_edge_weight (delta, source, sink, 2 * tw1);
					inc_edge_weight (delta, source, k, tw1);
					inc_edge_weight (delta, k, sink, tw1);
				
					inc_edge_weight (count, source, sink, tw1);
					inc_edge_weight (count, source, k, tw1);
					inc_edge_weight (count, k, sink, tw1);
					
				} else if (!right1 && !right2) { // (2)
					inc_edge_weight (delta, source, sink, -2 * tw2);
					inc_edge_weight (delta, source, k, -1 * tw2);
					inc_edge_weight (delta, k, sink, -1 * tw2);
				
					inc_edge_weight (count, source, sink, -1 * tw2);
					inc_edge_weight (count, source, k, -1 * tw2);
					inc_edge_weight (count, k, sink, -1 * tw2);
				
				}	else if (right1 && !right2) { // (3)
					inc_edge_weight (delta, source, k, +1 * tw2);
					inc_edge_weight (delta, k, sink, -1 * tw1);
				
				}	 else if (!right1 && right2) { // (4)
					inc_edge_weight (delta, source, k, -1 * tw1);
					inc_edge_weight (delta, k, sink, +1 * tw2);
				}
			}
		}
		
		order o = digraph_to_order (orient);
		print_order (stdout, o);
  }
  return 0;
}