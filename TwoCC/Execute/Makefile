LIBDIR= ../Library
CFLAGS += -I$(LIBDIR) -L$(LIBDIR)

all: costs.exe #unsample.exe randomize.exe

LIBOBJS= $(LIBDIR)/labelling.o $(LIBDIR)/sigraph.o $(LIBDIR)/parser.o $(LIBDIR)/cost.o $(LIBDIR)/rand.o $(LIBDIR)/util.o $(LIBDIR)/array.o

costs.exe: costs.o $(LIBOBJS)
	$(CC) $(CFLAGS) -o costs.exe $^
	
unsample.exe: unsample.o $(LIBOBJS)
	$(CC) $(CFLAGS) -o unsample.exe $^
	
randomize.exe: randomize.o $(LIBOBJS)
	$(CC) $(CFLAGS) -o randomize.exe $^

costs.o: $(LIBDIR)/labelling.h $(LIBDIR)/sigraph.h $(LIBDIR)/cost.h 
sample.o: $(LIBDIR)/order.h $(LIBDIR)/digraph.h 
randomize.o: $(LIBDIR)/array.h $(LIBDIR)/digraph.h 
