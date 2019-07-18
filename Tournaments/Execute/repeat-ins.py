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
reps_tourn = DD + "reps_tourn"
reps_input = DD + "reps_input"
reps = DD + "reps"
reps_ins = DD + "reps_ins"
reps_ups = DD + "reps_ups"
reps_ins_ups = DD + "reps_ins_ups"

insert   = './Algorithms/insertionsort.exe'
calc_ups = './Execute/calc_upsets.exe'

def const(x):
	return lambda y: x
	
def sqrt (n):
	return int (math.floor (math.sqrt (n)))
	
def id (x):
	return x


def main():	
	alg = sys.argv[1]
	rep = sys.argv[2]

	r = re.compile (r'\[[\d, \.]+\]')

	if rep == 'n':
		rep = id
	elif rep == 'sqrtn':
		rep = sqrt
	else:
		rep = const (int (rep))
	
	ti = 0
	for tourn in sys.stdin:
		ti+=1
		
		tourn = tourn.strip ()
		
		m = r.search (tourn)
		n = len (eval (m.group ())) + 1

		times = rep (n)
		f = open (reps_input, 'w')
		for i in range (times):
			print >> f, tourn
		f.close ()
		
		# file with just one tourn in it
		f = open (reps_tourn, 'w')
		print >> f, tourn
		f.close
			
		err = os.system ('cat %s | %s > %s' % (reps_input, alg, reps))
		if (err != 0):
			print "alg failed"
			sys.exit (1)
			
		err = os.system ('paste -d "\n" %s %s | %s -i > %s'
								% (reps_input, reps, insert, reps_ins))
		if (err != 0):
			print "insertion_sort failed"
			sys.exit (1)
			
				
		err = os.system ('head -n 1 %s | cat - %s | %s 1 %d > %s'
							 % (reps_input, reps, calc_ups, times, reps_ups))
		if (err != 0):
			print "calc_ups failed"
			sys.exit (1)

		err = os.system ('head -n 1 %s | cat - %s | %s 1 %d > %s'
							 % (reps_input, reps_ins, calc_ups, times, reps_ins_ups))
		if (err != 0):
			print "calc_ups (ins) failed"
			sys.exit (1)


		min = n * n
		min_i = 0
		
		f = open (reps_ins_ups)
		line = f.readline ().strip ("\n,")
		i = 1
		for upset in line.split (","):
			upset = float (upset)
			if (upset < min):
				min = upset
				min_i = i
			i += 1
		
		# ok we know the minimum
		os.system ('head -n %d %s | tail -n 1' % (min_i, reps_ins))
					
		# also, we'd like to save the upsets data... this is a bit of a hack, 
		# but hey, there are hacks all over the place aren't there?
		save_dir = DD + os.path.basename(alg)[0:-4] # split off the .exe
		os.system ('mkdir -p %s/' % save_dir)
		os.system ('cp %s %s/ups_%d' % (reps_ups, save_dir, ti))
		os.system ('cp %s %s/ins_ups_%d' % (reps_ins_ups, save_dir, ti))
if __name__ == '__main__':
	main()

