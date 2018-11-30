<?php
//$arr = array(array('campaignId'=>2016091913465),'campaignId'=>2016091913465);
$arr = array(2016091913465, 2016091913465);
echo json_encode($arr);
echo "\n";

$s = implode(",", $arr);
echo $s;
echo "\n";
$where = "campaignId in (".$s.")";
echo $where;
echo "\n";


?>
