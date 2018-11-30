<?php

$field = '5xx_1';


$i = strpos($field, '_');

echo "$i\n";

$s = substr($field, 0, strpos($field, '_'));
echo "$s\n";



echo time();


$oFilter = array();
$oFilter["page"]='t';

$page = 1;
if(isset($oFilter["page"]))
{
    $page = intval($oFilter['page']);
	if($page <=0)
	{
		$page = 1;
	}
}
$num = 50;
$offset = ($page-1)*$num;


echo "\nnum=$num, offset=$offset \n";


$arr = array("blue","red","green","yellow");
$test = array();
$test =   str_replace("red","pink",$arr,$i);
echo "替换数：$i";
print_r($test);


$value = "屌丝";
$s_1 = iconv("UTF-8", "GBK//TRANSLIT", $value);
echo "\n\n$s_1";

$s_2 = mb_convert_encoding($value, "GB2312", "UTF-8");
echo "\n\n$s_2";

$s_2 = "t@t";
$s_1 = str_replace("@", "&", $s_2);
echo "$s_1";



?>
