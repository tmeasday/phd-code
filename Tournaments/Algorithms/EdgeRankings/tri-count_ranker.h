#ifndef _TRICOUNT_RANKER_H_
#define _TRICOUNT_RANKER_H_

void tricount_prepare ();
void tricount_init (digraph, digraph);
edge get_tricount_first ();
edge get_tricount_next ();
void tricount_update (edge);

static edge_ranker tr = {&tricount_prepare, &tricount_init, 
	&get_tricount_first, &get_tricount_next, &tricount_update};
edge_ranker* tricount_ranker = &tr;

#endif /* _TRICOUNT_RANKER_H_ */
