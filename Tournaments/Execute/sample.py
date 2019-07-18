#!/usr/bin/env python
# encoding: utf-8
"""
sample.py

Created by Tom Coleman on 2007-07-01.
"""

import sys
import getopt
import random

help_message = '''
Usage: sample.py n

where n is the size of the sample
'''


class Usage(Exception):
	def __init__(self, msg):
		self.msg = msg

def sample (number):
	for line in sys.stdin:
		tourn = eval (line)
		size = len (tourn)
		
		# ok we need to pick number of [size]
		if (size < number) :
			nodes = range (size)
		else:
			nodes = random.sample (range (size), number)
		nodes.sort ()
		
		new_tourn = []
		for node1 in nodes:
			new_line = []
			for node2 in nodes:
				if node2 <= node1:
					continue
					
				new_line.append (tourn[node1][node2-node1-1])
			
			new_tourn.append (new_line)
			
		print "%s\t%s" % (str(nodes), str(new_tourn))
		
def main(argv=None):
	if argv is None:
		argv = sys.argv
	try:
		try:
			opts, args = getopt.getopt(argv[1:], "h", ["help"])
		except getopt.error, msg:
			raise Usage(msg)
	
		# option processing
		for option, value in opts:
			if option in ("-h", "--help"):
				raise Usage(help_message)
	
		try:
			number = int(args[0])
		except:
			raise Usage("Please provide a sample size")
		sample (number)

	except Usage, err:
		print >> sys.stderr, sys.argv[0].split("/")[-1] + ": " + str(err.msg)
		print >> sys.stderr, "\t for help use --help"
		return 2


if __name__ == "__main__":
	sys.exit(main())
