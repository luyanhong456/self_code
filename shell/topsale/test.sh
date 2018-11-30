#!/bin/sh
total=0

function test
{
	total= $[$1 + $total]
}

cat $1 | while read num
do
	total=$[$num + $total]
#	echo $total
done
echo $total
