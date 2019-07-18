/*
   random_ranker.c
   Tournaments
   
   Created by Tom Coleman on 2007-08-24.
   Copyright 2007 The University of Melbourne. All rights reserved.
*/
#include <assert.h>

#include "edge_rank_algorithm.h"
#include "array.h"

void rand_prepare ();
void rand_init (digraph dg, digraph orient);
edge get_rand_first ();
edge get_rand_next ();
void rand_update (edge);

static edge_ranker rr = {&rand_prepare, &rand_init, &get_rand_first, 
	&get_rand_next, &rand_update};
edge_ranker* random_ranker = &rr;

void
rand_prepare ()
{
	// do nothing
}

static edge* edges;
static int no;

void
rand_init (digraph dg, digraph orient)
{
  int n_edges = dg.size * (dg.size - 1) / 2;

	// generate random edges
	int *edge_nos = rand_array (n_edges*2);
	edges = malloc (sizeof (edge) * n_edges);
	assert (edges != NULL);
	
	int i, j = 0;
	for (i = 0; i < n_edges*2; i += 1) {
		edge e = get_edge (edge_nos[i], dg.size);
		if (dg_weight (orient, e.from, e.to) == 1) {
			edges[j++] = e;
		}
	}	
}

edge
get_rand_first ()
{
	no = 0;
	return edges[no];
}

edge
get_rand_next ()
{
	return edges[no++];
}

void 
rand_update (edge e)
{
	// do nothing
}