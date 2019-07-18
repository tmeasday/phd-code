// 
//  insertionsort.c
//  Tournaments
//  
//  Created by Tom Coleman on 2008-04-12.
//  Copyright 2008 Tom Coleman. All rights reserved.
// 

#include "insertionsort.h"
#include "digraph.h"


void insertionsortfrom (digraph dg, int nodes[], int start, int end)
{
	int i, j;
	float cost, min_cost;
	int min_pos;
	
	for (i = start; i < end; i++)
	{
		// insert node at pos i into list from 0 to i-1
		cost = 0;
		min_cost = 0;
		min_pos  = 0;
		for (j = 0; j < i; j++)
		{
			// if there is an edge from i to j, we would prefer to put i on the left
			cost += (dg_weight (dg, nodes[i], nodes[j]) - 0.5);
			
			if (cost < min_cost) {
				min_cost = cost;
				min_pos = j+1;
			}
		}
		
		// ok we have the best position. Now we have to move nodes[i] there
		for (j = i; j > min_pos; j--) {
			int_swap (&nodes[j], &nodes[j-1]);
		}
	}
}


void insertionsort (digraph dg, int nodes[], int end)
{
	insertionsortfrom (dg, nodes, 0, end);
}