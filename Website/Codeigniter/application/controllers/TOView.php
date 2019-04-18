<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class TOView extends CI_Controller {

	
	public function index()
	{
            getShared();
            $_share = new Shared();
            $data = array(
                "rand"=>rand(-1000000, 10000000),
                "fileList"=>$_share->getAllCsvFiles(),
                "toFileList"=>$_share->getAllJsonFiles()
                );
		$this->load->view('topoview', $data);
	}
}
