LIBDIR= ../Library

.PHONY: Synthetic
Synthetic:
	$(MAKE) -C Synthetic

CFLAGS += -I$(LIBDIR) -L$(LIBDIR)

all: analyze_tourns.exe round_tournament.exe complete_digraph.exe triangles.exe Synthetic

LIBOBJS= $(LIBDIR)/order.o $(LIBDIR)/digraph.o $(LIBDIR)/parser.o $(LIBDIR)/upsets.o $(LIBDIR)/rand.o $(LIBDIR)/util.o $(LIBDIR)/array.o $(LIBDIR)/edge_weights.o

%.exe: %.o $(LIBOBJS)
	$(CC) $(CFLAGS) -o $@ $^
	
analyze_tourns.o: $(LIBDIR)/digraph.h $(LIBDIR)/util.h
round_tournament.o: $(LIBDIR)/digraph.h $(LIBDIR)/rand.h
complete_digraph.o: $(LIBDIR)/digraph.h $(LIBDIR)/rand.h
triangles.o: $(LIBDIR)/digraph.h $(LIBDIR)/util.h $(LIBDIR)/edge_weights.h
