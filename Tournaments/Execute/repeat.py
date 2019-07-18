#!/usr/bin/env python
# encoding: utf-8
"""
repeat.py

Created by Tom Coleman on 2007-09-23.
Copyright (c) 2007 The University of Melbourne. All rights reserved.
"""

import sys
import os
import math
import re

DD = 'Temp/'
reps_input = DD + "reps_input"
reps = DD + "reps"
reps_ups = DD + "reps_ups"

calc_ups = './Execute/calc_upsets.exe'

def const(x):
	return lambda y: x
	
def sqrt (n):
	return int (math.floor (math.sqrt (n)))
	
def id (x):
	return x


def main():
	os.system ('rm Temp/insertionsort_starting_orders')
	
	alg = sys.argv[1]
	rep = sys.argv[2]

	r = re.compile (r'\[[\d, \.]+\]')

	if rep == 'n':
		rep = id
	elif rep == 'sqrtn':
		rep = sqrt
	else:
		rep = const (int (rep))
	
	for tourn in sys.stdin:
		tourn = tourn.strip ()
		
		m = r.search (tourn)
		n = len (eval (m.group ())) + 1

		times = rep (n)
		f = open (reps_input, 'w')
		for i in range (times):
			print >> f, tourn
		f.close ()
			
		err = os.system ('cat %s | %s > %s' % (reps_input, alg, reps))
		if (err != 0):
			print "alg failed"
			sys.exit (1)
				
		err = os.system ('head -n 1 %s | cat - %s | %s 1 %d > %s'
							 % (reps_input, reps, calc_ups, times, reps_ups))
		if (err != 0):
			print "calc_ups failed"
			sys.exit (1)

		min = n * n
		min_i = 0
		
		f = open (reps_ups)
		line = f.readline ().strip ("\n,")
		i = 1
		for upset in line.split (","):
			upset = float (upset)
			if (upset < min):
				min = upset
				min_i = i
			i += 1
		
		# ok we know the minimum
		os.system ('head -n %d %s | tail -n 1' % (min_i, reps))
		
		sor = 'Temp/insertionsort_starting_orders_reps'
		so  = 'Temp/insertionsort_starting_orders'
		os.system ('head -n %d %s | tail -n 1 >> %s' % (min_i, sor, so))
			
if __name__ == '__main__':
	main()

