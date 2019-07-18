#!/usr/bin/env python
# encoding: utf-8
"""
weighted.py

Takes in a random order and returns a random weighted tournament

Created by Tom Coleman on 2007-07-17.
"""

import sys
import os
import math
import random

def reverse_perm (alist):
	new_list = {}
	for i in range (len (alist)):
		for j in range (len (alist)):
			if alist[j] == i:
				new_list[i] = j
				
	return new_list

def main():
	p = float (sys.argv[1])
	assert (0 <= p)
	assert (p <= 1)
	
	# We're going to use the following function:
	# p(x) = (n+1) x^n
	# giving p = (n+1) / (n+2)
	# so n = (2p - 1) / (1-p)
	n = (2 * p - 1) / (1.0 - p)
	
	
	# p(x) = (n+1) x^n
	# so P(x) = x^(n+1)
	# so we can generate p with P^{-1} (U) = U^{1/(n+1)}
	exponent = 1 / (n+1)
	
	random.seed ()

	for line in sys.stdin:
		order = eval (line)

		rev_order = reverse_perm (order)

		tourn = []
		for i in range (len (order)):
			line = []
			for j in range (i+1, len (order)):
				weight = random.random () ** exponent
				
				if rev_order[i] > rev_order[j]:
					weight = 1 - weight
				
				line.append (weight)
			tourn.append (line)
		print (tourn)
					
if __name__ == '__main__':
	main()

