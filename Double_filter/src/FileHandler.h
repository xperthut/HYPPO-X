
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: FileHandler.h
 Objective: Class to handle file management like read, write, etc
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              02/21/2016      Initial version
 
 **************************************************************************************************/


#ifndef FileHandler_h
#define FileHandler_h

#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <unordered_map>
#include "DataPoint.h"

using namespace std;

class FileHandler{
private:
    string fileName;
    string currentDateTime;
    const string GetCurrentDateTime();
    
public:
    FileHandler(string fileName);
    ~FileHandler();
    void ReadFileData(list<DataPoint*> *dataPointList);
    void readNodePosition(string fileName, unordered_map<long, float*> *positionMap);
    string WriteDataToFile(string fileName, string extention, string data, bool addTimeStemp);
};

#endif /* FileHandler_h */
