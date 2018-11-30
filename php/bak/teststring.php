<?php
class B {
  public function __toString() {
    return "Instances of B() can be treated as a strings!\n";
  }
}  

$b = new B();
print($b); //Instances of B() can be treated as a strings!
print(is_string($b) ? 'true' : 'false'); //false


echo "\n";
$str = [];
if(empty($str))
{
	echo "yes\n";
}
else
{
	echo "false\n";
}

?>
