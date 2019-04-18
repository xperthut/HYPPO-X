<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

class Shared {

    private $groupName;
    private $projectNames;
    private $dataFileNames;
    private $objectFileNames;
    private $coordFileNames;
    private $SUCCESS_CODE;
    private $FAILURE_CODE;

    function __construct() {
        $this->groupName = "";
        $this->projectNames = Array();
        $this->dataFileNames = Array();
        $this->objectFileNames = Array();
        $this->coordFileNames = Array();

        $this->SUCCESS_CODE = 200;
        $this->FAILURE_CODE = 404;
    }

    function getAllCsvFiles() {
        try {
            $fList = scandir(data_url() . "csv", 1);
        } catch (Exception $exc) {
            return "[]";
        }


        $fn = "[";

        foreach ($fList as $file) {

            if (strcmp(".", substr($file, 0, 1)) != 0) {
                if (strlen($fn) > 1) {
                    $fn .= ",";
                }
                $fn .= '"' . $file . '"';
            }
        }
        $fn .= "]";

        return $fn;
    }

    function getAllJsonFiles() {
        try {
            $fList = scandir(data_url() . "json", 1);
        } catch (Exception $exc) {
            return "[]";
        }


        $_f = "[";
        foreach ($fList as $file) {

            if (strcmp(".", substr($file, 0, 1)) != 0) {
                if (strlen($_f) > 1) {
                    $_f .= ",";
                }

                $_f .= "{'name':'$file', 'files':[";

                try {
                    $jfList = scandir(data_url() . "json/" . $file, 1);
                } catch (Exception $exc) {
                    return "[]";
                }

                $_jsonFile = "";
                foreach ($jfList as $jfile) {
                    if (strcmp(".", substr($jfile, 0, 1)) != 0 && strcmp("coord_", substr($jfile, 0, 6)) != 0) {
                        if (strlen($_jsonFile) > 0) {
                            $_jsonFile .= ",";
                        }
                        $_jsonFile .= "'" . $jfile . "'";
                    }
                }
                $_f .= $_jsonFile . "]}";
            }
        }
        $_f .= "]";

        return $_f;
    }

    function setCoordinate($projectName, $fileName, $coordData) {
        if (is_null($projectName) || is_null($fileName) || is_null($coordData)) {
            return $this->FAILURE_CODE;
        }


        $succ = TRUE;
        if (!is_dir(data_url() . "json/" . $projectName)) {
            try {
                mkdir(data_url() . "json/" . $projectName, 0766, true);
            } catch (Exception $exc) {
                $succ = FALSE;
            }
        }

        if ($succ) {
            $dir = data_url() . "json/" . $projectName . "/coord_" . $fileName;

            $file = fopen($dir, "w");
            fwrite($file, $coordData);
            fclose($file);
        } else {
            return $this->FAILURE_CODE;
        }

        return $this->SUCCESS_CODE;
    }

    function getCoordinate($projectName, $fileName) {
        if (is_null($projectName) || is_null($fileName)) {
            return $this->FAILURE_CODE;
        }

        $dir = data_url() . "json/" . $projectName . "/coord_" . $fileName;

        if (file_exists($dir)) {
            $file = fopen($dir, "r");
            $json = preg_replace("/\r|\n/", "", fgets($file));
            fclose($file);

            return '[' . $json . ']';
        }

        return $this->FAILURE_CODE;
    }

    function setColor($projectName, $fileName, $colorData) {
        if (is_null($projectName) || is_null($fileName) || is_null($colorData)) {
            return $this->FAILURE_CODE;
        }

        $dir = data_url() . "json/" . $projectName . "/" . $fileName;

        //chmod($dir,0755);

        if (file_exists($dir)) {
            $file = fopen($dir, "w");
            fwrite($file, $colData);
            fclose($file);

            return $this->SUCCESS_CODE;
        }

        return $this->FAILURE_CODE;
    }

    function loadJSONData($projectName, $fileName) {
        if (is_null($projectName) || is_null($fileName)) {
            return $this->FAILURE_CODE;
        }

        $dir = data_url() . "json/" . $projectName . "/" . $fileName;

        if (file_exists($dir)) {
            return file_get_contents($dir);
        }

        return $this->FAILURE_CODE;
    }

    private function get_row_col($csv, $row, $col) {
        $handle = fopen("$csv", 'r');
        $counter = -1;
        $result = array();

        while (($rdata = fgetcsv($handle)) !== FALSE) {
            $counter++;
            if (in_array($counter, $row)) {
                $cData = array();
                foreach ($col as $value) {
                    array_push($cData, $rdata[$value]);
                }
                array_push($result, $cData);

                unset($row[array_search($counter, $row)]);
            }
        }

        $filtered_result = array_map("unserialize", array_unique(array_map("serialize", $result)));

        $result = array();
        foreach ($filtered_result as $value) {
            array_push($result, $value);
        }

        return $result;
    }

    private function write_row_col($csv, $row, $col, $csvFileName) {
        try {
            $handle = fopen("$csv", 'r');
        } catch (Exception $e) {
            return '[{"code":' . $this->FAILURE_CODE . ', "Error":"' . $ex->getMessage() . '"}]';
        }
        $a = explode(".", $csvFileName);
        $wFn = data_url() . "tmp/" . $a[0] . "_" . time() . ".csv";

        try {
            $wHandle = fopen($wFn, "a");
        } catch (Exception $e) {
            return '[{"code":' . $this->FAILURE_CODE . ', "Error":"' . $ex->getMessage() . '"}]';
        }

        $counter = -1;

        while (($rdata = fgetcsv($handle)) !== FALSE) {
            $counter++;
            if ($counter == 0 || in_array($counter, $row)) {
                $cData = "";
                foreach ($col as $value) {
                    if (strlen($cData) > 0)
                        $cData .= ",";
                    $cData .= $rdata[$value];
                }
                fwrite($wHandle, $cData . "\n");
            }
        }

        fclose($handle);
        fclose($wHandle);


        return '[{"code":' . $this->SUCCESS_CODE . ', "fn":"' . $wFn . '"}]';
    }

    function writeDataToTmpFile($options, $rowIds, $csvFileName) {
        if (is_null($csvFileName) || is_null($options) || is_null($rowIds) || strlen($options)==0 || strlen($csvFileName)==0 || strlen($rowIds)==0) {
            return $this->FAILURE_CODE;
        }

        $cols = explode(",", $options);
        $rows = explode(",", $rowIds);
        sort($rows);

        foreach ($cols as &$value) {
            $value = $value - 1;
        }

        $csv = data_url() . "csv/" . $csvFileName;

        if (file_exists($csv)) {
            return $this->write_row_col($csv, $rows, $cols, $csvFileName);
        }

        return $this->FAILURE_CODE;
    }

    function loadPerfs($options, $rowIds, $csvFileName) {
        if (is_null($csvFileName) || is_null($options) || is_null($rowIds)) {
            return $this->FAILURE_CODE;
        }

        $cols = explode(",", $options);
        $rows = explode(",", $rowIds);
        sort($rows);

        foreach ($cols as &$value) {
            $value = $value - 1;
        }

        $csv = data_url() . "csv/" . $csvFileName;

        if (file_exists($csv)) {
            return $this->get_row_col($csv, $rows, $cols);
        }

        return $this->FAILURE_CODE;
    }

}
