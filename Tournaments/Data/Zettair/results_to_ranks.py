#!/usr/bin/python

import sys
import os

DIR="ranks/"

def save (lines, i, j):
	try:
		os.makedirs (DIR + str(i))
	except:
		pass

	filename = DIR + str(i) + "/" + str(j)
	fp = open (filename, "w")

	for line in lines:
		print >> fp, line.split(" ")[1]

	fp.close ()

j = 0
for filename in sys.argv[1:]:
	print filename
	
	lines = []
	start = False
	i = 0
	for line in file (filename):
		if line.strip () == '':
			i += 1
			# we are at the end of of a query
			save (lines, i, j)
			
			lines = []
			start = True
		elif start:
			start = False
		else:
			lines.append (line)
			
	#save (lines, i, j)
	j += 1
