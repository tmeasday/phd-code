/*
   indegree_flipper.c
   Tournaments
   
   Created by Tom Coleman on 2007-08-24.
   Copyright 2007 The University of Melbourne. All rights reserved.
*/

#include "edge_rank_algorithm.h"
#include "tri-delta_flipper.h"
#include "digraph.h"
#include "triangles.h"

edge_flipper* tridelta_flipper = &tdf;

#ifndef MIN
#define MIN_IMP 0.01
#endif

static digraph orient;

void
tridelta_prepare ()
{
	register_triangle_user (0, 1);
}

void 
tridelta_initialize (digraph _orient)
{
	orient = _orient;
	init_triangle_scores (orient, orient);
}

int
tridelta_valid (edge e)
{
	return get_triangle_delta (e) > MIN_IMP;
}

int
tridelta_complete (void)
{
	return complete ();
}

void
tridelta_update (edge e)
{	
	update_triangle_scores (orient, e);	
}