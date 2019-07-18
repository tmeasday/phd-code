// 
//  tree.c
//  TwoCC
//  
//  Created by Tom Coleman on 2008-03-05.
//  Copyright 2008 Tom Coleman. All rights reserved.
// 

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "tree.h"
#include "array.h"
#include "sigraph.h"
#include "util.h"
#include "rand.h"
#include "edge.h"

void print_tree_off (FILE* out, tree* t, int off, int first);
void lfst_rec (labelling* l, sigraph sg, tree* t, int label);
void vertices_from_st_rec (tree* t, tree** vertices);

tree*
create_tree_node (int content, tree* parent)
{
	tree* t = malloc (sizeof (tree));
	assert (t != NULL);
	
	t->content = content;
	t->parent = parent;
	t->children = NULL;
	t->n_children = 0;
	
	return t;
}

tree*
add_tree_child (tree* t, int child_vert)
{
	tree* new = create_tree_node (child_vert, t);
	
	t->children = realloc (t->children, sizeof (tree*) * (t->n_children + 1));
	t->children[t->n_children++] = new;
	
	return new;
}

tree*
bfs_st (sigraph sg, int root, int must_span)
{
	int i;
	int n = sg.size;
	tree* t = create_tree_node (root, NULL);
	
	// ok. no premature optimisation. but I'm sure there is potential here
	// for instance, some of the guys could be lists rather than arrays...
	int* not_in_t = sorted_array (n);
	move_array_entry (not_in_t, root, n-1);
	int n_not_in_t = n-1;
	
	// a queue of nodes to search from
	tree** to_search = malloc (sizeof (tree*) * n);
	assert (to_search != NULL);
	to_search[0] = t;
	int n_to_search = 1;
	
	while (n_not_in_t > 0) {
		// we ain't connected
		if (n_to_search <= 0) {
			if (must_span) {
				free_tree (t);
				return NULL;
			} else {
				return t;
			}
		}
		
		tree* curr_t = *(to_search++); // this is a bit tricky. but good
		n_to_search--;
	
		// find the kids of curr. put them in the n_kids spots before n_not_in_t.
		int n_kids = 0;
		for (i = 0; i < n_not_in_t; i++) {
			if (sg_sign (sg, curr_t->content, not_in_t[i-n_kids])) {
				// not_in_t[i] is a kid of curr.
				move_array_entry (not_in_t, i-n_kids, n_not_in_t - n_kids - 1);
				n_kids += 1;
			}
		}
	
		// ok now make the children array;
		curr_t->n_children = n_kids;
		curr_t->children = malloc (sizeof (tree*) * n_kids);
		assert (curr_t->children != NULL);
		
		for (i = 0; i < n_kids; i++) {
			tree* new_t = create_tree_node (not_in_t[n_not_in_t - i - 1], curr_t);
			
			curr_t->children[i] = new_t;
			to_search[n_to_search++] = new_t;
		}
		// ok now the n_kids are in the tree, so...
		n_not_in_t -= n_kids;
	}
	
	return t;
}

tree*
random_walk_st (sigraph sg, int start)
{
	int* visited = zero_array (sg.size);
	int n_visited = 0;
	
	visited[start] = 1;
	n_visited = 1;
	
	tree* trees[sg.size];
	trees[start] = create_tree_node (start, NULL);
	
	int curr = start, next;
	while (n_visited < sg.size) {
		do { // take a random step
			next = get_rand (sg.size);
		} while (sg_sign (sg, curr, next) == 0);
		
		if (!visited[next]) {
			visited[next] = 1;
			n_visited += 1;

			trees[next] = add_tree_child (trees[curr], next);
		}
		
		curr = next;
	}
	
	free (visited);
	return trees[start];
}


labelling
labelling_from_st (sigraph sg, tree* t)
{
	labelling l = new_labelling (sg.size);
	
	lfst_rec (&l, sg, t, 1);
	
	return l;
}

void
lfst_rec (labelling* l, sigraph sg, tree* t, int label)
{
	l->labels[t->content] = label;
	
	int i;
	for (i = 0; i < t->n_children; i++) {
		int new_label = label * sg_sign (sg, t->content, t->children[i]->content);
		lfst_rec (l, sg, t->children[i], new_label);
	}
}

void
print_tree (FILE* out, tree* t)
{
	print_tree_off (out, t, 0, 0);
}

void
print_tree_off (FILE* out, tree* t, int off, int first)
{	
	int i, j;

	int my_off = off;
	if (first) {
		my_off = 1;
	}
	
	for (j = 0; j < my_off; j++) {
		fprintf (out, " ");
	}
	fprintf (out, "%d", t->content);

	if (t->n_children == 0) {
		printf ("\n");
	}

	int width = t->content < 10 ? 1 : ceil (log10 (t->content));
	// printf ("width is %d\n", width);
	for (i = 0; i < t->n_children; i++)
	{
		print_tree_off (out, t->children[i], off+width+1, i == 0);	
	}
}

