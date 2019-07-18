#include <assert.h>
#include <math.h>
#include "order.h"
#include "digraph.h"
#include "rand.h"
#include "edge_weights.h"
#include "util.h"

// V1
#define triangle_weight(i,j,k) (dg_weight (dg, i, j) + dg_weight (dg, j, k) \
															  + dg_weight (dg, k, i))

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
		
		for (i = 0; i < dg.size; i += 1) {
			for (j = 0; j < dg.size; j += 1) {
				if (!dg_weight (orient, i, j)) {
					continue;
				}
				
				//printf ("edge %d -> %d: ", i, j);
				//printf ("triangle_count: %.1f, ", get_edge_weight (count, i, j));
				//printf ("in(%d): %.1f, in(%d): %.1f, diff: %.1f\n",
				//	i, indegree (dg, i), j, indegree (dg, j), 
				//	indegree (dg, i) - indegree (dg, j));
				
				float val = indegree (dg, i) - indegree (dg, j) + (dg.size -1);
				printf ("%.1f\t%.lf\n", get_edge_weight (count, i, j), val);
			}
		}
		
	}
}