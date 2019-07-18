#include <assert.h>
#include "order.h"
#include "digraph.h"
#include "rand.h"
#include "util.h"


void sort (digraph dg, int nodes[], int n)
{
	if (n == 1) {
		return;
	}
	
	// sort sublists
	int half = n / 2;
	sort (dg, nodes, half);
	sort (dg, nodes + half, n - half);
	
	// now merge sublists
	int head1 = 0, head2 = half;
	
	int i;
	for (i = 0; i < n; i++) {
		// which should go at position i? head1 or head2?
		float cmp = dg_weight (dg, nodes[head1], nodes[head2]);
		if (cmp == 0.5) {
			cmp = get_rand_frac (1);
		}
		
		// there's an edge from head1 to head2, so head1 should go first
		// which means we don't have to do anything
		if (cmp > 0.5) {
			head1++;
		} else {
			// we've gotta move nodes[head2] to position i
			int j;
			for (j = head2; j > i; j--) {
				int_swap (&nodes[j], &nodes[j-1]);
			}
			head2++;
			head1++; // head1 has moved up one position
		}
		
		// if one reaches n, we can leave things as is
		if (head1 == n || head2 == n) {
			break;
		}
	}
}


int
main (int argc, char* argv[])
{
  int i;

	digraph dg;
	while ((dg = read_digraph (stdin)).size != -1) {
    order o = new_order (dg.size);

    for (i = 0; i < o.size; i++) {
      o.nodes [i] = i;
    }
    
    sort (dg, o.nodes, dg.size);

    print_order (stdout, o);
  }
  return 0;
}
