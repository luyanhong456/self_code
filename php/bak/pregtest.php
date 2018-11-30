<?php
$a [] = "#%^&*";
//$arr = preg_grep("/[#\%\^&<>'"\/\\\$\*\s]/", $a);
$arr = preg_grep("/[#\%\^&<>'\"\/\\\$\*\s]/", $a);
//var_dump($arr);
print_r($arr);
$i = count($arr);
print_r($i);


$c = count(preg_grep("/[#\%\^&<>'\"\/\\\$\*\s]/", $a));
if($c > 0)
{
	print_r("yes");
}

if( count(preg_grep("/[#\%\^&<>'\"\/\\\$\*\s]/", $a)) > 0)
{
	print_r("yes\n");
}

$s = "#%^&*";
$i = preg_match("/[#\%\^&<>'\"\/\\\$\*\s]/", $s);
print_r("$i\n");

$s = "luyanhong";
$i = preg_match("/[#\%\^&<>'\"\/\\\$\*\s]/", $s);
print_r("$i\n");
?>
