#include <assert.h>
#include <stdlib.h>
#include <strings.h>
#include "rand.h"

#include <stdio.h>

double get_rand_frac ()
{
  static int first = 1;

  if (first) {
    //printf ("seeding, %d\n", time (NULL));
		char* env = getenv ("SEED");
		if (env == NULL || strcmp (env, "") == 0) {
			srand (time (0));
		} else {
			srand (atoi (env));
		}
    first = 0;

    // what is wrong here?
    // (doesn't work right on mac, first call to rand is always the same)
    rand ();
    //printf ("%d\n", rand ());
  }

  return (double)rand() / ((double)RAND_MAX +1);
}


int get_rand (int range)
{
	assert (range > 0);
  return (int) (get_rand_frac() * range);
}