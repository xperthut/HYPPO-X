
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: Box.cpp
 Objective: Implementation of classes 
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              04/01/2017      Initial version
 
 **************************************************************************************************/

#include "Box.h"

//#pragma Area_Methods

Area::Area(){}

string Area::toString(){
    return "(" + to_string(xMin) + "," + to_string(yMin)+ "," + to_string(xMax) + "," + to_string(yMax) + ")";
}

//#pragma Box_Methods

Box::Box(int first, int second, int third, int fourth, float xMin, float xMax, float yMin, float yMax, bool horizontalOverlap, bool twoWayOverlap, long *clsID, short *clusterIndex){
    this->first = first;
    this->second = second;
    this->third = third;
    this->fourth = fourth;
    this->area.xMin = xMin;
    this->area.yMin = yMin;
    this->area.xMax = xMax;
    this->area.yMax = yMax;
    this->horizontalOverlap = horizontalOverlap;
    this->twoWayOverlap = twoWayOverlap;
    
    for(short i=0; i<4; i++){
        this->clusterID[i] = clsID[i];
        this->clusterIndex[i] = clusterIndex[i];
    }
    
    
}

string Box::toString(){
    string str = "";
    
    string hv = (horizontalOverlap)?"Horizontal":"Vertical";
    string way = (twoWayOverlap)?" 2 way":" 4 way";
    
    str += "\n\n" + hv + way + " overlap between Box(" + to_string(first) + "," + to_string(second) + "," + to_string(third) +
    "," + to_string(fourth) + ")\nSearching in " + area.toString() + "\n";
    
    return str;
}

bool Box::matchClusterIndex(short *clusterIndex){
    if(this->twoWayOverlap){
        if(this->clusterIndex[0] == clusterIndex[0] && this->clusterIndex[1] == clusterIndex[1]){
            return true;
        }
    }else{
        if(this->clusterIndex[0] == clusterIndex[0] && this->clusterIndex[1] == clusterIndex[1] && this->clusterIndex[2] == clusterIndex[2] && this->clusterIndex[3] == clusterIndex[3]){
            return true;
        }
    }
    
    return false;
}

