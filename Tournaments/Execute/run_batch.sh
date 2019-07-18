#!/bin/bash -

ALG_FILE="Algorithms/algorithms"
EXE_DIR=Execute
DD=Temp

#EXE_FILE="$EXE_DIR/synthetic.sh"
#EXE_FILE="$EXE_DIR/weighted.sh"

EXE_FILE=$1; shift
n=$1; shift
run=$1; shift
datadir=$1; shift


if [[ $run == '' ]]; then
	echo "usage: ./run_batch.sh exefile n runs [datadir] [args]"
	exit
fi

ps=$(cat $DD/ps)

cp "$ALG_FILE" "$DD/algorithms"
. "$EXE_DIR/algorithms.sh"; read_algorithms
algs="p	original $algs"
if [[ $BRANCH != '' ]]; then
	algs="$algs optimal"
fi

for x in '' '_loc' '_ins'; do
	results="$DD/results$x.csv"
	timing="$DD/timing_results$x.csv"
	
	echo -n > $results
	echo -n > $timing
	for alg in $algs; do
  	echo -n "${alg}," >> $results
  	echo -n "${alg}," >> $timing
  done;
	echo >> $results
	echo >> $timing
done

if [[ $datadir != '' ]]; then
 	dir="$datadir/$n/"
	mkdir "$dir"
fi

for p in $ps
do
	echo "$EXE_FILE" $n $run $p $@
	
	
	for x in '' '_loc' '_ins'; do
		echo -n "$p," >> "$DD/results$x.csv"
		echo -n "$p," >> "$DD/timing_results$x.csv"
	done
	
	"$EXE_FILE" $n $run $p $@
	
	for x in '' '_loc' '_ins'; do
		cat "$DD/output$x.csv" | "./Library/read_output.py" >> "$DD/results$x.csv"
		
		line=$(cat "$DD/timing$x" | grep user | sed 's/[^0-9\.]*//')
		if [[ $x == '' ]]; then # put a blank in for original
			echo -n "," >> "$DD/timing_results$x.csv"
		fi
		for field in $line; do
			echo -n "$field," >> "$DD/timing_results$x.csv"
		done
		echo >> "$DD/timing_results$x.csv"
		
		
		if [[ $datadir != '' ]]; then
			cp "$DD/output$x.csv" "$dir/output${x}-$p.csv"
		fi
	done
	
	if [[ $datadir != '' ]]; then
		cp "$DD/tourns" "$dir/tourns-$p"
	fi
done

if [[ $datadir != '' ]]; then
	for x in '' '_loc' '_ins'; do
		cp "$DD/results$x.csv" "$dir"
		cp "$DD/timing_results$x.csv" "$dir"
	done
fi