#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <stdlib.h>

int read_int_array (FILE*, int**, int);
int read_float_array (FILE*, float**, int);
int* sorted_array (int);
int* zero_array (int n);
int* rand_array (int);
int* rand_perm (int size);


void print_array (FILE* out, int array[], int size);

#endif /* _ARRAY_H_ */
