/*
   triangle_both_faster.c
   Tournaments
   
   Created by Tom Coleman on 2007-08-28.
   Copyright 2007 The University of Melbourne. All rights reserved.
*/

#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "order.h"
#include "digraph.h"
#include "edge_weights.h"
#include "util.h"
#include "triangles.h"
#include "list.h"

#define ALPHA 1.65
#define MIN_IMP 0.001

#define mult(x) (x*x*x + 0.047)
//#define mult(x) (x*x + 0.15)
//#define mult(x) (x*x*x*x + 0.015)

int
main (int argc, char* argv[])
{
	register_triangle_user (1, 1);
	
	digraph dg;
  while ((dg = read_digraph (stdin)).size != -1) {
		digraph orient = round_digraph (dg, 0);
		init_triangle_scores (dg, orient);
		
		int m = dg.size * (dg.size - 1) / 2;

		int debug = 1;

//		int K = m - 1;
		int K = dg.size;
		
		// sort edges by triangle_count
		list_t* list = new_list ();
		int i, j;
		for (i = 0; i < dg.size; i += 1) {
			for (j = i+1; j < dg.size; j += 1) {
				edge* e = malloc (sizeof (edge));
				assert (e != NULL);
								
				if (dg_weight (orient, i, j)) {
					e->from = i;
					e->to   = j;
				} else {
					e->from = j;
					e->to   = i;
				}
			
				// TESTING --
				//float diff = fabs (dg_weight (dg, i, j) - dg_weight (orient, i, j));
				//new_count *= mult (diff);
			
				insert_at_head (list, e, get_triangle_count (*e));
			}
		}
		sort_list (list);  // O(n^2 logn)
		
		// get the Kth element of the list
		node_t* kth = list->head;
		for (i = 0; i < K; i += 1) {
			kth = kth->next;
		}
		
		
//		printf ("here\n");
		
		int iterations = 0;
		// ok, now destroy triangles
		while (1) {
			iterations++;	
					
			// ok. the assumption is that the first K of the nodes are sorted, 
			// we'll make sure the first has positive delta
			int moves = 0;
			while (get_triangle_delta (*(edge*)(list->head->data)) <= MIN_IMP) {
				if (debug) {
					fprintf (stderr, "moving (%d -> %d) [%.0f, %.0f] to back\n", 
									 ((edge*) list->head->data)->from, ((edge*) list->head->data)->to,
									 get_triangle_count (*(edge*)(list->head->data)),
									 get_triangle_delta (*(edge*)(list->head->data)));
				}
				
				head_to_foot (list);
				moves += 1;
				if (moves == m) {
					break;
				}
				
				if (list->head == kth) {
					// this will happen every now and again.
					// When it does, we resort the next k elements
					for (i = 0; i < K; i += 1) {
						kth->key = get_triangle_count (*(edge*)(kth->data));
						kth = kth->next;
					}
					list_t sorted = quicksort_nodes (list->head, K);
					list->head = sorted.head;
					sorted.foot->next = kth;
				}
			}
					
			edge* best_edge = (edge*)(list->head->data);
			if (get_triangle_count (*best_edge) == 0) {
				// we are out of triangles
				break;
			}
		
			// let's reverse it
			set_dg_weight (orient, best_edge->from, best_edge->to, 0);
			
			if (debug) {
				fprintf (stderr, "reversing the edge from %d to %d\n", 
								 best_edge->from, best_edge->to);
			}
				
			update_triangle_scores (orient, *best_edge);
							
			// update the edge
			int_swap (&best_edge->from, &best_edge->to);
			
			// re-sort the nodes
			head_to_foot (list);
			
			// reset kth and the keys (of the first K)
			kth = list->head;
			for (i = 0; i < K; i += 1) {
				kth->key  = get_triangle_count (*(edge*) kth->data);
				kth = kth->next;
			}
			list_t sorted = quicksort_nodes (list->head, K);
			list->head = sorted.head;
			sorted.foot->next = kth;
		}
		
		order o = digraph_to_order (orient);
		print_order (stdout, o);
//		printf ("in %d iterations\n", iterations);
  }
  return 0;
}
