<?php
$str = "【】@￥！；‘。，）-+？：“";
echo preg_replace("/([x80-xff])/","jd",$str);

