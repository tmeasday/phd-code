#include "rand.h"

// requires n to be defined
#define edge_no(i, j) (i*(n-1) + (j > i ? j-1 : j))
#define from_node(no) (no / (n-1))
// is this inefficient?
#define to_node(no)  ((no % (n-1)) + \
                      ((no / (n-1)) > (no % (n-1)) ? 0 : 1))

order gww (digraph, int);
int choose_edge (int[], int);

int
main (int argc, char* argv[])
{
  assert (argc == 2);
  int run;
  int runs = atoi (argv[1]);

  for (run = 0; run < runs; run++) {
    digraph dg = read_digraph (stdin);

    // we are guaranteed that edges is a complete transitive tournament
    order o = gww (dg, 1);

    print_order (stdout, o);
  }
  return 0;
}

order
gww (digraph dg, int B)
{
  int i, j;
  
  particle* ps[B];
  int n_alive = B;
  for (i = 0; i < B; i++) {
    ps[i] = new_particle (dg); 
  }

  while (1) {
    // descend
    for (i = 0; i < n_alive; i++) {
      int edge_chosen = choose_edge (ps[i]->pot_edges, ps[i]->n_pot_edges);
      set_particle_edge (ps[i], edge_chosen);

      // move dead particles to the end of the queue
      if (ps[i]->n_pot_edges == 0) {
        pointer_swap ((void**) &ps[i], (void**)&ps[n_alive-1]);
        n_alive--; i--;
      }
    }

    // redistribute
    if (n_alive == 0) {
      break;
    } else if (n_alive != B) {
      int frac_alive = B / n_alive;
      
      // try to distribute the copies evenly??
      for (i = 0; i < frac_alive - 1; i++) {
        for (j = 0; j < n_alive; j++) {
          int index = (i+1)*n_alive + j;
          free_particle (ps[index]);
          ps[index] = particle_copy (ps[j]);
        }
      }

      // now fill in the gaps
      for (i = 0; i < B % n_alive; i++) {
        int index = frac_alive * n_alive + i;
        free_particle (ps[index]);
        ps[index] = particle_copy (ps[get_rand(n_alive)]);
      }
    }
    
    n_alive = B;
  }

  return particle_to_order (ps[0]);
}

int
choose_edge (int pot_edges[], int n_pot_edges)
{
  int i;
  
  // choose one of the edges
  int edge_chosen = 0;
  int edge_no = get_rand (n_pot_edges);
  for (i = 0; i < edge_no; i++) {
    // skip past the ith potential edge
    while (!pot_edges[edge_chosen++]);
  }
  // skip to the get_rand'th potential edge
  while (!pot_edges[edge_chosen]) edge_chosen++;

  return edge_chosen;
}
