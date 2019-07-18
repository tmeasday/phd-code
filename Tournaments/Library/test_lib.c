#include "upsets.h"

int main (int argc, char* argv[])
{
  order o = read_order (stdin);
  digraph dg = read_digraph (stdin);

  print_order (stdout, o);
  print_digraph (stdout, dg);

  printf ("%f\n", upsets (dg, o));
  printf ("%f\n", indegree (dg, 0));

  return 0;
}
