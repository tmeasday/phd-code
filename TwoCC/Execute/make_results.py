#!/usr/bin/env python
# encoding: utf-8
"""  
make_results.py
TwoCC
  
Created by Tom Coleman on 2008-03-13.
Copyright 2008 Tom Coleman. All rights reserved.
"""

import sys
import os


def main():
	costs = []
	
	nprob = 0
	for line in sys.stdin:
		nprob += 1
		
		row = []
		for field in line.strip().split (","):
			if field.strip () == '': 
				continue
			row.append (int (field))
		costs.append (row)
	
	nalg = len (costs[0])
	
	wins = [0] * nalg
	sums = [0] * nalg
	for i in range (nprob):
		min_j = -1
		min_c = -1
		for j, c in enumerate (costs[i]):
			sums[j] += c
			if min_c == -1 or c < min_c:
				min_j = j
				min_c = c
				no_winner = False
			elif c == min_c:
				no_winner = True
		
		if not no_winner:
			wins[min_j] += 1
		
	print "avg:",
	print ", ".join ([str (float (s) / nprob) for s in sums])
	print "wins:",
	print ", ".join ([str (w) for w in wins])
	
if __name__ == '__main__':
	main()

