LIBDIR= ../Library

all: save_for_bsg.exe Synthetic

.PHONY: Synthetic
Synthetic:
	$(MAKE) -C Synthetic

CFLAGS += -I$(LIBDIR) -L$(LIBDIR)
LIBOBJS= $(LIBDIR)/sigraph.o $(LIBDIR)/parser.o $(LIBDIR)/array.o $(LIBDIR)/util.o $(LIBDIR)/rand.o


%.exe: %.o $(LIBOBJS)
	$(CC) $(CFLAGS) -o $@ $^
	
save_for_bsg.o: $(LIBDIR)/sigraph.h

# 
# all: analyze_tourns.exe round_tournament.exe complete_digraph.exe triangles.exe Synthetic
# 
# 
# 	
# analyze_tourns.o: $(LIBDIR)/digraph.h $(LIBDIR)/util.h
# round_tournament.o: $(LIBDIR)/digraph.h $(LIBDIR)/rand.h
# complete_digraph.o: $(LIBDIR)/digraph.h $(LIBDIR)/rand.h
# triangles.o: $(LIBDIR)/digraph.h $(LIBDIR)/util.h $(LIBDIR)/edge_weights.h
