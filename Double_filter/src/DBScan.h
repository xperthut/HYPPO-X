
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: DBScan.h
 Objective: Implementation of density based clustering DBSCAN algo
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              03/12/2016      Initial version
 
 **************************************************************************************************/

#ifndef DBScan_h
#define DBScan_h

#include "Cluster.h"
#include "QuickSort.h"
#include <unordered_map>
#include <unordered_set>
#include <iostream>

#define NOISE -1

using namespace std;

class DBScan{
private:
    float Eps;
    float minPoints;
    Phenotype** phenotypeList;
    DataPoint** dataPointList;
    long size;
    bool setUniqueId;
    long lastClusterId;
    QuickSort* quickSort;
    
    void DBSCAN();
    void ExpandCluster(long index, long clusterID);
    void GetSeeds(list<long> *phIndexList, long index);
    
    // For gen_loc
    int ExpandCluster(long index, long clusterID, int gen_loc_weight);
    void GetSeeds(list<long> *phIndexList, long index, int gen_loc_weight);
    
    // For hamming distance
    void DBSCAN_HD();
    void ExpandCluster_HD(long index, long clusterID);
    void GetSeeds_HD(list<long> *phIndexList, long index);
    
    
public:
    DBScan(float Eps, float minPoints, bool setUniqueId);
    ~DBScan();
    void MakeCluster(list<Phenotype*> *phList);
    void MakeCluster(list<DataPoint*> *dpList);
};

#endif /* DBScan_h */
