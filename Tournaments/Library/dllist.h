#ifndef _DLLIST_H_
#define _DLLIST_H_

typedef struct dlnode_ts dlnode_t;

struct dlnode_ts {
	void* data;
	dlnode_t* next;
	dlnode_t* prev;
};

typedef struct {
	dlnode_t* head;
	dlnode_t* foot;
} dllist_t;

dllist_t* new_dllist ();
dlnode_t* new_dlnode (void* data);

dlnode_t* dl_insert_at_head (dllist_t* list, void* data);
dlnode_t* dl_insert_at_foot (dllist_t* list, void* data);
dlnode_t* dl_insert_before (dllist_t* list, dlnode_t* node, void* data);
void reinsert_node_before (dllist_t* list, dlnode_t* node, dlnode_t* ref);


void delete_from_dllist (dllist_t* list, dlnode_t* node);

int dl_length (dllist_t* list);
void** dl_to_array (dllist_t* list);

#endif /* _LIST_H_ */
