<?php
$begintime = time();
echo "$begintime \n";

//$file = "jd_user_act_info_20160601_2";
$file = "test";
$line = 0;
$conent = "\n";

$fp = fopen($file, 'a');
fwrite($fp, $conent);
fclose($fp);

$fp = fopen($file, 'r') or die("open file failure!");
while(!feof($fp))
{
	if($data=fread($fp, 1024*1024*2))
	{
		$num=substr_count($data, "\n");
		$line+=$num;
	}
}
fclose($fp);

echo "$line \n\n";
$line1=0;

$fp = fopen($file, 'r');
while(!feof($fp))
{
	$linechar = fgets($fp, 1024);
	echo strlen($linechar);

	if(strlen($linechar) == 0){continue;}
	echo $linechar;
	$line1++;
	echo "$line1 \n";
}
fclose($fp);
echo "line1 = $line1 \n";
$endtime = time();
echo "$endtime \n";
?>
