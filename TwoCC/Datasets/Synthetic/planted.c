// 
//  planted.c
//  TwoCC
//  
//  Created by Tom Coleman on 2008-03-05.
//  Copyright 2008 Tom Coleman. All rights reserved.
// 

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "sigraph.h"
#include "rand.h"
#include "util.h"
#include "array.h"

// usage: planted.exe n_pts split_size p_exist p_flip
int main (int argc, char const *argv[])
{
	int i, j;
	assert (argc == 6);
	
	int n_runs = atoi (argv[1]);
	
	int n_pts = atoi (argv[2]);
	int split = atoi (argv[3]);
	
	float p_exist = atof (argv[4]);
	float p_flip  = atof (argv[5]);
	
	// first choose the split--a subset of size split
	split = min (split, n_pts - split);
	
	int run;
	for (run = 0; run < n_runs; run++) {
		// we will delete split points from subset
		int* subset = rand_perm (n_pts);
		// print_array (stdout, subset, n_pts);
	
		// now make a dictionary out of subset--much faster
		int* in_subset = zero_array (n_pts);
		for (i = 0; i < split; i++) {
			in_subset [subset[i]] = 1;
		}
		// print_array (stdout, in_subset, n_pts);
	
		sigraph sg = new_sigraph (n_pts);
	
		for (i = 0; i < n_pts; i++) {
			for (j = i+1; j < n_pts; j++) {
				int sign = 0;
			
				if (get_rand_frac () < p_exist) {
					sign = -2 * ((in_subset[i] + in_subset[j]) % 2) + 1;
				
					if (get_rand_frac () < p_flip) {
						sign *= -1;
					}
				}
				set_sg_sign (sg, i, j, sign);
			}
		}
	
		print_sigraph (stdout, sg);
	}
	return 0;
}