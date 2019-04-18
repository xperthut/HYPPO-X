<?php

defined('BASEPATH') OR exit('No direct script access allowed');

class DataHandler extends CI_Controller {
    
    private $_share;
    private $_lzw;
            
    function __construct() {
        parent::__construct();
        
        getShared();
        getLZW();
        
        $this->_share = new Shared();
        $this->_lzw = new LZW();
    }
            
    function setcoord() {
        $coord = $this->input->post("data");
        $fo = $this->input->post('folderName');
        $f = $this->input->post('fileName');

        echo $this->_share->setCoordinate($fo, $f, $coord);
    }

    function getcoord() {
        $fo = $this->input->post('folderName');
        $f = $this->input->post('fileName');

        echo $this->_share->getCoordinate($fo, $f);
    }
    
    function setcolor(){
        $colorData = $this->input->post("data");
        $fo = $this->input->post('folderName');
        $f = $this->input->post('fileName');

        echo $this->_share->setColor($fo, $f, $colorData);
    }
    
    function loaddata(){
        $fo = $this->input->post('folderName');
        $f = $this->input->post('fileName');
        
        echo $this->_share->loadJSONData($fo, $f);
    }
    
    function writedata(){
        $opt = $this->input->post("type");
        $rowIds = $this->input->post("data");
        $fo = $this->input->post('folderName');
        
        $rv = $this->_share->writeDataToTmpFile($opt, $rowIds, $fo);
        
        if(is_array($rv)){
            $rv = json_encode($rv);
        }
        
        echo $rv;
    }
    
    function getperf(){
        $opt = $this->input->post("type");
        $rowIds = $this->input->post("data");
        $fo = $this->input->post('folderName');
        
        $rv = $this->_share->loadPerfs($opt, $rowIds, $fo);
        
        if(is_array($rv)){
            $rv = json_encode($rv);
        }
        
        echo $rv;
    }
    
}
