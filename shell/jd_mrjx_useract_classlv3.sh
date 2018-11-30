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
tablename="app.app_adsz_m_dmp_jd_mrjxranking_classlv3_day"

function insertData
{
    if [ $# -eq 0 ]
    then
        dt=`date '+%Y-%m-%d' --date="-1 day"`
    else
        dt=$1
    fi

    hql="set hive.merge.mapredfiles = true;
    set hive.merge.smallfiles.avgsize=16000000;
    set mapreduce.job.max.split.locations=1800;
    insert overwrite table $tablename partition(dt='$dt') 
    select jdpin,concat_ws(' ',collect_set(item_third_cate_cd))
    from
    (
        select t2.jdpin,t2.item_third_cate_cd
        from
        (
            select jdpin 
            from
            (
                select jdpin,count(jdpin) as cn 
                from
                (
                    select distinct jdpin,item_third_cate_cd 
                    from 
                        app.app_szad_m_dmp_user_basic_info_day
                    where dt='$dt' and (jdpin is not null or jdpin <>'') and (item_third_cate_cd is not null or item_third_cate_cd <>'')
                )a
                group by jdpin
            )b
            where cn>1
        )t1
        left join
        (
            select distinct jdpin,item_third_cate_cd
            from 
                app.app_szad_m_dmp_user_basic_info_day
            where dt='$dt' and (jdpin is not null or jdpin <>'') and (item_third_cate_cd is not null or item_third_cate_cd <>'')
        )t2
        on
        (t1.jdpin=t2.jdpin)
    )t3
    group by jdpin 
    "
    hive -e "set mapred.output.compress=true;set hive.exec.compress.output=true;set mapred.output.compression.codec=com.hadoop.compression.lzo.LzopCodec;$hql" 
    echo $hql
}


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

    hql="
    select 
        concat_ws(' ',
            if(split(classlv3,' ')[0] is not null, split(classlv3,' ')[0],''),
            if(split(classlv3,' ')[1] is not null, split(classlv3,' ')[1],''),
            if(split(classlv3,' ')[2] is not null, split(classlv3,' ')[2],''),
            if(split(classlv3,' ')[3] is not null, split(classlv3,' ')[3],''),
            if(split(classlv3,' ')[4] is not null, split(classlv3,' ')[4],'')
        )
        from
            (select distinct classlv3 from ${tablename} where dt='$dt')a
    "

	hive -e "$hql" > $file
    echo $hql
    
	gzip -c $file > ${gzfile}

	md5str=`md5sum ${gzfile}`

    echo $md5str | awk -F'/' '{print $1,$NF}' | awk '{print $1,$2}' > ${checkfile}

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
    insertData $1
    getData $1
}
main $1
echo "[`date '+%Y-%m-%d %H:%M:%S'`] end getclasslv3.sh"