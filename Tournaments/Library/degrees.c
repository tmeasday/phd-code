/*
   degrees.c
   Tournaments
   
   Created by Tom Coleman on 2007-08-24.
   Copyright 2007 The University of Melbourne. All rights reserved.
*/

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "degrees.h"
#include "array.h"

degrees*
new_degrees (int n)
{
	degrees* new = malloc (sizeof (degrees));
	assert (new != NULL);
	
	new->degs      = zero_array (n);
	new->n_of_size = zero_array (n);
	// we assume everything starts at indeg 0
	new->n_of_size[0] = n;
	new->n_zeros = n-1; // everything bar 0
	
	return new;
}

// this function is why we have this structure.
void
set_degree (degrees* degs, int index, int degree)
{
	degs->n_of_size[degs->degs[index]] -= 1;
	int size = degs->n_of_size[degs->degs[index]];
	assert (size >= 0);
	if (size == 0) {
		degs->n_zeros += 1;
	}
	
	degs->degs[index] = degree;
	
	size = degs->n_of_size[degs->degs[index]];
	if (size == 0) {
		degs->n_zeros -= 1;
	}
	degs->n_of_size[degs->degs[index]] += 1;
}

void
inc_degree (degrees* degs, int index)
{
	set_degree (degs, index, degs->degs[index] + 1);
}

void
dec_degree (degrees* degs, int index)
{
	set_degree (degs, index, degs->degs[index] - 1);
}

void
print_degrees (degrees* degs, int n)
{
	int deg, i;
	
	for (deg = 0; deg < n; deg += 1) {
		int count = 0;
		for (i = 0; i < n; i += 1) {
			if (degs->degs[i] == deg) {
				count++;
			}
		}
		
		printf ("%d ", count);
	}
	printf ("\n");
}

// all the degrees up to this guy are all good
int
can_disregard (degrees* degs, int i, int n)
{
	int deg = degs->degs[i];
	
	if (degs->n_of_size[deg] != 1) {
		return 0;
	}
	
	// now we need either 1s all the way to the top, or the bottom
	int disreg = 1, j;
	for (j = deg+1; j < n; j += 1) {
		if (degs->n_of_size[j] != 1) {
			disreg = 0;
			break;
		}
	}
	if (disreg) return 1;

	for (j = deg-1; j >= 0; j -= 1) {
		if (degs->n_of_size[j] != 1) {
			return 0;
		}
	}
	return 1;
}