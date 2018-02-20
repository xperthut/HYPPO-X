
/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: phenotype.cpp
 Objective: Implementation of member functions
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              02/21/2016      Initial version
 
 **************************************************************************************************/

#include "phenotype.h"

Phenotype::Phenotype(int varietyID, string genotype, string site, float value){
    this->value = value;
    this->varietyID = varietyID;
    this->genotype = genotype;
    this->site = site;
    this->gen_loc_weight= -1;
    this->date = "";
    this->time = "";
    this->filterValue = (float*)malloc(sizeof(float)*Config::FILTER);
    this->envValue = (float*)malloc(sizeof(float)*Config::ENV_ATTR);
    
    this->assign_gen_loc_weight(genotype, site);
}

Phenotype::Phenotype(string varietyID, string genotype, string site, float value){
    this->value = value;
    this->varietyID = 0;
    this->genotype = genotype;
    this->site = site;
    this->gen_loc_weight= -1;
    this->filterValue = (float*)malloc(sizeof(float)*Config::FILTER);
    this->envValue = (float*)malloc(sizeof(float)*Config::ENV_ATTR);
    
    assign_gen_loc_weight(genotype, site);
    
    size_t pos;
    while((pos=varietyID.find(" # "))!=string::npos){
        string plantID = varietyID.substr(0,pos);
        varietyID = varietyID.substr(pos+3,varietyID.length()-pos-3);
        
        this->plantIDList.push_back(stol(plantID));
    }
    
    this->plantIDList.push_back(stol(varietyID));
}

Phenotype::~Phenotype(){
    free(this->filterValue);
    free(this->envValue);
}

void Phenotype::assign_gen_loc_weight(string gen, string loc){
    //A
    if(gen.compare("F42 x DJ7")==0||gen.compare("A")==0){
        if(loc.compare("KS")==0||loc.compare("K")==0) this->gen_loc_weight = 0;
        else if(loc.compare("NE")==0||loc.compare("N")==0) this->gen_loc_weight = 1;
    }
    //B
    else{
        if(loc.compare("NE")==0||loc.compare("N")==0) this->gen_loc_weight = 2;
        else if(loc.compare("KS")==0||loc.compare("K")==0) this->gen_loc_weight = 3;
    }
}

//long Phenotype::GetUniqueID(){return this->ci.GetUniqueID();}
//void Phenotype::SetUniqueId(long oid){this->ci.SetUniqueId(oid);}
float Phenotype::GetValue(){return this->value;}
int Phenotype::GetVariety(){return this->varietyID;}
string Phenotype::GetSite(){return this->site;}
string Phenotype::GetDateTime(){return this->dateTime;}
list<long>& Phenotype::getPlantIdList(){return plantIDList;}

string Phenotype::getGenotype(){return genotype;}
int Phenotype::getDAP(){return dap;}
string Phenotype::getDate(){return date;}
string Phenotype::getTime(){return time;}
void Phenotype::setGenotype(string g){genotype=g;}
void Phenotype::setDAP(int d){dap=d;}
void Phenotype::setDate(string d){date = d;}
void Phenotype::setTime(string t){time=t;}
void Phenotype::setDateTime(string dt){dateTime=dt;}

//void Phenotype::SetStatus(bool status){this->ci.SetStatus(status);}
//bool Phenotype::GetStatus(){return this->ci.GetStatus();}
string Phenotype::GetID(){return (to_string(varietyID)+date+site);}
int Phenotype::GetVarietyID(){return varietyID;}
int Phenotype::getGenLocWeight(){return gen_loc_weight;}
string Phenotype::getGenLoc(){return (genotype+"#"+site);}
string Phenotype::getGenLocId(){return (genotype+"#"+site+"#"+to_string(varietyID));}
/*void Phenotype::SetType(string ch){
    this->ci.SetType(ch);
}
    
string Phenotype::GetType(){
    
    return this->ci.GetType();
}

string Phenotype::GetType(short index1, short index2){
    return this->ci.GetType(index1, index2);
}

void Phenotype::AddClusterID(long id){
    this->ci.AddClusterID(id);
}

void Phenotype::DecreaseClusterIndex(){
    this->ci.DecreaseClusterIndex();
}

void Phenotype::UpdateClusterID(short index, long id){
    this->ci.UpdateClusterID(index, id);
}

void Phenotype::UpdateType(short index, string ch){
    this->ci.UpdateType(index, ch);
}

string Phenotype::GetClusterIdList(){
    
    return this->ci.GetClusterIdList();
}

void Phenotype::GetIDList(long *idList){
    
    this->ci.GetIDList(idList);
}

short Phenotype::GetTotalIdCount(){return this->ci.GetTotalIdCount();}

bool Phenotype::HasClusterID(long cID){
    
    return this->ci.HasClusterID(cID);
}*/

string Phenotype::toString(){
    return "[ID:<" + to_string(varietyID) + "," + date + "," + site + ">, Type:<" + GetType() + "> UID:" + to_string(this->GetUniqueID()) + " ClusterIDs:<" + GetClusterIdList() + ">, Value:" + to_string(value) + " FaceWeight:" + to_string(GetWeight()) + "]\n";
    
}

string Phenotype::toString(short index1, short index2){
    return "[ID:<" + to_string(varietyID) + "," + date + "," + site + ">, Type:<" + GetType(index1, index2) + "> UID:" + to_string(this->GetUniqueID()) + " ClusterIDs:<" + GetClusterIdList() + ">, Value:" + to_string(value) + " FaceWeight:" + to_string(GetWeight(index1, index2)) + "]\n";
}

/*float Phenotype::GetWeight(){
    return this->ci.GetWeight();
  
}

float Phenotype::GetWeight(long clusterID){
    
    return this->ci.GetWeight(clusterID);
}

float Phenotype::GetWeight(short index1, short index2){
    return this->ci.GetWeight(index1, index2);
}*/

void Phenotype::setCoOrdinate(float value, int index){
    this->filterValue[index]=value;
}

float Phenotype::getCoOrdinate(int index){return filterValue[index];}

bool Phenotype::ComparePoint(Phenotype *ph){
    
    bool comp = (this->varietyID == ph->GetVariety() && this->genotype.compare(ph->getGenotype()) == 0 && this->site.compare(ph->GetSite()) == 0 && this->dap==ph->getDAP());
    
    
    /*for(int i=0;i<FILTER;i++){
        comp = comp && (this->filterValue[i]==ph->getCoOrdinate(i));
    }*/
    
    return comp;
}

/*void Phenotype::ResetClusterInformation(){
    this->ci.ResetClusterInformation();
}*/

string Phenotype::getIndividualId(){
    return (genotype+"#"+site+"#"+to_string(dap));
}

void Phenotype::setEnvValue(int index, float value){this->envValue[index]=value;}
float Phenotype::getEnvValue(int index){return this->envValue[index];}
void Phenotype::setClusterValue(float clsValue){this->cluster_value = clsValue;}
float Phenotype::getClusterValue(){return this->cluster_value;}

