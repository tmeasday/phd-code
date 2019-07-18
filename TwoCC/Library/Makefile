CFLAGS += -lm
OBJECTS= sigraph.o labelling.o cost.o parser.o rand.o util.o array.o tree.o

all: $(OBJECTS) test_lib.exe

test_lib.exe: test_lib.o labelling.o sigraph.o parser.o cost.o rand.o array.o util.o tree.o edge.o
	$(CC) $(CFLAGS) -o test_lib.exe $^
		

test_lib.exe: sigraph.h labelling.h cost.h tree.h


sigraph.o: sigraph.h parser.h labelling.h array.h
labelling.o: labelling.h parser.h rand.h array.h
cost.o: cost.h labelling.h sigraph.h
parser.o: parser.h
rand.o: rand.h util.h
util.o: util.h
array.o: array.h
tree.o: tree.h array.h sigraph.h edge.h
edge.o: edge.h