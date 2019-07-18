/*
   edge_list.c
   Tournaments
   
   Created by Tom Coleman on 2007-08-31.
   Copyright 2007 The University of Melbourne. All rights reserved.
*/

#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "dllist.h"
#include "edge.h"
#include "edge_list.h"
#include "util.h"
#include "rand.h"

edge_list*
new_edge_list ()
{
	edge_list* list = new_dllist ();
	return list;
}

edge_bucket*
new_edge_bucket (int key)
{	
	edge_bucket* bucket = malloc (sizeof (edge_bucket));
	assert (bucket != NULL);
	
	bucket->key = key;
	bucket->edges = new_dllist ();
	
	return bucket;
}

edge_list_data*
new_edge_list_data (edge e, float key)
{
	edge_list_data* data = malloc (sizeof (edge_list_data));
	assert (data != NULL);
	
	data->e = e;
	data->key = key;
	data->bucket = NULL;
	
	return data;
}

dlnode_t*
insert_into_edge_list (edge_list* list, edge e, float key)
{
	int bucket_key = floor (key);

	edge_list_data* data = new_edge_list_data (e, key);

	dlnode_t* node = list->head;
	edge_bucket* bucket;
	dlnode_t* new_node;
	
	if (node == NULL) {
		bucket = new_edge_bucket (bucket_key);
		new_node = dl_insert_at_head (bucket->edges, data);
		
		// now put the bucket into the list
		data->bucket = dl_insert_at_head (list, bucket);
		
	} else {
		// search down the spine for the bucket for this node (if it exists)
		edge_bucket* node_bucket;
		while (node != NULL) {
			node_bucket = node->data;
			if (node_bucket->key <= bucket_key) {
				break;
			}
			node = node->next;
		}
		
		if (node == NULL) {
			bucket = new_edge_bucket (bucket_key);
			new_node = dl_insert_at_head (bucket->edges, data);
			
			// in this case the bucket's going at the end
			data->bucket = dl_insert_at_foot (list, bucket);
			
		} else if (node_bucket->key < bucket_key) {
			// we need a new bucket
			bucket = new_edge_bucket (bucket_key);
			new_node = dl_insert_at_head (bucket->edges, data);

			// this time its going somewhere inside
			data->bucket = dl_insert_before (list, node, bucket);
			
		} else { // ok. the right bucket already exists.
			bucket = node->data;
			assert (bucket->edges->head != NULL); // this shouldn't happen
			
			data->bucket = node;
			new_node = insert_into_bucket (bucket, data);
		}	
	}
	
	return new_node;
}

edge
get_first_edge (edge_list* list)
{
	assert (list->head != NULL);
	edge_bucket* bucket = list->head->data;
	edge_list_data* data = bucket->edges->head->data;
	return data->e;
}

dlnode_t*
inc_edge_key (edge_list* list, dlnode_t* node, float increment)
{
	edge_list_data* data = node->data;
	return change_edge_key (list, node, data->key + increment);
}


dlnode_t*
change_edge_key (edge_list* list, dlnode_t* node, float new_key)
{
	edge_list_data* data = node->data;

//	printf ("setting %d->%d from %.2f to %.2f\n", data->e.from, data->e.to, data->key, new_key);
	
	// ok. is the change +ve, -ve, or equal (ish)
	int sgn = floor (new_key) - floor (data->key);
	// set the new key
	data->key = new_key;
	
	int bucket_key =  floor (new_key);
	dlnode_t* bucket_node = data->bucket;
	
	if (sgn != 0) {		
		// remove node from bucket
		edge_bucket* bucket = bucket_node->data;
		dllist_t* node_list = bucket->edges;
		delete_from_dllist (node_list, node);
	
		// if the list is now empty
		if (node_list->head == NULL) {
			dlnode_t* temp = bucket_node->next;
			if (temp == NULL) {
				temp = bucket_node->prev;
			}
		
			// delete the bucket
			delete_from_dllist (list, bucket_node);
			
			free (bucket->edges);
			free (bucket);
			free (bucket_node);
		
			bucket_node = temp;
		}
		
		// find the bucket to be inserted to.
		
		if (sgn > 0) { // going up the list
			while (bucket_node != NULL) {
				bucket = bucket_node->data;
				if (bucket->key >= bucket_key) {
					break;
				}
				bucket_node = bucket_node->prev;
			}
			
			// now, if we need a new bucket, we'll need to go back one
			if (bucket->key != bucket_key) {
				if (bucket_node == NULL) {
					bucket_node = list->head;
				} else {
					bucket_node = bucket_node->next;
				}
			}
			
		} else { // going down the list
			while (bucket_node != NULL) {
				bucket = bucket_node->data;
				if (bucket->key <= bucket_key) {
					break;
				}
				bucket_node = bucket_node->next;
			}
		}
	
		// insert into correct bucket
		if (bucket_node == NULL) { // we're at the foot
			// we need a new bucket
			bucket = new_edge_bucket (bucket_key);
			free (node);
			node = dl_insert_at_head (bucket->edges, data);

			data->bucket = dl_insert_at_foot (list, bucket);
			
		} else {
			bucket = bucket_node->data;
			if (bucket->key != bucket_key) {
				// we need a new bucket
				bucket = new_edge_bucket (bucket_key);
				free (node);
				node = dl_insert_at_head (bucket->edges, data);

				data->bucket = dl_insert_before (list, bucket_node, bucket);
				
			} else {
				// put node into the right spot of bucket's list
				data->bucket = bucket_node;
				free (node);
				node = insert_into_bucket (bucket, data);
			}
		}
		
	// didn't need to move bucket
	} else {		
		edge_bucket* bucket = data->bucket->data;
		
		// we could be smarter and incrementally move, but I don't think 
		// this will happen much, so it's probably not worth any effort.
		// (note - data->bucket has not changed)
		delete_from_dllist (bucket->edges, node);
		free (node);
		node = insert_into_bucket (bucket, data);
	}	
	
	return node;
}

