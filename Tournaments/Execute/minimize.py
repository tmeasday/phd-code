#!/usr/bin/python

import sys

for line in sys.stdin:
	min = -1
	for field in line.split ():
		field = float (field)
		
		if min == -1 or field < min:
			min = field
			
	print min