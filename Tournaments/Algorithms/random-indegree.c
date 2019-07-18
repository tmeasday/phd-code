/*
   random-indegree.c
   Tournaments
   
   Created by Tom Coleman on 2007-08-24.
   Copyright 2007 The University of Melbourne. All rights reserved.
*/

#include "EdgeRankings/edge_rank_algorithm.h"
#include "EdgeRankings/random_ranker.c"
#include "EdgeRankings/indegree_flipper.c"
#include "digraph.h"
#include "order.h"


int main (int argc, char const* argv[])
{
	edge_rank_algorithm (random_ranker, indegree_flipper);
	return 0;
}