include Makefile.local

LIBDIR = ../Library

CFLAGS += -I$(LIBDIR) -L$(LIBDIR) -I/opt/local/include -lm

all: order_by_indegree.exe quicksort.exe eades.exe eades_imp.exe gww-simple.exe gww-complex.exe insertionsort.exe mergesort.exe bubblesort.exe triangle_count.exe triangle_delta.exe triangle_both.exe moves.exe swaps.exe branch_and_bound.exe sim-annealing.exe lp-pivot.exe lp-transitive.exe 

LIBOBJS= $(LIBDIR)/order.o $(LIBDIR)/digraph.o $(LIBDIR)/parser.o $(LIBDIR)/rand.o $(LIBDIR)/util.o $(LIBDIR)/list.o $(LIBDIR)/edge_weights.o $(LIBDIR)/upsets.o

%.exe: %.o $(LIBOBJS)
	$(CC) $(CFLAGS) -o $@ $^

lp-pivot.exe: lp-pivot.o $(LIBOBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LP_FLAGS)

lp-transitive.exe: lp-transitive.o $(LIBOBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LP_FLAGS)
		
STDINCS= $(LIBDIR)/order.h $(LIBDIR)/digraph.h $(LIBDIR)/rand.h 

order_by_indegree.o: $(STDINCS) $(LIBDIR)/util.h
quicksort.o: $(STDINCS) $(LIBDIR)/util.h
insertionsort.o: $(STDINCS) $(LIBDIR)/util.h $(LIBDIR)/upsets.h
mergesort.o: $(STDINCS) $(LIBDIR)/util.h
bubblesort.o: $(STDINCS) $(LIBDIR)/util.h
eades.o: $(STDINCS)
eades_imp.o: $(STDINCS)
gww-simple.o: $(STDINCS) $(LIBDIR)/util.h gww.c
gww-complex.o: $(STDINCS) $(LIBDIR)/util.h gww.c
triangle_count.o: $(STDINCS)
triangle_delta.o: $(STDINCS)
triangle_both.o: $(STDINCS) $(LIBDIR)/edge_weights.h
lp-pivot.o: $(STDINCS)
lp-transitive.o: $(STDINCS)
moves.o: $(STDINCS)
sim-annealing.o: $(STDINCS) $(LIBDIR)/util.h
swaps.o: $(STDINCS)
branch_and_bound.o: $(STDINCS)
