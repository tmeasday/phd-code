#ifndef _EDGE_RANK_ALGORITHM_H_
#define _EDGE_RANK_ALGORITHM_H_

#include "order.h"
#include "digraph.h"
#include "edge.h"

// FORMAT of functions needed
// note we aren't going to be too smart here, we'll use globals and stuff

// from edge_ranking scheme:
// here we may be interested in both dg and orient
// int[] get_ranking (digraph dg, digraph orient)
// void update (edge changed)

typedef struct {
	void (*prepare) (void);
	void (*initialize) (digraph, digraph);
	edge (*get_first) (void);
	edge (*get_next) (void);
	void (*update) (edge);
} edge_ranker;

// from edge_ranking -> node_ranking method
// void initialize (dg orient)
// int valid (edge to_change)
// void update (edge changed)

typedef struct {
	void (*prepare) (void);
	void (*initialize) (digraph);
	int (*valid) (edge);
	int (*complete) (void);
	void (*update) (edge);
} edge_flipper;

void edge_rank_algorithm (edge_ranker* ranker, edge_flipper* flipper);
order	rank_by_edges (edge_ranker* ranker, edge_flipper* flipper, digraph dg);


#endif /* _EDGE_RANK_ALGORITHM_H_ */
