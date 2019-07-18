export CC= gcc
export CFLAGS= -Wall -O3 #-gdwarf-2 -pg 


all: Library Algorithms Execute Data

.PHONY: Library Algorithms Execute Data

Library:
	$(MAKE) -C Library

Algorithms:
	$(MAKE) -C Algorithms

Execute:
	$(MAKE) -C Execute

Data:
	$(MAKE) -C Data
