/*
   edge_list_iterator.c
   Tournaments
   
   Created by Tom Coleman on 2007-09-14.
   Copyright 2007 The University of Melbourne. All rights reserved.
*/

#include <assert.h>
#include <stdlib.h>

#include "edge_list_iterator.h"
#include "edge_list.h"
#include "edge.h"

el_iterator*
new_el_iterator (edge_list* list)
{
	el_iterator* it = malloc (sizeof (el_iterator));
	assert (it != NULL);
	
	it->list = list;
	it->curr = NULL;
	
	return it;
}


edge
get_it_first_edge (el_iterator* it)
{
 	edge_bucket* bucket = it->list->head->data;

	it->curr = bucket->edges->head;

	edge_list_data* data = it->curr->data;
	return data->e;
}

edge
get_it_next_edge (el_iterator* it)
{
	assert (it->curr != NULL);
	
	if (it->curr->next != NULL) {
		it->curr = it->curr->next;
	} else {
		edge_list_data* data = it->curr->data;
		dlnode_t* bucket_node = data->bucket;
		
		// this means we are at the very end of the edge_list
		if (bucket_node->next == NULL) {
			it->curr = NULL;
			return (edge) {0,0};
		}
		
		// else
		bucket_node = bucket_node->next;
		edge_bucket* bucket = bucket_node->data;
		it->curr = bucket->edges->head;
	}
	
	edge_list_data* data = it->curr->data;
	return data->e;
}