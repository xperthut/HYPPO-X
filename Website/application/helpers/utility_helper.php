<?php
defined('BASEPATH') OR exit('No direct script access allowed');
/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

// ------------------------------------------------------------------------

if ( ! function_exists('assets_url'))
{
	function assets_url()
	{
		return base_url().'assets/';
	}
}

// ------------------------------------------------------------------------

if ( ! function_exists('data_url'))
{
	function data_url()
	{
		return FCPATH.'Data/';
	}
}

// ------------------------------------------------------------------------

if ( ! function_exists('images_url'))
{
	function images_url()
	{
		return base_url().'images/';
	}
}