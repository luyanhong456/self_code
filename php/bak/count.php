<?php
$begintime = time();
echo "$begintime \n";

$file = "jd_user_act_info_20160601_2";
//$file = "test";
$line = 0;
$conent = "\n";

$fp = fopen($file, 'a');
fwrite($fp, $conent);
fclose($fp);

$fp = fopen($file, 'r') or die("open file failure!");

while (!feof($fp)) 
{
	$str =  trim(fgets($fp));
	if(strlen($str))
	{
		$line++;
	}
}
/*
while(stream_get_line($fp, 1024, "\n"))
{
	$line++;
}
*/
fclose($fp);
echo "$line \n";
$endtime = time();
echo "$endtime \n";

?>
