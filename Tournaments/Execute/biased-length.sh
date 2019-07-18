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
TYPE=$4

if [[ $TYPE == '' ]] 
then
  echo "ERROR: enter parameters: n runs p type"
  exit
fi

"$GD/make_orders.exe" $N $RUN > $DD/orders
cat $DD/orders | "$GD/biased-length.py" $P $TYPE> $DD/tourns

cp "$ALG_FILE" "$DD/algorithms"

"$ED/do_run.sh" || exit 1
