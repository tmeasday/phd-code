// 
//  pasta_general.c
//  TwoCC
//  
//  Created by Tom Coleman on 2008-03-07.
//  Copyright 2008 Tom Coleman. All rights reserved.
// 

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "tree.h"
#include "sigraph.h"
#include "labelling.h"
#include "array.h"
#include "cost.h"
#include "util.h"
#include "local_search.h"

// i and j are connected on the tree
#define connected(vertices, i, j) vertices[i]->parent == vertices[j] || \
																	vertices[j]->parent == vertices[i]

void
walk_flip_record (tree* t, labelling l, int desc[])
{
	l.labels[t->content] *= -1;
	
	int i;
	for (i = 0; i < t->n_children; i++) {
		walk_flip_record (t->children[i], l, desc);
	}
	
	desc[t->content] = 1;
}

int
apply_changes (int errors[], int change_at_node[], tree* t, tree* from)
{
	int i, total_change = 0;
	
	if (t->parent != from && t->parent != NULL) {
			total_change += apply_changes (errors, change_at_node, t->parent, t);
			errors[t->content] -= total_change;		
	} 

	for (i = 0; i < t->n_children; i++) {
		if (t->children[i] != from) {
			total_change += apply_changes (errors, change_at_node, t->children[i], t);
		}
	}
	total_change += change_at_node[t->content];
	if (t->parent == from) {
		errors[t->content] -= total_change;
	}	
	
	return total_change;
}

void
get_node_labels (int depth, int node_labels[][depth], tree* t)
{
	// do the label
	if (t->parent == NULL) {
		node_labels[t->content][0] = -1;
	}

	int i, j;	
	for (i = 0; i < t->n_children; i++) {
		int kid = t->children[i]->content;
		
		for (j = 0; node_labels[t->content][j] != -1; j++) {
			node_labels[kid][j] = node_labels[t->content][j];
		}
		node_labels[kid][j] = i;
		node_labels[kid][j+1] = -1;
		
		get_node_labels (depth, node_labels, t->children[i]);
	}
}

// for labels
int
common_prefix (int x[], int y[])
{
	int i = 0;
	while (x[i] == y[i]) {
		if (x[i] == -1) {
			return i;
		}
		i++;
	}
	return i;
}

int
prefix_len (int x[])
{
	int i = 0;
	while (x[i++] != -1);
	return i - 1;
}

