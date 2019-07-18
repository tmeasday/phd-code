#!/usr/bin/python

import sys

shell=True
delim="\t"
try:
	if (sys.argv[1] == 'latex'):
		shell=False
		delim="&"
except:
	pass

for line in sys.stdin:
	min = -1
	fields = []
	try:
		for field in line.split (delim):
			field = field.strip ()
			if field == '':
				continue
#			print "'%s'" % field
			field = float(field)
            
			if min == -1 or field < min:
				min = field          
			fields.append (field)
    
		first = True
		for field in fields:
			if not first:
				print delim,
			first = False
			
			if field == min:
				if shell:
					print "\033[1m%.2f\033[0m" % field,
				else:
					print "\\textbf{%.2f}" % field,
			else:
				print "%.2f" % field,
		print
			
	except ValueError:
		print line,
