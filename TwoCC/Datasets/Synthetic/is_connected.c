// 
//  filter_is_connected.c
//  TwoCC
//  
//  Created by Tom Coleman on 2008-03-17.
//  Copyright 2008 Tom Coleman. All rights reserved.
// 

#include "sigraph.h"
#include "tree.h"

int main (int argc, char const *argv[])
{
	sigraph sg;
	while ((sg = read_sigraph (stdin)).size != -1) {
		if (bfs_st (sg, 0, 1) != NULL) {
			print_sigraph (stdout, sg);
		}
	}
	return 0;
}