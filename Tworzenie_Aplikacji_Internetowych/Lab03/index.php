<?php

require "Contact.php";

$cos = 5;
$a = "swinka morska";
$b = "kotek malutki";

$tab = [1, 2, 3, 4, "cos" => "Ala ma kota"];

print_r($tab);

echo "<hr>";

echo $tab["cos"];

if($b){
    echo $b;
}elseif ($a) {
    echo "siabadadaba";
}

echo "<hr>";

$cond = 8;
switch ($cond) {
    case 1:
        echo "To jest 1<br>";
        break;
    case 2:
        echo "To jest 2<br>";
        break;
    case 3:
        echo "To jest 3<br>";
        break;
    case 4:
        echo "To jest 4<br>";
        break;
    default:
        echo "nic<br>";
        break;
}

for($i = 0; $i < 10; $i++){
    echo $i . ' ';
}

echo "<br>";

foreach ($tab as $key => $value) {
    echo $key . ' => ' . $value . "<br>";
}

echo "<br>";

foreach($tab as $v){
    echo $v . "<br>";
}

echo "<br>";

function item(&$array, $index = 1, $value){
    $array[$index] = $value;
    return $array;
}

print_r($tab);
item($tab, 0, $value=10);
echo "<hr>";
print_r($tab);

echo "<br><br>";

Contact::static_test("Witam, przyjacielu!");

$contact = new Contact();

echo "<br>";

// $contact->test();

if(Contact::TRUE){
    $test = new Contact();
    $test->test();
}

?>

<?php

phpinfo();
