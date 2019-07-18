/*
   degree_difference_approx.c
   Tournaments
   
   Created by Tom Coleman on 2007-09-13.
   Copyright 2007 The University of Melbourne. All rights reserved.
*/

#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "order.h"
#include "digraph.h"
#include "edge.h"
#include "util.h"
#include "rand.h"
#include "array.h"

typedef struct {
	float* indegrees;
	int* nodes;
	int* nodes_pos;
	int  size;
	
	int* first_of_indeg;
	int* good_l;
	int  n_good_l;
	int* good_r;
	int  n_good_r;
} indegrees_data;


int
update_list (int list[], int len, int indeg, int add)
{
	// find out if indeg is in the list
	int j; for (j = 0; j < len; j += 1) {
		if (list [j] == indeg) {
			break;
		}
	}
	
	if (add) {
		// only need to add if we aint already in there
		if (j == len) {
			list[j] = indeg; 
			len += 1;
		}
	} else {
		if (j != len) {
			len--;
			while (j < len) {
				list[j] = list[j+1];
				j++;
			}
		}
	}
	
	return len;
}

void
do_good_lists (indegrees_data* data, int from, int to)
{
	int i; for (i = max (from, 1); i <= min (to, data->size-2); i += 1) {
		// is i a good (l|r) indegree??
		int left = 0, right = 0;

		int nof_i = data->first_of_indeg[i+1] - data->first_of_indeg[i];
	
		// either #of indegree (i) > 1
		if (nof_i > 1) {
			left = right = 1;
		}
		
		// or a 1 with a 0 next-door
		else if (nof_i == 1) {
			// do we want to move right
			if (i == data->size - 2 && data->first_of_indeg[i+1] == data->size) {
				right = 1;
			} else if (data->first_of_indeg[i+1] == data->first_of_indeg[i+2]) {
				right = 1;
			}
			
			if (data->first_of_indeg[i] == data->first_of_indeg[i-1]) {
				left = 1;
			}
		}
	
		data->n_good_l = update_list (data->good_l, data->n_good_l, i, left);
		data->n_good_r = update_list (data->good_r, data->n_good_r, i, right);
	}
}


indegrees_data*
new_indegrees_data (digraph orient)
{
	indegrees_data* data = malloc (sizeof (indegrees_data));
	assert (data != NULL);
	
	data->size = orient.size;
	data->nodes = zero_array (orient.size);
	data->nodes_pos = zero_array (orient.size);
	data->indegrees = malloc (sizeof (float) * orient.size);
	assert (data->indegrees != NULL);
	
	int i; for (i = 0; i < data->size; i += 1) {
		data->indegrees[i] = indegree (orient, i);
		data->nodes[i] = i;
		int j; for (j = i; j > 0; j -= 1) {
			if (data->indegrees[i] >= data->indegrees[data->nodes[j-1]]) {
				break;
			}
			int_swap (&data->nodes[j], &data->nodes[j-1]);
			data->nodes_pos[data->nodes[j]] += 1;
		}
		
		// j is the final position of i
		data->nodes_pos[i] = j;
	}
	
	// now work out the heuristic info
	data->first_of_indeg = zero_array (orient.size);
	data->first_of_indeg[0] = 0;
	int pos = 0;
	for (i = 1; i < orient.size; i += 1) {
		if (pos == orient.size) {
			data->first_of_indeg[i] = pos;
			continue;
		}
		// find the first node with indegree i
		while (data->indegrees[data->nodes[pos]] < i) {
			pos += 1;
			if (pos == orient.size) {
				break;
			}
		}
		data->first_of_indeg[i] = pos;
	}
	
	data->good_l = zero_array (orient.size);
	data->n_good_l = 0;
	data->good_r = zero_array (orient.size);
	data->n_good_r = 0;
	
	do_good_lists (data, 1, orient.size-1);
	
	return data;
}


