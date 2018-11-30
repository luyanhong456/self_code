<?php

//$urlPregMatch1 = "/^https?:\/\/img\d{1,2}\.360buyimg\.com/";
//$urlPregMatch = "/^https?:\/\/img\d{1,2}\.360buyimg\.com\//";
//$urlPregMatch = "/\bindex\.php\b/i";
$urlPregMatch = "/^https?:\/\/img\d{1,2}\.360buyimg\.com\/[0-9a-zA-Z\/]+\.(jpg|png|gif|bmp)\b/i";


$url="http://img99.360buyimg.COM/ppexp/jfs/t6307/41/2540999653/253452/cbddbc3/5963a49fN79fdb3ef.PNG.index.php?sign=5e34294f2bdbc8067f0f243e2c9d9b2a/index.php";
//$url="http://a.vpimg3.com/upload/flow/2018/09/06/135/15362225955730.jpg";
//$url = "http://img1.360buyimg.com.uxuuu.cn/imgs/360buyimg.php?.png?sign=fsdfsrgd8gdtr8781asr969dt11";
//$url="Index.php";

$iRet = preg_match($urlPregMatch, $url, $matches);
print_r($iRet."\n");
print_r($matches);

?>
