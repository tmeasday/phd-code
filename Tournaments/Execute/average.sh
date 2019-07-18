#!/bin/bash

count=0
first=1
while read line
do
	i=0
	for field in $line
	do
		if [[ $first == 1 ]]; then
			sum[$i]=$field
		else
			sum[$i]=$(echo ${sum[$i]} + $field | bc)
		fi
		
		i=$(echo $i+1 | bc)
	done 

	first=0
  n_cols=$i
	count=$(echo $count+1 | bc)
done

for (( i = 0; i < $n_cols; i++ )); do
	echo -n "${sum[$i]}	"
done
echo

for (( i = 0; i < $n_cols; i++ )); do
	avg=$(echo 3 k ${sum[$i]} $count / p | dc)
	echo -n "$avg	"
done
echo