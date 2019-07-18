#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define NO_VOTE -2

int
main (int argc, char** argv)
{
	int i, j, k;
	int person, item;
	float vote;
	
	if (argc < 4)
	{
		printf ("Please enter a threshold, + the relevant sizes\n");
		exit (1);
	}
	
	float threshold = atof (argv[1]);

	// this is actually the _highest_ # not the actual # (cos there are gaps)
	int n_people    = atoi (argv[2]);
	int n_items     = atoi (argv[3]);
	
//	float votes[n_people][n_items];
	float** votes = malloc (sizeof (float*) * n_people);
	assert (votes != NULL);
	for (i = 0; i < n_people; i += 1) {
		votes[i] = malloc (sizeof (float) * n_items);
		assert (votes[i] != NULL);
		for (j = 0; j < n_items; j += 1) {
			votes[i][j] = NO_VOTE;
		}
	}
	
	// read in the votes
	while (scanf ("%d%d%f", &person, &item, &vote) == 3) {
		assert (person < n_people);
		assert (item < n_items);
		assert (-1 <= vote && vote <= 1);
		votes[person][item] = vote;
	}
	
	for (i = 0; i < n_people; i += 1) {
		for (j = i+1; j < n_people; j += 1) {
			float corr = 0;
			for (k = 0; k < n_items; k += 1) {
				if (votes[i][k] != NO_VOTE && votes[j][k] != NO_VOTE) {
					corr += votes[i][k] * votes[j][k];
				}
			}
			
			if (corr >= threshold) {
				printf ("%d\t%d\t%f\n", i, j, corr);
			}
		}
	}
	
	return 0;
}