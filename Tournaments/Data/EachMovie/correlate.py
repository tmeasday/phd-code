#!/usr/bin/python

import sys
from votes import read_votes

votes = read_votes ()

print >> sys.stderr, "Finished reading votes"

people = votes.keys ()
people.sort ()

#biggest, smallest = 0, 0
for a in people:
	print >> sys.stderr, a
	for b in people:
		
		if (a >= b): continue
		
		corr = 0
		# calculate correlation
		for item in votes[a].keys ():
			if item in votes[b].keys ():
				corr += votes[a][item] * votes[b][item]
				
		print "%d\t%d\t%f" % (a, b, corr)
		
#		if (corr > biggest):
#			biggest = corr
#		if (corr < smallest):
#			smallest = corr
			
#print biggest, smallest