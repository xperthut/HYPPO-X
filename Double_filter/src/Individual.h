
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: Individual.h
 Objective: Class holds individual's information, individual=<genotyoe,location,plantid>
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              04/02/2017      Initial version
 
 **************************************************************************************************/


#ifndef Individual_h
#define Individual_h

#include "CustomCollection.cpp"
#include <string>

class Individual{
private:
    string genotype;
    string location;
    short plantID;
    
    CustomHashSet<long> nodeList;
    CustomHashMap<long, CustomHashSet<long>*> pathNodeTable;
    CustomMap<long, long> pathWithLengthMap;
    CustomMap<string, float> averagePhValue;
    
public:
    Individual(string genotype, string location, short plantID);
    ~Individual();
    
    string getGenotype();
    string getLocation();
    short getPlantId();
    void addToTable(long pathNumber, long nodeID, long pathLength);
    string getTableInCsv();
    void addNodeId(long id);
    string getNodeList();
    string getHeader();
};

#endif /* Individual_h */
