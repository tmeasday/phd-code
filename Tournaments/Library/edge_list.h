#ifndef _EDGE_LIST_H_
#define _EDGE_LIST_H_

#include <stdio.h>
#include "edge.h"
#include "dllist.h"

typedef dllist_t edge_list;

typedef struct {
	dllist_t* edges;
	int key;
} edge_bucket;

typedef struct {
	edge e;
	float key;
	dlnode_t* bucket; // bucket->key == floor (key)
} edge_list_data;

edge_list* new_edge_list ();
edge_bucket* new_edge_bucket (int key);
edge_list_data* new_edge_list_data (edge e, float key);
dlnode_t* insert_into_edge_list (edge_list* list, edge e, float key);
edge get_first_edge (edge_list* list);

// NOTE: this is relatively expensive. multiple increments != not good
dlnode_t* inc_edge_key (edge_list* list, dlnode_t* node, float increment);
dlnode_t* change_edge_key (edge_list* list, dlnode_t* node, float new_key);
dlnode_t* insert_into_bucket (edge_bucket* bucket, edge_list_data* data);
void reverse_edge_list_edge (dlnode_t* node);

void print_edge_list (FILE* out, edge_list* list);
void print_edge_list_short (FILE* out, edge_list* list, int n);

edge* edge_list_to_array (edge_list* list);

#endif /* _EDGE_LIST_H_ */