void
update_indegrees_data (indegrees_data* data, edge e)
{
	//  better update the data structure too
	// first move best.to to the left
	data->indegrees[e.to] -= 1;
	int i; for (i = data->nodes_pos[e.to]; i > 0; i -= 1) {
		if (data->indegrees[e.to] >= data->indegrees[data->nodes[i-1]]) {
			break;
		}
		
		int_swap (&data->nodes[i], &data->nodes[i-1]);
		data->nodes_pos[data->nodes[i]] += 1;
	}
	
	// and e.to will end up at i
	data->nodes_pos[e.to] = i;
	
	int y = data->indegrees[e.to];	
	data->first_of_indeg [y+1] += 1;
	do_good_lists (data, y-1, y+2);
	
	// now move best.from right
	data->indegrees[e.from] += 1;
	for (i = data->nodes_pos[e.from]; i < data->size-1; i += 1) {
		if (data->indegrees[e.from] <= data->indegrees[data->nodes[i+1]]) {
			break;
		}
		
		int_swap (&data->nodes[i], &data->nodes[i+1]);
		data->nodes_pos[data->nodes[i]] -= 1;
	}
	
	// and e.from will end up at i
	data->nodes_pos[e.from] = i;
	
	
	y = data->indegrees[e.from];	
	data->first_of_indeg [y] -= 1;
	do_good_lists (data, y-2, y+1);
}

void
print_indegrees_data (FILE* out, indegrees_data* data)
{
//	print_array (out, data->nodes, data->size);
	
	int n_of_size[data->size];
	int i; for (i = 0; i < data->size; i += 1) {
		// could be smarter but who really cares
		n_of_size[i] = 0;
		int j; for (j = 0; j < data->size; j += 1) {
			if (data->indegrees [j] == i) {
				n_of_size[i] += 1;
			}
		}
	}
	
	print_array (out, n_of_size, data->size);
	
	print_array (out, data->good_l, data->n_good_l);
	print_array (out, data->good_r, data->n_good_r);
}

int
first_meaningful (indegrees_data* data, int first)
{
	while (data->indegrees[data->nodes[first]] == first) {
		first += 1;
		if (first == data->size) {
			break;
		}
	}
	
	return first;
}

int
last_meaningful (indegrees_data* data, int last)
{
	while (data->indegrees[data->nodes[last]] == last) {
		last -= 1;
		assert (last > 0);
	}
	
	return last;
}

int
main (int argc, char* argv[])
{
	digraph dg;
  while ((dg = read_digraph (stdin)).size != -1) {
		digraph orient = round_digraph (dg, 0);
	
		// set up our data structure
		indegrees_data* data = new_indegrees_data (orient);
		
		int iterations = 0;
		// ok, now destroy triangles
		while (1) {
			iterations++;
			
			edge best = {0, 0};
			int best_length = -1;
		
#ifdef DEBUG
			print_indegrees_data (stdout, data);
#endif			
			
			// are we done
			if (data->n_good_l == 0) {
				break;
			}
			
			int i; for (i = 0; i < data->n_good_l; i += 1) {
				int to_in = data->good_l[i];
				
				int j; for (j = 0; j < data->n_good_r; j += 1) {
					int from_in = data->good_r[j];
					
					// ok.. is this length interesting?
					int length = from_in - to_in;
					if (length <= best_length) {
						continue; // these guys aren't in order any more
					}
				
					// right. Now let's see if we can find an edge
					int from = data->first_of_indeg[from_in];
					int to   = data->first_of_indeg[to_in];
					int to_start = to;
					int from_stop = data->first_of_indeg[from_in + 1];
					int to_stop   = data->first_of_indeg[to_in + 1];
					
					// length == 0.. we're happy
					if (length == 0) {
						// from == to, etc so we need to increment one
						to += 1;
						if (to == to_stop) { // only one node of this indegree, no good
							continue;
						} 
						
						edge e = (edge) {data->nodes[from], data->nodes[to]};
						if (dg_weight (orient, e.from, e.to)) {
							best = e;
						} else {
							best = reverse_edge (e);
						}
						best_length = length;
						continue;
					}
										
					for (; from < from_stop; from += 1) {
						for (to = to_start; to < to_stop; to += 1) {
							edge e = (edge) {data->nodes[from], data->nodes[to]};
							if (dg_weight (orient, e.from, e.to)) {
								best = e;
								best_length = length;
								break;
							}	
						}
					}
				}
			}
			
			// let's reverse it
			set_dg_weight (orient, best.from, best.to, 0);

#ifdef DEBUG
			fprintf (stdout, "reversing the edge from %d to %d\n", best.from, best.to);
#endif

			update_indegrees_data (data, best);
		}
		
		order o = digraph_to_order (orient);
		print_order (stdout, o);
//		printf ("in %d iterations\n", iterations);
  }
  return 0;
}
