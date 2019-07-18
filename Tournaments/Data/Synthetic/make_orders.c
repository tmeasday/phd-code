#include <assert.h>
#include <stdlib.h>
#include "order.h"

int main (int argc, char const* argv[])
{
	assert (argc == 3);
	int n = atoi (argv[1]);
	int runs = atoi (argv[2]);
	
	int i;
	for (i = 0; i < runs; i += 1) {
		order o = random_order (n);
		print_order (stdout, o);
	}
	
	return 0;
}