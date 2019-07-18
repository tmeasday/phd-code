#include <stdlib.h>
#include <assert.h>
#include "edge_weights.h"

edge_weights
create_edge_weights (int size)
{
	edge_weights ew;
	ew.size = size;
	ew.weights = malloc (sizeof (float*) * size);
	assert (ew.weights != NULL);
	
	int i, j;
	for (i = 0; i < size; i += 1) {
		ew.weights[i] = malloc (sizeof (float) * size);
		assert (ew.weights[i] != NULL);
		
		for (j = 0; j < size; j += 1) {
			if (i < j) {
				ew.weights [i][j] = 0;
			} else {
				ew.weights [i][j] = -1;
			}

		}
	}
	
	return ew;
}

float get_edge_weight (edge_weights ew, int i, int j)
{
	int min, max;
	if (i < j) {
		min = i; max = j;
	} else {
		min = j; max = i;
	}
	
	return ew.weights[min][max];
}

void inc_edge_weight (edge_weights ew, int i, int j, float inc)
{
	int min, max;
	if (i < j) {
		min = i; max = j;
	} else {
		min = j; max = i;
	}

	ew.weights[min][max] += inc;
}

void set_edge_weight (edge_weights ew, int i, int j, float weight)
{
	int min, max;
	if (i < j) {
		min = i; max = j;
	} else {
		min = j; max = i;
	}

	ew.weights[min][max] = weight;
}