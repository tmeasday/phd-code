GHCFLAGS= --make -i../Types -i../Util -i../Local

all: order_by_indegree quicksort swaps moves hamiltonian

.PHONY: order_by_indegree calc_upsets quicksort swaps moves hamiltonian
order_by_indegree: 
	ghc $(GHCFLAGS) order_by_indegree.hs -o order_by_indegree 

quicksort: 
	ghc $(GHCFLAGS) quicksort.hs -o quicksort

swaps:
	ghc $(GHCFLAGS) swaps.hs -o swaps

moves:
	ghc $(GHCFLAGS) moves.hs -o moves

hamiltonian:
	ghc $(GHCFLAGS) hamiltonian.hs -o hamiltonian
