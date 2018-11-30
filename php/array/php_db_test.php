<?php

$svc = "mrjxranking-seo";

$svc = trim($svc, "- ");
echo $svc."\n";//mrjxranking-seo

$pos = strrpos($svc, '-');
echo $pos."\n";//11

$name = $pos ? substr($svc, 0, $pos) : $svc;
echo $name."\n";//mrjxranking

$side = $pos ? substr($svc, $pos+1) : "";
echo $side."\n";//seo

$side = "xx";
$svc = ($side !== "") ? "{$name}.db.mysql-$side" : "${name}.db.mysql";
echo $svc."\n";//mrjxranking.db.mysql-seo

?>