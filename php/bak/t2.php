<?php
//$s = '"    {\\n        \\"dwId\\" : 0,\\n        \\"dwRetCode\\" : 0,\\n        \\"sErrMsg\\" : \\"\\"\\n    }"';
$s[] = 1;
$s[] = 2;
$s[] = 3;
$a['vecId'] = $s;

$j = json_encode($a);
print_r("$j\n");


if(!method_exists($a, "json_decode"))
{
    print_r("has json_encode\n");
}
else
{
   print_r("no has json_encode\n");
}   


//$tj = json_decode($a);
//print_r("$tj\n");

if(is_object($s))
{
	print_r("yes\n");
}
else
{
	print_r("no object\n");
}
if(is_string($a))
{
	print_r("is string\n");
}
else
{
	print_r("no string\n");
}
if(is_string($j))
{
	print_r("is string\n");
}
else
{
	print_r("no string\n");
}

$s = "";
if(is_string($s))
{
	print_r("is string\n");
}
$s = "testtesttest";
$s = "test中国";
$r = substr($s, 0, 7);
print_r($r);
print_r("\n");
?>
