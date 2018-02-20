
/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: cluster.cpp
 Objective: Implementation of methods of the class
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              03/13/2016      Initial version
 
 **************************************************************************************************/

#include "cluster.h"

long Cluster::clusterID = 0;

long Cluster::GetClusterID(){return clusterID;}

void Cluster::SetClusterID(long id){clusterID = id;}

void Cluster::AddToCluster(Phenotype *ph){
    this->phList.push_back(ph);
}

void Cluster::GetPhenotypeListOfThisCluster(list<Phenotype *> *ph){
    list<Phenotype *>::iterator itr;
    
    for(itr = this->phList.begin(); itr != this->phList.end(); itr++){
        Phenotype* p = *itr;
        
        ph->push_back(p);
    }
}

void Cluster::AddToCluster(DataPoint *dp){
    this->dpList.push_back(dp);
}

void Cluster::GetPhenotypeListOfThisCluster(list<DataPoint *> *dp){
    list<DataPoint *>::iterator itr;
    
    for(itr = this->dpList.begin(); itr != this->dpList.end(); itr++){
        DataPoint* p = *itr;
        
        dp->push_back(p);
    }
}


