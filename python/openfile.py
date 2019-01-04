#!/usr/bin/env python
#coding=utf-8[]
import sys
today_file=str(sys.argv[1])
yestoday_file=str(sys.argv[2])

class Transmit:
    def __int__(self):
        self.today=set(open(today_file).readlines())
        self.yestoday=set(open(yestoday_file).readlines())
    def Difference(self):
        print today
        try:
            difTY=self.today.difference(self.yestoday)
            inTodayNotInYestoady=open('inTodayNotInYestoady','w')
            for each_line in difTY:
                try:
                    print (each_line,)
                    print >>inTodayNotYestoday,each_line,
                except:
                    pass
            inTodayNotYestoday.close()
        except:
            print('The file is not find')
if __name__=="__main__":
    Transmit().Difference()

