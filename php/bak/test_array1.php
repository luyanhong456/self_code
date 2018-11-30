<?php
$arr = array(array("a"=>1, "b"=>2), array("c"=>3));
$str = "";
$str1 = "";

foreach ($arr as $key_a => $eacharr) 
{
	//方法2
	$str1 = $str1 . implode(",", $eacharr).",";

	foreach ($eacharr as $key => $value) 
	{
		echo "$key => $value \n";
		//方法1
		$str = $str . $value.",";
		
	}
}
echo "str=$str \n";
$str = substr($str, 0, strlen( $str)-1);
echo "after, str=$str \n";

echo "str1=$str1\n";
$str1 = substr($str1, 0, strlen( $str1)-1);
echo "after, str=$str1 \n";
?>