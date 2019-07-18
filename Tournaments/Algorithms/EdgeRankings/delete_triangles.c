/*
   delete_triangles.c
   A library for applying a list of edges to try and delete all triangles..
   
   Created by Tom Coleman on 2007-08-20.
   Copyright 2007 The University of Melbourne. All rights reserved.
*/

#include <assert.h>
#include <stdlib.h>
#include "digraph.h"
#include "order.h"
#include "array.h"
#include "util.h"
#include "edge.h"


void
delete_triangles (digraph orient, edge edges[])
{
	// ok, first off we calculate the in-degrees
	int n = orient.size; // this is the number of nodes
  int n_edges = orient.size * (orient.size - 1) / 2;
	
	degrees* indegs = new_degrees (n);
	
	// O (n^2) -- calculate indegrees
	int i;
	for (i = 0; i < n; i += 1) {
		set_degree (indegs, i, indegree (orient, i));
	}
	
	// Ok now the real work
	int count = 0;
	while (indegs->n_zeros > 0) {
		// we are reversing the edge from -> to
		// so indeg(e.from) += 1, indeg(e.to) -= 1
		edge e;
		
		// only reverse an edge if it `improves' the indegree situation
		int improve, empty_run = 0;
		do {		
			// start from the start of the list again...
			if (count == n_edges) {
				count = 0;
				assert (empty_run == 0);
				empty_run = 1;
			}
			
			e = edges[count++]; 
			
			// check the edge still points forwards
			if (dg_weight (orient, e.from, e.to) != 1) {
				improve = -1;
				continue;
			}
	
			improve = (indegs->degs[e.from] >= indegs->degs[e.to]);

			// let's try out what we are going to do, and see if the n_zeros improves
/*		int old = indegs->n_zeros;
			inc_degree (indegs, e.from);
			dec_degree (indegs, e.to);	
			improve = old - indegs->n_zeros;
			dec_degree (indegs, e.from);
			inc_degree (indegs, e.to);	*/
			
			
/*			improve = 0;
			int size = indegs->n_of_size[indegs->degs[e.from]];
			if (size == 1) improve -= 1;
			if (size == 2) improve += 1;
			// improve += (size == 1) ? -1 : +1;
			size = indegs->n_of_size[indegs->degs[e.from] + 1];
			if (size == 1) improve -= 1;
			if (size == 0) improve += 1;
			// improve += (size == 0) ? +1 : -1;
			size = indegs->n_of_size[indegs->degs[e.to]];
			if (size == 1) improve -= 1;
			if (size == 2) improve += 1;
			// improve += (size == 1) ? -1 : +1;
			size = indegs->n_of_size[indegs->degs[e.to] - 1];
			if (size == 1) improve -= 1;
			if (size == 0) improve += 1;			
			// improve += (size == 0) ? +1 : -1;
*/

		} while (improve <= 0);

		printf ("reversing %d -> %d (%d)\n", e.from, e.to, indegs->n_zeros);
		print_degrees (indegs, n);

		// ok, reverse the edge
		set_dg_weight (orient, e.from, e.to, 0);
		inc_degree (indegs, e.from);
		dec_degree (indegs, e.to);	
	}
	
}

int main (int argc, char const* argv[])
{
	digraph dg;
	while ((dg = read_digraph (stdin)).size != -1) {
		digraph orient = round_digraph (dg, 0);


	  int n_edges = dg.size * (dg.size - 1) / 2;
		// generate random edges
		int *edge_nos = rand_array (n_edges*2);
		edge edges[n_edges];
		int i, j = 0;
		for (i = 0; i < n_edges*2; i += 1) {
			edge e = get_edge (edge_nos[i], dg.size);
			if (dg_weight (orient, e.from, e.to) == 1) {
				edges[j++] = e;
			}
		}
		
		delete_triangles (orient, edges);
		
		order o = digraph_to_order (orient);
		print_order (stdout, o);
	}
	return 0;
}
