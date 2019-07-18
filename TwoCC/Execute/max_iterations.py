#!/usr/bin/python

import sys

maxits = 0
for line in sys.stdin:
	try:
		its = int (line.split (' ')[1])
		if (its > maxits):
			maxits = its
	except:
		pass	
		
print maxits