/*
   triangles.c
   Tournaments
   
   Created by Tom Coleman on 2007-08-24.
   Copyright 2007 The University of Melbourne. All rights reserved.
*/

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "triangles.h"
#include "edge_weights.h"
#include "edge_list.h"
#include "edge_list_iterator.h"
#include "edge.h"

// FIXME -- delete
#include "degrees.h"

static int use_delta = 0;
static edge_weights delta;
static int use_count = 0;
static edge_weights count;

static edge_list* tclist;
static dlnode_t** edges;
static el_iterator* it;

// this number counts how many subsystems will use this code
static int n_users = 0;
static int op_rep  = 0;

// the original digraph. Will remain unmodified
static digraph dg;
static digraph orient; // really should just be a pointer...

// count how many edges have positive delta
int n_pos_delta = 0;

// the assumption is that i,j,k is (or could be) a triangle under orient

// V0
#define triangle_weight(i,j,k) 1

// V1
//#define triangle_weight(i,j,k) (dg_weight (dg, i, j) + dg_weight (dg, j, k) 
//															  + dg_weight (dg, k, i))

// V2
// #define triangle_weight(i,j,k) (max (max (dg_weight (dg, i, j), 
//                                dg_weight (dg, j, k)), dg_weight(dg, k, i)))

#define MIN_IMP 0.001

void inc_triangle_count (edge e, float inc, int do_ew);
void inc_triangle_delta (edge e, float inc);


// users should call this once only
void
register_triangle_user (int _use_count, int _use_delta)
{
	use_count = use_count || _use_count;
	use_delta = use_delta || _use_delta;
	n_users += 1;
}


void
init_triangle_scores (digraph _dg, digraph _orient)
{
	// make sure this only gets executed once
	if (op_rep > 0) {
		op_rep += 1;
		if (op_rep == n_users) {
			op_rep = 0;
		}
		return;
	}
	
	dg = _dg;
	orient = _orient;
	
	if (use_count) {
		count = create_edge_weights (orient.size);
	}
	
	if (use_delta) {
		delta = create_edge_weights (orient.size);
	}
	
	// count the initial number of triangles
	int i, j, k;
	for (i = 0; i < orient.size; i += 1) {
		for (j = 0; j < orient.size; j += 1) {
			for (k = i+1; k < orient.size; k += 1) {
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
					if (use_delta) inc_triangle_delta ((edge) {i, k}, tw);
					if (use_count) inc_triangle_count ((edge) {i, k}, tw, 0);
				} else if ((l2r && right) || (!l2r && left)) {
					if (use_delta) inc_triangle_delta ((edge) {i, k}, -tw);
				}
			}
		}
	}
	
	// now, intialize our edge bucket data structure
	if (use_count) {
		tclist = new_edge_list ();		
		int n_edges = dg.size * (dg.size - 1);
		
		edges = malloc (sizeof (dlnode_t*) * n_edges);
		assert (edges != NULL);
		
		int i; for (i = n_edges-1; i >= 0; i -= 1) {
			edge e = get_edge (i, dg.size);
			if (dg_weight (orient, e.from, e.to)) {
				edges[i] = insert_into_edge_list (tclist, e, get_triangle_count (e));
			} else {
				edges[i] = NULL;
			}	
		}
		
		it = new_el_iterator (tclist);
	}
	
}

