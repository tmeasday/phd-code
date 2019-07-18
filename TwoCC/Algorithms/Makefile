#include Makefile.local

LIBDIR = ../Library

CFLAGS += -I$(LIBDIR) -L$(LIBDIR) -I/opt/local/include -lm #-DDEBUG

all: past.exe pasta_complete.exe ptas_complete.exe local_search.exe pasta_general.exe sample_and_insert.exe pasta_swap.exe
	
LIBOBJS= $(LIBDIR)/labelling.o $(LIBDIR)/sigraph.o $(LIBDIR)/parser.o $(LIBDIR)/rand.o $(LIBDIR)/util.o $(LIBDIR)/array.o $(LIBDIR)/cost.o $(LIBDIR)/tree.o $(LIBDIR)/edge.o $(LIBDIR)/local_search.o
%.exe: %.o $(LIBOBJS)
	$(CC) $(CFLAGS) -o $@ $^
		
STDINCS= $(LIBDIR)/labelling.h $(LIBDIR)/sigraph.h $(LIBDIR)/rand.h $(LIBDIR)/util.h 

past.o: $(STDINCS) $(LIBDIR)/cost.h $(LIBDIR)/tree.h $(LIBDIR)/local_search.h
ptas_complete.o: $(STDINCS) $(LIBDIR)/cost.h $(LIBDIR)/array.h
sample_and_insert.o: $(STDINCS) $(LIBDIR)/cost.h $(LIBDIR)/array.h
pasta_complete.o: $(STDINCS) $(LIBDIR)/cost.h $(LIBDIR)/array.h $(LIBDIR)/tree.h
local_search.o: $(STDINCS) $(LIBDIR)/cost.h $(LIBDIR)/array.h $(LIBDIR)/tree.h $(LIBDIR)/local_search.h
pasta_general.o: $(STDINCS) $(LIBDIR)/tree.h $(LIBDIR)/array.h $(LIBDIR)/cost.h
pasta_general.o: $(STDINCS) $(LIBDIR)/tree.h $(LIBDIR)/array.h $(LIBDIR)/cost.h $(LIBDIR)/local_search.h
