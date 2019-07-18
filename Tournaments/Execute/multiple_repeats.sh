#!/bin/bash -

dir=$1
P=0.6

rm -rf Temp/*
cp Algorithms/algorithms Temp

echo -n randomizing...
#cat Data/EachMovie/Demo-Tourns.txt_2 | ./Execute/sample.py 100 | cut -f 2 | ./Execute/randomize.exe > Temp/tourns
./Data/Synthetic/biased.exe 100 2 0.6 | cut -f 2 > Temp/tourns

for m in 1 2 4 8 16 32 64 128 256; do
	echo "$m"
	mkdir -p "$dir/$m"
	REP=$m ./Execute/do_run.sh

	cp Temp/output.csv "$dir/$m/"
	cp Temp/timing "$dir/$m/"
	
	# now save the data
	for d in Temp/*; do
		if [ -d "$d" ]; then
			cp -r "$d" "$dir/$m"
		fi
	done
done
