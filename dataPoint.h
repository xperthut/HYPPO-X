
/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: dataPoint.h
 Objective: Holds a data point informaiton with its phenotype
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              02/21/2016      Initial version
 
 **************************************************************************************************/


#ifndef dataPoint_h
#define dataPoint_h

#include <math.h>
#include "phenotype.h"
#include <map>
#include <set>

using namespace std;

class DataPoint:public ClusterInformation{
private:
    bool* varietyList;
    int maxIndex;
    
    float *position;
    long boxID;
    list<Phenotype*> phenotypeList;
    
public:
    
    DataPoint(float *pos);
    ~DataPoint();
    
    void addPhenotypeList(list<Phenotype*> phList);
    void addPhenotypeList(Phenotype* ph);
    float getPosition(int index);
    void setPosition(float value, int index);
    list<Phenotype*>& getPhList();
    void clearList();
    void setBoxId(long id);
    long getBoxId();
    
    float GetAveragePhenotypeData();
    
    //void AddClusterID(long id);
    //string GetClusterIdList();
    //void SetStatus(bool status);
    //bool GetStatus();
    //void GetIDList(long* idList);
    //bool HasClusterID(long cID);
    //short GetTotalIdCount();
    //void SetType(string ch);
    //string GetType();
    //string GetType(short index1, short index2);
    //long GetUniqueID();
    //void SetUniqueId(long oid);
    //void UpdateClusterID(short index, long id);
    //void UpdateType(short index, string ch);
    //void DecreaseClusterIndex();
    //float GetWeight();
    //float GetWeight(short index1, short index2);
    //float GetWeight(long clusterID);
    //void ResetClusterInformation();
    bool ComparePoint(DataPoint* dp);
    float GetValue();
    string toString();
    string toString(short index1, short index2);
    
    void CreateVarietyList(int size, map<int, int>* varMap);
    float HammingDistance(DataPoint* dp);
    bool GetValueOfIndex(int index);
};

#endif /* DataPoint_h */
