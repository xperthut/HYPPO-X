
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: Cluster.h
 Objective: Holds information about a cluster
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              03/13/2016      Initial version
 
 **************************************************************************************************/


#ifndef Cluster_h
#define Cluster_h

#include "DataPoint.h"
#include <set>
#include <map>

class Cluster{
private:
    static long clusterID;
    list<Phenotype*> phList;
    list<DataPoint*> dpList;
    
public:
    Cluster(){}
    ~Cluster(){this->phList.clear();}
    static long GetNextID(){clusterID++; return clusterID;}
    static long GetClusterID();
    static void SetClusterID(long id);
    
    void AddToCluster(Phenotype* ph);
    void GetPhenotypeListOfThisCluster(list<Phenotype*> *ph);
    void AddToCluster(DataPoint* dp);
    void GetPhenotypeListOfThisCluster(list<DataPoint*> *dp);
    
};

#endif /* Cluster_h */


