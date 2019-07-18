#!/usr/bin/env python
# encoding: utf-8
"""
average.py

Created by Tom Coleman on 2007-07-16.
Copyright (c) 2007 __MyCompanyName__. All rights reserved.
"""

import sys
import os


def main():
	sum = []
	first = True
	count = 0
	for line in sys.stdin:
		count += 1
		i = 0
		for field in line.strip().split (","):
			if field.strip () == '': 
				continue
			if first:
				sum.append (float (field))
			else:
				sum[i] += float (field)
			i += 1
		first = False
		
	n_cols = i
	
	for i in range (n_cols):
		print "%f," % sum[i],
	print
	
	for i in range (n_cols):
		print "%f," % (sum[i] / count),
	print

if __name__ == '__main__':
	main()

