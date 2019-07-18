// 
//  ptas_complete.c
//  TwoCC
//  
//  Created by Tom Coleman on 2008-03-06.
//  Copyright 2008 Tom Coleman. All rights reserved.
// 

#include <math.h>

#include "rand.h"
#include "sigraph.h"
#include "labelling.h"
#include "cost.h"
#include "util.h"
#include "array.h"

#define K 2

labelling maxAg (sigraph sg, float eps, float delta);
labelling minDis (sigraph sg, float eps, float delta);


int main (int argc, char const *argv[])
{
	sigraph sg;
	while ((sg = read_sigraph (stdin)).size != -1) 
	{
		labelling l = minDis (sg, 0.25, 0.1);
		
		print_labelling (stdout, l);
	}
	return 0;
}

labelling
minDis (sigraph sg, float eps, float delta)
{
	int i, j, k;
	float c1 = 1/20;
	float beta = c1 * eps / (16 * K * K);
	int sample_size = ceil (5 * log2 (sg.size) / (beta * beta));
	
	// bugger off with you u.a.r with replacement
	// S = the first sample_size of partition
	int* sample = rand_perm (sg.size);
	
	labelling best_l = maxAg (sg, eps*eps * c1* c1 /(32 * pow(K,4)), delta);
	int best_c = cost (sg, best_l);
	
	// for each clustering of S
	for (i = 0; i < pow (2, sample_size); i++) {
		labelling l = new_labelling (sg.size);
		
		int ind = i;
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
			
			// should this be randomized?
			l.labels[sample[j]] = pval > 0 ? 1 : -1;
		}

		// TODO: I don't think the Large/Small stuff is actually necessary for 2 clusters
		int c = cost (sg, l);
		if (c < best_c) {
			best_l = l;
			best_c = c;
		}
	}

	return best_l;
}

labelling
maxAg (sigraph sg, float eps, float delta)
{
	int i, j, k, h;
	int m = ceil (4/eps);
	// the length of a part
	int partl = ceil (sg.size / (m * 1.0));
	int partls[m], part_starts[m+1];
	for (i = 0; i < m; i++) {
		if (i < sg.size - (partl-1) * m) {
			partls[i] = partl;			
		} else {
			partls[i] = partl-1;
		}
		
		part_starts[i] = i==0 ? 0 : part_starts[i-1] + partls[i-1];
	}
	part_starts[m] = sg.size;
	
	
	int r = ceil ((pow(32,2) / (2 * pow (eps,2))) *
	              log2 ((64*m*K) / (eps * delta)));
	
	// the first partl elements is part 1 etc.
	int* partition = rand_perm (sg.size);
	
	labelling best_l = random_labelling (sg.size);
	int best_c = cost (sg, best_l);
	
	
	//////////// Choose  the S^i ////////////
	int* sample[m];
	for (i = 0; i < m; i++) {
		int size_super = sg.size - partls[i];
		
		sample[i] = zero_array (r);
		for (j = 0; j < r; j++) {
			int x = get_rand (size_super);
			
			// push out everything past part_starts[i] by partls[i]
			if (i < (m-1) && x >= part_starts[i]) {
				x += partls[i];
			}
			
			sample[i][j] = partition[x];
			
			// check that we haven't just repeated ourselves
			if (in_array (sample[i][j], sample[i], j)) {
				// we'll resample now
				j--;
			}
		}
	}
	
		
	//////////// Cluster V^i ////////////
	// there are 2^r clusterings of S^i
	// (could probably eliminate half here.. todo)
	for (j = 0; j < pow (2,r*m); j++) {
		labelling l = new_labelling (sg.size);
		int ind = j;
		for (i = 0; i < m; i++) {
			labelling sam_l = new_labelling (sg.size);
			for (k = 0; k < r; k++) {
				sam_l.labels[sample[i][k]] = 2 * (ind & 1) - 1;
				ind >>= 1;
			}
			
			// lets cluster vertex k from partition
			for (h = part_starts[i]; h < part_starts[i+1]; h++) {
				int v = partition[h];
				
				int sign = 0;
				// ok. Which cluster should v be in?
				for (k = 0; k < m; k++) {
					int sam_p = sample[i][k];
					sign += sam_l.labels[sam_p] * sg_sign (sg, sam_p, v);
				}
				
				// ok. I deal with 0 pretty arbitrarily here.. could be smarter.
				l.labels[v] = sign > 0 ? 1 : -1;
			}
		}
		
		int c = cost (sg, l);
		if (c < best_c) {
			best_l = l;
			best_c = c;
		}
	}
	
	return best_l;
}