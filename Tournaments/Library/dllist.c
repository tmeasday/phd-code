/*
   list.c
   Tournaments

		Linked list functions
   
   Created by Tom Coleman on 2007-08-27.
   Copyright 2007 The University of Melbourne. All rights reserved.
*/

#include <assert.h>
#include <stdlib.h>
#include "dllist.h"

dllist_t*
new_dllist ()
{
	dllist_t* list = malloc (sizeof (dllist_t));
	assert (list != NULL);
	list->head = list->foot = NULL;
	return list;
}

dlnode_t*
new_dlnode (void* data)
{
	dlnode_t* node = malloc (sizeof (dlnode_t));
	assert (node != NULL);
	
	node->data = data;

	return node;
}

dlnode_t*
dl_insert_at_head (dllist_t* list, void* data)
{
	dlnode_t* node = new_dlnode (data);
	
	node->next = list->head;
	node->prev = NULL;
	
	if (list->head != NULL) {
		list->head->prev = node;
	} else {
		list->foot = node;
	}
	
	list->head = node;
	
	return node;
}

dlnode_t*
dl_insert_at_foot (dllist_t* list, void* data)
{
	dlnode_t* node = new_dlnode (data);
	
	node->next = NULL;
	node->prev = list->foot;

	if (list->foot != NULL) {
		list->foot->next = node;
	} else {
		list->head = node;
	}

	list->foot = node;
	
	return node;
}

dlnode_t*
dl_insert_before (dllist_t* list, dlnode_t* node, void* data)
{
	dlnode_t* new_node = new_dlnode (data);
	
	new_node->next = node;
	new_node->prev = node->prev;
	node->prev = new_node;
	
	if (node == list->head) {
		list->head = new_node;
	} else {
		new_node->prev->next = new_node;
	}
	
	return new_node;
}

void
delete_from_dllist (dllist_t* list, dlnode_t* node)
{
	if (node == list->head) {
		list->head = node->next;
	} else {
		node->prev->next = node->next;
	}
	
	if (node == list->foot) {
		list->foot = node->prev;
	} else {
		node->next->prev = node->prev;
	}
}

void
reinsert_node_before (dllist_t* list, dlnode_t* node, dlnode_t* ref)
{
	if (ref == NULL) {
		// tail of the list
		if (list->foot == NULL) {
			list->head = list->foot = node;
			node->next = node->prev = NULL;
		} else {
			list->foot->next = node;
			node->prev = list->foot;
			list->foot = node;
			node->next = NULL;
		}
	} else {
		node->prev = ref->prev;
		node->next = ref;
		if (ref == list->head) {
			list->head = node;
		} else {
			ref->prev->next = node;
		}
		ref->prev = node;
	}
}

int
dl_length (dllist_t* list) {
	int len = 0;
	dlnode_t* node = list->head;
	while (node != NULL) {
		len++;
		node = node->next;
	}
	return len;
}


void**
dl_to_array (dllist_t* list)
{
	int size = dl_length (list);
	void** array = malloc (sizeof (void *) * size);
	assert (array != NULL);
	
	dlnode_t* node = list->head;
	int i;
	for (i = 0; i < size; i += 1) {
		array[i] = node->data;
		node = node->next;
	}
	
	return array;
}