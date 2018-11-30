#!/bin/sh

function getdir
{
	filepre="jd_sku_topsale"
	filedir="/tmp/data/$filepre"
	if [ -d $filedir ]
	then
		echo yes
	else
		mkdir -p $filedir
	fi
	dt="111"
    file="${filedir}/${filepre}_${dt}"
	
	echo "$filepre   $filedir    $file   $file"
	
	echo "test" > $file
}
getdir
