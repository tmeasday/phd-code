LIBDIR= ../../Library

CFLAGS += -I$(LIBDIR) -L$(LIBDIR)

all: biased.exe make_orders.exe

LIBOBJS= $(LIBDIR)/rand.o $(LIBDIR)/order.o $(LIBDIR)/util.o $(LIBDIR)/array.o $(LIBDIR)/parser.o

%.exe: %.o $(LIBOBJS)
	$(CC) $(CFLAGS) -o $@ $^
	
biased.exe: biased.o $(LIBDIR)/rand.o
	$(CC) $(CFLAGS) -o $@ $^
	
biased.o: $(LIBDIR)/rand.h
make_orders.o: $(LIBDIR)/order.h