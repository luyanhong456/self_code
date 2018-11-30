#!/bin/sh

IFS="X"

str="aXbXcX"

for i in $str
do
	echo $i
done


$*='cXdX'
