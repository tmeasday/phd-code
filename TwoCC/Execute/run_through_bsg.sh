#!/bin/bash

n_probs=$(cat Temp/sigraphs | wc -l)

rm Temp/opts
for (( i = 1; i <= $n_probs; i++ )); do
	head -n $i Temp/sigraphs | tail -n 1 | ./Datasets/save_for_bsg.exe \
		| ~/Desktop/bsg-1.0/bsg -s >> Temp/opts
done