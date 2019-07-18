LIBDIR= ../Library
CFLAGS += -I$(LIBDIR) -L$(LIBDIR)

all: calc_upsets.exe unsample.exe randomize.exe

LIBOBJS= $(LIBDIR)/order.o $(LIBDIR)/digraph.o $(LIBDIR)/parser.o $(LIBDIR)/upsets.o $(LIBDIR)/rand.o $(LIBDIR)/util.o $(LIBDIR)/array.o

calc_upsets.exe: calc_upsets.o $(LIBOBJS)
	$(CC) $(CFLAGS) -o calc_upsets.exe $^
	
unsample.exe: unsample.o $(LIBOBJS)
	$(CC) $(CFLAGS) -o unsample.exe $^
	
randomize.exe: randomize.o $(LIBOBJS)
	$(CC) $(CFLAGS) -o randomize.exe $^

calc_upsets.o: $(LIBDIR)/order.h $(LIBDIR)/digraph.h $(LIBDIR)/upsets.h 
sample.o: $(LIBDIR)/order.h $(LIBDIR)/digraph.h 
randomize.o: $(LIBDIR)/array.h $(LIBDIR)/digraph.h 
