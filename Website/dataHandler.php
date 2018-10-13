<?php

ob_start();

$type = $_REQUEST['type'];

//chmod("test.txt",0755);

if (strcmp($type, "set-coordinate") == 0) {
    $coord = $_REQUEST["data"];
    $fo = $_REQUEST['folderName'];
    $f = $_REQUEST['fileName'];
    
    $succ=TRUE;
    if(!is_dir("Data/json/" . $fo)){
        try {
            mkdir("Data/json/" . $fo, 0766, true);
        } catch (Exception $exc) {
            $succ = FALSE;
        }
    }
    
    if($succ){
        $dir = "Data/json/" . $fo . "/coord_" . $f;

        $file = fopen($dir, "w");
        fwrite($file, $coord);
        fclose($file);
    }else{
        echo 404;
    }
    
} else if (strcmp($type, "get-coordinate") == 0) {
    $fo = $_REQUEST['folderName'];
    $f = $_REQUEST['fileName'];

    $dir = "Data/json/" . $fo . "/coord_" . $f;

    if (file_exists($dir)) {
        $file = fopen($dir, "r");
        $json = preg_replace("/\r|\n/", "", fgets($file));
        fclose($file);

        echo '[' . $json . ']';
    } else {
        echo 404; // Code for file does not exists
    }
}else if (strcmp($type, "set-color") == 0) {
    $colData = $_REQUEST["data"];
    $fo = $_REQUEST['folderName'];
    $f = $_REQUEST['fileName'];

    $dir = "Data/json/" . $fo . "/" . $f;
    
    //chmod($dir,0755);

    if (file_exists($dir)) {
        $file = fopen($dir, "w");
        fwrite($file, $colData);
        fclose($file);

        echo 'Save successfully!!';
    } else {
        echo 404; // Code for file does not exists
    }
}

?>
