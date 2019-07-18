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
} indegrees_data;


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
		int first = 0, last = orient.size - 1;
		while (1) {
			iterations++;
			
			edge e, best = {0, 0};
			float best_length = -1;
		
#ifdef DEBUG
			print_indegrees_data (stdout, data);
#endif
	
			// ok .. we need to be able to tell if we are done.
			first = first_meaningful (data, first);
			if (first == orient.size) {
				// we're done.. its sorted
				break;
			}
			last = last_meaningful (data, last);
			
			// this is the effective size, when we don't consider the nodes which are 
			// already in position
			int size = last - first + 1;
			//printf ("%d-%d (%d)\n", first, last, size);
		
			while (best.from == 0 && best.to == 0) {
				
				// take a ~log(n)^2 sized sample, with some bias
				int i; for (i = 1; i < size; i *= 2) {
					// now we need to choose one from the given range
					// so choice is in 0 .. size-1
					int choice = (i-1) + get_rand (min (i, size - (i-1)));
					int to = first + choice;
//					printf ("%d: ", to);
				
					int to_size = last - to;
					int j; for (j = 1; j < to_size; j *= 2) {						
						// so choice is in 0 .. to_size-1
						int choice = (j-1) + get_rand (min (j, to_size - (j-1)));

						int from = last - choice;
//						printf ("%d, ", from);

						
						e.to = data->nodes[to];
						e.from = data->nodes[from];
					
						// ok.. is it good?
						float length = data->indegrees[e.from] - data->indegrees[e.to];
						length *= edge_mult (dg, orient, e);
						if (length <= best_length) {
							continue; // sadly no longer true when using multipliers
//							break;  // we aint doing better for this e.to
						}
					
						// awesome. we found an edge
						if (length == 0) {
							if (dg_weight (orient, e.from, e.to)) {
								best = e;
							} else {
								best = reverse_edge (e);
							}
							best_length = length;
						} else if (dg_weight (orient, e.from, e.to)) {
							best = e;
							best_length = length;
						}
					}
//					printf ("\n");
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
