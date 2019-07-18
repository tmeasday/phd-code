#include <stdio.h>

#ifndef LABELLING_H
#define LABELLING_H

typedef struct {
  int size;
  int* labels;
} labelling;

labelling read_labelling (FILE* in);

labelling new_labelling (int);
labelling random_labelling (int);

labelling copy_labelling (labelling);
void free_labelling (labelling l);


void invert_labelling (labelling l);

void print_labelling (FILE* out, labelling l);

#define labels_right(sg, l, i, j) \
	sg_sign (sg, i, j) * l.labels[i] * l.labels[j]

#endif /* LABELLING_H */