/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: library.h
 Objective: Implementation of interface methods, public access as library functions
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              01/03/2018      Initial version
 
 **************************************************************************************************/

#ifndef library_h
#define library_h

#include "phpcpp.h"
#include "interface.h"
#include <list>

class Library:public Interface{
public:
    Library();
    ~Library();
    string requestHandler(string fncName, list<string> *paramList);
private:
    string getD3GraphObject();
};

#endif /* library_h */
