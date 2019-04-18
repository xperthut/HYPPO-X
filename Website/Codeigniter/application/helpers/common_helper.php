<?php

defined('BASEPATH') OR exit('No direct script access allowed');
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

if (!function_exists('getShared')) {

    function getShared() {
        require_once (APPPATH . "shared/Shared.php");
    }

}

if (!function_exists('getLZW')) {

    function getLZW() {
        require_once (APPPATH . "shared/LZW.php");
    }

}
