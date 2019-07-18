#include "cost.h"
#include "assert.h"

int
cost (sigraph sg, labelling l)
{
  int pos[l.size];
  int i;
  for (i = 0; i < l.size; i++) {
    pos[i] = i;
  }
  
  return cost_pos (sg, l, pos, l.size);
}

int
cost_pos (sigraph sg, labelling l, int pos[], int npos)
{
  int i, j;
  int cost = 0;
  
  assert (l.size == l.size);
  assert (npos <= l.size);
  
  for (i = 0; i < npos; i++) {
    for (j = i+1; j < npos; j++) {
      // do the labels agree with this edge?
			if (labels_right (sg, l, pos[i], pos[j]) == -1) {
				cost += 1;
			}
    }
  }

  return cost;
}
