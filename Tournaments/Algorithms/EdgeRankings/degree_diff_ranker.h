#ifndef _DEGREE_DIFF_RANKER_H_
#define _DEGREE_DIFF_RANKER_H_

void degreediff_prepare ();
void degreediff_init (digraph _dg, digraph _orient);
edge get_degreediff_first ();
edge get_degreediff_next ();
void degreediff_update (edge);

static edge_ranker ddr = {&degreediff_prepare, &degreediff_init,
		&get_degreediff_first, &get_degreediff_next, &degreediff_update};
edge_ranker* degree_diff_ranker = &ddr;

#endif /* _DEGREE_DIFF_RANKER_H_ */
