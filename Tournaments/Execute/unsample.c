#include <assert.h>
#include "order.h"
#include "digraph.h"
#include "rand.h"
#include "util.h"
#include "array.h"

void unsample (digraph dg, int sample[], order so, order o)
{	
	int i, j;
	float cost, min_cost;
	int min_pos;
	
	// we will insert into the sampled order
	for (i = 0; i < so.size; i++) {
		o.nodes[i] = sample[so.nodes[i]];
	}
	
	int next = 0;
	for (i = so.size; i < o.size; i++, next++)
	{
		// what is the next node? (make sure it isn't in the sample)
		for (j = 0; j < so.size; j++) {
			if (sample[j] == next) {
				next++;
			}
		}
		o.nodes[i] = next;
		
		// insert node at pos i into list from 0 to i-1
		cost = 0;
		min_cost = 0;
		min_pos  = 0;
		for (j = 0; j < i; j++)
		{
			// if there is an edge from i to j, we would prefer to put i on the left
			cost += (dg_weight (dg, o.nodes[i], o.nodes[j]) - 0.5);
			
			if (cost < min_cost) {
				min_cost = cost;
				min_pos = j+1;
			}
		}
		
		// ok we have the best position. Now we have to move nodes[i] there
		for (j = i; j > min_pos; j--) {
			int_swap (&o.nodes[j], &o.nodes[j-1]);
		}
	}
}


int
main (int argc, char* argv[])
{
  assert (argc == 3);
  int run, alg;
  int runs = atoi (argv[1]);
	int nalgs = atoi (argv[2]);
	

  for (run = 0; run < runs; run++) {
    digraph dg = read_digraph (stdin);
		int* sample;
		int ssize = read_int_array (stdin, &sample, -1);
		
		print_digraph (stdout, dg);
		
		for (alg = 0; alg < nalgs; alg++) {
			// this will be our final order
			order o = new_order (dg.size);
			
			order so = read_order (stdin);
			assert (so.size == ssize);
			
			unsample (dg, sample, so, o);
			
			print_order (stdout, o);
		}
  }
  return 0;
}
