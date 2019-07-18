#!/usr/bin/env python
# encoding: utf-8
"""
edge_crossings.py

Created by Tom Coleman on 2007-08-29.
Copyright (c) 2007 The University of Melbourne. All rights reserved.
"""

import sys
import os
import random

def main():
	n_0 = int (sys.argv[1])
	m = int (sys.argv[2])
	p = float (sys.argv[3])

	n_1 = n_0 # could do something different here...
	
	random.seed ()
	
	for run in range (m):
		# construct the bipartite graph to be 'crossed'
		graph = []
		for i in range (n_0):
			row = []
			for j in range (n_1):
				row.append (random.random () < p)
			graph.append (row)
	
		#print graph

		# now make the tournament out of it...
		top_weight = 0
		tourn = []
		for i in range (n_0):
			row = []
			for j in range (i+1, n_0):
				weight = 0
				for k in range (n_1):
					if graph[i][k]:
						weight -= sum ([1 for b in graph[j][:k] if b])
						weight += sum ([1 for b in graph[j][k+1:] if b])
				row.append (weight)
				top_weight = max (abs(weight), top_weight)
			
			tourn.append (row)
	
	#	print tourn

		# now scale the tourn to look like one of ours
		if top_weight != 0:
			a = top_weight # shorter line
			tourn = map (lambda l: map (lambda w: (w + a) / (2.0*a), l), tourn)

		print tourn


if __name__ == '__main__':
	main()

