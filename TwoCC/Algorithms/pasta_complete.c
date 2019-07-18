// 
//  pasta_complete.c
//  TwoCC
//  
//  Created by Tom Coleman on 2008-03-05.
//  Copyright 2008 Tom Coleman. All rights reserved.
// 

#include "tree.h"
#include "sigraph.h"
#include "labelling.h"
#include "array.h"
#include "cost.h"

labelling
pasta_complete_single (sigraph sg, int v)
{
	int i, j;
	tree* t = bfs_st (sg, v, 1);
	labelling l = labelling_from_st (sg, t);

	// now we locally improve l, but 
	// ignoring edges inside the spanning tree (emanating from v)

	// work out initial errors O(n^2)	
	int* errors = zero_array (sg.size);
	for (i = 0; i < sg.size; i++) {
		if (i == v) continue;
		for (j = i+1; j < sg.size; j++) {
			if (j == v) continue;
			
			int change = labels_right (sg, l, i, j);
			errors[i] -= change;
			errors[j] -= change;
		}
	}
	
	// the question is how many calls to this loop we have?
	int n_its = 0;
	while (1) {
		n_its++;
		
		// get the max errors--we can do this in O(n) so don't be too tricky
		int most_e = 0;
		int most_v = 0;
		for (i = 0; i < sg.size; i++) {
			if (i == v) continue;
			
			if (errors[i] > most_e) {
				most_e = errors[i];
				most_v = i;
			}
		}
		
		// ok now switch vertex most_v if it has more errors than non errors
		if (most_e <= 0) { // we're done.
			break;
		}
		
		// flip the label
		l.labels[most_v] *= -1;
		
		// fix up errors O(n) here--thats why the last bit could be O(n)
		errors[most_v] *= -1;
		for (i = 0; i < sg.size; i++) {
			if (i == v || i == most_v) {
				continue;
			}
			
			// else, was this edge right or wrong before?
			errors[i] -= labels_right (sg, l, i, most_v) * 2;
		}
	}
	
	free (errors);
	free_tree (t);
	
	printf ("n_its: %d\n", n_its);
	
	return l;
}

int main (int argc, char const *argv[])
{
	sigraph sg;
	while ((sg = read_sigraph (stdin)).size != -1) 
	{
		labelling best_l;
		int i, best_c = sg.size * sg.size;
		
		for (i = 0; i < sg.size; i++) {
			labelling l = pasta_complete_single (sg, i);
			
			int c = cost (sg, l);
			if (c < best_c) {
				best_l = l;
				best_c = c;
			} else {
				free_labelling (l);
			}
		}
		
		print_labelling (stdout, best_l);
		free_labelling (best_l);
	}

	return 0;
}