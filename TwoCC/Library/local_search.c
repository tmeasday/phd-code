// 
//  local_search.c
//  TwoCC
//  
//  Created by Tom Coleman on 2008-03-24.
//  Copyright 2008 Tom Coleman. All rights reserved.
// 

#include <stdlib.h>

#include "sigraph.h"
#include "labelling.h"
#include "array.h"

labelling
search (sigraph sg, labelling l)
{
	// now we locally improve l
	int i, j;
	
	// work out initial errors O(n^2)	
	int* errors = zero_array (sg.size);
	for (i = 0; i < sg.size; i++) {
		for (j = i+1; j < sg.size; j++) {
			int change = labels_right (sg, l, i, j);
			errors[i] -= change;
			errors[j] -= change;
		}
	}

	// the question is how many calls to this loop we have?
	// int n_its = 0;
	while (1) {
		// n_its++;

		// get the max errors--we can do this in O(n) so don't be too tricky
		int most_e = 0;
		int most_v = 0;
		for (i = 0; i < sg.size; i++) {

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
			if (i == most_v) {
				continue;
			}

			// else, was this edge right or wrong before?
			errors[i] -= labels_right (sg, l, i, most_v) * 2;
		}
	}

	free (errors);
	
	// printf ("n_its: %d\n", n_its);
	return l;
}