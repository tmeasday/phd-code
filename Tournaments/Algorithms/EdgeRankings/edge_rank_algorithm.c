/*
   edge_rank_algorithm.c
   Tournaments

			Use this to create algorithms which are combinations of edge_ranking schemes, 
			and edge_ranking -> node_ranking methods.
   
   Created by Tom Coleman on 2007-08-24.
   Copyright 2007 The University of Melbourne. All rights reserved.
*/
#include <assert.h>
#include "edge_rank_algorithm.h"
#include "order.h"
#include "digraph.h"
#include "edge.h"


void 
edge_rank_algorithm (edge_ranker* ranker, edge_flipper* flipper)
{
	ranker->prepare ();
	flipper->prepare ();
	digraph dg;
	while ((dg = read_digraph (stdin)).size != -1) {
		print_order (stdout, rank_by_edges (ranker, flipper, dg));
	}

	return;
}

order
rank_by_edges (edge_ranker* ranker, edge_flipper* flipper, digraph dg)
{	
	digraph orient = round_digraph (dg, 0);
//	int n = dg.size, n_edges = n * (n - 1) / 2;

 	ranker->initialize (dg, orient);
	flipper->initialize (orient);
		
	// Ok now the real work
	while (!flipper->complete ()) {
		edge e = ranker->get_first ();
		while (!flipper->valid (e)) {
			e = ranker->get_next ();
			
			// we are at the end.. the flipper should have realised
			assert (e.from != 0 || e.to != 0);
		}
		
//		printf ("reversing %d -> %d\n", e.from, e.to);

		// ok, reverse the edge
		set_dg_weight (orient, e.from, e.to, 0);
		ranker->update (e);
		flipper->update (e);
	}

	// ok. flipper.complete should guarantee that orient is transitive
	return digraph_to_order (orient);
}