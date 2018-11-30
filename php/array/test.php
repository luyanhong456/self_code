<?php

$t = array('5','4');

if(in_array('4', $t))
{
	echo "yes\n";
}
else
{
	echo "no\n";
}



$vClassid3Price = array();
$vClassid3Price['1'] = 18;
$vClassid3Price['2'] = 28;
$vClassid3Price['3'] = 38;
$vClassid3Price['4'] = 48;
$vClassid3Price['5'] = 58;


$s = json_encode($vClassid3Price);
echo "$s\n";


$v = json_decode($s);
print_r($v);

foreach($v as $c=>$p)
{
	echo "$c, $p\n";
}
?>
