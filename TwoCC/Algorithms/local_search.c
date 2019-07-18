// 
//  local_search.c
//  TwoCC
//  
//  Created by Tom Coleman on 2008-03-07.
//  Copyright 2008 Tom Coleman. All rights reserved.
// 

#include <assert.h>
#include <string.h>

#include "labelling.h"
#include "sigraph.h"
#include "rand.h"
#include "array.h"
#include "local_search.h"

#define MIN_IMP 0.001

int
main (int argc, char* argv[])
{
	int initial = 0;
	
	if (argc > 1) {
		if (strcmp (argv[1], "-i") == 0) {
			initial = 1;
		}
	}

	sigraph sg; labelling l;
	while ((sg = read_sigraph (stdin)).size != -1) {
		if (initial) {
			l = read_labelling (stdin);
			assert (l.size == sg.size);
		} else {
			l = random_labelling (sg.size);
		}
		
		l = search (sg, l);
		
    print_labelling (stdout, l);
		free_labelling (l);
  }
  return 0;
}
