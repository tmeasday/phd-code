#!/usr/bin/env python
# encoding: utf-8
"""
generate_tourns.py

Created by Tom Coleman on 2007-09-25.
Copyright (c) 2007 The University of Melbourne. All rights reserved.
"""

import sys
import os
import random

def main():
	num = int (sys.argv[1])
	size = int (sys.argv[2])
	
	LEN = 25203858
	
	for i in range (num):
		print "tournament", i

		sample = random.sample (xrange(LEN), size)
		sample.sort ()

		try:
			os.makedirs ('short/%d/' % i)
		except:
			pass
		try:
			os.makedirs ('sorted/%d/' % i)
		except:
			pass
		
		for n in ["2.1", "2.2", "3.1", "3.2", "3.3", "4.1", "4.2", "4.3", "4.4"]:
			print "sampling " + n
			data = 'data/peerRank' + n
			short = 'short/%d/peerRank%s' % (i, n)
			sort  = 'sorted/%d/peerRank%s' % (i, n)
			
		 	inp = open (data)
			out = open (short, "w")
			
			asample = sample

			lineno = -1
			for line in inp:
				if asample == []:
					break
				if lineno == asample[0]:
					print >> out, line,
					asample = asample[1:]
				lineno += 1
			inp.close ()
			out.close ()
			
			os.system ('cat %s | ./scored_to_ranked.py > %s' % (short, sort))
			

if __name__ == '__main__':
	main()
