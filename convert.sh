#!/bin/zsh

while read NUM1	NUM2
do
if (( NUM1 < 0 )); then
	((NUM1--))
else
	((NUM1++))
fi
if (( NUM2 < 0 )); then
	((NUM2--))
else
	((NUM2++))
fi
echo $NUM1 $NUM2
done < $1