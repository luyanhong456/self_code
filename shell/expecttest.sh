#!/usr/bin/expect -f
set timeout 5
spawn ssh boss@172.16.150.27

expect "*assword*"
send "Boss@idc\r"
expect "$"
send "ifconfig\r"
expect eof