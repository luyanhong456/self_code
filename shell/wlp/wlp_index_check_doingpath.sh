#!/bin/sh

BASE="/data/bossapp/wlpindex"
LOG_PATH="$BASE/logs/"
LOG_FILE="${LOG_PATH}$0.log"

#告警
function send_alarm()
{
	localip=$(/sbin/ifconfig  | grep "inet addr" |grep -v "127.0.0.1" | cut -d ":" -f 2 | cut -d " " -f 1 | cut -d "." -f 1-4)
	warngroup="ump/wlp/index/checkfile"
    #LOG "/data/bossapp/monitor/tools/sendalarm $1 ip[${localip}]$2";
    #/data/bossapp/monitor/tools/sendalarm "$warngroup" "ip[${localip}][$*]";
    echo "[`date '+%Y-%m-%d %H:%M:%S'`] $warngroup" "ip[${localip}][$*]" >> $LOG_FILE
}

#监控doing目录是否有积压
function check_doingpath()
{
	doingpath="../data/doing"
	filenum=`ls $doingpath | wc -l`
	if [[ $filenum -gt 300 ]]; then
		warnmsg="doing path filenum[$filenum] over 300"
		send_alarm $warnmsg
	fi
}

#查看/dev/shm目录看使用是否超过70%。避免影响索引主从
function check_dev_shm()
{
	shmuseper=`df /dev/shm/ | tail -n1 | awk '{print $5}'`
	shmuse=${shmuseper%%%}
	if [[ $shmuse -gt 70 ]]; then
		warnmsg="/dev/shm use $shmuseper"
		send_alarm $warnmsg
	fi

	for i in `find /data/bossapp/wlpindex/xp-db-slave/ -name "change*" |xargs ls -t | awk '{if(NR>10) print }'`
	do
		rm $i
		echo "[`date '+%Y-%m-%d %H:%M:%S'`] rm $i" >> $LOG_FILE
	done
}


check_doingpath
check_dev_shm
