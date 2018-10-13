<?php

$fList = scandir("Data/json", 1);
$_f = "[";
foreach ($fList as $file) {

    if (strcmp(".", substr($file, 0, 1)) != 0) {
        if (strlen($_f) > 1)
            $_f .= ",";

        $_f .= "{'name':'$file', 'files':[";

        $jfList = scandir("Data/json/" . $file, 1);

        $_jsonFile = "";
        foreach ($jfList as $jfile) {
            if (strcmp(".", substr($jfile, 0, 1)) != 0 && strcmp("coord_", substr($jfile, 0, 6)) != 0) {
                if (strlen($_jsonFile) > 0)
                    $_jsonFile .= ",";
                $_jsonFile .= "'" . $jfile . "'";
            }
        }
        $_f .= $_jsonFile."]}";
    }
}
$_f .= "]";

echo $_f;
?>

