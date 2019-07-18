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


"$GD/biased.exe" $N $RUN $P > $DD/input

cat "$DD/input" | cut -f 1 > "$DD/orders"
cat "$DD/input" | cut -f 2 > "$DD/tourns"

cp "$ALG_FILE" "$DD/algorithms"

"$ED/do_run.sh" || exit 1
