#!/usr/bin/python

import sys

info = False
try:
	if sys.argv[1] == '-i':
		info = True
except:
	pass

lineno = 1
for line in sys.stdin:
	a,b = line.split (',')
	a = int (a.strip ())
	b = int (b.strip ())
	
	if (a > b):
		print lineno
		if info:
			print a, b
	
	lineno += 1