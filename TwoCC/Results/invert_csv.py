#!/usr/bin/env python
# encoding: utf-8
"""
invert_csv.py

Created by Tom Coleman on 2008-04-03.
Copyright (c) 2008 . All rights reserved.
"""

import sys
import os


def main():
	fields = []
	for line in sys.stdin:
		fields.append (line.split (','))
		
	n = len (fields)
	col = 0
	while True:
		printed = False
		for i in range (n):
			if (len (fields[i]) > col):
				printed = True
				content = fields[i][col].strip ()
			else:
				content = ''
			print "%s, " % content,
			
		col += 1
		print
		if not printed:
			break


if __name__ == '__main__':
	main()

