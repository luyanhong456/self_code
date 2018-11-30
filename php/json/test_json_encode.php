<?php

//$typearray = array(0=>"标题&描述", 1=>"素材文案", 2=>55);


$typearray = array('uin'=>"标题&描述", 'pin'=>"素材文案", 'uint1'=>55);

print_r($typearray);

$s = json_encode($typearray);

print_r($s);


?>
