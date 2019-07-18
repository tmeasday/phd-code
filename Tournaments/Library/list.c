/*
   list.c
   Tournaments

		Linked list functions
   
   Created by Tom Coleman on 2007-08-27.
   Copyright 2007 The University of Melbourne. All rights reserved.
*/

#include <assert.h>
#include "list.h"
#include "rand.h"

list_t*
new_list ()
{
	list_t* list = malloc (sizeof (list_t));
	assert (list != NULL);
	list->head = list->foot = NULL;
	return list;
}

node_t*
new_node (void* data, float key)
{
	node_t* node = malloc (sizeof (node_t));
	assert (node != NULL);
	
	node->data = data;
	node->key  = key;

	return node;
}

void
insert_at_head (list_t* list, void* data, float key)
{
	node_t* node = new_node (data, key);
	
	node->next = list->head;
	
	if (list->head == NULL) {
		list->foot = node;
	}
	
	list->head = node;
}

void
insert_at_foot (list_t* list, void* data, float key)
{
	node_t* node = new_node (data, key);
	
	node->next = NULL;
	if (list->foot != NULL) {
		list->foot->next = node;
	} else {
		list->head = node;
	}
	list->foot = node;
}


int
length (list_t* list) {
	int len = 0;
	node_t* node = list->head;
	while (node != NULL) {
		len++;
		node = node->next;
	}
	return len;
}

// quick sort?
void
sort_list (list_t* list)
{	
	// first off, calculate the size of the list
	node_t* node = list->head;
	if (node == NULL) return;

	int size = 1;
	while (node->next != NULL) {
		size++;
		node = node->next;
	}
	
	// now sort
	list_t new_list = quicksort_nodes (list->head, size);
	
	list->head = new_list.head;
	list->foot = new_list.foot;
}

list_t 
quicksort_nodes (node_t* head, int size)
{
	// what's the pivot? -- grab him
	int pivot_n = get_rand (size);
	node_t* pivot = head;

	int i;
	for (i = 0; i < pivot_n; i += 1) {
		pivot = pivot->next;
	}
	
	// ok, now run through the nodes
	node_t *less, *less_end, *equal, *equal_end, *more, *more_end;
	less = equal = more = NULL;
	int n_less = 0, n_more = 0;
	
	node_t *node = head, *curr;
	for (i = 0; i < size; i += 1) {
		curr = node;
		assert (curr != NULL);
		node = node->next;
		curr->next = NULL; // it'll go on the end of some list

		if (curr->key < pivot->key) {
			if (less == NULL) {
				less = curr;
			} else {
				less_end->next = curr;
			}
			less_end = curr;
			n_less++;

		} else if (curr->key == pivot->key) {
			if (equal == NULL) {
				equal = curr;
			} else {
				equal_end->next = curr;
			}
			equal_end = curr;

		} else { // equal
			if (more == NULL) {
				more = curr;
			} else {
				more_end->next = curr;
			}
			more_end = curr;
			n_more++;
		}
	}
	
	// ok, now recurse
	list_t new_list;
	
	if (n_less > 0) {
		list_t less_list = quicksort_nodes (less, n_less);
		equal_end->next = less_list.head;
		new_list.foot = less_list.foot;
	} else {
		new_list.foot = equal_end;
	}
	if (n_more > 0) {
		list_t more_list = quicksort_nodes (more, n_more);
		more_list.foot->next = equal;
		new_list.head = more_list.head;
	} else {
		new_list.head = equal;
	}
	
	return new_list;
}


void
head_to_foot (list_t* list)
{
	assert (list->head != NULL);
	if (list->head->next == NULL) {
		// possibly this should be an error
		return;
	}
	
	list->foot->next = list->head;
	list->head = list->head->next;
	list->foot = list->foot->next;
	list->foot->next = NULL;
}

/*void
node_to_foot (list_t* list, node* prev)
{
	// we are going to work under the assumption that prev is in list, 
	// and has a child. So no asserts...
	
	list->foot->next = prev->next;
	list->foot = list->foot->next;
	prev->next = list->foot->next;
	list->foot->next = NULL;
}*/

void**
to_array (list_t* list)
{
	int size = length (list);
	void** array = malloc (sizeof (void *) * size);
	assert (array != NULL);
	
	node_t* node = list->head;
	int i;
	for (i = 0; i < size; i += 1) {
		array[i] = node->data;
		node = node->next;
	}
	
	return array;
}