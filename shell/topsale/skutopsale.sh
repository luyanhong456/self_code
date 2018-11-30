#!/bin/sh

filepre="jd-sku-topsale"
filedir="/data0/mrjx/topsale/data/"
if [ -d $filedir ]
then
    echo yes
else
    mkdir -p $filedir
fi
file=""

tablename="app.app_adsz_m_dmp_jd_topsale_day"

function insertTopsale1
{
    if [ $# -eq 0 ]
    then
        dt=`date '+%Y-%m-%d' --date="-1 day"`
        dt7=`date '+%Y-%m-%d' --date="-7 day"`
        filedt=`date '+%Y%m%d' --date="-1 day"`
    else
        dt=$1
        dt_timestamp=`date -d "$1" '+%s'`
        timestamp=`expr ${dt_timestamp} - 86400 \* 6`
        dt7=`date -d @${timestamp} '+%Y-%m-%d'`
        filedt=`echo ${dt} | awk -F'-' '{print $1$2$3}'`
    fi

    #7天商品销量情况
    hql="insert overwrite table $tablename partition(dt='$dt',del_flag=1) 
        select item_sku_id,item_name,item_first_cate_cd,item_first_cate_name,item_second_cate_cd,item_second_cate_name,item_third_cate_cd,item_third_cate_name, sumsale
        from
        (
           select item_sku_id,item_name,item_first_cate_cd,item_first_cate_name,item_second_cate_cd,item_second_cate_name,item_third_cate_cd,item_third_cate_name, sum(valid_ord_qtty) as sumsale
            from
            gdm.gdm_sku_pur_attrib
            where dt<='$dt' and dt>='${dt7}' and free_goods_flag=0 and sale_flag=1
            group by item_sku_id,item_name,item_first_cate_cd,item_first_cate_name,item_second_cate_cd,item_second_cate_name,item_third_cate_cd,item_third_cate_name having sum(valid_ord_qtty)>0
        )t1"

    hive -e "set mapred.output.compress=true;set hive.exec.compress.output=true;set mapred.output.compression.codec=com.hadoop.compression.lzo.LzopCodec;$hql" 

}

function bak
{
    if [ $# -eq 0 ]
    then
        dt=`date '+%Y-%m-%d' --date="-1 day"`
        dt7=`date '+%Y-%m-%d' --date="-7 day"`
        filedt=`date '+%Y%m%d' --date="-1 day"`
    else
        dt=$1
        dt_timestamp=`date -d "$1" '+%s'`
        timestamp=`expr ${dt_timestamp} - 86400 \* 6`
        dt7=`date -d @${timestamp} '+%Y-%m-%d'`
        filedt=`echo ${dt} | awk -F'-' '{print $1$2$3}'`
    fi

    file="${filedir}/${filepre}_${filedt}"

    #7天销量大于10的top1W个商品
    hql="insert overwrite table $tablename partition(dt='$dt',del_flag=1) 
        select item_sku_id,item_name,item_first_cate_cd,item_first_cate_name,item_second_cate_cd,item_second_cate_name,item_third_cate_cd,item_third_cate_name, sumsale
        from
        (
            select item_sku_id,item_name,item_first_cate_cd,item_first_cate_name,item_second_cate_cd,item_second_cate_name,item_third_cate_cd,item_third_cate_name,sumsale,row_number(item_third_cate_cd) as rn, free_goods_flag,sale_flag
            from
            (
                select * 
                from
                (
                    select item_sku_id,item_name,item_first_cate_cd,item_first_cate_name,item_second_cate_cd,item_second_cate_name,item_third_cate_cd,item_third_cate_name, sum(valid_ord_qtty) as sumsale,free_goods_flag,sale_flag 
                    from 
                    gdm.gdm_sku_pur_attrib where dt<='$dt' and dt>='${dt7}'
                    group by item_sku_id,item_name,item_first_cate_cd,item_first_cate_name,item_second_cate_cd,item_second_cate_name,item_third_cate_cd,item_third_cate_name,free_goods_flag,sale_flag  having sum(valid_ord_qtty)>=10
                )t0
                where free_goods_flag=0 and sale_flag=1 
                sort by item_third_cate_cd desc, sumsale desc
            )t1
        )t2
        where rn<=10000 sort by sumsale desc"

    hive -e "set mapred.output.compress=true;set hive.exec.compress.output=true;set mapred.output.compression.codec=com.hadoop.compression.lzo.LzopCodec;$hql" 


}

#两个分区做比较，在昨天分区的，不在今天分区的，那么就是删除的数据,del_flag=2
function insertTopsale2
{
    if [ $# -eq 0 ]
    then
        dt=`date '+%Y-%m-%d' --date="-1 day"`
        dt2=`date '+%Y-%m-%d' --date="-2 day"`
    else
        dt=$1
        dt_timestamp=`date -d "$1" '+%s'`
        timestamp=`expr ${dt_timestamp} - 86400`
        dt2=`date -d @${timestamp} '+%Y-%m-%d'`
    fi
    hql="insert overwrite table $tablename partition(dt='$dt',del_flag=2)
        select y.item_sku_id,y.item_name,y.tem_first_cate_cd,y.item_first_cate_name,y.item_second_cate_cd,y.item_second_cate_name,y.item_third_cate_cd,y.item_third_cate_name, y.salenum
        from
        (select * from app.app_adsz_m_dmp_jd_topsale_day where dt='$dt2' and del_flag=1)y
        left outer join
        (select * from app.app_adsz_m_dmp_jd_topsale_day where dt='$dt' and del_flag=1)t
        on
        (y.item_sku_id=t.item_sku_id)
        where t.item_sku_id is null" 

    hive -e "set mapred.output.compress=true;set hive.exec.compress.output=true;set mapred.output.compression.codec=com.hadoop.compression.lzo.LzopCodec;$hql" 

}

function getTopsale
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
    
    hql="select item_sku_id, del_flag, salenum from $tablename where dt='$dt' and salenum>=10 sort by salenum desc
        "

    hive -e "set mapred.reduce.tasks=500;$hql" > $file

    echo "[`date '+%Y-%m-%d %H:%M:%S'`] begin getTopsale, uploadjss file: ${file}"
    ret=`./uploadjss ${file}`
    echo $ret
    rm ${file}
    echo "[`date '+%Y-%m-%d %H:%M:%S'`] end getTopsale, uploadjss file: ${file}"
    
}

function main
{
    insertTopsale1 $1
    insertTopsale2 $1
	getTopsale $1 
}

main $1
echo "[`date '+%Y-%m-%d %H:%M:%S'`] end skutopsale"





