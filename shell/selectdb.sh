#!/bin/sh
DB="mysql -h172.16.176.16  -P3306 -umyppdb -pWy1gxy2boss! mrjx -A --default-character-set=utf8"
function selectdb
{
	tableno=$[32 * $1]
	tableend=$[32 * ($1 + 1) - 1]
	tablename="UserRelation$1"
	sql="select openid,pin,id,inserttime,lastupdatetime from ${tablename} order by inserttime asc limit 1"
	$DB -N -e "${sql}" >> file
}

for i in `seq $1 $2`;
do
	selectdb $i
done