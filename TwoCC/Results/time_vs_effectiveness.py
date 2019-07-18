#!/usr/bin/env python
# encoding: utf-8
"""
time_vs_effectiveness.py

Created by Tom Coleman on 2008-04-04.
Copyright (c) 2008 . All rights reserved.
"""

import sys
import os
import math

skip1 = ['PASTA-toss-rand', 'PASTA-flip-rand', 'PASTA-flip-cover',
	'PASTA-toss-cover', 'PASTA-flip-10', 'PASTA-flip-25',
	'PASTA-flip-50', 'PASTA-toss-10', 'PASTA-toss-25', 'PASTA-toss-50']
skip2 =  ['DasGupta', 'Spectral', 'PAST', 'PTAS-10', 'PTAS-15', 'LocalSearch']

skip = skip2
radius = 10000000000000

def main():
	TYPES = ['']
	
	algs = []
	costs = []
	times = []
	
	first = True
	for line in sys.stdin:
		if first:
			first = False
			continue
		
		(alg, cost, time) = [x.strip () for x in line.split (',')]
		algs.append (alg)
		costs.append (float (cost))
		times.append (float (time))
	
	print "set term postscript eps solid 'Times-Bold' 14"
	print "set output 'plot.eps'" 	
	print "set size 1.1,0.7"
	
	#print "set term png"
	#print "set output 'plot.png'"

	print "set xlabel 'Time (in seconds)'"
	print "set ylabel 'Average number of errors'"
	print "set logscale x"
	print "plot '-' notitle pt 9, '-' with labels offset 5,0.5 notitle"
		
	for j in [1,2]:
		for i in range (len (algs)):
			if algs[i] in skip:
				continue
		
			time = times[i]
			avg = costs[i]
					
			
			size = math.sqrt (time*time + avg*avg)
			if (size < radius):
				print time,
				print avg,
				print algs[i]
		print 'e'


		
if __name__ == '__main__':
	main()

