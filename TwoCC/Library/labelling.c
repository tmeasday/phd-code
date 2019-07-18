#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "parser.h"
#include "labelling.h"
#include "rand.h"
#include "util.h"
#include "array.h"

#define INITIAL_SIZE 10

// LABELS are +- 1

labelling
read_labelling (FILE* infile)
{
  labelling l;
	l.size = read_int_array (infile, &l.labels, -1);
  return l;
}

void
print_labelling (FILE* outfile, labelling l)
{
  int i;

  fprintf (outfile, "[");
  for (i = 0; i < l.size; i++) {
    if (i > 0) {
      fprintf (outfile, ", ");
    }
    fprintf (outfile, "%d", l.labels[i]);
  }
  fprintf (outfile, "]\n");
}

labelling
new_labelling (int size)
{
  labelling l;
  l.size = size;
  l.labels = malloc (size * sizeof(int));
  assert (l.labels != NULL);

  return l;
}

labelling 
random_labelling (int size)
{
  int i;
  labelling l = new_labelling (size);
 
  for (i = 0; i < size; i++) {
    l.labels[i] = 2 * get_rand (2) - 1;
  }

  return l;
}

labelling
copy_labelling (labelling l)
{
	labelling new = new_labelling (l.size);
	
	int i;
	for (i = 0; i < l.size; i += 1) {
		new.labels[i] = l.labels[i];
	}
	
	return new;
}

void 
invert_labelling (labelling l)
{
	int i;
	for (i = 0; i < l.size; i += 1) {
		l.labels[i] *= -1;
	}
}

void
free_labelling (labelling l)
{
	free (l.labels);
}