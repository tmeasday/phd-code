#include "upsets.h"
#include "assert.h"

float
upsets (digraph dg, order o)
{
  int pos[o.size];
  int i;
  for (i = 0; i < o.size; i++) {
    pos[i] = i;
  }
  
  return upsets_pos (dg, o, pos, o.size);
}

float
upsets_pos (digraph dg, order o, int pos[], int npos)
{
  int i, j;
  float cost = 0;
  
  assert (dg.size == o.size);
  assert (npos <= o.size);
  
  for (i = 0; i < npos; i++) {
    for (j = i+1; j < npos; j++) {
      // what is the weight of the back edge?
      cost += dg_weight (dg, o.nodes[pos[j]], o.nodes[pos[i]]);
    }
  }

  return cost;
}
