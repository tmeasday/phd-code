#include <stdio.h>
#include <stdlib.h>

#include "digraph.h"
#include "util.h"

float
count_triangles (digraph dg)
{
	float sum = 0;
	int i, j, k;
	for (i = 0; i < dg.size; i += 1) {
		for (j = i+1; j < dg.size; j += 1) {
			for (k = j+1; k < dg.size; k += 1) {
				sum += min (min (dg_weight (dg, i, j), dg_weight (dg, j, k)),
					dg_weight (dg, k, i));
				sum += min (min (dg_weight (dg, j, i), dg_weight (dg, k, j)),
					dg_weight (dg, i, k));
			}
		}
	}
	
	return sum;
}

int main (int argc, char const* argv[])
{
	digraph dg;
	while ((dg = read_digraph (stdin)).size != -1) {
		float tc = count_triangles (dg);
		int n = dg.size;
		
		printf ("Digraph, size: %d, weight of triangles: %.2f (%.2f %%)\n", 
						dg.size, tc, 100 * 24 * tc / (n * (n-1) * (n-2)));
	}
	
	return 0;
}