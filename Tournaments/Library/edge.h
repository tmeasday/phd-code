#ifndef _EDGE_H_
#define _EDGE_H_

#include <math.h>

typedef struct {
	int from;
	int to;
} edge;

int edge_no (edge, int);
edge get_edge (int, int);
edge reverse_edge (edge e);
	
// useful little macro I guess
//#define edge_mult(a,b,e) \
	pow(fabs(dg_weight (a, e.from, e.to) - dg_weight (b, e.from, e.to)), 3) + 0.47
	
#define edge_mult(a,b,e) 1
	
//#define rev_edge_mult(a,b,e) \
		pow(fabs(dg_weight (a, e.from, e.to) - (1 - dg_weight (b, e.from, e.to))), 3) + 0.47

#define rev_edge_mult(a,b,e) 1

#endif /* _EDGE_H_ */
