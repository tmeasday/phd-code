// 
//  sample_and_insert.c
//  TwoCC
//  
//  Created by Tom Coleman on 2008-03-12.
//  Copyright 2008 Tom Coleman. All rights reserved.
// 

#include <assert.h>
#include <stdlib.h>
#include <math.h>

#include "labelling.h"
#include "sigraph.h"
#include "cost.h"
#include "rand.h"
#include "array.h"
#include "util.h"

labelling
sample_and_insert (sigraph sg, int sample_size, int n_labellings)
{
	int i, j, k;
	
	if (sample_size > sg.size) {
		sample_size = sg.size;
		if (n_labellings > pow (2, sample_size)) {
			n_labellings = pow (2, sample_size);
		}
	}
	
	// take a sample of size sample_size
	int* sample;
	
	if (sample_size == sg.size) {
		sample = sorted_array (sg.size);
	} else {
		sample = rand_perm (sg.size); // first sample_size is the sample
	}
	
	// now we need to iterate through the labellings some how. 
	// we need to choose n_labellings numbers UAR from 2^sample_size
	
	// knuth again
	int* labellings;
	if (n_labellings == pow (2, sample_size)) {
		labellings = sorted_array (pow (2, sample_size));
	} else {
		labellings = rand_perm (pow (2, sample_size));
	}
	
	labelling best_l = random_labelling (sg.size);
	int best_c = cost (sg, best_l);

	labelling l = new_labelling (sg.size);	
	for (i = 0; i < n_labellings; i++) {
		int ind = labellings[i];
		for (j = 0; j < sample_size; j++) {
			l.labels[sample[j]] = 2 * (ind & 1) - 1;
			ind >>= 1;
		}
		
		// for the rest of the vertices
		for (; j < sg.size; j++) {
			int pval = 0;
			// place j in the most obvious way
			for (k = 0; k < sample_size; k++) {
				pval += sg_sign (sg, sample[j], sample[k]) * l.labels[sample[k]];
			}
			
			if (pval == 0) {
				pval = get_rand (2) == 1 ? 1 : -1;
			}
			l.labels[sample[j]] = pval > 0 ? 1 : -1;
		}

		int c = cost (sg, l);
		if (c < best_c) {
			// copy l into best_l
			array_copy (l.labels, best_l.labels, sg.size);
			best_c = c;
		}
	}
	
	// printf ("%d\n", best_c);
	return best_l;
}

int main (int argc, char const *argv[])
{
	assert (argc >= 2);

	int sample_size = atoi (argv[1]);
	
	int n_labellings;
	if (argc > 2) {
		n_labellings = atoi (argv[2]);
		assert (n_labellings <= pow (2, sample_size));
	} else {
		n_labellings = pow (2, sample_size);
	}


	sigraph sg;
	while ((sg = read_sigraph (stdin)).size != -1) {
		labelling l = sample_and_insert (sg, sample_size, n_labellings);
		
    print_labelling (stdout, l);
		free_labelling (l);
  }

	return 0;
}