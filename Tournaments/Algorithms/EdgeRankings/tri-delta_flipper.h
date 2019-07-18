#ifndef _TRIDELTA_FLIPPER_H_
#define _TRIDELTA_FLIPPER_H_

void tridelta_prepare (void);
void tridelta_initialize (digraph);
int tridelta_valid (edge);
int tridelta_complete (void);
void tridelta_update (edge);

static edge_flipper tdf = {&tridelta_prepare, &tridelta_initialize, 
	&tridelta_valid, &tridelta_complete, &tridelta_update};
	
extern edge_flipper* tridelta_flipper;

#endif /* _TRIDELTA_FLIPPER_H_ */
