#!/usr/bin/env python
# encoding: utf-8
"""
best_worst_average.py

Created by Tom Coleman on 2008-10-07.
Copyright (c) 2008 __MyCompanyName__. All rights reserved.
"""

import sys
import os


def main():
	for line in sys.stdin:
		results = [float(x) for x in line.strip(",\n").split(',')]
		n = len(results)
	
		print "%f,%f,%f" % (min(results), max(results), sum(results) / n)


if __name__ == '__main__':
	main()