/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: Box.h
 Objective: Spliting window, holds information about data point
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              04/01/2016      Initial version
 
 **************************************************************************************************/

#ifndef Box_h
#define Box_h

#include "Cluster.h"

class BoxRectangle{
public:
    long id;
    set<long> clusterIdList;
    
    BoxRectangle(){}
    ~BoxRectangle(){}
    void GetClusterID(set<long>* mainClsIDSet){
        long id =0;
        set<long>::iterator itr;
        
        for(itr=clusterIdList.begin(); itr!=clusterIdList.end(); itr++){
            id = *itr;
            
            mainClsIDSet->insert(id);
        }
        
    }
};

class Area{
public:
    float xMin, xMax, yMin, yMax;
    
    Area();
    string toString();
};

class Box{
public:
    int first, second, third, fourth;
    Area area;
    list<Phenotype*> phenoList;
    list<DataPoint*> dpList;
    bool horizontalOverlap, twoWayOverlap;
    long clusterID[TOTAL_CLUSTER_IDS];
    short clusterIndex[TOTAL_CLUSTER_IDS];
    
    Box(int first, int second, int third, int fourth, float xMin, float xMax, float yMin, float yMax, bool horizontalOverlap, bool twoWayOverlap, long *clsID, short *clusterIndex);
    string toString();
};

#endif /* Box_h */
