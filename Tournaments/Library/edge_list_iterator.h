#ifndef _EDGE_LIST_ITERATOR_H_
#define _EDGE_LIST_ITERATOR_H_

#include "edge.h"
#include "edge_list.h"

typedef struct {
	edge_list* list;
	dlnode_t* curr;
} el_iterator;

el_iterator* new_el_iterator (edge_list* list);
edge get_it_first_edge (el_iterator* it);
edge get_it_next_edge (el_iterator* it);


#endif /* _EDGE_LIST_ITERATOR_H_ */
