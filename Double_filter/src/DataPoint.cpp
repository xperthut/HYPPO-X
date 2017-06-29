
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: DataPoint.h
 Objective: Implementation of member functions
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              02/21/2016      Initial version
 
 **************************************************************************************************/

#include "DataPoint.h"
#include <stdlib.h>

DataPoint::DataPoint(float pos[FILTER]){
    this->boxID = 0;
    this->varietyList = NULL;
    this->maxIndex = 0;
    
    for(int i=0; i<FILTER; i++){
        this->position[i] = pos[i];
    }
}

DataPoint::~DataPoint(){
    if(this->varietyList != NULL){
        free(this->varietyList);
        this->varietyList = NULL;
        this->maxIndex = 0;
    }
}

void DataPoint::addPhenotypeList(list<Phenotype*> phList){
    
    for(list<Phenotype*>::iterator itr=phList.begin(); itr!=phList.end(); itr++){
        Phenotype* ph = *itr;
        
        this->phenotypeList.push_back(ph);
    }
}

void DataPoint::addPhenotypeList(Phenotype* ph){
    this->phenotypeList.push_back(ph);
}

float DataPoint::getPosition(int index){return this->position[index];}
void DataPoint::setPosition(float value, int index){this->position[index]=value;}
list<Phenotype*>& DataPoint::getPhList(){return this->phenotypeList;}
void DataPoint::setBoxId(long id){this->boxID = id;}
long DataPoint::getBoxId(){return this->boxID;}
void DataPoint::clearList(){this->phenotypeList.clear();}

float DataPoint::GetAveragePhenotypeData(){
    float avgValue = 0.0;
    
    list<Phenotype*>::iterator itr = phenotypeList.begin();
    size_t size = phenotypeList.size();
    
    for(; itr!=phenotypeList.end(); itr++){
        Phenotype* ph = *itr;
        
        avgValue += ph->GetValue();
    }
    
    avgValue /= size;
    
    return avgValue;
}

void DataPoint::AddClusterID(long id){this->ci.AddClusterID(id);}
string DataPoint::GetClusterIdList(){return this->ci.GetClusterIdList();}
void DataPoint::SetStatus(bool status){this->ci.SetStatus(status);}
bool DataPoint::GetStatus(){return this->ci.GetStatus();}
void DataPoint::GetIDList(long* idList){this->ci.GetIDList(idList);}
bool DataPoint::HasClusterID(long cID){return this->ci.HasClusterID(cID);}
short DataPoint::GetTotalIdCount(){return this->ci.GetTotalIdCount();}
void DataPoint::SetType(string ch){this->ci.SetType(ch);}
string DataPoint::GetType(){return this->ci.GetType();}
string DataPoint::GetType(short index1, short index2){return this->ci.GetType(index1, index2);}
long DataPoint::GetUniqueID(){return this->ci.GetUniqueID();}
void DataPoint::SetUniqueId(long oid){this->ci.SetUniqueId(oid);}
void DataPoint::UpdateClusterID(short index, long id){this->ci.UpdateClusterID(index, id);}
void DataPoint::UpdateType(short index, string ch){this->ci.UpdateType(index, ch);}
void DataPoint::DecreaseClusterIndex(){this->ci.DecreaseClusterIndex();}
float DataPoint::GetWeight(){return this->ci.GetWeight();}
float DataPoint::GetWeight(short index1, short index2){return this->ci.GetWeight(index1, index2);}
float DataPoint::GetWeight(long clusterID){return this->ci.GetWeight(clusterID);}
void DataPoint::ResetClusterInformation(){this->ci.ResetClusterInformation();}
bool DataPoint::ComparePoint(DataPoint *dp){
    bool m=false;
    
    for(int i=0; i<FILTER; i++){
        m = (m&&this->position[i]==dp->getPosition(i));
    }
    
    return m;
}
float DataPoint::GetValue(){
    list<Phenotype*>::iterator itr;
    float value = 0.0;
    
    for(itr = phenotypeList.begin(); itr != phenotypeList.end(); itr++){
        Phenotype* ph = *itr;
        value += ph->GetValue();
    }
    
    return value;
}

string DataPoint::toString(){
    string s = "";
    for(int i=0; i<FILTER; i++){
        if(s.length()>0) s += ",";
        s+=to_string(position[i]);
    }
    
    return "["+ s + ">, Type:<" + GetType() + "> UID:" + to_string(this->ci.GetUniqueID()) + " ClusterIDs:<" + GetClusterIdList() + ">, FaceWeight:" + to_string(GetWeight()) + "]\n";
    
}

string DataPoint::toString(short index1, short index2){
    string s = "";
    for(int i=0; i<FILTER; i++){
        if(s.length()>0) s += ",";
        s+=to_string(position[i]);
    }
    
    return "["+ s + ">, Type:<" + GetType(index1, index2) + "> UID:" + to_string(this->ci.GetUniqueID()) + " ClusterIDs:<" + GetClusterIdList() + ">, FaceWeight:" + to_string(GetWeight(index1, index2)) + "]\n";
}

void DataPoint::CreateVarietyList(int size, map<int, int>* varMap){
    if(size > 0 && this->varietyList == NULL){
        this->varietyList = (bool*)malloc(size*sizeof(bool));
        this->maxIndex = size;
        
        for(int i=0; i<size; i++){
            this->varietyList[i] = false;
        }
    }
    
    list<Phenotype*>::iterator itr;
    set<int> varSet;
    
    for(itr = phenotypeList.begin(); itr != phenotypeList.end(); itr++){
        Phenotype* ph = *itr;
        varSet.insert(ph->GetVarietyID());
    }
    
    set<int>::iterator sitr;
    map<int,int>::iterator mitr;
    for(sitr = varSet.begin(); sitr!=varSet.end(); sitr++){
        int vid = *sitr;
        
        mitr = varMap->find(vid);
        int index = mitr->second-1;
        
        this->varietyList[index] = true;
    }
}

float DataPoint::HammingDistance(DataPoint *dp){
    int dist = 0;
    for(int i=0; i<this->maxIndex; i++){
        if(this->varietyList[i] != dp->GetValueOfIndex(i)){
            dist++;
        }
    }
    
    return (float)dist;
}

bool DataPoint::GetValueOfIndex(int index){
    if(index>=0 && index <=maxIndex){
        return this->varietyList[index];
    }
    
    return false;
}
