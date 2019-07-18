#include "edge.h"

int
edge_no (edge e, int n)
{
	if (e.to > e.from) {
		return e.from * (n-1) + e.to - 1;
	} else {
		return e.from * (n-1) + e.to;		
	}
	
}

edge
get_edge (int no, int n)
{
	edge e;
	
	e.from = no / (n-1);
	e.to   = no % (n-1);
	if (e.to >= e.from) {
		e.to += 1;
	}
	
	return e;
}


edge
reverse_edge (edge e)
{
	return (edge) {e.to, e.from};
}