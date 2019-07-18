// 
//  save_for_bsg.c
//  TwoCC
//  
//  Created by Tom Coleman on 2008-04-01.
//  Copyright 2008 Tom Coleman. All rights reserved.
// 

#include "sigraph.h"

int main (int argc, char const *argv[])
{
	
	int i, j;
	
	sigraph sg;
	while ((sg = read_sigraph (stdin)).size != -1) {
		for (i = 0; i < sg.size; i++) {
			for (j = i+1; j < sg.size; j++) {
				if (sg_sign (sg, i, j) != 0) {
					printf ("v%d v%d %d\n", i, j, (sg_sign (sg, i, j) - 1) / (-2));
				}
			}
		}
	}
	return 0;
}