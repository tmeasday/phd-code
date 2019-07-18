#ifndef _DEGREES_H_
#define _DEGREES_H_

typedef struct {
	int* degs;
	int* n_of_size;
	int n_zeros;
} degrees;



degrees* new_degrees (int n);
void set_degree (degrees* degs, int index, int degree);
void inc_degree (degrees* degs, int index);
void dec_degree (degrees* degs, int index);
void print_degrees (degrees* degs, int n);

int can_disregard (degrees* degs, int i, int n);

#endif /* _DEGREES_H_ */
