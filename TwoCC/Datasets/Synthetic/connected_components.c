// 
//  connected_components.c
//  TwoCC
//  
//  Created by Tom Coleman on 2008-04-01.
//  Copyright 2008 Tom Coleman. All rights reserved.
// 

// read in sigraph(s) and spit out connected components

#include "sigraph.h"
#include "tree.h"
#include "util.h"

int main (int argc, char const *argv[])
{
	int i, j;
	
	sigraph sg;
	while ((sg = read_sigraph (stdin)).size != -1) {
		int used[sg.size];
		zeroes (used, sg.size);
		int n_used = 0;
		
		while (n_used < sg.size) {
			// who is still unused?
			for (i = 0; used[i]; i++);
			
			tree* t = bfs_st (sg, i, 0);
			//print_tree (stdout, t);
			
			int comp_size;
			int* comp_vs = tree_vertices (t, &comp_size);
			n_used += comp_size;
			
			for (j = 0; j < comp_size; j++) {
				used[comp_vs[j]] = 1;
			}
			
			if (comp_size > 1) {
				print_sigraph (stdout, subsigraph (sg, comp_vs, comp_size));			
			}
		}
	}
	return 0;
}