<?php

//$jdurl = "http://img30.360buyimg.com/test/jfs/t34/18/470245886/82224/4e9a990e/58ea6f79N0e934f94.gif";
$jdurl = ".360buyimg.com";
//$jdurlzg = "/^([hH][tT]{2}[pP]:\/\/|[hH][tT]{2}[pP][sS]:\/\/)(([A-Za-z0-9-~]+)\.)([360buyimg.com]\/)(.{1,})(\.[gif])$/";
//$jdurlzg = "/^([hH][tT]{2}[pP]:\/\/|[hH][tT]{2}[pP][sS]:\/\/)([A-Za-z0-9-~]+)(.360buyimg.com\/)(.{1,})$/";
$jdurlzg = "/[.360buyimg.com\/]$/";

$iRet = preg_match($jdurlzg, $jdurl);
echo $iRet;
echo "\n";


?>
