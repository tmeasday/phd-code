#include <assert.h>
#include "order.h"
#include "digraph.h"
#include "util.h"

typedef struct {
  int size;
  int* edges;
  int* pot_edges;
  int n_pot_edges;
  int** reach;
} particle;

particle* new_particle (digraph);
void free_particle (particle*);
int* free_particle_save_edges (particle*);
order particle_to_order (particle*);
void set_particle_edge (particle*, int);
particle* particle_copy (particle*);

#include "gww.c"

void
set_particle_edge (particle* pp, int edge_chosen)
{
  int i, j, n = pp->size;

  // ok. We've chosen
  pp->edges[edge_chosen] = 1;

  // ok now update the potential edges
  pp->pot_edges[edge_chosen] = 0;
  pp->n_pot_edges--;

  // now update the reach values
  // first get the nodes involved
  for (i = 0; i < n; i++) {
    if (pp->reach[i][from_node(edge_chosen)]) {
      for (j = 0; j < n; j++) {
        if (pp->reach[to_node(edge_chosen)][j]) {
          // loop check
          assert (i != j);
          pp->reach[i][j] = 1;

          // backwards of this edge is no longer good
          if (pp->pot_edges[edge_no(j, i)]) {
            pp->pot_edges[edge_no(j, i)] = 0;
            pp->n_pot_edges--;
          }
        }
      }
    }
  }

}

particle*
new_particle (digraph dg)
{
  int n = dg.size, m = (n*(n-1));
  int i, j;

  particle* pp = malloc (sizeof (particle));
  pp->size = n;
  assert (pp != NULL);

  // initialize stuff
  pp->edges = malloc (sizeof(int) * m);
  assert (pp->edges != NULL);
  for (i = 0; i < m; i++) {
    pp->edges[i] = 0;
  }

  pp->reach = malloc (sizeof(int*) * n);
  assert (pp->reach != NULL);
  for (i = 0; i < n; i++) {
    pp->reach[i] = malloc (sizeof(int) * m);
    assert (pp->reach[i] != NULL);

    for (j = 0; j < n; j++) {
      if (i == j) {
        pp->reach[i][j] = 1;
      } else {
        pp->reach[i][j] = 0;
      }
    }
  }

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

particle*
particle_copy (particle* old)
{
  int i;
  int n = old->size, m = n*(n-1);
  
  particle* new = malloc (sizeof (particle));
  assert (new != NULL);

  new->size = old->size;
  new->edges = array_duplicate (old->edges, m);
  new->pot_edges = array_duplicate (old->pot_edges, m);
  new->n_pot_edges = old->n_pot_edges;

  new->reach = malloc (sizeof(int*) * n);
  assert (new->reach != NULL);
  for (i = 0; i < n; i++) {
    new->reach[i] = array_duplicate (old->reach[i], n);
  }

  return new;
}

order
particle_to_order (particle *pp)
{

    int i, j, ni, nj;
    int nodes_rem[pp->size];
    int n_nodes_rem = pp->size;
    int n = pp->size;

    order o = new_order (n);
    int* edges = free_particle_save_edges (pp);
    
    first_n (nodes_rem, n);

    while (n_nodes_rem > 0) {
    
      // find the nodes with no edges pointing into it
      // (there WILL be such a node, as the edge are transitive)
      for (i = 0; i < n; i++) {
        ni = nodes_rem[i];
        int good = 1;
        
        for (j = 0; j < n_nodes_rem; j++) {
          if (i == j) continue;

          nj = nodes_rem[j];
          if (edges[edge_no (nj, ni)]) {
            good = 0;
            break;
          }
        }

        if (good) {
          o.nodes [n - n_nodes_rem] = ni;
          n_nodes_rem--;
          int_swap (&nodes_rem[i], &nodes_rem[n_nodes_rem]);
          break;
        }
      }

      // should never get here
      assert (1 != 0);
    }

    return o;
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
  int i;
  int* edges = pp->edges;
  
  free (pp->pot_edges);
  for (i = 0; i < pp->size; i++) {
    free (pp->reach[i]);
  }
  free (pp->reach);
  free (pp);

  return edges;
}

