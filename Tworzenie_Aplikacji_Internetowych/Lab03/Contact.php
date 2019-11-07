<?php

class Contact {
    const TRUE = 1;
    const FALSE = 0;

    public $a;
    public $b = 1;

    private $_c = true;

    public function test(){ // to samo co function test()
        echo "test";
    }

    public static function static_test($test){
        echo $test;
    }

    public function __construct()
    {
        
    }
}