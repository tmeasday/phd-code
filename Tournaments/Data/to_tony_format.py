#!/usr/bin/env python
# encoding: utf-8
"""
to_tony_format.py

Created by Tom Coleman on 2008-05-01.
Copyright (c) 2008 . All rights reserved.
"""

import sys
import os


def main():
	tourn = eval (sys.stdin.read ())
	for x in range (len (tourn)):
		line = tourn[x]
		for y in range (len (line)):
			print "t_%d t_%d %d" % (x, y+x+1, int ((line[y]-0.5) * 2))


if __name__ == '__main__':
	main()

