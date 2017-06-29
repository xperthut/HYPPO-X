
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: Phenotype.h
 Objective: Class holds phenotypic information
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              02/21/2016      Initial version
 
 **************************************************************************************************/

#ifndef Phenotype_h
#define Phenotype_h

#include <string>
#include <list>
#include "ClusterInformation.h"

using namespace std;

class Phenotype{
private:
    int varietyID;
    string dateTime;
    string site;
    float value;
    ClusterInformation ci;
    float filterValue[FILTER];
    float envValue[ENV_ATTR];
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
    
    void AddClusterID(long id);
    string GetClusterIdList();
    string toString();
    string toString(short index1, short index2);
    void SetStatus(bool status);
    bool GetStatus();
    string GetID();
    void GetIDList(long* idList);
    bool HasClusterID(long cID);
    short GetTotalIdCount();
    void SetType(string ch);
    string GetType();
    string GetType(short index1, short index2);
    float GetWeight();
    float GetWeight(short index1, short index2);
    float GetWeight(long clusterID);
    void setCoOrdinate(float value, int index);
    float getCoOrdinate(int index);
    bool ComparePoint(Phenotype* ph);
    void ResetClusterInformation();
    long GetUniqueID();
    void SetUniqueId(long oid);
    void UpdateClusterID(short index, long id);
    void UpdateType(short index, string ch);
    void DecreaseClusterIndex();
    int GetVarietyID();
    int getGenLocWeight();
    string getGenLoc();
    string getGenLocId();
    string getIndividualId();
};


#endif /* Phenotype_h */
