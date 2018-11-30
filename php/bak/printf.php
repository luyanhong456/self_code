<?php
printf("%.2f%%\n", 1/3*100);

$num = 1/3;
echo "$num%\n";

$num1 = round(1/3*100, 5);

echo "$num1\n";
$num2 = sprintf("%.2f%%", 1/3*100);
echo "$num2\n";
?>
