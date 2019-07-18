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


"$GD/edge_crossings.py" $N $RUN $P > $DD/tourns

cp "$ALG_FILE" "$DD/algorithms"
rm -f $DD/orders

"$ED/do_run.sh" || exit 1
