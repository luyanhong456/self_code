<?php
$file="1351.txt";
//在文件的最后一行添加一个换行符，add by luyanhong 2016/7/18
$content = "\n";
if (is_writable($file))
{
	$fp = fopen($file, 'a');
	if (fwrite($fp, $content) === FALSE)
	{
		printf("write failed filename:$file");
	}

	printf("write success filename:$file\n");
	fclose($fp);
}

//做一下dos2unix转换
system("dos2unix $file");
printf("system dos2unix ".$file."\n");

//转换mac下的换行符
$cmd = "sed -i 's/\\r/\\n/g' $file";
system($cmd);
printf("system sed ".$file."\n");

$line = 0;
if (is_readable($file))
{
        $fp = fopen($file, 'r');
        while (!feof($fp)) 
        {
                $str =  trim(fgets($fp));//这里做trim的主要原因是因为害怕文件中有空行的
                if(strlen($str))
                {
                        $line++;
//                      echo "$line\n";
                }
        }

        fclose($fp);
}

echo "line = $line \n";
?>
