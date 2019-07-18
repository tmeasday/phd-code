CFLAGS += -lm
OBJECTS= digraph.o order.o upsets.o parser.o rand.o util.o array.o degrees.o edge.o triangles.o list.o dllist.o edge_list.o util.o degree_diff.o edge_list_iterator.o

all: $(OBJECTS) test_lib.exe test_edge_list.exe

test_lib.exe: test_lib.o order.o digraph.o parser.o upsets.o rand.o array.o util.o
	$(CC) $(CFLAGS) -o test_lib.exe $^
	
test_edge_list.exe: test_edge_list.o edge_list.o dllist.o edge.o util.o rand.o
	$(CC) $(CFLAGS) -o test_edge_list.exe $^
	

test_lib.exe: digraph.h order.h upsets.h
test_edge_list.exe: dllist.h edge_list.h


digraph.o: digraph.h parser.h order.h array.h
order.o: order.h parser.h rand.h array.h
upsets.o: upsets.h order.h digraph.h
parser.o: parser.h
rand.o: rand.h util.h
util.o: util.h
array.o: array.h
edge.o: edge.h
degrees.o: degrees.h array.h
triangles.o: triangles.h
list.o: list.h rand.h
dllist.o: dllist.h
edge_list.o: edge_list.h dllist.h edge.h
edge_list_iterator.o: edge_list.h edge.h
degree_diff.o: edge_list.h dllist.h edge.h digraph.h degrees.h
