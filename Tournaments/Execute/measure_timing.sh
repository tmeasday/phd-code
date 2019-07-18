#!/bin/bash -

EXE_DIR=Execute
DD=Temp

run=1000
p=0.6

#ns="10 20 40 50 75 100 125 150 175 200 250 300 400 500 750 1000"
ns="500 750 1000 1500 2000"
#ns="5 10 15 20"

vars=( '' '_loc' '_ins' )
#vars=( '' )
nvars=${#vars[*]}

echo -n "," > "$DD/timing_results.csv"

cp "Algorithms/algorithms" "$DD/algorithms"
. "$EXE_DIR/algorithms.sh"; read_algorithms
both_algs=

for ((i=0 ; i<$nvars ; i++)); do
	x=${vars[${i}]}

	for alg in $algs; do
		echo -n "${alg}${x}," >> "$DD/timing_results.csv"
	done
done
echo >> "$DD/timing_results.csv"

for n in $ns
do
	echo "$n"
	echo -n "$n," >> "$DD/timing_results.csv"
	./$EXE_DIR/biased.sh $n $run $p || exit

	for ((i=0 ; i<$nvars ; i++)); do
		x=${vars[${i}]}
		line=$(cat "$DD/timing${x}" | grep user | sed 's/[^0-9\.]*//')
	
		for field in $line; do
			echo -n "$field," >> "$DD/timing_results.csv"
		done
	done
	echo >> "$DD/timing_results.csv"
done
