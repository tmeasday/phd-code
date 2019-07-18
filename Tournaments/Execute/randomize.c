#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "array.h"
#include "digraph.h"

// randomize a list of tournaments (i.e. don't change them, just muddle 
// their orders a bit)


int main (int argc, char const* argv[])
{
	digraph old;
	while ((old = read_digraph (stdin)).size != -1) {
		digraph new = new_digraph (old.size);
	
		int* order = rand_array (old.size);
	
		int i, j;
		for (i = 0; i < old.size; i += 1) {
			for (j = i+1; j < old.size; j += 1) {
				set_dg_weight (new, i, j, dg_weight (old, order[i], order[j]));
			}
		}
	
		print_digraph (stdout, new);
	}
	return 0;
}