
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: ClusterInformation.h
 Objective: Class holds clustering information of an individual
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              09/20/2016      Initial version
 
 **************************************************************************************************/

#ifndef ClusterInformation_h
#define ClusterInformation_h

#include <string>
#include "config.h"

using namespace std;

class ClusterInformation{
private:
    long clusterID[TOTAL_CLUSTER_IDS];
    short clusterIndex;
    bool status;
    string type[TOTAL_CLUSTER_IDS];
    long uniqueID;
    
public:
    
    ClusterInformation();
    ~ClusterInformation();
    
    void AddClusterID(long id);
    string GetClusterIdList();
    void SetStatus(bool status);
    bool GetStatus();
    void GetIDList(long* idList);
    bool HasClusterID(long cID);
    short GetTotalIdCount();
    void SetType(string ch);
    string GetType();
    string GetType(short index1, short index2);
    long GetUniqueID();
    void SetUniqueId(long oid);
    void UpdateClusterID(short index, long id);
    void UpdateType(short index, string ch);
    void DecreaseClusterIndex();
    float GetWeight();
    float GetWeight(short index1, short index2);
    float GetWeight(long clusterID);
    void ResetClusterInformation();
};

#endif /* ClusterInformation_h */
