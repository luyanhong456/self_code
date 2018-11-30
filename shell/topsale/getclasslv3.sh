#!/bin/sh
filepre="jd-useract-classlv3"
filedir="/data0/mrjx/useractclasslv3/"
if [ -d $filedir ]
then
    echo yes
else
    mkdir -p $filedir
fi

file=""
gzfile=""
checkfile=""

function getData
{
	if [ $# -eq 0 ]
    then
        dt=`date '+%Y-%m-%d' --date="-1 day"`
        filedt=`date '+%Y%m%d' --date="-1 day"`
    else
        dt=$1
        filedt=`echo ${dt} | awk -F'-' '{print $1$2$3}'`
    fi

	file="${filedir}/${filepre}_${filedt}"
	gzfile="${file}.gz"
	checkfile="${file}.check"

	hql="select concat_ws(' ',collect_set(item_third_cate_cd))
		from
		(select distinct jdpin, item_third_cate_cd from app.app_szad_m_dmp_user_basic_info_day where dt='$dt' and (jdpin is not null or jdpin <>'') and (item_third_cate_cd is not null or item_third_cate_cd <>'') )a
		group by jdpin"

	hive -e "set mapred.reduce.tasks=500;$hql" > $file

	gzip -c $file > ${gzfile}

	md5sum ${gzfile} > ${checkfile}

	echo "[`date '+%Y-%m-%d %H:%M:%S'`] uploadjss file: ${gzfile}"
    ./classlv3jss ${gzfile}

    echo "[`date '+%Y-%m-%d %H:%M:%S'`] uploadjss file: ${checkfile}"
    ./classlv3jss ${checkfile}

    rm ${file}
    rm ${gzfile}
    rm ${checkfile}
    echo "[`date '+%Y-%m-%d %H:%M:%S'`] end getData"
}


function main
{
    getData $1
}
main $1
echo "[`date '+%Y-%m-%d %H:%M:%S'`] end getclasslv3.sh"