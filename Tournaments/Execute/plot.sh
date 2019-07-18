#!/bin/bash -

DD=Temp
ALG_DIR=Algorithms

file="$DD/results"
if [[ "$1" != "" ]]; then
	file="$1"
fi

outfile="$DD/plot.jpg"
if [[ "$2" != "" ]]; then
  outfile="$2"
fi

#echo $algs

cut -f 1 -d "," "$file" > $DD/xvals

#echo "set term postscript eps" > $DD/plot
echo "set term postscript eps solid color lw 3" > $DD/plot
#echo "set term png" > $DD/plot
echo "set output '$outfile'" >> $DD/plot
echo "set xlabel 'p'" >> $DD/plot
echo "set ylabel 'Relative Error'" >> $DD/plot
echo -n "plot " >> $DD/plot

len=$(cat $file | wc -l)
echo $len
len=$(expr $len - 1)
echo $len

i=0
for alg in $(head -n 1 $file | sed "s/,/ /g" )
do
	i=$(expr $i + 1)
  if [[ $alg == 'p' || $alg == 'original' || $alg == 'bubblesort' ]]; then 
		continue; 
	fi

	case $alg in
		order_by_indegree)
			name='Iterated Kendall';;
		eades)
			name='Eades';;
		eades_imp)
			name='Eades Improved';;
		insertionsort)
			name='Insertionsort';;
		mergesort)
			name='Mergesort';;
		quicksort)
			name='Quicksort';;
		bubblesort)
			name='Bubblesort';;
		triangle_both)
			name='Triangle Both';;
		moves)
			name='Moves';;
		sim-annealing)
			name='Simulated Annealing';;
	esac

  echo $alg
  
	cut -f $i -d "," "$file" | sed 's/(.*)//' | paste $DD/xvals - \
	           | tail -n $len > $DD/${alg}.dat
	
	if [ $i -ne 3 ]
	then
		echo -n ", " >> $DD/plot
	fi
	echo -n "'$DD/${alg}.dat' w lines title '$name'" >> $DD/plot
done
echo >> $DD/plot

gnuplot < $DD/plot
