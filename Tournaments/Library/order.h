#include <stdio.h>

#ifndef ORDER_H
#define ORDER_H

typedef struct {
  int size;
  int* nodes;
} order;

order read_order (FILE*);

order new_order (int);
order sorted_order (int);
order random_order (int);

order copy_order (order);
void reverse_order (order o);
void free_order (order o);

void print_order (FILE*, order);
#endif