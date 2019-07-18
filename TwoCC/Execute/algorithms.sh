#!/bin/bash

TD=Temp

function read_algorithms {

	((i = 0))
	while read line
	do
	 	# echo algs[${i}]="$line"
		algs[${i}]="$line"
		((i += 1))
	done < "$TD/algorithms"
	
	# echo ${#algs[*]}
#	reps=$(cat $TD/algorithms | grep -v "^#" | sed 's/^[^ ]*//' | sed 's/^$/1/')
}
