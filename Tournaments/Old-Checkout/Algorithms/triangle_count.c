#include <assert.h>
#include "order.h"
#include "digraph.h"
#include "rand.h"

// TODO -- use edge_weights

int
main (int argc, char* argv[])
{
	digraph dg;
	while ((dg = read_digraph (stdin)).size != -1) {
		digraph orient = new_digraph (dg.size);
		
		int i, j, k;		
		// initialise the count and the orientation
		// FIXME -- could be a waste of memory
		int triangle_count[dg.size][dg.size]; // TODO -- should be floats
		// helpful for debugging
		for (i = 0; i < dg.size; i += 1) {
			for (j = 0; j < dg.size; j += 1) {
				triangle_count[i][j] = -1;
			}
		}
		
		
		for (i = 0; i < dg.size; i += 1) {
			for (j = i+1; j < dg.size; j += 1) {
				triangle_count[i][j] = 0;
				
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
		for (i = 0; i < dg.size; i += 1) {
			for (j = i+1; j < dg.size; j += 1) {
				for (k = j+1; k < dg.size; k += 1) {
					int tri1 = (dg_weight (orient, i, j) && dg_weight(orient, j, k) && 
										  dg_weight (orient, k, i));
					int tri2 = (dg_weight (orient, j, i) && dg_weight(orient, k, j) && 
											dg_weight (orient, i, k));
					if (tri1 || tri2) {
						triangle_count[i][j] += 1;
						triangle_count[i][k] += 1;
						triangle_count[j][k] += 1;
					}
				}
			}
		}
		
		// ok, now destroy triangles
		while (1) {
			// find the maximum triangle count
			int max = 0, max_i = 0, max_j = 0;
			for (i = 0; i < dg.size; i += 1) {
				for (j = i+1; j < dg.size; j += 1) {
					if (triangle_count[i][j] > max) {
						max = triangle_count[i][j];
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
						
			for (k = 0; k < dg.size; k += 1) {
				if (k == max_i || k == max_j) continue;
				
				int left  = (dg_weight (orient, k, max_i) && dg_weight (orient, max_j, k));
				int right = (dg_weight (orient, max_i, k) && dg_weight (orient, k, max_j));

				int min, med, max;
				if (k < max_i) {
					min = k; med = max_i; max = max_j;
				} else {
					min = max_i;
					if (k < max_j) {
						med = k; max = max_j;
					} else {
						med = max_j; max = k;
					}
				}

				// a current triangle
				if ((left && l2r) || (right && !l2r)) {
					// decrease the counts
					triangle_count[min][med] -= 1;
					triangle_count[min][max] -= 1;
					triangle_count[med][max] -= 1;
				} 
				// will now be a triangle
				if ((right && l2r) || (left && !l2r)) {
					// increase the counts
					triangle_count[min][med] += 1;
					triangle_count[min][max] += 1;
					triangle_count[med][max] += 1;
				}
			}
		}
		
		order o = digraph_to_order (orient);
		print_order (stdout, o);
  }
  return 0;
}
