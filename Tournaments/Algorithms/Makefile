include Makefile.local

LIBDIR = ../Library

CFLAGS += -I$(LIBDIR) -L$(LIBDIR) -I/opt/local/include -lm

all: EdgeRankings order_by_indegree.exe order_by_indegree-random.exe quicksort.exe eades.exe eades_imp.exe gww-simple.exe gww-complex.exe insertionsort.exe mergesort.exe bubblesort.exe triangle_count.exe triangle_delta.exe triangle_both.exe triangle_both_old.exe moves.exe swaps.exe branch_and_bound.exe sim-annealing.exe random-indegree.exe tricount-indegree.exe degreediff-delta.exe triangle_both_faster.exe triangle_both_el.exe degree_difference.exe degree_difference_approx.exe degree_difference_approx_v2.exe quicksort_triangle.exe random.exe moves-insert.exe sample-and-insert.exe #moves-gen.exe lp-pivot.exe lp-transitive.exe 

.PHONY: EdgeRankings
EdgeRankings:
	$(MAKE) -C EdgeRankings


EROBJS= EdgeRankings/edge_rank_algorithm.o EdgeRankings/tri-count_ranker.o EdgeRankings/tri-delta_flipper.o EdgeRankings/degree_diff_ranker.o

LIBOBJS= $(LIBDIR)/order.o $(LIBDIR)/digraph.o $(LIBDIR)/parser.o $(LIBDIR)/rand.o $(LIBDIR)/util.o $(LIBDIR)/array.o $(LIBDIR)/edge_weights.o $(LIBDIR)/upsets.o $(LIBDIR)/edge.o $(LIBDIR)/degrees.o $(LIBDIR)/triangles.o $(LIBDIR)/list.o $(EROBJS) $(LIBDIR)/edge_list.o $(LIBDIR)/dllist.o $(LIBDIR)/degree_diff.o $(LIBDIR)/edge_list_iterator.o $(LIBDIR)/array.o $(LIBDIR)/insertionsort.o

%.exe: %.o $(LIBOBJS)
	$(CC) $(CFLAGS) -o $@ $^

lp-pivot.exe: lp-pivot.o $(LIBOBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LP_FLAGS)

lp-transitive.exe: lp-transitive.o $(LIBOBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LP_FLAGS)
		
STDINCS= $(LIBDIR)/order.h $(LIBDIR)/digraph.h $(LIBDIR)/rand.h 

order_by_indegree.o: $(STDINCS) $(LIBDIR)/util.h
order_by_indegree-random.o: $(STDINCS) $(LIBDIR)/util.h
quicksort.o: $(STDINCS) $(LIBDIR)/util.h
insertionsort.o: $(STDINCS) $(LIBDIR)/util.h $(LIBDIR)/upsets.h $(LIBDIR)/insertionsort.o
mergesort.o: $(STDINCS) $(LIBDIR)/util.h
bubblesort.o: $(STDINCS) $(LIBDIR)/util.h
eades.o: $(STDINCS)
eades_imp.o: $(STDINCS)
gww-simple.o: $(STDINCS) $(LIBDIR)/util.h gww.c
gww-complex.o: $(STDINCS) $(LIBDIR)/util.h gww.c
triangle_count.o: $(STDINCS)
triangle_delta.o: $(STDINCS)
triangle_both.o: $(STDINCS) $(LIBDIR)/triangles.h
triangle_both_old.o: $(STDINCS) $(LIBDIR)/edge_weights.h
triangle_both_faster.o: $(STDINCS) $(LIBDIR)/triangles.h $(LIBDIR)/list.h
triangle_both_el.o: $(STDINCS) $(LIBDIR)/triangles.h $(LIBDIR)/edge_list.h
moves.o: $(STDINCS)
moves-insert.o: $(STDINCS)
moves-gen.o: $(STDINCS)
sim-annealing.o: $(STDINCS) $(LIBDIR)/util.h
swaps.o: $(STDINCS)
branch_and_bound.o: $(STDINCS)
lp-pivot.o: $(STDINCS)
lp-transitive.o: $(STDINCS)
random_indegree.o: $(STDINCS) EdgeRankings/edge_rank_algorithm.h
tricount_indegree.o: $(STDINCS) EdgeRankings/edge_rank_algorithm.h
degreediff-delta.o: $(STDINCS) EdgeRankings/edge_rank_algorithm.h
degree_difference.o: $(STDINCS) $(LIBDIR)/degree_diff.h
degree_difference_approx.o: $(STDINCS) $(LIBDIR)/degrees.h $(LIBDIR)/array.h
degree_difference_approx_v2.o: $(STDINCS) $(LIBDIR)/degrees.h $(LIBDIR)/array.h
quicksort_triangle.o: $(STDINCS)
random.o: $(STDINCS)
sample-and-insert.o: $(STDINCS)