void
update_triangle_scores (digraph _orient, edge e)
{	
	// make sure this only gets executed once
	if (op_rep > 0) {
		op_rep += 1;
		if (op_rep == n_users) {
			op_rep = 0;
		}
		return;
	}
	
	orient = _orient; // stupid code
	
	if (use_count) {
		// first reverse the multiplier...
		float score = get_edge_weight (count, e.from, e.to);
		score /= rev_edge_mult (dg, orient, e);
		score *= edge_mult (dg, orient, e);
		set_edge_weight (count, e.from, e.to, score);
	}

	
	int i;	
	for (i = 0; i < orient.size; i += 1) {
		if (i == e.from || i == e.to) continue;

		int right1 = dg_weight (orient, e.from, i);
		int right2 = dg_weight (orient, i, e.to);

		float tw1 = triangle_weight (e.from, i, e.to);
		float tw2 = triangle_weight (e.from, e.to, i);

		// Ok. There are 4 cases to consider
		if (right1 && right2) { // (1)
			if (use_delta) {
				inc_triangle_delta (e, 2 * tw1);
				inc_triangle_delta ((edge) {e.from, i}, tw1);
				inc_triangle_delta ((edge) {i, e.to}, tw1);
			}
			if (use_count) {
				inc_triangle_count (e, tw1, 0);
				inc_triangle_count ((edge) {e.from, i}, tw1, 1);
				inc_triangle_count ((edge) {i, e.to}, tw1, 1);
			}
		} else if (!right1 && !right2) { // (2)
			if (use_delta) {
				inc_triangle_delta (e, -2 * tw2);
				inc_triangle_delta ((edge) {i, e.from}, -1* tw2);
				inc_triangle_delta ((edge) {e.to, i}, -1*tw2);
			}
			if (use_count) {
				inc_triangle_count (e, -1 * tw2, 0);
				inc_triangle_count ((edge) {i, e.from}, -1 * tw2, 1);
				inc_triangle_count ((edge) {e.to, i},-1 * tw2, 1);
			}
		}	else if (right1 && !right2) { // (3)
			if (use_delta) {
				inc_triangle_delta ((edge) {e.from, i}, +1*tw2);
				inc_triangle_delta ((edge) {i, e.to}, -1*tw1);
			}
		}	 else if (!right1 && right2) { // (4)
			if (use_delta) {
				inc_triangle_delta ((edge) {e.from, i}, -1*tw1);
				inc_triangle_delta ((edge) {i, e.to}, +1*tw2);
			}
		}
	}
	
	// now we need to make sure the edge that is turned is up-to-date by tc
	if (use_count) {
		int old_en = edge_no (e, dg.size);
		int new_en = edge_no ((edge) {e.to, e.from}, dg.size);
	
		assert (edges[old_en] != NULL);
		assert (edges[new_en] == NULL);
		
		edges[new_en] = change_edge_key (tclist, edges[old_en], get_triangle_count (e));
		edges[old_en] = NULL;

		reverse_edge_list_edge (edges[new_en]);		
	}
}

// we need a function here to keep our datastructure up to date
void
inc_triangle_count (edge e, float inc, int do_ew)
{
	// multiply
	inc *= edge_mult (dg, orient, e);
	
	// first off
	inc_edge_weight (count, e.from, e.to, inc);

	if (do_ew) {
		// now update the bucket order
		int edge_num = edge_no (e, dg.size);
		assert (edges[edge_num] != NULL);
		edges[edge_num] = inc_edge_key (tclist, edges[edge_num], inc);	
	}
}

void
inc_triangle_delta (edge e, float inc)
{
	float old = get_edge_weight (delta, e.from, e.to);
	float new = old + inc;
	
	// do the real work
	inc_edge_weight (delta, e.from, e.to, inc);
	
	// now check to see if we've gone past 0
	if (old <= 0 && new > 0) {
		n_pos_delta += 1;
	} else if (old > 0 && new <= 0) {
		n_pos_delta -= 1;
	}

}

float
get_triangle_count (edge e)
{
	assert (use_count);
	return get_edge_weight (count, e.from, e.to);
}

float
get_triangle_delta (edge e)
{
	assert (use_delta);
	return get_edge_weight (delta, e.from, e.to);
}

edge
get_best_tc_with_delta ()
{
	assert (use_count && use_delta);
	// TODO -- this code should be in edge_list somehow
	
	// TODO -- count the number of such traversals we do (should be pretty low)
	// traverse the buckets
	dlnode_t* bucket_node = tclist->head;
	while (bucket_node != NULL) {
		edge_bucket* bucket = bucket_node->data;
		
		// traverse the edge list
		dlnode_t* node = bucket->edges->head;
		while (node != NULL) {
			edge_list_data* data = node->data;
			if (get_triangle_delta (data->e) > MIN_IMP) {
				return data->e;
			}
			
			node = node->next;
		}
		bucket_node = bucket_node->next;
	}
	return (edge) {0, 0};
}

// get the list of edges, sorted by triangle count
edge*
get_edges_by_count ()
{
 	return edge_list_to_array (tclist);
}

edge
get_first_tc_edge ()
{
	return get_it_first_edge (it);
}

edge
get_next_tc_edge ()
{
	return get_it_next_edge (it);
}

int
complete () {
	assert (use_delta);
//	printf ("n_pos_delta: %d\n", n_pos_delta);
	return n_pos_delta == 0;
}
