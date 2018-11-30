#!/bin/sh


BASE="/data/bossapp/wlpindex"
TODO_PATH="$BASE/data/todo"
DOING_PATH="$BASE/data/doing/"

#检查目录是否存在

[ ! -d $TODO_PATH ] && mkdir $TODO_PATH
[ ! -d $DOING_PATH ] && mkdir $DOING_PATH

TIME_MIN=`date +%Y%m%d%H%M`

for i in $TODO_PATH/index_incr* ;do
	if test -f $i;then
	    ft=`basename "$i" | awk -F'_' '{print $3}'`
	    if [ $ft -lt $TIME_MIN ];then
	            mv $i $DOING_PATH
	    fi
	fi
done

