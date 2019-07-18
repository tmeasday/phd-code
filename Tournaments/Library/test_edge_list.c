#include <stdio.h>
#include <stdlib.h>
#include "edge_list.h"

int main (int argc, char* argv[])
{
	edge_list* list = new_edge_list ();
	
	float counts[] = {4.0, 2.0, 1.0, 3.0, 2.0, 1.0};
	dlnode_t* edges[6];
	
	int countn = 0, i, j;
	for (i = 0; i < 4; i += 1) {
		for (j = i+1; j < 4; j += 1) {
			edges[countn] = insert_into_edge_list (list, (edge) {i, j}, counts[countn]);
			countn += 1;
			
			print_edge_list (stdout, list);
		}
	}

	edge e =  get_first_edge (list);
	printf ("first edge: %d -> %d\n\n", e.from, e.to);
	
	edges[0] = change_edge_key (list, edges[0], 0.9);
	print_edge_list (stdout, list);
	
	edges[2] = change_edge_key (list, edges[2], 0.7);
	print_edge_list (stdout, list);
	
	e =  get_first_edge (list);
	printf ("first edge: %d -> %d\n\n", e.from, e.to);
	
	return 0;
}
