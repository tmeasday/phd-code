// 
//  past.c
//  TwoCC
//  
//  Created by Tom Coleman on 2008-03-05.
//  Copyright 2008 Tom Coleman. All rights reserved.
// 

#include <assert.h>
#include <string.h>

#include "sigraph.h"
#include "labelling.h"
#include "tree.h"
#include "cost.h"

int main (int argc, char const *argv[])
{	
	sigraph sg;
	while ((sg = read_sigraph (stdin)).size != -1) 
	{
		labelling best_l;
		int i, best_c = sg.size * sg.size;
		
		for (i = 0; i < sg.size; i++) {
			tree* t = bfs_st (sg, i, 1);
			labelling l = labelling_from_st (sg, t);
			
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