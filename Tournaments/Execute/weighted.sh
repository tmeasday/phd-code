#!/bin/bash -


AD=Algorithms
GD=Data/Synthetic
ED=Execute
DD=Temp

ALG_FILE="$AD/algorithms"
TIME=/usr/bin/time

N=$1
RUN=$2
P=$3

if [[ $P == '' ]] 
then
  echo "ERROR: enter parameters: n runs p"
  exit
fi

"$GD/make_orders.exe" $N $RUN > $DD/orders
cat $DD/orders | "$GD/weighted.py" $P > $DD/tourns

cp "$ALG_FILE" "$DD/algorithms"

"$ED/do_run.sh" || exit 1
