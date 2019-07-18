#include <strings.h>
#include <assert.h>

#include "digraph.h"
#include "rand.h"

int main (int argc, char const* argv[])
{
	int random = 0;
	if (argc == 2) {
		assert (strcmp (argv[1], "random") == 0);
		random = 1;
	}

	digraph dg;
	while ((dg = read_digraph (stdin)).size != -1) {
		// of course this is a memory leak
		dg = round_digraph (dg, random);
		print_digraph (stdout, dg);
	}
	return 0;
}