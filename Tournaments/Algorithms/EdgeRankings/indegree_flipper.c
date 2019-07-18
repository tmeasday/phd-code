/*
   indegree_flipper.c
   Tournaments
   
   Created by Tom Coleman on 2007-08-24.
   Copyright 2007 The University of Melbourne. All rights reserved.
*/

#include "edge_rank_algorithm.h"
#include "degrees.h"

void indeg_prepare (void);
void indeg_initialize (digraph);
int indeg_valid (edge);
int indeg_complete (void);
void indeg_update (edge);

static edge_flipper inf = {&indeg_prepare, &indeg_initialize, 
	&indeg_valid, &indeg_complete, &indeg_update};
edge_flipper* indegree_flipper = &inf;

degrees* indegs;

void
indeg_prepare ()
{
	// do nothing
}

void 
indeg_initialize (digraph orient)
{
	// ok, first off we calculate the in-degrees
	int n = orient.size; // this is the number of nodes
	
	indegs = new_degrees (n);
	
	// O (n^2) -- calculate indegrees
	int i;
	for (i = 0; i < n; i += 1) {
		set_degree (indegs, i, indegree (orient, i));
	}	
}

int
indeg_valid (edge e)
{
	return (indegs->degs[e.from] >= indegs->degs[e.to]);
}

int
indeg_complete (void)
{
	return (indegs->n_zeros == 0);
}

void
indeg_update (edge e)
{	
	inc_degree (indegs, e.from);
	dec_degree (indegs, e.to);	
}