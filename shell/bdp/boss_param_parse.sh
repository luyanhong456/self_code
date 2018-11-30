#!/bin/sh
#=============================
#@���� ���в����滻
#��YYYYMMDD ��YYYY-MM-DD�滻Ϊ����ʱ��
#param: 1)�����õ�jason �ַ���
#		2)����ʱ�����ʱ��֮��Ĳ�ֵ����λΪСʱ		
#		3)���������õ�����ʱ��
#ֻ��һ����ʱĬ��Ϊ��һ��Сʱ
#
# update: ����֧�����������ʽ(YYYYMM��YYYY-MM) by duanhui6@20151215 am
#
#=============================
#path=`dirname $0`
#/. $path/boss_lz_common.sh
echo $#
echo $1
echo $2

if [ $# -lt 2 ];then
    date_str_ym="-1 months"
	date_str_ymd="-1 hours"
	date_str_hour="-1 hours"	
	echo "#=$#  $date_str_ym   ${date_str_ymd}   ${date_str_hour}"

else
	if [[ ! "$2" =~ ^[0-9]{1,}$ ]];then
        echo $2
		echo "usage: json hours YYYYMMDDHH:$*"
		#SMSG "boss_param param error:$*"
		exit 1
	fi
	if [ $# -ge 3 ];then
		#���ݵ�����ʷ�����߼���YYYYMMDD��ʽ
		echo $1
		echo $2
		echo $3
        tmp_date_str="$3"
		echo $tmp_date_str

        if [[ "${tmp_date_str}" =~ ^[0-9]{8}$ ]]; #YYYYMMDD
        then
            tmp_date_str="${3}00"
        elif [[ "${tmp_date_str}" =~ ^[0-9]{6}$ ]]; #YYYYMM
        then
            tmp_date_str="${3}0100"
        fi
		echo $tmp_date_str

		if [[ ! "$tmp_date_str" =~ ^[0-9]{10,}$ ]];then
			echo "usage: the format of time cound be YYYYMM, YYYYMMDD or YYYYMMDDHH"
			exit 1
		fi

		date_str=${tmp_date_str}
        date_str_ym="${date_str:0:6}01"
		date_str_ymd="${date_str:0:8}"
		date_str_hour="${date_str_ymd} ${date_str:8:2}"
	
	else
        date_str_ym="-$2 months"
		date_str_ymd="-$2 hours"
		date_str_hour="-$2 hours"
    fi
	
fi

YYYYMM=`date -d"${date_str_ym}" +"%Y%m"`
YYYYMMDD=`date -d"${date_str_ymd}" +"%Y%m%d"`
YYYYMMDDHH=`date -d"${date_str_hour}" +"%Y%m%d%H"`

YYYY_MM=`date -d"${date_str_ym}" +"%Y-%m"`
YYYY_MM_DD=`date -d"${date_str_ymd}" +"%Y-%m-%d"`
YYYY_MM_DD_HH=`date -d"${date_str_hour}" +"%Y-%m-%d-%H"`

PARAM=$1
echo "after parse1: ${PARAM}"
echo

PARAM=${PARAM//\$\{YYYYMM\}/$YYYYMM}
PARAM=${PARAM//\$\{YYYYMMDD\}/$YYYYMMDD}
PARAM=${PARAM//\$\{YYYYMMDDHH\}/$YYYYMMDDHH}

PARAM=${PARAM//\$\{YYYY-MM\}/$YYYY_MM}
PARAM=${PARAM//\$\{YYYY-MM-DD\}/$YYYY_MM_DD}
PARAM=${PARAM//\$\{YYYY-MM-DD-HH\}/$YYYY_MM_DD_HH}

echo "after parse: ${PARAM}"

#/home/jd_ad/task/jdw2file/sql2f.sh $PARAM
