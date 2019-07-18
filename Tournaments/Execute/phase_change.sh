#!/bin/bash -

dir="Results/Phase_Change"

for (( i = 0; i < 10; i++ )); do
	for n in 100 200 400; do
		for p in 0.5 0.51 0.55 0.6 0.8 0.9 0.95 0.99; do
			echo $p $n $i
			./Data/Synthetic/biased.exe $n 1 $p | cut -f 2 | ./Algorithms/degree_difference.exe > $dir/$p-$n-$i
		done
	done
done