labelling
pasta_general_single (sigraph sg, tree* t)
{
	int i, j, k;
	
#ifdef DEBUG
	print_tree (stdout, t);
#endif

	labelling l = labelling_from_st (sg, t);

	// a pointer to where each vertex lives in the tree
	tree** v_trees = vertices_from_st (t, sg.size);

	// now we locally improve l, but 
	// ignoring edges inside the spanning tree (emanating from v)

	// the labels on the nodes allow us to quickly compute shortest paths
	int depth = tree_depth (t);
	int node_labels[sg.size][depth];
	get_node_labels (depth, node_labels, t);

	// work out initial errors O(n^3) ?
	// `edge[i]' is the edge with i as the child.
	int* errors = zero_array (sg.size);
	for (i = 0; i < sg.size; i++) {
		for (j = i+1; j < sg.size; j++) {
			// i and j are connected on the tree
			if (connected (v_trees, i, j)) {
				continue;
			}
			
			// there is no edge from i to j
			if (sg_sign (sg, i, j) == 0) {
				continue;
			}
			
			int change = labels_right (sg, l, i, j);
			int ij_cp = common_prefix (node_labels[i], node_labels[j]);
			
			tree* curr = v_trees[i];
			for (k = prefix_len (node_labels[i]); k > ij_cp; k--) {
				errors[curr->content] -= change;
				curr = curr->parent;
			}
			
			curr = v_trees[j];
			for (k = prefix_len (node_labels[j]); k > ij_cp; k--) {
				errors[curr->content] -= change;
				curr = curr->parent;
			}
		}
	}
	
	// the question is how many calls to this loop we have?
	int n_its = -1;
	while (1) {
		n_its++;
		
		// get the max errors--we can do this in O(n) so don't be too tricky
		int most_e = 0;
		int most_v = 0;
		for (i = 0; i < sg.size; i++) {			
			if (errors[i] > most_e) {
				most_e = errors[i];
				most_v = i;
			}
		}
		
		
#ifdef DEBUG
		print_array (stdout, l.labels, sg.size);
		print_array (stdout, errors, sg.size);
#endif
		
		// ok now switch vertex most_v if it has more errors than non errors
		if (most_e <= 0) { // we're done.
			break;
		}
		// printf ("flipping %d\n", most_e);
		
		// ok. in general we need to flip quite a few labels.
		// walk down the tree from most_v, flipping labels as we go.
		// also, mark down who is a descendant of most_v (as in who got flipped)
		int* desc = zero_array (sg.size);
		walk_flip_record (v_trees[most_v], l, desc);

		int x = most_v, y = v_trees[most_v]->parent->content; // x is below

		int n_desc = count_ones (desc, sg.size);
		int xs[n_desc], ys[sg.size - n_desc];
		j = 0; k = 0;
		for (i = 0, j = 0; i < sg.size; i++) {
			if (desc[i]) {
				xs[j++] = i;
			} else {
				ys[k++] = i;
			}
		}

		int change_at_node[sg.size];
		zeroes (change_at_node, sg.size);		
		for (i = 0; i < n_desc; i++) {
			for (j = 0; j < sg.size - n_desc; j++) {
				// we are looking at edge e
				if (xs[i] == x && ys[j] == y) continue;
				if (sg_sign (sg, xs[i], ys[j]) == 0) continue;

				// otherwise
				int change = labels_right (sg, l, xs[i], ys[j]) * 2;
				change_at_node[xs[i]] += change;
				change_at_node[ys[j]] += change;
			}
		}
		
		// now filter the changes down
		apply_changes (errors, change_at_node, v_trees[x], v_trees[y]);
		apply_changes (errors, change_at_node, v_trees[y], v_trees[x]);
	}
	
	free (errors);
	
#ifdef DEBUG
	printf ("n_its: %d\n", n_its);
#endif
	
	return l;
}

int main (int argc, char const *argv[])
{
	int n_trees = 0;
	int random = 0;
	int covered = 0;
	int local = 0;
	
	if (argc > 1) {
		if (strcmp (argv[1], "-r") == 0) {
			random = 1;
		}	else if (strcmp (argv[1], "-c") == 0) {
			covered = 1;
		} else if (strcmp (argv[1], "-l") == 0) {
			local = 1;
		} else {
			assert (strcmp (argv[1], "-b") == 0);
		}
		if (argc > 2) {
			n_trees = atoi (argv[2]);			
		}
	}
	
	sigraph sg;
	while ((sg = read_sigraph (stdin)).size != -1) 
	{
		labelling best_l;
		int i, best_c = sg.size * sg.size;
		
		int n_trees_ins = n_trees == 0 || n_trees > sg.size ? sg.size : n_trees;
		
		int* roots;
		if (n_trees_ins == sg.size) {
			roots = sorted_array (sg.size);
		} else {
			roots = rand_perm (sg.size);
		}

		tree** ts;
		if (covered) {
			spanning_sts (sg, &ts, &n_trees_ins);
		}
		
		for (i = 0; i < n_trees_ins; i++) {
			tree* t;
			if (random) {
				t = random_walk_st (sg, roots[i]);
			} else if (covered) {
				t = ts[i];
			} else {
				t = bfs_st (sg, roots[i], 1);
			}

			labelling l = pasta_general_single (sg, t);
			
			if (local) {
				l = search (sg, l);
			}
			
			int c = cost (sg, l);
			if (c < best_c) {
				// free_labelling (best_l); // first time problem
				best_l = l;
				best_c = c;
				// printf ("using %d\n", i);
			} else {
				free_labelling (l);
			}
			
			// printf ("%d\n", i);
			
			free_tree (t);
		}
		
		// printf ("%d\n", best_c);
		print_labelling (stdout, best_l);
		free_labelling (best_l);
	}

	return 0;
}