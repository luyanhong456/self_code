<?php
$str="今天";
$s=json_encode($str);
echo $s;
$t = urldecode(json_encode(urlencode($str)));
echo $t;
?>
