<?php
$sql = "select * from aa where id=:id and name=:name";
$param[":id"] = 1;
$param[":name"] = "test";

$patterns = array_keys($param);
echo "patterns_1:".var_export($patterns, true)."\n\n";

$patterns = array_map(function($v){return "/$v/";}, $patterns);
//echo "patterns_2:".print_r($patterns)."\n\n";//print_r返回值为1
echo "patterns_2:".var_export($patterns, true)."\n\n";

$param_t = array_map(function($v){if(gettype($v) == "string"){return "'$v'";}else{return $v;}}, $param);
//print_r($param);
echo "param_t:".var_export($param_t, true)."\n\n";

$sql_t = preg_replace($patterns, $param_t, $sql);

$i = print_r($sql_t);
echo "\n";
echo $i."\n";
?>
