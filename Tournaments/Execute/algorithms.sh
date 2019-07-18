#!/bin/bash

DD=Temp

function read_algorithms {
	# ((i = 0))
	# while read line
	# do
	# 	if echo "$line" | grep '^#' > /dev/null; then
	# 		continue
	# 	fi
	# 	
	#  	# echo algs[${i}]="$line"
	# 	algs[${i}]="$line"
	# 	((i += 1))
	# done < "$DD/algorithms"

	algs=$(cat $DD/algorithms | grep -v "^#" | cut -f 1 -d ' ')
	reps=$(cat $DD/algorithms | grep -v "^#" | sed 's/^[^ ]*//' | sed 's/^$/1/')
}
