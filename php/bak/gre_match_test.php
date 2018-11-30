<?php
$test = "1234@qq.11com";
//$test = "1234xqq.com";
$result = 1;


//$zhengze = '/^[a-zA-Z0-9][a-zA-Z0-9._-]*\@[a-zA-Z0-9]+\.[a-zA-Z0-9\.]+$/A';
$emailzg = '/^[a-zA-Z0-9._-]*\@[a-zA-Z0-9]+\.[a-zA-Z]{2,4}$/A';
//$email = "[\w!#$%&'*+/=?^_`{|}~-]+(?:\.[\w!#$%&'*+/=?^_`{|}~-]+)*@(?:[\w](?:[\w-]*[\w])?\.)+[\w](?:[\w-]*[\w])?";
//$result = preg_match($zhengze,$test);
$result = preg_match($emailzg, $test, $matches);
print_r($matches);

print_r($result."\n");

//公司电话
$phonezg = "/^\d{3}-\d{8}|\d{4}-\d{7,8}|^1\d{10}$/";
$phone = "18664953171";
$result = preg_match($phonezg, $phone, $matches);
print_r($matches);

print_r($result."\n");

$mobilephonezg = "/^1\d{10}$/";
$mobile = "18664953171";

$result = preg_match($mobilephonezg, $mobile, $matches);
print_r($matches);

print_r($result."\n");

$test1 = "test.jpg2";

$photozg = "/\.(gif|jpg|jpeg|png|GIF|JPG|PNG)$/";

$iRet = preg_match($photozg, $test1, $matches);

print_r($iRet."\n");



$num = "/^\d+$/";
$test_num = "12324545624362_t";
$result = preg_match($num, $test_num, $matches);
print_r($result."\n");


?>
