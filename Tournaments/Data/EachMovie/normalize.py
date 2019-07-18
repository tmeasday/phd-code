#!/usr/bin/python
import sys
from math import sqrt

def print_votes (person, votes):
	n_votes = len (votes)
	if n_votes == 0:
		return

	# first work out the relevant stuff
	sum = 0
	for vote in votes.values ():
		sum += vote
	mean = sum / n_votes
	
	sum = 0
	for vote in votes.values ():
		sum += (vote - mean)**2
	std_dev = sqrt (sum)

	for item, vote in votes.items ():
		if std_dev != 0:
			normed = (vote - mean) / std_dev
		else:
			normed = 0
		print "%d\t%d\t%f" % (person, item, normed)

last_person = 0
for line in sys.stdin:
	(person, item, vote, weight, date) = line.split ("\t")
	person = int (person)
	item   = int (item)
	vote   = float (vote)
	weight = float (weight)
	
	if person != last_person:
		if last_person != 0:
			print_votes (last_person, votes)
		
		last_person = person
		votes = {}
	
	if weight == 1:
		votes[item] = vote
		
# print out the last person's stuff
print_votes (last_person, votes)