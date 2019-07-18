#!/usr/bin/python

import sys

def read_votes (infile = sys.stdin):
	votes = {}
	for line in infile:
		(person, item, vote) = line.split ("\t")
		person = int (person)
		item   = int (item)
		vote   = float (vote)

		if not votes.has_key (person):
			votes[person] = {}
			
		votes[person][item] = vote
		
	return votes

# this will pretty much read in and print out again
def main ():
	votes = read_votes ()
	
	for person in votes.keys ():
		for item in votes[person].keys ():
			print "%d\t%d\t%f" % (person, item, votes[person][item])

if __name__ == '__main__':
	main()