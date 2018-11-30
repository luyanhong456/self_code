<?php

$tmpfile = "./test.csv";
 
$oFile = fopen($tmpfile, 'r');
if ($oFile == false)
{
    print_r("read csv file fail, path:[".$tmpfile."]");
}

$row = 0;
$vItemTotal = [];

while ($vItem = fgetcsv($oFile))//按行读入
{
	$row++;
	print_r($vItem);
	$vItem = array_filter($vItem);//过滤空的元素
	print_r($vItem);

	$vItemTotal = array_merge($vItemTotal, $vItem);//数据合并
	

	$vUnique = array_unique($vItemTotal);//去重复

	$count1 = count($vItemTotal);
	$count2 = count($vUnique);

	if($count1 != $count2)
	{
		print_r("have repeate word\n");
	}
}

$jimdbvalue = "";
foreach ($vItemTotal as $value) 
{
	$jimdbvalue .= $value;
	$jimdbvalue .= "@";
}

$content = json_encode($vItemTotal, JSON_UNESCAPED_UNICODE);//JSON_UNESCAPED_UNICODE避免中文被Unicode

print_r("content:$content\n");
print_r("jimdbvalue:$jimdbvalue\n");
print_r("row=$row\n");
?>
