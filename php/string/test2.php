<?php

$str = '[{"code":1,"data":{"companyName":[{"name":"上海热浪实业有限公司"},{"name":"test"}],"endDate":"2014-10-29","formatContractId":3848,"formatContractNum":"1","startDate":"2014-10-20"},"msg":"","success":true}]';

$ret = json_decode($str, true);

print_r($ret);

print_r($ret[0]['code']);
foreach($ret as $item)
{
	print_r($item);
}

$str1 = '"[{\\"code\\":1,\\"data\\":{\\"companyName\\":[\\"上海热浪实业有限公司\\"],\\"endDate\\":\\"2014-10-29\\",\\"formatContractId\\":3848,\\"formatContractNum\\":\\"1\\",\\"startDate\\":\\"2014-10-20\\"},\\"msg\\":\\"\\",\\"success\\":true}]"';

var_dump($str1);

$ret_1 = json_decode($str1, true);
var_dump($ret_1);

$ret_2 = preg_replace('/\\\\/', '', $ret_1);

var_dump($ret_2);



?>
