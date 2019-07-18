/*
   degree_diff.c
   Tournaments
   
   Created by Tom Coleman on 2007-09-06.
   Copyright 2007 The University of Melbourne. All rights reserved.
*/

#include <stdlib.h>
#include <assert.h>

#include "degree_diff.h"
#include "degrees.h"
#include "edge_list.h"
#include "edge.h"
#include "digraph.h"
#include "dllist.h"

degree_diff*
new_degree_diff (digraph* dg, digraph* orient)
{
	degree_diff* dd = malloc (sizeof (degree_diff));
	assert (dd != NULL);
	
	dd->dg     = dg;
	dd->orient = orient;
	
	// some wasted effort here but probably doesn't really matter--todo
	dd->indegrees = new_degrees (orient->size);
	int i; for (i = 0; i < orient->size; i += 1) {
		set_degree (dd->indegrees, i, indegree (*orient, i));
	}
	
	// now, intialize our edge bucket data structure
	dd->ddlist    = new_edge_list ();
	int n_edges = orient->size * (orient->size - 1);
	
	dd->edges = malloc (sizeof (dlnode_t*) * n_edges);
	assert (dd->edges != NULL);
	
	for (i = n_edges-1; i >= 0; i -= 1) {
		edge e = get_edge (i, orient->size);
		if (dg_weight (*orient, e.from, e.to)) {
			float degree_diff = dd->indegrees->degs[e.from] - dd->indegrees->degs[e.to];
			
			degree_diff *= edge_mult (*dd->dg, *dd->orient, e);
			
			dd->edges[i] = insert_into_edge_list (dd->ddlist, e, degree_diff);
		} else {
			dd->edges[i] = NULL;
		}	
	}
	
	return dd;
}

// e is being turned
void 
update_degree_diff (degree_diff* dd, edge e)
{

#ifdef DEBUG
		{int edge_num = edge_no (e, dd->orient->size);
		 edge_list_data* data = dd->edges[edge_num]->data;
			
			fprintf (stdout, "reversing %d -> %d[%.0f]:\n", e.from, e.to, data->key);
			print_degrees (dd->indegrees, dd->orient->size);
			
			int i; for (i = 0; i < dd->orient->size; i += 1) {
				if (i == dd->indegrees->degs[e.to] || i == dd->indegrees->degs[e.from]) {
					printf ("^ ");
				} else {
					printf ("  ");
				}
			}
			if (dd->indegrees->n_of_size [dd->indegrees->degs[e.to]] == 1) {
				if (dd->indegrees->n_of_size [dd->indegrees->degs[e.to]-1] != 0) {
					printf ("!");
				}
			}
			
			if (dd->indegrees->n_of_size [dd->indegrees->degs[e.from]] == 1) {
				if (dd->indegrees->n_of_size [dd->indegrees->degs[e.from]+1] != 0) {
					printf ("!");
				}
			}
			printf ("\n");

		
		// count the number of `large' edges length
/*		int count = 0;
		int i; for (i = 0; i < dd->orient->size; i += 1) {
			if (can_disregard (dd->indegrees, i, dd->orient->size)) {
				continue;
			}
			
			int j; for (j = i+1; j < dd->orient->size; j += 1) {
				if (can_disregard (dd->indegrees, j, dd->orient->size)) {
					continue;
				}
				
				if (abs (dd->indegrees->degs [i] - dd->indegrees->degs[j]) > data->key) {
					count += 1;
				}
			}
		}
		printf ("%d\n", count);*/
		
		}
#endif

	inc_degree (dd->indegrees, e.from);
	dec_degree (dd->indegrees, e.to);

	int edge_num;
	// fix the degree_diff of all edges that join with this edge
	int i; for (i = 0; i < dd->orient->size; i += 1) {
		if (i == e.from || i == e.to) continue;
		
		if (dg_weight (*dd->orient, i, e.from)) {
			edge chg_e = (edge) {i, e.from};
			edge_num = edge_no (chg_e, dd->orient->size);
			
			float mult = edge_mult (*dd->dg, *dd->orient, chg_e);
			dd->edges[edge_num] = inc_edge_key (dd->ddlist, dd->edges[edge_num], -1 * mult);
		} else {
			edge chg_e = (edge) {e.from, i};
			edge_num = edge_no (chg_e, dd->orient->size);

			float mult = edge_mult (*dd->dg, *dd->orient, chg_e);
			dd->edges[edge_num] = inc_edge_key (dd->ddlist, dd->edges[edge_num], 1 * mult);
		}
			
		if (dg_weight (*dd->orient, i, e.to)) {
			edge chg_e = (edge) {i, e.to};
			edge_num = edge_no (chg_e, dd->orient->size);

			float mult = edge_mult (*dd->dg, *dd->orient, chg_e);
			dd->edges[edge_num] = inc_edge_key (dd->ddlist, dd->edges[edge_num], 1 * mult);
		} else {
			edge chg_e = (edge) {e.to, i};
			edge_num = edge_no (chg_e, dd->orient->size);

			float mult = edge_mult (*dd->dg, *dd->orient, chg_e);
			dd->edges[edge_num] = inc_edge_key (dd->ddlist, dd->edges[edge_num], -1 * mult);
		}
	}

	// now switch the edge around
	int old_en = edge_no (e, dd->orient->size);
	
	edge new_e = reverse_edge (e);
	int new_en = edge_no (new_e, dd->orient->size);

	assert (dd->edges[old_en] != NULL);
	assert (dd->edges[new_en] == NULL);

	edge_list_data* data = dd->edges[old_en]->data;
	
	float new_dd = dd->indegrees->degs[new_e.from] - dd->indegrees->degs[new_e.to];
// why did I have this before? was I retarted?
//	float new_dd = (-1) * data->key - 2; // triple-check this
	
	// multiplier
	new_dd *= edge_mult (*dd->dg, *dd->orient, new_e);
	
	dd->edges[new_en] = change_edge_key (dd->ddlist, dd->edges[old_en], new_dd);
	dd->edges[old_en] = NULL;

	reverse_edge_list_edge (dd->edges[new_en]);
	
//	printf ("Finished\n");
}

edge
get_best_dd_edge (degree_diff* dd)
{
	edge e = get_first_edge (dd->ddlist);
		
	int edge_num = edge_no (e, dd->orient->size);
	edge_list_data* data = dd->edges[edge_num]->data;
	
	// check that it is a back-edge
	if (data->key < -0.001) { // FIXME MAGIC NUMBER--- (rounding errors)
//		printf ("key is: %f\n", data->key);
		return (edge) {0, 0};
	} else {
		return e;
	}
}

// get the list of edges, sorted by degree difference
edge*
get_edges_by_degree_diff (degree_diff* dd)
{
 	return edge_list_to_array (dd->ddlist);
}