/*
   random.c
   Tournaments

		Here I want to test how long it takes to read in the tournament, then do nothing with it.
   
   Created by Tom Coleman on 2007-09-14.
   Copyright 2007 The University of Melbourne. All rights reserved.
*/

#include <assert.h>
#include <string.h>
#include "order.h"
#include "digraph.h"

int
main (int argc, char* argv[])
{
	digraph dg;
	while ((dg = read_digraph (stdin)).size != -1) {

		order o = random_order (dg.size);
    print_order (stdout, o);
  }
  return 0;
}
