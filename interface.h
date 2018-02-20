/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: interface.h
 Objective: Implementation of interface methods, public access to library functions
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              01/03/2018      Initial version
 
 **************************************************************************************************/

#ifndef interface_h
#define interface_h

#include <string>

using namespace std;

class Interface{
public:
    virtual string getD3GraphObject()=0;
};

#endif /* interface_h */
