#!/bin/bash -
# 
#  save_run.sh
#  TwoCC
#  
#  Created by Tom Coleman on 2008-03-13.
#  Copyright 2008 Tom Coleman. All rights reserved.
# 

RD='Results'
TD='Temp'

SAVE="$RD/$1"
if [[ $SAVE == "$RD/" ]]; then
	datestr=$(date "+%Y-%m-%d_%H-%M-%S")
	SAVE="$RD/save-$datestr"
	echo "$SAVE"
fi

mkdir "$SAVE" || exit

date "+%d/%m/%Y %H:%M:%S" > "$SAVE/description"

echo "Enter description of dataset:"
while read line
do
	echo $line >> "$SAVE/description"
done


cp "$TD/algorithms" "$TD/sigraphs" "$TD/labellings" "$TD/costs" "$TD/results" "$SAVE"