// insert from the front
/*dlnode_t*
insert_into_bucket (edge_bucket* bucket, edge_list_data* data)
{
	dlnode_t* node = bucket->edges->head; 
	edge_list_data* node_data;
	while (node != NULL) {
		node_data = node->data;
		if (node_data->key <= data->key) {
			break;
		}
		node = node->next;
	}

	if (node == NULL) {
		return dl_insert_at_foot (bucket->edges, data);
	} else {
		return dl_insert_before (bucket->edges, node, data);
	}
}*/

// insert from the back
/*dlnode_t*
insert_into_bucket (edge_bucket* bucket, edge_list_data* data)
{
	dlnode_t* node = bucket->edges->foot; 
	edge_list_data* node_data;
	while (node != NULL) {
		node_data = node->data;
		if (node_data->key >= data->key) {
			break;
		}
		node = node->prev;
	}

	if (node == NULL) {
		return dl_insert_at_head (bucket->edges, data);
	} else if (node == bucket->edges->foot) {
		return dl_insert_at_foot (bucket->edges, data);
	} else {
		return dl_insert_before (bucket->edges, node->next, data);
	}
}*/

// insert from the back (randomized)
dlnode_t*
insert_into_bucket (edge_bucket* bucket, edge_list_data* data)
{
	dlnode_t* node = bucket->edges->foot; 
	edge_list_data* node_data;
	while (node != NULL) {
		node_data = node->data;
		if (node_data->key > data->key) {
			break;
		} else if (node_data->key == data->key) {
			if (get_rand_frac () > 0.5) {
				break;
			}
		}
		node = node->prev;
	}

	if (node == NULL) {
		return dl_insert_at_head (bucket->edges, data);
	} else if (node == bucket->edges->foot) {
		return dl_insert_at_foot (bucket->edges, data);
	} else {
		return dl_insert_before (bucket->edges, node->next, data);
	}
}


void
print_edge_list (FILE* out, edge_list* list)
{
	dlnode_t* bucket_node = list->head;
	while (bucket_node != NULL) {
		edge_bucket* bucket = bucket_node->data;
		fprintf (out, "%d: ", bucket->key);
		
		dlnode_t* node = bucket->edges->head;
		while (node != NULL) {
			edge_list_data* data = node->data;
			fprintf (out, "%d->%d (%.1f), ", data->e.from, data->e.to, data->key);	
			node = node->next;
		}	
		
		fprintf (out, "\n");
		bucket_node = bucket_node->next;
	}
	fprintf (out, "\n");
}

// this guy is assuming we aren't sampling, or using weights
// TAKE note!
void
print_edge_list_short (FILE* out, edge_list* list, int n)
{	
	int counts[n];
	int i; for (i = 0; i < n; i += 1) {
		counts[i] = 0;
	}
	
	dlnode_t* bucket_node;
	for (bucket_node = list->head; bucket_node != NULL; bucket_node = bucket_node->next) {
		edge_bucket* bucket = bucket_node->data;
		counts[bucket->key] = dl_length (bucket->edges);
	}
	
	for (i = 0; i < n; i += 1) {
		fprintf (out, "%d ", counts[i]);
	}
	fprintf (out, "\n");
}

void reverse_edge_list_edge (dlnode_t* node)
{
	edge_list_data* data = node->data;
	int_swap (&(data->e.from), &(data->e.to));
}

// FIXME -- this function is far from optimized
// but then again, probably is only going to be used for prototyping
edge* 
edge_list_to_array (edge_list* list)
{
	// let's avoid needless reallocs (TODO -- could be faster with the reallocs)
	int length = 0;
	dlnode_t* bucket_node = list->head;
	while (bucket_node != NULL) {
		edge_bucket* bucket = bucket_node->data;
		length += dl_length (bucket->edges);
	}
	
	edge* array = malloc (sizeof (edge) * length);
	
	// and a second run through the list
	bucket_node = list->head;
	int pos = 0;
	while (bucket_node != NULL) {
		edge_bucket* bucket = bucket_node->data;
		dlnode_t* node = bucket->edges->head;
		while (node != NULL) {
			edge_list_data* data = node->data;
			array[pos++] = data->e;
			node = node->next;
		}
	}
	
	return array;
}
