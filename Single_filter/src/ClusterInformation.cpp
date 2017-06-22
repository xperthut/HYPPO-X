
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: ClusterInformation.cpp
 Objective: Implementation of member functions
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              09/20/2016      Initial version
 
 **************************************************************************************************/

#include "ClusterInformation.h"

ClusterInformation::ClusterInformation(){
    this->status = true;
    this->clusterIndex = -1;
    
    for(int i=0; i<TOTAL_CLUSTER_IDS; i++){
        this->type[i] = "";
        this->clusterID[i] = 0;
    }
}

ClusterInformation::~ClusterInformation(){}

long ClusterInformation::GetUniqueID(){return this->uniqueID;}
void ClusterInformation::SetUniqueId(long oid){this->uniqueID = oid;}
void ClusterInformation::SetStatus(bool status){this->status = status; if(!status && this->clusterIndex<TOTAL_CLUSTER_IDS-1) this->clusterIndex++;}
bool ClusterInformation::GetStatus(){return this->status;}
void ClusterInformation::SetType(string ch){
    if(this->clusterIndex >= 0 && this->clusterIndex<TOTAL_CLUSTER_IDS){
        this->type[this->clusterIndex] = ch;
    }else{
        for(short i=0; i<TOTAL_CLUSTER_IDS; i++){
            if(this->type[i].length() == 0){
                this->type[i] = ch;
                break;
            }
        }
    }
    
}
string ClusterInformation::GetType(){
    string idList = "";
    
    for(int i=0; i<=this->clusterIndex; i++){
        if(idList.length() > 0){
            idList += ",";
        }
        
        idList += this->type[i];
    }
    
    return idList;
}

string ClusterInformation::GetType(short index1, short index2){
    string idList = "";
    
    if(index1 <= this->clusterIndex && index2 <= this->clusterIndex){
        idList = this->type[index1] + "," + this->type[index2];
    }
    
    return idList;
}

void ClusterInformation::AddClusterID(long id){
    if(this->clusterIndex >= 0 && this->clusterIndex<TOTAL_CLUSTER_IDS){
        this->clusterID[this->clusterIndex] = id;
        this->status = true;
    }else{
        for(short i=0; i<TOTAL_CLUSTER_IDS; i++){
            if(this->clusterID[i] == 0){
                this->clusterID[i] = id;
                this->status = true;
                break;
            }
        }
    }
}

void ClusterInformation::DecreaseClusterIndex(){
    if(this->clusterIndex >= 0) this->clusterIndex--;
}

void ClusterInformation::UpdateClusterID(short index, long id){
    if(index>=0 && index<TOTAL_CLUSTER_IDS){
        this->clusterID[index] = id;
    }
}

void ClusterInformation::UpdateType(short index, string ch){
    if(index>=0 && index<TOTAL_CLUSTER_IDS){
        this->type[index] = ch;
    }
}

string ClusterInformation::GetClusterIdList(){
    string idList = "";
    
    for(int i=0; i<=this->clusterIndex; i++){
        if(idList.length() > 0){
            idList += ",";
        }
        
        idList += to_string(this->clusterID[i]);
    }
    
    return idList;
}

void ClusterInformation::GetIDList(long *idList){
    
    for(int i=0; i<TOTAL_CLUSTER_IDS; i++){
        idList[i] = this->clusterID[i];
    }
}

short ClusterInformation::GetTotalIdCount(){return (this->clusterIndex+1);}

bool ClusterInformation::HasClusterID(long cID){
    
    for(int i=0; i<=this->clusterIndex; i++){
        
        if(this->clusterID[i] == cID){
            return true;
        }
    }
    
    return false;
}

float ClusterInformation::GetWeight(){
    short c=0,p=0;
    
    for(int i=0; i<TOTAL_CLUSTER_IDS; i++){
        
        if(this->type[i].compare("C") == 0){
            c++;
        }else if(this->type[i].compare("P") == 0){
            p++;
        }
    }
    
    return (c*CORE_POINT_WEIGHT) + (p*PERIF_WEIGHT);
    
}

float ClusterInformation::GetWeight(long clusterID){
    
    short index = -1;
    
    for(short i=0; i<TOTAL_CLUSTER_IDS; i++){
        if(this->clusterID[i] == clusterID){
            index = i;
            break;
        }
    }
    
    if(index >= 0 && index <= 4){
        if(this->type[index].compare("C") == 0){
            return (CORE_POINT_WEIGHT);
        }else if(this->type[index].compare("P") == 0){
            return (PERIF_WEIGHT);
        }
    }
    
    return -1;
}

float ClusterInformation::GetWeight(short index1, short index2){
    short c=0,p=0;
    
    if(index1 >= 0 && index1 <= this->clusterIndex){
        if(this->type[index1].compare("C") == 0){
            c++;
        }else if(this->type[index1].compare("P") == 0){
            p++;
        }
    }
    
    if(index2 >= 0 && index2 <= this->clusterIndex){
        if(this->type[index2].compare("C") == 0){
            c++;
        }else if(this->type[index2].compare("P") == 0){
            p++;
        }
    }
    
    return (c*CORE_POINT_WEIGHT) + (p*PERIF_WEIGHT);
}

void ClusterInformation::ResetClusterInformation(){
    this->status = true;
    this->clusterIndex = -1;
    
    for(int i=0; i<TOTAL_CLUSTER_IDS; i++){
        this->type[i] = "";
        this->clusterID[i] = 0;
    }
}


