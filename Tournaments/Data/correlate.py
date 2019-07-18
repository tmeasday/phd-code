#!/usr/bin/env python
# encoding: utf-8
"""
correlate.py

simply correlate two series of numbers

Created by Tom Coleman on 2007-08-21.
Copyright (c) 2007 The University of Melbourne. All rights reserved.
"""

import sys
import os
import math

def main():
	series_1 = []
	series_2 = []
	
	for line in sys.stdin:
		(data_1, data_2) = line.split ("\t")
	
		series_1.append (float (data_1))
		series_2.append (float (data_2))
	
	count = len (series_1)
	
	mean_1 = sum (series_1) / count
	mean_2 = sum (series_2) / count
	
	# not really the standard deviations, but sortof similar
	stddev_1 = math.sqrt (sum ([(d - mean_1)**2 for d in series_1]))
	stddev_2 = math.sqrt (sum ([(d - mean_1)**2 for d in series_2]))

	corr = 0
	for i in range (count):
		corr += (series_1[i] - mean_1) * (series_2[i] - mean_2)
	print corr / (stddev_1 * stddev_2)

if __name__ == '__main__':
	main()

