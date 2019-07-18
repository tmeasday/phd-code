#!/usr/bin/python

import sys

if len (sys.argv) < 2:
	print "threshold needed!"
	sys.exit (1)

threshold = float (sys.argv [1])

for line in sys.stdin:
	(a, b, corr) = line.split ("\t")
	a = int (a)
	b = int (b)
	corr = float (corr)
	
	if corr >= threshold:
		print "%d\t%d" % (a, b)