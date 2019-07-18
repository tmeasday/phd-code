#!/bin/sh

n=100
edge_p=1
flip_p=0.45


for size in 10 20 30 40 50 75 100 200 300 400 500
do
	./Datasets/Synthetic/planted.exe $n $size 0 $edge_p $flip_p > Temp/sigraphs
	./Execute/do_run.sh
	echo "planted.exe $n $size 0 $edge_p $flip_p" | ./Execute/save_run.sh "timings-test/$size"
done