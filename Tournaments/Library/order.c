#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "parser.h"
#include "order.h"
#include "rand.h"
#include "util.h"
#include "array.h"

#define INITIAL_SIZE 10

order
read_order (FILE* infile)
{
  order o;
	o.size = read_int_array (infile, &o.nodes, -1);
  return o;
}

void
print_order (FILE* outfile, order o)
{
  int i;

  fprintf (outfile, "[");
  for (i = 0; i < o.size; i++) {
    if (i > 0) {
      fprintf (outfile, ", ");
    }
    fprintf (outfile, "%d", o.nodes[i]);
  }
  fprintf (outfile, "]\n");
}

order
new_order (int size)
{
  order o;
  o.size = size;
  o.nodes = malloc (size * sizeof(int));
  assert (o.nodes != NULL);

  return o;
}

order
sorted_order (int size)
{
	order o = new_order (size);

	int i;
	for (i = 0; i < size; i++) {
		o.nodes[i] = i;
	}
	
	return o;
}

order 
random_order (int size)
{
  int i, n;
  order o;

  o.size = size;
  o.nodes = malloc (size * sizeof (int));

  for (i = 0; i < size; i++) {
    o.nodes[i] = i;
  }

  // selection-sort random
  for (i = size-1; i > 0; i--) 
  {
    n = get_rand (i+1);

    int_swap (&o.nodes[n], &o.nodes[i]);
  }

  return o;
}

order
copy_order (order o)
{
	order new = new_order (o.size);
	
	int i;
	for (i = 0; i < o.size; i += 1) {
		new.nodes[i] = o.nodes[i];
	}
	
	return new;
}

void 
reverse_order (order o)
{
	int i;
	for (i = 0; i < o.size / 2; i += 1) {
		int_swap (&o.nodes[i], &o.nodes[o.size-i-1]);
	}
}

void
free_order (order o)
{
	free (o.nodes);
}