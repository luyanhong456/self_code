#!/bin/sh

#告警
function send_alarm()
{
	localip=$(/sbin/ifconfig  | grep "inet addr" |grep -v "127.0.0.1" | cut -d ":" -f 2 | cut -d " " -f 1 | cut -d "." -f 1-4)
	warngroup="adsz/ump/mrjx/ranking/process"
    echo "[`date '+%Y-%m-%d %H:%M:%S'`] $warngroup" "ip[${localip}][$*]"
    /data/bossapp/monitor/tools/sendalarm "$warngroup" "ip[${localip}][$*]"
    
}

warnmsg="nowtime[$nowdate], entirefile is coming, but datatime[$dt] over 2 hours, pls check"
echo "$entirefile updatetime[$updatetime]" 
send_alarm $warnmsg