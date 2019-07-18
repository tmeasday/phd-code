#!/usr/bin/env python
# encoding: utf-8
"""
demographics.py

Created by Tom Coleman on 2007-07-26.
Copyright (c) 2007 The University of Melbourne. All rights reserved.
"""

import sys
import os


def main():
	fems  = {}
	males = {}
	for line in sys.stdin:
		try:
			(id, age, sex, zip) = line.split ("\t")
			if sex == 'M':
				sex = males
			elif sex == 'F':
				sex = fems
			else:
				raise ValueError
			
			age = int (age)
			id = int (id)
			if sex.has_key (age):
				sex[age].append (id)
			else:
				sex[age] = [id]
		except ValueError:
			pass
		except:
			raise
			
	for sex in [fems, males]:
		for age, demo in sex.items ():
			#print age, ":", demo
			print demo
			
if __name__ == '__main__':
	main()

