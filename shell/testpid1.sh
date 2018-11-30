#!/bin/sh

cat test.txt| while read num
do
	ps -ef |grep "$0" | grep -v grep
	echo
done
