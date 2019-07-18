#include <assert.h>
#include "order.h"
#include "digraph.h"
#include "rand.h"

// TODO -- use edge_weights

int** create_delta (int size);
int get_delta (int ** delta, int size, int i , int j);
void inc_delta (int ** delta, int size, int i, int j, int inc);

int
main (int argc, char* argv[])
{
	digraph dg;
	while ((dg = read_digraph (stdin)).size != -1) {
		digraph orient = new_digraph (dg.size);
		
		int** delta = create_delta (dg.size);
		
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
					
					if ((l2r && left) || (!l2r && right)) {
						inc_delta (delta, dg.size, i, k, 1);
					} else if ((l2r && right) || (!l2r && left)) {
						inc_delta (delta, dg.size, i, k, -1);
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
					int new_delta = get_delta (delta, dg.size, i, j);
					
					if (new_delta > max) {
						max = new_delta;
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

				// OK. There are 4 cases to consider
				if (right1 && right2) { // (1)
					inc_delta (delta, dg.size, source, sink, 2);
					inc_delta (delta, dg.size, source, k, 1);
					inc_delta (delta, dg.size, k, sink, 1);
				
				} else if (!right1 && !right2) { // (2)
					inc_delta (delta, dg.size, source, sink, -2);
					inc_delta (delta, dg.size, source, k, -1);
					inc_delta (delta, dg.size, k, sink, -1);
				
				}	else if (right1 && !right2) { // (3)
					inc_delta (delta, dg.size, source, k, +1);
					inc_delta (delta, dg.size, k, sink, -1);
				
				}	 else if (!right1 && right2) { // (4)
					inc_delta (delta, dg.size, source, k, -1);
					inc_delta (delta, dg.size, k, sink, +1);
				}
			}
		}
		
		order o = digraph_to_order (orient);
		print_order (stdout, o);
  }
  return 0;
}


int**
create_delta (int size)
{
	int** delta = malloc (sizeof (int*) * size);
	assert (delta != NULL);
	
	int i, j;
	for (i = 0; i < size; i += 1) {
		delta[i] = malloc (sizeof (int) * size);
		assert (delta[i] != NULL);
		
		for (j = 0; j < size; j += 1) {
			if (i < j) {
				delta [i][j] = 0;
			} else {
				delta [i][j] = -1;
			}

		}
	}
	
	return delta;
}

int get_delta (int ** delta, int size, int i, int j)
{
	int min, max;
	if (i < j) {
		min = i; max = j;
	} else {
		min = j; max = i;
	}
	
	return delta[min][max];
}

void inc_delta (int ** delta, int size, int i, int j, int inc)
{
	int min, max;
	if (i < j) {
		min = i; max = j;
	} else {
		min = j; max = i;
	}

	delta[min][max] += inc;
}