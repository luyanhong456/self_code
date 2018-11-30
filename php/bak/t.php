<?php
$a = "q";
if(empty($a))
{
	print_r("empty\n");
}
else
{
	print_r("no empty\n");
}

$q = 'b';

if(is_numeric($q))
{
	print_r("is_numeric\n");
}
else
{
	print_r("no is_numeric\n");
}

$s = '1,2,3';
if(empty($s))
{
	print_r("empty\n");
}
else
{
	print_r("no empty\n");
}

if(is_numeric($s))
{
	print_r("is_numeric\n");
}
else
{
	print_r("no is_numeric\n");
}

$c = "中国xx";
print_r(strlen($c));
print_r("\n");

$str = "中国111php编程";
$i = preg_match("/^[\x4e00-\x9fa5]+$/",$str);
print_r("$i\n");

if ($i) 
{
print("该字符串全部是中文\n");
}
else 
{
print("该字符串不全部是中文\n");
}

$rep = preg_replace("/[\x{4e00}-\x{9fa5}]/u", 'jd', $str, -1, $count);
var_dump($rep);
print_r("$rep\n");
print_r("$count\n");
echo strlen($rep);
echo (preg_replace("/^[\x4e00-\x9fa5]+$/", 'jd', $str));

print_r("\n");
echo (preg_replace('/编/', 'jd', $str));

print_r("\n");





$s = "中文+字呐（【@！；‘。，）-“";

$r = preg_replace("/[\x{4e00}-\x{9fa5}]/u", 'jd', $s);

$i = strlen($r);
print_r("$i\n");

?>
