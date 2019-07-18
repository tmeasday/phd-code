#ifndef _LIST_H_
#define _LIST_H_

typedef struct node_ts node_t;

struct node_ts {
	void* data;
	float key;
	node_t* next;
};

typedef struct {
	node_t* head;
	node_t* foot;
} list_t;

list_t* new_list ();
node_t* new_node (void* data, float key);
void insert_at_head (list_t* list, void* data, float key);
void insert_at_foot (list_t* list, void* data, float key);

int length (list_t* list);
	
void sort_list (list_t* list);
list_t quicksort_nodes (node_t* head, int size);
void head_to_foot (list_t* list);
/*void node_to_foot (list_t* list, node* prev)*/


void** to_array (list_t* list);

#endif /* _LIST_H_ */
