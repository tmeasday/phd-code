#!/usr/bin/python
import sys

def read_file (infile, baseline):
	algs = infile.readline ().split (',')
	algs = [alg.strip () for alg in algs if alg.strip () != '']
	
	results = {}
	for alg in algs:
		results[alg] = []
		
	for line in infile:
		if line.strip () == '':
			break
			
		data = line.split (',')
		for alg in algs:
			results[alg].append (float (data.pop (0)))

	count = len (results[algs[0]])

	sums = {}
	wins = {}
	for alg in algs:
		sums[alg] = 0
		wins[alg] = 0
	
	if (baseline):	
		bsres = results[baseline]
	else:
		bsres = False
		
	if algs[0] == 'original':
		wins_algs = algs[1:]
	else:
		wins_algs = algs
	for i in range (count):
		best = results[wins_algs[0]][i]
		best_algs = [wins_algs[0]]
		
		for alg in wins_algs:
			result = results[alg][i]
			if bsres:
				if bsres[i] == 0:
					sums[alg] += (result - bsres[i])
				else:
					sums[alg] += (result - bsres[i]) / bsres[i]
			else:
				sums[alg] += result
			
			if abs (result - best) < 0.01:
					best_algs.append (alg)
			elif result < best:
				best = result
				best_algs = [alg]

		# if len (best_algs) == 0:
		# 	print best, results[algs[0]][i]
			
		# print >> sys.stderr, best_algs
		for alg in best_algs:
			wins[alg] += 1.0 / len (best_algs)
	
	for key, val in wins.iteritems ():
		wins[key] = (val / count) * 100
	
	most_wins = 0
	best_avg  = 0
	
	avgs = {}	
	for alg in algs:
		avgs[alg] = sums[alg] / count
		
		most_wins = max (most_wins, wins[alg])
		if best_avg == 0:
			best_avg = avgs[alg]
		else:
			best_avg = min (best_avg, avgs[alg])
		
	return (algs, avgs, wins, best_avg, most_wins)

if __name__ == '__main__':
  algs, avgs, wins, best_avg, most_wins = read_file (sys.stdin, 'insertionsort') 

  for alg in algs:
    print "%.4f (%.2f)," % (avgs[alg], wins[alg]),
  
