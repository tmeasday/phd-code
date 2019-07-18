#!/usr/bin/python

"""Usage: ranks_to_tourn.py [-n num] file1 ... filek

Takes a set of files, each ranking some set of N objects, 
and returns a tournament of size n <= N (default), 
of the consensus view of the first n of the N objects."""

import sys
import getopt

class Usage(Exception):
	def __init__(self, msg):
		self.msg = msg

def get_tourn (rankings):
	size = len (rankings [0])
	tourn = []
	for i in range (size):
		line = []
		for j in range (i+1, size):
			
			diff, count = 0, 0
			for ranking in rankings:
				if ranking.index (i) > ranking.index (j):
					diff += 1
				else:
					diff -= 1
				count += 1
			
			line.append (((float (diff) / count) + 1) / 2)
		tourn.append (line)
	
	print tourn

def do_stuff (files, n):
	# ok, maybe we could do this smarter, but for the moment
	rankings = []
	for afile in files:
		ranking = []
		for line in file (afile):
			ranking.append (line)
		rankings.append (ranking)
	n_ranks = len (rankings)
	
		
	# ok what should n be?
	if n == None:
		items = rankings [0]
	else:
		items = []
		i, n_items = 0, 0
		while n_items < n:
			item = rankings[i / n_ranks][i % n_ranks]
			
			if item not in items:
				items.append (item)
				n_items += 1
			i += 1
	
	# turn the rankings into something easier to deal with
	new_rankings = []
	for ranking in rankings:
		new_ranking = []
		for item in ranking:
			try:
				index = items.index (item)
			except ValueError:
				continue

			new_ranking.append (index)
		new_rankings.append (new_ranking)

	get_tourn (new_rankings)

def main():
	try:
	   # parse command line options
		try:
			opts, args = getopt.getopt(sys.argv[1:], "hn:", ["help", "number"])
		except getopt.error, msg:
			raise Usage (msg)
			
		n = None
    # process options
		for o, a in opts:
			if o in ("-h", "--help"):
				print __doc__
				sys.exit(0)
			if o in ("-n", "--number"):
				try:
					n = int (a)
				except:
					raise Usage ("invalid n value")
    # process arguments
		if len (args) == 0:
			raise Usage ("please enter some files to be used")
			
		do_stuff (args, n)
		
	except Usage, err:
		print >> sys.stderr, err.msg
		print >> sys.stderr, "for help use --help"
		sys.exit (2)
		

if __name__ == "__main__":
    main()