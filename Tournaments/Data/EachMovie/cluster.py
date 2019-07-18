#!/usr/bin/python

import sys

clusters = []
for line in sys.stdin:
	(a, b) = line.split ("\t")
	a = int (a)
	b = int (b)
	
	a_clust, b_clust = -1, -1
	for i in range (len (clusters)):
		if a_clust == -1 and a in clusters[i]:
			a_clust = i
		if b_clust == -1 and b in clusters[i]:
			b_clust = i
			
	# new cluster needed
	if a_clust == -1 and b_clust == -1:
		clusters.append([a,b])
	
	# changes needed
	elif a_clust != b_clust:
		if a_clust == -1:
			clusters[b_clust].append (a)
		
		elif b_clust == -1:
			clusters[a_clust].append (b)
	
		else:
			a_clust = clusters.pop (a_clust)
			b_clust = clusters.pop (b_clust)
			
			clusters.append (a_clust + b_clust)
			
for cluster in clusters:
	print cluster