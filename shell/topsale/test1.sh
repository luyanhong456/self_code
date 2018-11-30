#!/bin/sh
total=0

function test
{
	total= $[$1 + $total]
}

 while read num
do
	total=$[$num + $total]
#	echo $total
done < $1
echo $total
