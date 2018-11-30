<?php

$file = "test_LL";

$cmd = "sed -i 's/[A-Z]/\\L&/g' $file";

system($cmd);

printf("s\n");
?>
