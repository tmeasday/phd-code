#!/usr/bin/env python
# encoding: utf-8
"""
biased-length.py

Created by Tom Coleman on 2007-08-29.
Copyright (c) 2007 The University of Melbourne. All rights reserved.
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
	method = int (sys.argv[2])
	
	q = 1 - p
	assert (0 <= p)
	assert (p <= 1)
	
	random.seed ()
	
	for line in sys.stdin:
		order = eval (line)

		rev_order = reverse_perm (order)

		tourn = []
		for i in range (len (order)):
			line = []
			for j in range (i+1, len (order)):
				dist = abs (rev_order[j] - rev_order[i])
				
				if (method == 1):
					fq = q**(dist) 
				else:
					fq = q / (dist*dist)
				
				if rev_order[i] < rev_order[j]:
					prob = 1 - fq
				else:
					prob = fq
					
				if random.random () < prob:
					weight = 1
				else:
					weight = 0
					
				line.append (weight)
			tourn.append (line)
		print (tourn)




if __name__ == '__main__':
	main()

