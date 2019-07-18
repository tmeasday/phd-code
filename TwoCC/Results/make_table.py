#!/usr/bin/env python
# encoding: utf-8
"""
make_table.py

Created by Tom Coleman on 2008-04-04.
Copyright (c) 2008 . All rights reserved.
"""

import sys
import os

file1 = 'Results/Epidermal_2/epi_2_summary.csv'
file2 = 'Results/Sparse_Expt_2/sparse_2_summary.csv'
file3 = 'Results/Complete_Expt_2/dense_2_summary.csv'

alg_table = \
	{'DasGupta': '\\sdp',
	 'Spectral': '\\spectral',
	 'PAST': '\\past',
	 'PASTA-flip': '\\pastaf',
	 'PASTA-toss': '\\pastat',
	 'LocalSearch': '\\local'}

compare = 'LocalSearch'

def main():
	fps = []
	
	fps.append (open (file1, 'r'))
	fps.append (open (file2, 'r'))
	fps.append (open (file3, 'r'))
	
	for i in [0,1,2]:
		fps[i].readline ()
	
	algs = []
	costs = []
	times = []

	cont = True
	while cont:
		for i in [0,1,2]:
			line = [x.strip () for x in fps[i].readline ().split (',')]
			if line[0] == '':
				cont = False
				break 
			
			if i == 0:
				if (line[0] == compare):
					compare_idx = len (costs)
				algs.append (line[0])
			costs.append (float (line[1]))
			times.append (float (line[2]))
	
	# printing
	min_costs = costs[compare_idx:compare_idx + 3]

	idx = 0
	for alg in algs:
		if alg == '':
			break

		if alg_table.has_key (alg):
			alg = alg_table[alg]

		print "%s" % alg,
		
		for i in [0,1,2]:
			
			cost = ((costs[idx] - min_costs[i]) / min_costs[i]) * 100
			print " & & %.3f & %.2f" % (cost, times[idx]),
			idx+=1
		
		print '\\\\'	
		
		# print  & %s & %s & %s \\\\" % \
		# 		(alg, costs[idx], costs[idx+1], costs[idx+2])
		# 
		# print " & %s & %s & %s \\\\" % \
		# 		(times[idx], times[idx+1], times[idx+2])
				

if __name__ == '__main__':
	main()

