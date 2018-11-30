#!/bin/sh

BASE="/data/bossapp/wlpindex"
LOG_PATH="$BASE/logs/"
LOG_FILE="${LOG_PATH}$0.log"

TODO_PATH="$BASE/data/todo"
DOING_PATH="$BASE/data/doing/"

#检查目录是否存在

[ ! -d $TODO_PATH ] && mkdir $TODO_PATH
[ ! -d $DOING_PATH ] && mkdir $DOING_PATH

TIME_MIN=`date +%Y%m%d%H%M`


function mv_incr()
{
	for i in $TODO_PATH/index_incr* ;do
		if test -f $i;then
		    ft=`basename "$i" | awk -F'_' '{print $3}'`;
		    if [ $ft -lt $TIME_MIN ];then
		            mv $i $DOING_PATH
		    fi;
		fi
	done
}

#告警
function send_alarm()
{
	localip=$(/sbin/ifconfig  | grep "inet addr" |grep -v "127.0.0.1" | cut -d ":" -f 2 | cut -d " " -f 1 | cut -d "." -f 1-4)
	warngroup="ump/wlp/index/checkfile"
    #LOG "/data/bossapp/monitor/tools/sendalarm $1 ip[${localip}]$2"
    #/data/bossapp/monitor/tools/sendalarm "$warngroup" "ip[${localip}][$*]"
    echo "$warngroup" "ip[${localip}][$*]" >> $LOG_FILE
}

function mv_entire()
{
	entirefile=`ls $TODO_PATH | grep index_entire | head -n1`
	entirefile=$TODO_PATH/$entirefile

	if [ -f $entirefile ]; then
		dt=`date '+%Y-%m-%d'`
		checkfile="check_entire_file_${dt}"
		
		#如果当天的全量checkfile存在，则说明之前来过一次全量文件了，现在又来，则判断这次来的文件的accesstime($12)和checkfile最新的修改的时间($15)是否相差4个小时
		#如果相差4个小时，则可以说明，和上一次的全量文件不同，是第二次的全量文件
		#如果当天的全量checkfile不存在，则说明今天还没有检测到全量文件，则判断全量文件的数据时间
		if [ -f $checkfile ]; then

			lastcheckdate=`stat -t ${checkfile} |awk '{print $15}'`
			entirefiledate=`stat -t ${entirefile} |awk '{print $12}'`

			ft=$(( $entirefiledate - $lastcheckdate ))

			if [[ $ft -gt 14400 ]]; then
				updatetime=`head -n1 $entirefile | awk -F'\t' '{print $4}'`
				dt=`date -d @${updatetime} '+%Y-%m-%d %H:%M:%S'`
				echo "$entirefile $updatetime" >> $checkfile
				warnmsg="entire file come twice over 4 hours, datatime[$dt], pls check"
				send_alarm $warnmsg
			fi
		else
			updatetime=`head -n1 ${entirefile} | awk -F'\t' '{print $4}'`
			nowtime=`date '+%s'`
			ft=$(( $nowtime - $updatetime ))

			dt=`date -d @${updatetime} '+%Y-%m-%d %H:%M:%S'`

			if [[ $ft -gt 7200 ]]; then
				warnmsg="entirefile is comming, but datatime[$dt] over 2 hours, pls check"
				echo "$entirefile $updatetime" >> $checkfile
				send_alarm $warnmsg
			else
				warnmsg="entirefile is comming, datatime[$dt]"
				echo "$entirefile $updatetime" >> $checkfile
				send_alarm $warnmsg
			fi
		fi
	fi

	for i in $TODO_PATH/index_entire* ;do
		if test -f $i;then
		    ft=`basename "$i" | awk -F'_' '{print $3}'`;
		    if [ $ft -lt $TIME_MIN ];then
		            mv $i $DOING_PATH
		    fi;
		fi
	done
}

mv_incr
mv_entire
