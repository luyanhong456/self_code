#!/bin/sh
#管道触发两个子进程执行"|"两边的程序；而重定向是在一个进程内执行 
sum=0
cat test.txt | while read num
do
	sum=$(($sum+$num))
done
echo $sum


sum=0
while read num
do
	sum+=$num
done < test.txt
echo $sum

sum=0
while read num
do
	sum=$(($sum+$num))
done < test.txt
echo $sum
