#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#include "parser.h"
#include "sigraph.h"
#include "rand.h"
#include "array.h"

#define INITIAL_SIZE 10

sigraph
new_sigraph (int size)
{
  sigraph sg;
  sg.size = size;
  sg.signs = malloc (sizeof(int*) * (sg.size));
  assert(sg.signs != NULL);

  int i, j;
  for (i = 0; i < sg.size; i++) {
    sg.signs[i] = malloc ((sg.size - i - 1) * sizeof(int));
		assert (sg.signs[i] != NULL);

		// set everything to 0
		for (j = 0; j < sg.size-i-1; j++) {
			sg.signs[i][j] = 0;
		}
  }

  return sg;
}

void
free_sigraph (sigraph sg)
{
	int i;
	for (i = 0; i < sg.size-1; i++) {
		free (sg.signs[i]);
	}
	
	free (sg.signs);
}


sigraph
read_sigraph (FILE* infile)
{
  sigraph sg;
  int i = 0;
  
	char c = getnextc (infile);
	if (c == EOF) {
		sg.size = -1;
		sg.signs = NULL;
		return sg;
	}
	
	// otherwise
	assert (c == '[');

  // first read the first line (this will tell us size)
	int* line;
	int size = read_int_array (stdin, &line, -1);

  // so now we know how big it needs to be, lets allocate away
	sg.size = size+1;
	sg.signs = malloc (sizeof (int*) * sg.size);
	assert (sg.signs != NULL);
  sg.signs[0] = line;
  
  // now we can do the rest of the lines
  if ( sg.size > 1) {
    assert (getnextc (infile) == ',');
  }
  
  for (i = 1; i < sg.size; i++) {
    // read the line
		size = read_int_array (stdin, &sg.signs[i], sg.size - (i+1));
		assert (size == sg.size - (i+1));
		
    if (i < sg.size - 1) {
      assert (getnextc (infile) == ',');
    }
  }

	// close of the list
  assert (getnextc (infile) == ']');
  
  return sg;
}

/*
digraph
round_digraph (digraph old, int atrandom)
{
	digraph new = new_digraph (old.size);
	
	int i, j;
	for (i = 0; i < old.size; i += 1) {
		for (j = i+1; j < old.size; j += 1) {				
			float weight = dg_weight (old, i, j);
			float test = 0.5;
			if (atrandom) {
				test = get_rand_frac ();
			}
			
			if (test == weight) {
				set_dg_weight (new, i, j, get_rand (2));
			} else if (test < weight) {
				set_dg_weight (new, i, j, 1);
			} else {
				set_dg_weight (new, i, j, 0);
			}
		}
	}
	
	return new;
}
*/

void
print_sigraph (FILE* outfile, sigraph sg)
{
  int i, j;

  fprintf (outfile, "[");
  for (i = 0; i < sg.size; i++) {
    if (i > 0) {
      //fprintf (outfile, ",\n");
      fprintf (outfile, ", ");
    }
    fprintf (outfile, "[");
    for (j = 0; j < sg.size - i - 1; j++) {
      if (j > 0) { 
        fprintf (outfile, ", ");
      }
      
      fprintf (outfile, "%d", sg.signs[i][j]);
    }
    fprintf (outfile, "]");
  }
  fprintf (outfile, "]\n");
}

int
sg_sign (sigraph sg, int i, int j) {
  assert (j < sg.size);
  assert (i < sg.size);

  if (i == j) { 
    return 0;
  }

  if (i < j) {
    return sg.signs[i][j-i-1];
  } else {
    return sg.signs[j][i-j-1];
  }
}

void
set_sg_sign (sigraph sg, int i, int j, int sign) {
  assert (j < sg.size);
  assert (i < sg.size);
	assert (i != j);

  if (i < j) {
		sg.signs[i][j-i-1] = sign;
  } else {
    sg.signs[j][i-j-1] = sign;
  }
}

