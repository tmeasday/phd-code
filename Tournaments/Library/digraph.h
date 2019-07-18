#include <stdio.h>
#include "order.h"

#ifndef _DIGRAPH_H_
#define _DIGRAPH_H_


typedef struct {
  int size;
  float** weights;
} digraph;

digraph new_digraph (int);
digraph read_digraph (FILE*);
digraph round_digraph (digraph, int);

void print_digraph (FILE*, digraph);

float dg_weight (digraph, int, int);
void set_dg_weight (digraph, int, int, float);
void set_dg_weight_transitive (digraph, int, int);

float indegree (digraph, int);
float indegree_pos (digraph, int, int[], int);
float outdegree (digraph, int);
float outdegree_pos (digraph, int, int[], int);

order digraph_to_order (digraph);

digraph subdigraph (digraph dg, int pos[], int n_pos);

// FIXME -- what is happening with triangles.c ?????
int* get_vertex_triangle_counts (digraph dg);


#endif /* _DIGRAPH_H_ */
