<?php

$tmpfile = "./test.xlsx";
 
$oFile = fopen($tmpfile, 'r');
if ($oFile == false)
{
    print_r("read csv file fail, path:[".$tmpfile."]");
}

$vItemTotal = [];

$vItem = array("a", "b", "c");

$vItem1 = array("xxa", "b", "caa");

$vItem2 = array("aa", "b", "cxx");

$vItem3 = array("acc", "bxx", "c");

$vItemTotal = array_merge($vItem, $vItem1);

print_r($vItemTotal);
$vUnique = array_unique($vItemTotal);
print_r($vUnique);

$count1 = count($vItemTotal);
$count2 = count($vUnique);

if($count1 != $count2)
{
	print_r("have repeate word\n");
}

$content = json_encode($vItemTotal);
print_r("content:$content\n");

?>
