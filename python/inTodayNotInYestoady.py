#!/bin/sh env python
#coding=utf-8[]
import sys
today_file=str(sys.argv[1])
yestoday_file=str(sys.argv[2])
today=set(open(today_file).readlines())
yestoday=set(open(yestoday_file).readlines())
diff=today.difference(yestoday)
resultfile=open('inTodayNotInYestoday','w')
for each_line in diff:
    try:
        print >> resultfile,each_line,
    except:
        pass
        print ('The file is not find')
    resultfile.close()
    
