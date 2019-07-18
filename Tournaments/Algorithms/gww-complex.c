#include <assert.h>
#include "order.h"
#include "digraph.h"
#include "util.h"

typedef struct {
  int size;
  int* edges;
  int* pot_edges;
  int n_pot_edges;
  int* nodes;
  int n_nodes;
} particle;

particle* new_particle (digraph);
void free_particle (particle*);
int* free_particle_save_edges (particle*);
order particle_to_order (particle*);
void set_particle_edge (particle*, int);
particle* particle_copy (particle*);

void add_node (int, particle*);

#include "gww.c"

particle* 
new_particle (digraph dg)
{
  int i, j;
  int n = dg.size, m = (n*(n-1));

  particle* pp = malloc (sizeof(particle));
  pp->size = n;

  // initialize stuff
  pp->edges = malloc (sizeof(int) * m);
  assert(pp->edges != NULL);
  for (i = 0; i < m; i++) {
    pp->edges[i] = 0;
  }

  pp->nodes = malloc (sizeof(int) * n);
  assert (pp->nodes != NULL);
  pp->n_nodes = 0;

  // this is the bit where we can be smart about it
  pp->pot_edges = malloc (sizeof(int*) * m);
  assert (pp->pot_edges != NULL);
  pp->n_pot_edges = 0;
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      if (i == j) continue;
    
      // the edge is actually an option
      if (dg_weight (dg, i, j) > 0) {
        pp->pot_edges[edge_no (i, j)] = 1;
        pp->n_pot_edges++;
      } else {
        pp->pot_edges[edge_no (i, j)] = 0;
      }
    }
  }

  return pp;
}



void 
free_particle (particle* pp)
{
  int* edges = free_particle_save_edges (pp);
  free (edges);
}

int* 
free_particle_save_edges (particle* pp)
{
  int *edges = pp->edges;

  free (pp->nodes);
  free (pp->pot_edges);
  free (pp);

  return edges;
}

order
particle_to_order (particle* pp)
{
  int i, j;

  order o = new_order (pp->size);
  for (i = 0; i < pp->n_nodes; i++) {
    o.nodes[i] = pp->nodes[i];
  }

  // If we haven't completely specified o (this is a really bad sign)
  // fill out the remainder of o arbitrarily
  j = pp->n_nodes;
  if (pp->n_nodes < pp->size) {
    for (i = 0; i < pp->size; i++) {
      if ( ! in_array (i, pp->nodes, pp->n_nodes) ) {
        o.nodes[j++] = i;
      }
    }
  }

  return o;
}

particle*
particle_copy (particle* old)
{
  int n = old->size, m = n*(n-1);
  
  particle* new = malloc (sizeof (particle));
  assert (new != NULL);

  new->size = old->size;
  new->edges = array_duplicate (old->edges, m);

  new->pot_edges = array_duplicate (old->pot_edges, m);
  new->n_pot_edges = old->n_pot_edges;

  new->nodes = array_duplicate (old->nodes, n);
  new->n_nodes = old->n_nodes;

  return new;
}


void set_particle_edge (particle* pp, int edge_chosen)
{
  int n = pp->size;
  
  // ok. We've chosen
  pp->edges[edge_chosen] = 1;

  // ok now update the potential edges
  pp->pot_edges[edge_chosen] = 0;
  pp->n_pot_edges--;

  int start = from_node (edge_chosen);
  int end   = to_node (edge_chosen);

  if (!in_array (start, pp->nodes, pp->n_nodes)) {
    add_node (start, pp);
  }

  if (!in_array (end, pp->nodes, pp->n_nodes)) {
    add_node (end, pp);
  }
}

void
add_node (int node, particle* pp)
{
  int i, n = pp->size;

  pp->nodes[pp->n_nodes++] = node;
  for (i = 0; i < pp->size; i++) {
    if (pp->pot_edges[edge_no (i, node)]) {
      if (!in_array (i, pp->nodes, pp->n_nodes)) {
        pp->pot_edges[edge_no (i, node)] = 0;
        pp->n_pot_edges--;
      }
    }
  }
}
