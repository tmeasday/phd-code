#include <assert.h>
#include <math.h>
#include "order.h"
#include "digraph.h"
#include "rand.h"
#include "util.h"

order find_best_order (digraph);

int
main (int argc, char* argv[])
{
	digraph dg;
	while ((dg = read_digraph (stdin)).size != -1) {
    order o = find_best_order (dg);
    
    print_order (stdout, o);
  }
  return 0;
}

order 
find_best_order (digraph dg)
{
  int i, j, pi, pj, n = dg.size;

  float indeg[dg.size], outdeg[dg.size];
  for (i = 0; i < dg.size; i++) {
    indeg[i] = indegree (dg, i);
    outdeg[i] = outdegree (dg, i);
  }

  int pos[dg.size];
  first_n (pos, dg.size);

  order o = new_order (dg.size);
  int *start = o.nodes, *end = o.nodes + n - 1;
  
  while (n > 0) {
    // remove sources and sinks. This step perhaps makes little sense in the
    // weighted case. But will give a small speedup if it does happen.
    int cont = 0;
    for (i = 0; i < n; i++) {
      pi = pos[i];

      if (indeg[pi] == 0) { // source
        *start = pi;
        start++; n--;
        int_swap (&pos[i], &pos[n]);

        for (j = 0; j < n; j++) {
          pj = pos[j];
          indeg[pj] -= dg_weight (dg, pi, pj);
        }
        cont=1; // do another loop
        break;
        
      } else if (outdeg[pi] == 0) { // sink
        *end = pi;
        end--; n--;
        int_swap (&pos[i], &pos[n]);

        for (j = 0; j < n; j++) {
          pj = pos[j];
          outdeg[pj] -= dg_weight (dg, pj, pi);
        }

        cont=1; // do another loop
        break;
      }
    }

    if (cont) continue;

    // no more sources or sinks? 
    // then find the node with maximum |delta|
    float index_max = 0;
    float delta_max = fabs (indeg[pos[0]] - outdeg[pos[0]]);
    float delta;
    for (i = 1; i < n; i++) {
      pi = pos[i];
      delta = fabs (indeg[pi] - outdeg[pi]); 

      // FIXME -- should this be randomized?
      if ( delta > delta_max ) {
        delta_max = delta;
        index_max = i;
      }
    }

    // now remove the node with max delta
    i = index_max; pi = pos[i];
    // FIXME -- again, should be randomized??
    if (indeg[pi] > outdeg[pi]) {
      *end = pi;
      end--; n--;
      int_swap (&pos[i], &pos[n]);
    } else {
      *start = pi;
      start++; n--;
      int_swap (&pos[i], &pos[n]);
    }

    for (j = 0; j < n; j++) {
      pj = pos[j];
      outdeg[pj] -= dg_weight (dg, pj, pi);
      indeg[pj]  -= dg_weight (dg, pi, pj);
    }
  }

  return o;
}
