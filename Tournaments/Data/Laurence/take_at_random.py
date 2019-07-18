#!/usr/bin/python

"""
Read in a large list of numbers and filter them at random.

Works line-wise so is fast
"""

import sys
import random

chance = float (sys.argv[1])

random.seed ()

for line in sys.stdin:
	if random.random () < chance:
		print line,
		
