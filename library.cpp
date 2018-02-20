/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: library.cpp
 Objective: Inplementation of library methods
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              01/03/2018      Initial version
 
 **************************************************************************************************/

#include "library.h"

// Public methods
Library::Library(){}
Library::~Library(){}

// Request handler method, it handles public request
string Library::requestHandler(string fncName, list<string> *paramList){
    
    if(fncName.compare("D3Graph")==0){
        return getD3GraphObject();
    }
    
    return "";
}

// Private methods
string Library::getD3GraphObject(){
    return "";
}
