/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: main.cpp
 Objective: Main entry point of the code
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              01/03/2018      Initial version
 
 **************************************************************************************************/

#include <iostream>
#include <string>
#include "mapper.h"

int main(int argc, const char * argv[]) {
    
    Config::resetSettings();

    clock_t t1, t2;
    
    t1 = clock();
    
    Mapper* _mapper = new Mapper();
    _mapper->createMapper();
    delete _mapper;
    
    t2 = clock();
    
    float _time = (t2-t1)/CLOCKS_PER_SEC;
    string unit = "(sec)";
    
    if(_time<1){
        _time = ((t2-t1)*1000)/CLOCKS_PER_SEC;
        unit = "(ms)";
    }
    
    cout<<"Total time:"<<_time<<unit<<endl;
    
    return 0;
}
