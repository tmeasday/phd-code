for (( i = 1; i < 51; i++ )); do   ./intersect.py ranks/$i/* >
  intersection; mkdir rank-intersect/$i; for file in ranks/$i/*; do grep
  --file=intersection $file > rank-intersect/$i/$(basename $file); done;
done
