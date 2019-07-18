#include <assert.h>
#include "order.h"
#include "digraph.h"
#include "rand.h"
#include "util.h"


int
main (int argc, char* argv[])
{
  int i, j, n, pos;

	digraph dg;
	while ((dg = read_digraph (stdin)).size != -1) {
    order o = new_order (dg.size);

    // this is not at all efficient
    int remaining[dg.size];
    for (i = 0; i < dg.size; i++) {
      remaining[i] = i;
    }
    
    n = dg.size;
    for (i = 0; i < dg.size; i++) {
      /*printf("remaining(%d) is:\n", n);
      for (j = 0; j < n; j++) {
        printf("%d, ", remaining[j]);
      }
      printf("\n");*/

      
      pos = remaining[0];
      float min = indegree_pos (dg, pos, remaining, n);
      int min_index = 0;

      for (j = 1; j < n; j++) {
        float new_min = indegree_pos (dg, remaining[j], remaining, n);
        if (new_min < min) {
          min = new_min;
          min_index = j;
        }
      }

      // we have a winner
      o.nodes[i] = remaining[min_index];
      n = n-1;

      for (j = min_index; j < n; j++) {
        remaining[j] = remaining[j+1];
      }
    }
    
    print_order (stdout, o);
  }
  return 0;
}
