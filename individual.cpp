
/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: individual.h
 Objective: Class holds individual's information, individual=<genotyoe,location,plantid>
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              04/02/2017      Initial version
 
 **************************************************************************************************/


#include "individual.h"

Individual::Individual(string genotype, string location, short plantID){
    this->genotype=genotype;
    this->location=location;
    this->plantID=plantID;
}

Individual::~Individual(){}

string Individual::getGenotype(){return genotype;}
string Individual::getLocation(){return location;}
short Individual::getPlantId(){return plantID;}

void Individual::addToTable(long pathNumber, long nodeID, long pathLength){
    if(pathNodeTable.hasItem(pathNumber)){
        CustomHashSet<long> *tmpSet = pathNodeTable.getValue(pathNumber);
        tmpSet->addToSet(nodeID);
        
    }else{
        CustomHashSet<long> *tmpSet = new CustomHashSet<long>();
        tmpSet->addToSet(nodeID);
        
        pathNodeTable.addToMap(pathNumber, tmpSet);
    }
    
    if(!pathWithLengthMap.hasItem(pathNumber)){
        pathWithLengthMap.addToMap(pathNumber, pathLength);
    }
}

string Individual::getTableInCsv(){
    string csv = "";
    string header = getHeader();
    string tail = "";
    
    unordered_map<long, CustomHashSet<long>*> table = pathNodeTable.getMap();
    for(unordered_map<long, CustomHashSet<long>*>::iterator itr =table.begin(); itr!=table.end(); itr++){
        long pathNumber = itr->first;
        tail = ","+to_string(pathNumber) + ",";
        
        unordered_set<long> nList = itr->second->getSet();
        long totalNodes = nList.size();
        
        if(totalNodes>1){
            tail += "\"";
        }
        
        for(unordered_set<long>::iterator litr=nList.begin(); litr!=nList.end();litr++){
            long id = *litr;
            
            if(litr!=nList.begin())
                tail += ",";
            
            tail += to_string(id);
            
        }
        
        if(totalNodes>1){
            tail += "\"";
        }
        
        csv += header + tail + "," +to_string(pathWithLengthMap.getValue(pathNumber)) + "," + to_string(totalNodes)+ "\n";
    }
    
    return csv;
}

void Individual::addNodeId(long id){
    nodeList.addToSet(id);
}

string Individual::getNodeList(){
    unordered_set<long> nList = nodeList.getSet();
    long totalNodes = nList.size();
    string nS="";
    
    if(totalNodes>1){
        nS += "\"";
    }
    
    for(unordered_set<long>::iterator litr=nList.begin(); litr!=nList.end();litr++){
        long id = *litr;
        
        if(litr!=nList.begin())
            nS += ",";
        
        nS += to_string(id);
        
    }
    
    if(totalNodes>1){
        nS += "\"";
    }
    
    return nS;
}

string Individual::getHeader(){
    return (genotype+","+location+","+to_string(plantID));
}
