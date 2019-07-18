#!/usr/bin/python

import sys

items=[]
for filename in sys.argv[1:]:
	if items == []:
		for line in file (filename):
			items.append (line)
			
	else:
		new_items = []
		for line in file (filename):
			if line in items:
				new_items.append (line)
				
		items = new_items
		
		
for item in items:
	print item,