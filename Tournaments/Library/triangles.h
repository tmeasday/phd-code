#ifndef _TRIANGLES_H_
#define _TRIANGLES_H_

#include "digraph.h"
#include "edge.h"

void register_triangle_user (int _use_count, int _use_delta);
void init_triangle_scores (digraph dg, digraph orient);
void update_triangle_scores (digraph orient, edge e);
float get_triangle_count (edge e);
float get_triangle_delta (edge e);
edge get_best_tc_with_delta ();
edge* get_edges_by_count ();

edge get_first_tc_edge ();
edge get_next_tc_edge ();

int complete ();

#endif /* _TRIANGLES_H_ */
