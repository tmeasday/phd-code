#!/bin/bash -

n="$1"
dir="$2"

# rundir="$dir/laurence"
# mkdir -p "$rundir"
# cat Data/Laurence/Tourns/tourn-* | ./Execute/real-data.sh $n
# cp Temp/output*.csv Temp/timing Temp/timing_ins Temp/timing_loc "$rundir"
# 
# rundir="$dir/zettair"
# mkdir -p "$rundir"
# cat Data/Zettair/Tourns/tourn-* | ./Execute/real-data.sh $n
# cp Temp/output*.csv Temp/timing Temp/timing_ins Temp/timing_loc "$rundir"

rundir="$dir/eachmovie"
mkdir -p "$rundir"
cat Data/EachMovie/Demo-Tourns-Sample-100.txt_3 | cut -f 2 |./Execute/real-data.sh $n
cp Temp/output*.csv Temp/timing Temp/timing_ins Temp/timing_loc "$rundir"