/*
void
set_dg_weight_transitive (digraph dg, int i, int j)
{
	set_dg_weight (dg, i, j, 1);
	
	int k;
	int into_i[dg.size], n_into = 0, outof_j[dg.size], n_outof = 0;
	for (k = 0; k < dg.size; k += 1) {
		if (k == i || k == j) continue;
		
		if (dg_weight (dg, k, i) == 1) {
			set_dg_weight (dg, k, j, 1);
			into_i[n_into++] = k;
		} else if (dg_weight (dg, j, k) == 1) {
			set_dg_weight (dg, i, k, 1);
			outof_j[n_outof++] = k;
		}
	}
	
	for (i = 0; i < n_into; i += 1) {
		for (j = 0; j < n_outof; j += 1) {
			set_dg_weight (dg, into_i[i], outof_j[j], 1);
		}
	}
}
*/

/*
float
indegree (digraph dg, int node)
{
  int pos[dg.size];
  int i;
  for (i = 0; i < dg.size; i++) {
    pos[i] = i;
  }
  
  return indegree_pos (dg, node, pos, dg.size);
}

float
indegree_pos (digraph dg, int node, int pos[], int npos)
{
  int i;
  float deg = 0;

  for (i = 0; i < npos; i++) {
    deg += dg_weight (dg, pos[i], node);
  }

  return deg;
}

float
outdegree (digraph dg, int node)
{
  int pos[dg.size];
  int i;
  for (i = 0; i < dg.size; i++) {
    pos[i] = i;
  }
  
  return outdegree_pos (dg, node, pos, dg.size);
}

float
outdegree_pos (digraph dg, int node, int pos[], int npos)
{
  int i;
  float deg = 0;

  for (i = 0; i < npos; i++) {
    deg += dg_weight (dg, node, pos[i]);
  }

  return deg;
}
*/

/*
// if a digraph is transistive, return the order that it represents
// NOTE -- assumes unweighted dg.
order
digraph_to_order (digraph dg)
{
	order o = new_order (dg.size);
	int i, j;

	// work out the indegrees
	int in_degs[dg.size];
	for (i = 0; i < dg.size; i += 1) {
		in_degs[i] = 0;
	}
	for (i = 0; i < dg.size; i += 1) {
		for (j = i+1; j < dg.size; j += 1) {
			if (dg_weight (dg, i, j)) {
				in_degs[j] += 1;
			} else {
				in_degs[i] += 1;
			}
		}
	}

	for (i = 0; i < dg.size; i += 1) {
		for (j = 0; j < dg.size; j += 1) {
			if (in_degs[j] == i) {
				o.nodes[i] = j;
				break;
			}
		}
		// we _have_ to find something here
		assert (j != dg.size);
	}
	
	return o;
}
*/


sigraph
subsigraph (sigraph sg, int pos[], int n_pos)
{
	sigraph new = new_sigraph (n_pos);
	
	int i; for (i = 0; i < n_pos; i += 1) {
		int j;	for (j = i+1; j < n_pos; j += 1) {
			set_sg_sign (new, i, j, sg_sign (sg, pos[i], pos[j]));
		}
	}
	
	return new;
}

/*
// only works for unweighted tournaments
int*
get_vertex_triangle_counts (digraph dg)
{
	int* counts = malloc (dg.size * sizeof (int));
	assert (counts != NULL);
	int i; for (i = 0; i < dg.size; i += 1) {
		counts[i] = 0;
	}
	
	for (i = 0; i < dg.size; i += 1) {
		int j; for (j = i+1; j < dg.size; j += 1) {
			int k; for (k = j+1; k < dg.size; k += 1) {
				int w1 = dg_weight (dg, i, j);
				int w2 = dg_weight (dg, j, k);
				int w3 = dg_weight (dg, k, i);
				if (w1 == w2 && w2 == w3) {
					counts[i] += 1;
					counts[j] += 1;
					counts[k] += 1;
				}
			}
		}
	}
	
	return counts;
}
*/