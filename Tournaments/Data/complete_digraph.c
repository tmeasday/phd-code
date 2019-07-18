#include "digraph.h"
#include "rand.h"

enum method {Random, Triangles};

int main (int argc, char const* argv[])
{
	enum method how = Random;
	
	digraph dg;
	while ((dg = read_digraph (stdin)).size != -1) {
		int i, j;
		for (i = 0; i < dg.size; i += 1) {
			for (j = i+1; j < dg.size; j += 1) {
				if (dg_weight (dg, i, j) == 0.5) {
					// what do we set the new weight to??
					
					if (how == Random) {
						set_dg_weight (dg, i, j, get_rand (2));
					}
				}
			}
		}
		
		print_digraph (stdout, dg);
	}
	return 0;
}