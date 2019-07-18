// 
//  past.c
//  TwoCC
//  
//  Created by Tom Coleman on 2008-03-05.
//  Copyright 2008 Tom Coleman. All rights reserved.
// 

#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "sigraph.h"
#include "labelling.h"
#include "tree.h"
#include "cost.h"
#include "array.h"

#include "local_search.h"

enum lbls_gen { bfs, cover, rand_trees, rand_lbls };

int main (int argc, char const *argv[])
{
	enum lbls_gen type = bfs;
	int n_trees = 0;
	int local = 1;
	
	// pretty shitty options handling
	if (argc > 1) {
		if (strcmp (argv[1], "-b") == 0) {
			type = bfs;
		} else if (strcmp (argv[1], "-c") == 0) {
			type = cover;
		} else if (strcmp (argv[1], "-t") == 0) {
			type = rand_trees;
		} else if (strcmp (argv[1], "-r") == 0) {
			type = rand_lbls;
		}
		
		if (argc > 2) {
			n_trees = atoi (argv[2]);
			if (argc > 3) {
				assert (strcmp (argv[3], "-n") == 0);
				local = 0;
			}
		}
	}
	
	sigraph sg;
	while ((sg = read_sigraph (stdin)).size != -1) 
	{
		labelling best_l;
		int i, best_c = sg.size * sg.size;
		
		int n_trees_ins = n_trees == 0 || n_trees > sg.size ? sg.size : n_trees;
		
		int* roots;
		if (n_trees_ins == sg.size) {
			roots = sorted_array (sg.size);
		} else {
			roots = rand_perm (sg.size);
		}
		
		tree** ts;
		if (type == cover) {
			spanning_sts (sg, &ts, &n_trees_ins);
		}
		
		for (i = 0; i < n_trees_ins; i++) {
			labelling l;
			
			if (type == rand_lbls) {
				l = random_labelling (sg.size);
			} else {			
				tree* t;
				if (type == rand_trees) {
					t = random_walk_st (sg, roots[i]);
				} else if (type == cover) {
					t = ts[i];
				} else {
					t = bfs_st (sg, roots[i], 1);
				}
						
				l = labelling_from_st (sg, t);
			}
			
			
			if (local) {
				l = search (sg, l);
			}
			
			int c = cost (sg, l);
			if (c < best_c) {
				best_l = l;
				best_c = c;
			}
		}
		
		print_labelling (stdout, best_l);
	}

	return 0;
}