int
tree_size (tree* t) {
	int size = 1;
	int i;
	
	for (i = 0; i < t->n_children; i++) {
		size += tree_size (t->children[i]);
	}
	
	return size;
}


int
tree_width (tree* t)
{	
	int width = 0;
	
	int i;
	for (i = 0; i < t->n_children; i++) {
		width += tree_width (t->children[i]);
	}
	
	return width + 1;
}

int
tree_depth (tree* t)
{
	int depth = 0;
	
	int i;
	for (i = 0; i < t->n_children; i++) {
		int new_depth = tree_depth (t->children[i]);
		
		depth = max (depth, new_depth);
	}
	
	return depth + 1;
}

// basically forget about t being rooted, 
// what is the max width from t in the up direction?
int
tree_depth_up (tree* t)
{
	// we need to be careful here.
	if (t->parent == NULL) {
		return 0;
	}
	
	int depth = tree_depth_up (t->parent);
	int i;
	for (i = 0; i < t->parent->n_children; i++) {
		if (t->parent->children[i] == t) {
			continue;
		}
		
		int new_depth = tree_depth (t->parent->children[i]);
		
		depth = max (depth, new_depth);
	}
	
	// 1 for going up this link
	return 1 + depth;
}



void
free_tree (tree* t)
{
	int i;
	for (i = 0; i < t->n_children; i++) {
		free_tree (t->children[i]);
	}
	
	free (t->children);
	free (t);
}

// t doesn't have to be a spanning tree
int*
tree_vertices (tree* t, int *size)
{
	*size = tree_size (t);
	
	int* verts = malloc (sizeof (int) * *size);
	assert (verts != NULL);
	
	tree_vertices_rec (t, verts, 0);
	
	return verts;
}

int
tree_vertices_rec (tree* t, int *verts, int idx)
{
	verts[idx++] = t->content;
	
	int i;
	for (i = 0; i < t->n_children; i++) {
		idx = tree_vertices_rec (t->children[i], verts, idx);
	}
	
	return idx;
}

// obviously t needs to be a spanning tree of correct size
tree**
vertices_from_st (tree* t, int size)
{
	tree** vertices = malloc (sizeof (tree*) * size);
	assert (vertices != NULL);
	
	vertices_from_st_rec (t, vertices);
	return vertices;
}

void
vertices_from_st_rec (tree* t, tree** vertices)
{
	vertices[t->content] = t;
	int i;
	for (i = 0; i < t->n_children; i++) {
		vertices_from_st_rec (t->children[i], vertices);
	}
}

void
spanning_sts (sigraph sg, tree*** tsp, int* n_ts)
{
	int i, j;
	edge temp;
	tree** ts = NULL;
	// ok. not a very smart algorithm, but it should work...ish
	
	int n_rem_edges = 0;
	edge edges[sg.size * (sg.size-1) / 2];
	// first, grab the edges in order
	for (i = 0; i < sg.size; i++) {
		for (j = i+1; j < sg.size; j++) {
			if (sg_sign (sg, i, j) != 0) {
				edges[n_rem_edges].from = i;
				edges[n_rem_edges++].to = j;
			}
		}
	}
	int n_edges = n_rem_edges;
	
	// randomize that list of edges
	for (i = 0; i < n_rem_edges; i++) {
		int swap = i + get_rand (n_rem_edges - i);
		temp = edges[i];
		edges[i] = edges[swap];
		edges[swap] = temp;
	}
	
	// ok. now start making trees
	*n_ts = 0;
	tree* v_trees[sg.size];
	while (n_rem_edges > 0) {
		// start a new tree
		ts = realloc (ts, sizeof (tree*) * ++(*n_ts));
		
		int connected[sg.size];
		zeroes (connected, sg.size);
		int n_connected = 0;
		
		while (n_connected < sg.size) {
			int last = n_rem_edges;
			for (i = 0; i < n_edges; i++) {
				if (i == n_rem_edges) {
					if (last > i) {
						break;
					}
				}
				
				int x, y; // x is connected, y will be
				
				if (n_connected == 0) {
					n_connected += 1;
					x = edges[i].from;
					connected[x] = 1;
					ts[(*n_ts) - 1] = v_trees[x] = create_tree_node (x, NULL);
				}
				
				if (connected[edges[i].from] && !connected[edges[i].to]) {
					x = edges[i].from;
					y = edges[i].to;
				} else if (connected[edges[i].to] && !connected[edges[i].from]) {
					x = edges[i].to;
					y = edges[i].from;
				} else {
					continue;
				}
				
				n_connected += 1;
				// link y up
				connected[y] = 1;
				v_trees[y] = add_tree_child (v_trees[x], y);
								
				// resort edges
				if (i < n_rem_edges) {
					temp = edges[i];
					edges[i] = edges[--n_rem_edges];
					edges[n_rem_edges] = temp;
					i -= 1;
				} else {
					break;
				}
			}
		}
	}
	*tsp = ts;
}