#include "cost.h"
#include "tree.h"

int main (int argc, char* argv[])
{
  sigraph sg = read_sigraph (stdin);
  // labelling l = read_labelling (stdin);

  print_sigraph (stdout, sg);
  // print_labelling (stdout, l);

 // printf ("%d\n", cost (sg, l));

	tree* t = bfs_st (sg, 1);
	// printf ("I have the tree\n");
	print_tree (stdout, t);
	
  return 0;
}
