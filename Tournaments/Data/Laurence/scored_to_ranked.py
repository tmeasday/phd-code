#!/usr/bin/python

import sys
import operator

items = []
for line in sys.stdin:
	try:
		(score, item) = line.split (" ")
		score = float(score)
	except:
		continue
	
	items.append ((score, item))

items.sort()

# put the best first. Probably doens't really matter, but hey
items.reverse()

for (score, item) in items:
	print item,
