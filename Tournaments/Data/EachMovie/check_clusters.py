#!/usr/bin/python

import sys

people = []
sizes = {}
for line in sys.stdin:
	cluster = eval (line)
	
	for person in cluster:
		if person in people:
			print person, "repeated!"
			sys.exit (1)
		else:
			people.append (person)
	
	size = len (cluster)
	if sizes.has_key (size):
		sizes[size] += 1
	else:
		sizes[size] = 1

keys = sizes.keys ()
keys.sort ()

for key	 in keys:
	print "%d: %d" % (key, sizes[key])