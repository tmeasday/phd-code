LIBDIR = ../../Library
CFLAGS += -I$(LIBDIR) -L$(LIBDIR)


all: edge_rank_algorithm.o tri-count_ranker.o degree_diff_ranker.o tri-delta_flipper.o

STDINCS = edge_rank_algorithm.h $(LIBDIR)/digraph.h $(LIBDIR)/order.h  $(LIBDIR)/edge.h

edge_rank_algorithm.o: $(STDINCS)
tri-count_ranker.o: tri-count_ranker.h $(STDINCS) $(LIBDIR)/triangles.h
tri-delta_flipper.o: tri-delta_flipper.h $(STDINCS) $(LIBDIR)/triangles.h
degree_diff_ranker.o: degree_diff_ranker.h $(STDINCS) $(LIBDIR)/degree_diff.h $(LIBDIR)/edge_list_iterator.h