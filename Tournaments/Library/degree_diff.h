#ifndef _DEGREE_DIFF_H_
#define _DEGREE_DIFF_H_

#include "digraph.h"
#include "degrees.h"
#include "dllist.h"
#include "edge_list.h"

typedef struct {
	digraph* dg;
	digraph* orient;
	degrees* indegrees;
	dlnode_t** edges;
	edge_list* ddlist;
} degree_diff;

degree_diff* new_degree_diff (digraph *dg, digraph *orient);
void update_degree_diff (degree_diff* dd, edge e);
edge get_best_dd_edge (degree_diff* dd);
edge* get_edges_by_degree_diff (degree_diff* dd);

#endif /* _DEGREE_DIFF_H_ */
