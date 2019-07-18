#ifndef _TREE_H_
#define _TREE_H_

typedef struct tree_t tree;
struct tree_t {
	int content;
	tree* parent;
	tree** children;
	int n_children;
};

#include "sigraph.h"
#include "labelling.h"

tree* bfs_st (sigraph sg, int start, int must_span);
tree* random_walk_st (sigraph sg, int start);

labelling labelling_from_st (sigraph sg, tree* t);
tree** vertices_from_st (tree* t, int size);
int* tree_vertices (tree* t, int* size);
	
void print_tree (FILE* out, tree* t);

int tree_size (tree* t);
int tree_width (tree* t);
int tree_depth (tree* t);
int tree_depth_up (tree* t);

void free_tree (tree* t);


void spanning_sts (sigraph sg, tree*** ts, int* n_ts);

#endif /* _TREE_H_ */
