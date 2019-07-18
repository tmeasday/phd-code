#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "parser.h"
#include "array.h"
#include "util.h"
#include "rand.h"

#define INITIAL_SIZE 10

int
read_int_array (FILE* infile, int** itemsp, int expected_size)
{
  int i = 0, curr_size = expected_size;
	char c = ',';

	if (curr_size == -1) {
		curr_size = INITIAL_SIZE;
	}  

	int * items = malloc (curr_size * sizeof (int));
  assert (items != NULL);


  assert (getnextc (infile) == '[');
  while (fscanf (infile, "%d", &items[i++]) == 1) {
    //printf ("I'm in here\n");
    if (i == curr_size) {
      curr_size *= 2;
      items = realloc (items, curr_size * sizeof (int));
      assert (items != NULL);
    }

    c  = getnextc (infile);
    if (c == ']') {
      break;
    } else {
      assert (c == ',');
    }
  }

	if (c != ']') { // we didn't exit via the break
		assert (i-- == 1); // too big for boots
		assert (getnextc (infile) == ']');
	}

	*itemsp = items;
  return i;
}

// I'm sure there's a more generic way I can do it, but it feels too hard
int
read_float_array (FILE* infile, float** itemsp, int expected_size)
{
  int i = 0, curr_size = expected_size;
	char c = ',';
	float d;

	if (curr_size == -1) {
		curr_size = INITIAL_SIZE;
	}
	
	float * items = malloc (curr_size * sizeof (float));
  assert (items != NULL);

  assert (getnextc (infile) == '[');
  while (fscanf (infile, "%f", &d) == 1) {
		items[i++] = d;
    //printf ("I'm in here\n");
    if (i == curr_size) {
      curr_size *= 2;
      items = realloc (items, curr_size * sizeof (float));
      assert (items != NULL);
    }

    c = getnextc (infile);
    if (c == ']') {
      break;
    } else {
      assert (c == ',');
    }
  }

	if (c != ']') { // we didn't exit via the break
		assert (i == 0);
		assert (getnextc (infile) == ']');
	}

	*itemsp = items;
  return i;
}

int*
sorted_array (int n)
{
	int* array = malloc (sizeof (int) * n);
	assert (array != NULL);

	int i;
	for (i = 0; i < n; i++) {
		array[i] = i;
	}

	return array;
}


int*
zero_array (int n) {
	int* array = malloc (sizeof (int) * n);
	assert (array != NULL);

	int i;
	for (i = 0; i < n; i++) {
		array[i] = 0;
	}

	return array;
}

int*
rand_array (int size)
{
	int n = size, next;
	int* array = sorted_array (size);
	
	while (n > 0) {
		next = get_rand (n--);
		int_swap (&array[next], &array[n]);
	}
	
	return array;
}

void
print_array (FILE* out, int array[], int size)
{
	int i; for (i = 0; i < size; i += 1) {
		if (i == 0) {
			printf ("%d", array [i]);
		} else {
			printf (" %d", array[i]);
		}
	}
	printf ("\n");
}

int*
rand_perm (int size)
{
	int n = size, next;
	int* array = sorted_array (size);
	
	while (n > 0) {
		next = get_rand (n--);
		int_swap (&array[next], &array[n]);
	}
	
	return array;
}
