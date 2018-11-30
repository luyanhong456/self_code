<?php

$tmpfile = "/home/luyanhong/luyanhong/php/getjdurl/test.jpg";
$content = file_get_contents($tmpfile);
print_r("$content\n");
$finfo = finfo_open(FILEINFO_MIME);
print_r("$finfo======\n");
$mimetype = finfo_file($finfo, $tmpfile);

print_r("$mimetype==========\n");

finfo_close($finfo);

//$tmp = split(';', $mimetype);
//$tmp = split('/', $tmp[0]);

$tmp = explode(';', $mimetype);
$tmp = explode('/', $tmp[0]);
$img_type = $tmp[1];
$ctype = 'Content-Type:image/'.$img_type;
$type = 'type:0';
print_r("$ctype\n");

$content = file_get_contents($tmpfile);

//$img_tmp_name = $_FILES['Filedata']['tmp_name'];
//print_r("$img_tmp_name\n");

//$keycode = 'keycode:'.dechex(crc32($img_tmp_name)).filesize($img_tmp_name).'_';
//根据serPort获取对应的aucode


//TLOG_MSG("授权码： {$AucodeValue}");


?>
