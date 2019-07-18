export CC= gcc
export CFLAGS= -Wall -gdwarf-2 -pg #-DDEBUG -O3


all: Library Execute Datasets Algorithms

.PHONY: Library Execute Datasets Algorithms

Library:
	$(MAKE) -C Library

Algorithms:
	$(MAKE) -C Algorithms

Execute:
	$(MAKE) -C Execute

Datasets:
	$(MAKE) -C Datasets
