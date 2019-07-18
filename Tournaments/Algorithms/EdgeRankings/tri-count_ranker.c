/*
   random_ranker.c
   Tournaments
   
   Created by Tom Coleman on 2007-08-24.
   Copyright 2007 The University of Melbourne. All rights reserved.
*/
#include <assert.h>

#include "edge_list.h"
#include "edge_rank_algorithm.h"
#include "triangles.h"
#include "list.h"
#include "array.h"

static digraph dg, orient;
static edge_list* tclist;

void
tricount_prepare ()
{
	register_triangle_user (1, 0);
}

void
tricount_init (digraph _dg, digraph _orient)
{
	dg = _dg; orient = _orient;
	init_triangle_scores (dg, orient);
}

edge
get_tricount_first ()
{
	return get_first_tc_edge ();
}

edge
get_tricount_next ()
{
	return get_next_tc_edge ();
}

void 
tricount_update (edge e)
{
	update_triangle_scores (orient, e);
}