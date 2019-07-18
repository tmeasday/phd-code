#include <assert.h>
#include "order.h"
#include "digraph.h"
#include "rand.h"
#include "util.h"


// simple bubble sort, as speed isn't really an issue
// note this is not at all random. (not really)
// one way to get proper randomness: just force it to run n times (rather
// than using changed). Doesn't change worst case performance...
// OR just use selection sort or something
// OR merge sort probably wouldn't be too hard either FIXME
void sort_by (int values[], float keys[], int n)
{
  int changed = 1, i;


  while (changed) {
    changed = 0;
    for (i = 0; i < n-1; i++) {
      if (keys [i] > keys [i+1]) {
        float_swap (&keys[i], &keys[i+1]);
        int_swap (&values[i], &values[i+1]);

        changed = 1;
      } else if (keys[i] == keys[i+1]) {
        if (get_rand_frac() > 0.5) {
          float_swap (&keys[i], &keys[i+1]);
          int_swap (&values[i], &values[i+1]);
        }
      }
    }
  }
}

void iterated_kendall (digraph dg, int nodes[], int n) 
{
	float keys[n];
	
	int i;
	for (i = 0; i < n; i += 1) {
		keys[i] = indegree_pos (dg, nodes[i], nodes, n);
	}
	
	sort_by (nodes, keys, n);
	
	float curr = -1; int first = -1;
	for (i = 0; i < n; i += 1) {
		if (keys[i] != curr){
			// if i-1 == first then it was a single one
			int new_n = i - first;
			if (new_n > 1) {
				iterated_kendall (dg, nodes + first, new_n);
			}
			
			curr = keys[i];
			first = i;
		}
	}
}

void
jiggle (order o, int k)
{
	int i; for (i = 0; i < k; i += 1) {
		int from = get_rand (o.size);
		int to   = get_rand (o.size - 1);
		if (to >= from) to += 1;
		
		int_swap (&o.nodes[from], &o.nodes[to]);
	}
}

int
main (int argc, char* argv[])
{
	digraph dg;
	while ((dg = read_digraph (stdin)).size != -1) {
    order o = random_order (dg.size);

    iterated_kendall (dg, o.nodes, 10);

		jiggle (o, o.size);
    
    print_order (stdout, o);
  }
  return 0;
}
