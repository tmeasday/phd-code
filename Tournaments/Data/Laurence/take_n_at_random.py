#!/usr/bin/python

"""
Read in a large list of numbers and then take n of them.

Works on the whole list, so is fairly time consuming
"""

import sys
import random

lines = []
for line in sys.stdin:
		lines.append (line)

random.shuffle (lines)

for i in range (int (sys.argv[1])):
	print lines[i],