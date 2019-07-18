#!/bin/bash -

BATCH="Execute/run_batch.sh"
STDPS="Execute/std_ps"
ECROSSPS="Execute/edge_ps"
PSFILE="Temp/ps"

BIASED="Execute/biased.sh"
BIASED_LEN="Execute/biased-length.sh"
EDGE_CROSS="Execute/edge_crossings.sh"
WEIGHTED="Execute/weighted.sh"

ns="100 200 300 400"
size=1000

datestr=$(date +%F)
dir="Results/$datestr"

if [[ -e "$dir" ]]; then
	n=1
	while [[ -e "${dir}_$n" ]]; do
		n=$(expr $n + 1)
	done
	dir="${dir}_$n"
fi

mkdir -p "$dir"

biased_dir="$dir/biased"
mkdir -p $biased_dir

biased_len_a_dir="$dir/biased_len_a"
mkdir -p $biased_len_a_dir

biased_len_b_dir="$dir/biased_len_b"
mkdir -p $biased_len_b_dir

weighted_dir="$dir/weighted"
mkdir -p $weighted_dir

edge_crossings_dir="$dir/edge_crossings"
mkdir -p $edge_crossings_dir

for n in $ns; do
	cp $STDPS $PSFILE
	$BATCH $BIASED $n $size "$biased_dir"
	$BATCH $BIASED_LEN $n $size "$biased_len_a_dir" 1
	$BATCH $BIASED_LEN $n $size "$biased_len_b_dir" 0
	$BATCH $WEIGHTED $n $size "$weighted_dir"

	cp $ECROSSPS $PSFILE
	$BATCH $EDGE_CROSS $n $size "$edge_crossings_dir"
done
