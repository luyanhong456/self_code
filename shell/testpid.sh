#!/bin/sh


function testpid()
{
	ps -ef |grep $0 | grep -v grep
	echo 
}



function testpid1()
{
	ps -ef |grep "test" | grep -v grep
#	msg=`ps -ef |grep "$0" | grep -v grep`
	#echo $msg
	msg=`date +%s`
	echo
}

cat test.txt | while read num
do
	testpid1 $num
done
