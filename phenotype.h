
/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: phenotype.h
 Objective: Each point carries a phenotypic information, which is uniquely identified by <genotype,
            location,date/time>
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              02/21/2016      Initial version
 
 **************************************************************************************************/

#ifndef phenotype_h
#define phenotype_h

#include <string>
#include <list>
#include "clusterInformation.h"

using namespace std;

class Phenotype:public ClusterInformation{
private:
    int varietyID;
    string dateTime;
    string site;
    float value;
    float cluster_value;
    float *filterValue;
    float *envValue;
    string genotype;
    int dap;
    string date;
    string time;
    int gen_loc_weight;
    list<long> plantIDList;
    
    void assign_gen_loc_weight(string gen, string loc);
    
public:
    Phenotype(int varietyID, string genotype, string site, float value);
    Phenotype(string varietyID, string genotype, string site, float value);
    ~Phenotype();
    
    int GetVariety();
    float GetValue();
    string GetDateTime();
    string GetSite();
    list<long>& getPlantIdList();
    
    string getGenotype();
    int getDAP();
    string getDate();
    string getTime();
    void setGenotype(string g);
    void setDAP(int d);
    void setDate(string d);
    void setTime(string t);
    void setDateTime(string dt);
    void setEnvValue(int index, float value);
    float getEnvValue(int index);
    float getClusterValue();
    void setClusterValue(float clsValue);
    
    //void AddClusterID(long id);
    //string GetClusterIdList();
    string toString();
    string toString(short index1, short index2);
    //void SetStatus(bool status);
    //bool GetStatus();
    string GetID();
    //void GetIDList(long* idList);
    //bool HasClusterID(long cID);
    //short GetTotalIdCount();
    //void SetType(string ch);
    //string GetType();
    //string GetType(short index1, short index2);
    //float GetWeight();
    //float GetWeight(short index1, short index2);
    //float GetWeight(long clusterID);
    void setCoOrdinate(float value, int index);
    float getCoOrdinate(int index);
    bool ComparePoint(Phenotype* ph);
    //void ResetClusterInformation();
    //long GetUniqueID();
    //void SetUniqueId(long oid);
    //void UpdateClusterID(short index, long id);
    //void UpdateType(short index, string ch);
    //void DecreaseClusterIndex();
    int GetVarietyID();
    int getGenLocWeight();
    string getGenLoc();
    string getGenLocId();
    string getIndividualId();
};


#endif /* Phenotype_h */
