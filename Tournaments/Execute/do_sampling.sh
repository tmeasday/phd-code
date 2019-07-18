#!/bin/bash -

ALG_DIR=Algorithms
GEN_DIR=Generate
EXE_DIR=Execute

N=$1
RUN=$2
P=$3
SN=$4

if [ "x$N" == 'x' ] 
then
  echo "ERROR: enter parameters: n runs p sn"
  exit
fi

if [ "x$SN" == 'x' ]
then
	echo "ERROR: enter parameters: n runs p sn"
	exit
fi

DD=Data

"$GEN_DIR"/biased.exe $N $RUN $P > $DD/input

cat "$DD/input" | cut -f 1 > "$DD/orders"
cat "$DD/input" | cut -f 2 > "$DD/tourns"

cat "$DD/tourns" | "$EXE_DIR/sample.py" $SN > "$DD/sampled_tourns"
cat "$DD/sampled_tourns" | cut -f 1 > "$DD/samples"
cat "$DD/sampled_tourns" | cut -f 2 > "$DD/stourns"

ALGS=
ALG_FILES=
NALGS=0 # see fixme below
for alg in "$ALG_DIR"/*
do
  if [ -x $alg -a ! -d $alg ]
  then
    algname=$(basename $alg .exe)
    ALG_FILES="$ALG_FILES $DD/$algname"
    ALGS="$ALGS $algname"
    NALGS=$(expr $NALGS + 1)
    
    echo "running $algname.."
    #cat $DD/stourns | "$alg" $RUN > "$DD/$algname" || exit
    cat $DD/stourns | "$alg" $RUN > "$DD/${algname}"
  fi
done

# FIXME -- add original order in there somehow

echo $ALGS > "$DD/output"

echo "calculating upsets.."
echo "$DD/tourns" "$DD/samples" $ALG_FILES | xargs paste -d"\n" | \
    tee "$DD/all_orders_sampled" | "$EXE_DIR/unsample.exe" $RUN $NALGS | \
 		tee "$DD/all_orders" | "$EXE_DIR/calc_upsets.exe" $RUN $NALGS > "$DD/upsets"

echo | cat "$DD/upsets" - >> "$DD/output"

alg_no=0
while [ $alg_no -lt $NALGS ]
do
  totals[alg_no]=0
  alg_no=$(expr $alg_no + 1)
done

alg_no=0
for line in $(cat "$DD/upsets")
do
  #echo ${totals[alg_no]} $line
  #echo $(echo ${totals[alg_no]} + $line | bc)
  totals[alg_no]=$(echo ${totals[alg_no]} + $line | bc)

  alg_no=$(expr $alg_no + 1)
  if [ $alg_no -eq $NALGS ] 
  then
    alg_no=0
  fi
done

alg_no=0
while [ $alg_no -lt $NALGS ]
do
  echo -n ${totals[alg_no]} "	" >> "$DD/output"
  alg_no=$(expr $alg_no + 1)
done
echo >> "$DD/output"

alg_no=0
while [ $alg_no -lt $NALGS ]
do
  echo -n $(echo 3 k ${totals[alg_no]} $RUN / p | dc) "	" >> "$DD/output"
  alg_no=$(expr $alg_no + 1)
done
echo >> "$DD/output"
