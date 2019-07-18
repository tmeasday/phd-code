#!/bin/bash -

DD=Temp
ED=Execute

n="$1"

if [[ -e "$DD/orders" ]]; then rm "$DD/orders"; fi
	
cp "Algorithms/algorithms" "$DD/algorithms"

echo -n "randomizing.."
"$ED/sample.py" $n | cut -f 2 | "$ED/randomize.exe" > "$DD/tourns"
echo "done"
	
"$ED/do_run.sh"