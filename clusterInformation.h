
/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: clusterInformation.h
 Objective: Class holds clustering information of an individual
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              09/20/2016      Initial version
 Md. Kamruzzaman              01/04/2017      Adjust code with modified config file
 **************************************************************************************************/

#ifndef clusterInformation_h
#define clusterInformation_h

#include <string>
#include <stdlib.h>
#include "config.h"

using namespace std;

class ClusterInformation{
private:
    unsigned long *clusterID;
    short clusterIndex;
    bool status;
    string *type;
    unsigned long uniqueID;
    
public:
    
    ClusterInformation();
    ~ClusterInformation();
    
    void AddClusterID(unsigned long id);
    string GetClusterIdList();
    void SetStatus(bool status);
    bool GetStatus();
    void GetIDList(unsigned long* idList);
    bool HasClusterID(unsigned long cID);
    short GetTotalIdCount();
    void SetType(string ch);
    string GetType();
    string GetType(short index1, short index2);
    unsigned long GetUniqueID();
    void SetUniqueId(unsigned long oid);
    void UpdateClusterID(short index, unsigned long id);
    void UpdateType(short index, string ch);
    void DecreaseClusterIndex();
    float GetWeight();
    float GetWeight(short index1, short index2);
    float GetWeight(unsigned long clusterID);
    void ResetClusterInformation();
};

#endif /* ClusterInformation_h */
