// 
//  sample-and-insert.c
//  Tournaments
//  
//  Created by Tom Coleman on 2008-04-10.
//  Copyright 2008 Tom Coleman. All rights reserved.
// 
#include <assert.h>
#include <string.h>

#include "order.h"
#include "digraph.h"
#include "rand.h"
#include "util.h"
#include "array.h"
#include "upsets.h"
#include "insertionsort.h"

int fac (int n)
{
	if (n == 1) return 1;
	return n * fac (n-1);
}

int main (int argc, char const *argv[])
{
	int sample_size_1, sample_size_2, i, j, k;
	
	assert (argc > 1);
	sample_size_1 = atoi (argv[1]);
	if (argc > 2) {
		sample_size_2 = atoi (argv[2]);
	} else {
		sample_size_2 = 0;
	}
	
	digraph dg; order o;
	while ((dg = read_digraph (stdin)).size != -1) {
		int* sample = rand_perm (dg.size);
		int sample_size_ins_1 = sample_size_1 <= dg.size ? sample_size_1 : dg.size;
		int sample_size_ins_2 = sample_size_2 <= dg.size ? sample_size_2 : dg.size;

		sample_size_ins_1 = sample_size_ins_1 == 0 ? dg.size : sample_size_ins_1;
		sample_size_ins_2 = sample_size_ins_2 == 0 ? dg.size : sample_size_ins_2;
		
		order best_o;
		float best_cost = dg.size * dg.size;
		o = new_order (dg.size);
		for (i = 0; i < fac (sample_size_ins_1); i++) {
			int ins = i;
			
			fill (o.nodes, dg.size, -1);
			
			for (j = 0; j < dg.size; j++) {
				if (j < sample_size_ins_1) {
					int code = ins % (sample_size_ins_1 - j);
					ins = ins / (sample_size_ins_1 - j);
					
					for (k = 0; k < sample_size_ins_1; k++) {
						if (o.nodes[k] == -1) {
							code -= 1;
						}
						if (code == -1) {
							o.nodes[k] = sample[j];
							break;
						} 
					}
				} else {
					o.nodes[j] = sample[j];
				}
			}
			
			// now run insertion sort
			// print_order (stdout, o);
			insertionsortfrom (dg, o.nodes, sample_size_ins_1, sample_size_ins_2);
			// print_order (stdout, o);
			
			// we've only focused on the first sample_size_ins_2 nodes, but the end is the same
			// in each case, so the cost is the same
			float cost = upsets (dg, o);
			// printf ("%f\n\n", cost);
			if (cost < best_cost) {
				best_cost = cost;
				best_o = copy_order (o);
			}
		}
				
    // print_order (stdout, best_o);
		insertionsortfrom (dg, best_o.nodes, sample_size_ins_2, dg.size);
		print_order (stdout, best_o);
		// float cost = upsets (dg, best_o);
		// printf ("%f\n\n", cost);
  }
  return 0;
}