#/bin/sh

# BEGIN BLOCK
BEGIN{
    print "BEGIN SORT"
}


##############################################
# NR - 行数
# NF - 字段数
# $0 - 就表示一个记录，$1表示记录中的第一个字段 
# $1,$2 - the first column as $1, the second column as $2
##############################################


NR==1 {  # 命令行数==1
    for (i=1; i<=NF; i++) #NF文本列数
    { 
		if ( $i == fldName ) 
		{ 
		    fldNr = i #fldName，排序字段，按第几列排序
		} 
    }
    head = $0  # $0就表示一个记录，$1表示记录中的第一个字段
    next       # continue    
}

#循环赋值到values，将fldNr排序字段赋值到最前面，%32的格式化，相对应的接下来的substr的索引长度
{ values[NR] = sprintf("%32s%s", $fldNr, $0)}


# END BLOCK
# awk内置排序函数 asort是对数组的值进行排序，而asorti是对数组的下标进行排序。
END { 
    n = asort(values) # sorts the content of values and return the number of elments in the array source, the default order is ascending.)
	#print n
    print head 
    if( "desc" == order)
    {
		for (i=n; i>=1; i--) 
		{ 
			#print values[i]
		    print substr(values[i],33)  # substr awk中截取字段，substr(value, index, length)
		} 
    }
    else
    {
		for (i=1; i<=n; i++) 
		{ 
			#print values[i]
		    print substr(values[i],33)  
		} 
    }
} 

#gawk -v fldName="Salary" -v order="" -f tst.awk file