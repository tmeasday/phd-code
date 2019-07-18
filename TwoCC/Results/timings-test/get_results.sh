#!/bin/bash

for n in 10 20 30 40 50 75 100 200 300 400 500
do
	echo -n "$n,"
	cat Results/timings-test/$n/results | tail -n 1 | cut -f 2 -d ':'
done