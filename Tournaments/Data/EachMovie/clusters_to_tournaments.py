#!/usr/bin/python

import sys
from votes import read_votes

votes_file = "Normalized.txt"

if len (sys.argv) < 2:
	raise Exception ("Threshold needed!")
	
threshold = int (sys.argv[1])
votes = read_votes (file (votes_file))

for line in sys.stdin:
	cluster = eval (line)
	cluster = filter (votes.has_key, cluster)	
	if len (cluster) < threshold:
		continue
	
	# keep track of the items we've already recorded
	items = []
	tourn = []
	for person in cluster:
		for item in votes[person].keys ():
			if item in items:
				continue
				
			# cool. a new item. Let's compare it with all the items we've seen sofar
			tourn_line = []
			for i in items:
				score, count = 0, 0
				for p in cluster:
					if not (votes[p].has_key (i) and votes[p].has_key (item)):
						continue
					
					if votes[p][i] > votes[p][item]:
						score += 1
					count += 1
				
				if count == 0:
					tourn_line.append (0.5)
				else:
					tourn_line.append (float(score) / count)
					
			tourn.append (tourn_line)
			items.append (item)
			
	tourn.reverse ()
	print tourn
			
