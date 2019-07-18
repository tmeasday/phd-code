#include <stdio.h>
// #include "order.h"

#ifndef _SIGRAPH_H_
#define _SIGRAPH_H_


typedef struct {
  int size;
  int** signs;
} sigraph;

sigraph new_sigraph (int);
void free_sigraph (sigraph sg);
sigraph read_sigraph (FILE*);
//sigraph round_sigraph (sigraph, int);

void print_sigraph (FILE*, sigraph);

// maybe these should be macros? or inlined? or something...?
int sg_sign (sigraph, int, int);
void set_sg_sign (sigraph, int, int, int);
//void set_dg_weight_transitive (digraph, int, int);

// float indegree (digraph, int);
// float indegree_pos (digraph, int, int[], int);
// float outdegree (digraph, int);
// float outdegree_pos (digraph, int, int[], int);

// order digraph_to_order (digraph);

sigraph subsigraph (sigraph sg, int pos[], int n_pos);

// FIXME -- what is happening with triangles.c ?????
// int* get_vertex_triangle_counts (digraph dg);


#endif /* _SIGRAPH_H_ */
