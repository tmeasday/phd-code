LIBDIR= ../../Library

CFLAGS += -I$(LIBDIR) -L$(LIBDIR)

all: planted.exe is_connected.exe connected_components.exe

LIBOBJS= $(LIBDIR)/rand.o $(LIBDIR)/labelling.o \
				 $(LIBDIR)/util.o $(LIBDIR)/array.o $(LIBDIR)/parser.o \
				 $(LIBDIR)/sigraph.o $(LIBDIR)/tree.o
	
%.exe: %.o $(LIBOBJS)
	$(CC) $(CFLAGS) -o $@ $^
	
planted.o: $(LIBDIR)/rand.h $(LIBDIR)/sigraph.h $(LIBDIR)/util.h
is_connected.o: $(LIBDIR)/rand.h $(LIBDIR)/sigraph.h $(LIBDIR)/util.h $(LIBDIR)/tree.h
connected_components.o: $(LIBDIR)/sigraph.h $(LIBDIR)/util.h $(LIBDIR)/tree.h
