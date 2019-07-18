/*
   random_ranker.c
   Tournaments
   
   Created by Tom Coleman on 2007-08-24.
   Copyright 2007 The University of Melbourne. All rights reserved.
*/
#include <assert.h>

#include "edge_rank_algorithm.h"
#include "degree_diff.h"
#include "edge_list_iterator.h"

static digraph orient;
static degree_diff* dd;
static el_iterator* it;

void
degreediff_prepare ()
{
}

void
degreediff_init (digraph dg, digraph _orient)
{
	orient = _orient;
	dd = new_degree_diff (&dg, &orient);
	it = new_el_iterator (dd->ddlist);
}

edge
get_degreediff_first ()
{
	return get_it_first_edge (it);
}

edge
get_degreediff_next ()
{
	return get_it_next_edge (it);
}

void 
degreediff_update (edge e)
{
	update_degree_diff (dd, e);
}