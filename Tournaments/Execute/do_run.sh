#!/bin/bash -

# This file does a single run of all the algorithms $DD/algorithms
# on the tournaments in $DD/tourns
# placing the results in $DD/algname_orders
# and the timing data into $DD/timing
# then calculating the upsets into $DD/upsets
# including $DD/orders if it exists
# then creates a summary file $DD/results

DD=Temp
AD=Algorithms
ED=Execute

TIME="/usr/bin/time -p"

REPEAT="$ED/repeat-ins.py"
LOCAL="$AD/moves-insert.exe"
INSERT="$AD/insertionsort.exe"

n_runs=$(cat "$DD/tourns" | wc -l)

# the list of all the algorithms
n_algs=0
alg_files=
loc_alg_files=
ins_alg_files=

. "$ED/algorithms.sh"; read_algorithms

rm -f "$DD/timing" "$DD/timing_loc" "$DD/timing_ins"


# if [[ -e "$DD/orders" ]]; then
# 	alg_files="$DD/orders $alg_files"
# 	loc_alg_files="$DD/orders_local $loc_alg_files"
# 	ins_alg_files="$DD/orders_insert $ins_alg_files"
# 
# 	paste -d"\n" "$DD/tourns" "$DD/orders" > "$DD/tourns_orders"
# 	echo "locally improving starting order"
# 	$TIME 2>> "$DD/timing_loc" $LOCAL -i > "$DD/orders_local" < "$DD/tourns_orders"
# 	echo "insertionsorting starting order"
# 	$TIME 2>> "$DD/timing_ins" $INSERT -i > "$DD/orders_insert" < "$DD/tourns_orders"
# fi

for algname in $algs
do
  alg="$AD/${algname}.exe"
	orderfile="$DD/${algname}_orders"
  alg_files="$alg_files $orderfile"
  loc_orderfile="$DD/${algname}_orders_local"
	loc_alg_files="$loc_alg_files $loc_orderfile"
	ins_orderfile="$DD/${algname}_orders_insert"
	ins_alg_files="$ins_alg_files $ins_orderfile"

	n_algs=$(echo $n_algs + 1 | bc)

	rep=$(echo $reps | awk -F' ' "{ print \$$n_algs }")
  echo -n "running $algname ($rep times).."
  #cat $DD/tourns | "$alg" $RUN > "$DD/$algname" || exit
  	$TIME 2>> "$DD/timing" $REPEAT "$alg" $REP > "$orderfile" < "$DD/tourns"
	# $TIME 2>> "$DD/timing" "$alg" > "$orderfile" < "$DD/tourns"
	
	paste -d"\n" "$DD/tourns" "$orderfile" > "$DD/tourns_orders"
	echo -n "locally improving.."
	$TIME 2>> "$DD/timing_loc" $LOCAL -i > "$loc_orderfile" < "$DD/tourns_orders"
	echo "insertionsort"
	$TIME 2>> "$DD/timing_ins" $INSERT -i > "$ins_orderfile" < "$DD/tourns_orders"
done

# if [[ -e "$DD/orders" ]]; then
# 	algs="original	$algs"
# 	n_algs=$(echo $n_algs + 1 | bc)
# fi

echo -n "calculating upsets.."
echo "$DD/tourns" $alg_files | xargs paste -d"\n" > "$DD/all_orders"
cat  "$DD/all_orders" | "$ED/calc_upsets.exe" $n_runs $n_algs > "$DD/upsets" || exit 1

echo "$DD/tourns" $loc_alg_files | xargs paste -d"\n" > "$DD/all_orders_loc"
cat  "$DD/all_orders_loc" | "$ED/calc_upsets.exe" $n_runs $n_algs > "$DD/upsets_loc" || exit 1

echo "$DD/tourns" $ins_alg_files | xargs paste -d"\n" > "$DD/all_orders_ins"
cat  "$DD/all_orders_ins" | "$ED/calc_upsets.exe" $n_runs $n_algs > "$DD/upsets_ins" || exit 1
echo "done"


# run the branch and bound algorithm
# FIXME -- insertionsort
if [[ $BRANCH != '' ]]; then
	echo "calculating optimal solution"
	cat "$DD/upsets" | "$ED/minimize.py" > "$DD/bounds"
	cat "$DD/tourns" | "$AD/branch_and_bound.exe" -b "$DD/bounds" > "$DD/bounds_orders"
	paste -d"\n" "$DD/tourns" "$DD/bounds_orders" | "$ED/calc_upsets.exe" $n_runs 1 > "$DD/bounds_upsets"

	paste -d"\0" "$DD/upsets" "$DD/bounds_upsets" > "$DD/upsets_temp"
	mv "$DD/upsets_temp" "$DD/upsets"
	paste -d"\0" "$DD/upsets_loc" "$DD/bounds_upsets" > "$DD/upsets_temp"
	mv "$DD/upsets_temp" "$DD/upsets_loc"

	algs="$algs	optimal"
fi

for x in '' '_loc' '_ins'; do
	output="$DD/output$x.csv"
	upsets="$DD/upsets$x"

	echo -n > $output
	for alg in $algs; do
  	echo -n "${alg}, " >> $output
	done
	echo >> $output

	cat $upsets >> $output
	echo >> $output

	cat $upsets | "$ED/average.py" >> $output
done
