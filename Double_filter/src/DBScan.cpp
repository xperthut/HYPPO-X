/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: DBScan.cpp
 Objective: Implementation of Member functions
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              03/12/2016      Initial version
 
 **************************************************************************************************/


#include "DBScan.h"
#include <stdlib.h>

DBScan::DBScan(float Eps, float minPoints, bool setUniqueId){
    this->Eps = Eps;
    this->minPoints = minPoints;
    this->phenotypeList = NULL;
    this->dataPointList = NULL;
    this->setUniqueId = setUniqueId;
    this->lastClusterId = 0;
    this->quickSort = new QuickSort();
}

DBScan::~DBScan(){
    delete quickSort;
    
    if(this->phenotypeList != NULL){
        free(this->phenotypeList);
        this->size = 0;
        this->phenotypeList = NULL;
    }
    
    if(this->dataPointList != NULL){
        free(this->dataPointList);
        this->size = 0;
        this->dataPointList = NULL;
    }
}

void DBScan::MakeCluster(list<Phenotype *> *phList){
    
    this->quickSort->Sort(phList);
    
    /*cout<<"\nPh list";
     for(list<Phenotype*>::iterator itr=phList->begin(); itr!=phList->end(); itr++){
     Phenotype* ph = *itr;
     
     cout<<ph->GetValue()<<",";
     }
     
     cout<<"\n";*/
    
    this->size = phList->size();
    
    if(this->size > 0){
        this->phenotypeList = (Phenotype**)malloc(sizeof(Phenotype*)*this->size);
        
        list<Phenotype*>::iterator itr;
        size_t count = 0;
        
        for (itr = phList->begin(); itr != phList->end(); itr++)
        {
            this->phenotypeList[count] = *itr;
            this->phenotypeList[count]->SetStatus(false);
            count++;
        }
        
        //cout<<"Total phenotype data:"<<count<<endl;
        
        phList->clear();
        
        // Place clusting code here
        this->DBSCAN();
        
        for(count = 0; count<this->size; count++){
            phList->push_back(this->phenotypeList[count]);
        }
        
        if(this->phenotypeList != NULL){
            free(this->phenotypeList);
            this->size = 0;
            this->phenotypeList = NULL;
        }
    }
}

void DBScan::MakeCluster(list<DataPoint *> *dpList){
    
    this->size = dpList->size();
    
    if(this->size > 0){
        this->dataPointList = (DataPoint**)malloc(sizeof(DataPoint*)*this->size);
        
        list<DataPoint*>::iterator itr;
        size_t count = 0;
        
        for (itr = dpList->begin(); itr != dpList->end(); itr++)
        {
            this->dataPointList[count] = *itr;
            this->dataPointList[count]->SetStatus(false);
            count++;
        }
        
        //cout<<"Total phenotype data:"<<count<<endl;
        
        dpList->clear();
        
        // Place clusting code here
        this->DBSCAN_HD();
        
        for(count = 0; count<this->size; count++){
            dpList->push_back(this->dataPointList[count]);
        }
        
        if(this->dataPointList != NULL){
            free(this->dataPointList);
            this->size = 0;
            this->dataPointList = NULL;
        }
    }
}

#pragma Private_Methods
void DBScan::DBSCAN(){
    long clusterID = 0;
    
    for(long i=0; i<this->size; i++){
        
        if(this->phenotypeList[i]->GetStatus() == false){
            
            clusterID = Cluster::GetNextID();
            
            if(CLUSTER_BY_GENLOC){
                this->ExpandCluster(i, clusterID, this->phenotypeList[i]->getGenLocWeight());
            }else{
                this->ExpandCluster(i, clusterID);
            }
        }
    }
}

void DBScan::ExpandCluster(long index, long clusterID){
    
    list<long> seeds;
    list<long>::iterator itr;
    //Node* node = new Node(clusterID);
    
    this->GetSeeds(&seeds, index);
    
    if(seeds.size() >= this->minPoints){
        
        if(!this->setUniqueId){
            this->phenotypeList[index]->SetType("C");
        }
        
        for(itr = seeds.begin(); itr != seeds.end(); itr++){
            long id = *itr;
            
            if(this->setUniqueId){
                this->phenotypeList[id]->SetUniqueId(clusterID);
                this->phenotypeList[id]->SetStatus(true);
            }else{
                this->phenotypeList[id]->AddClusterID(clusterID);
            }
        }
        
        //cout<<"Seed size:"<<seeds.size()<<endl;
        
        seeds.remove(index);
        
        long currIndex = 0;
        list<long> results;
        
        while (seeds.size() > 0) {
            itr = seeds.begin();
            
            currIndex = *itr;
            
            this->GetSeeds(&results, currIndex);
            
            if(results.size() >= this->minPoints){
                if(!this->setUniqueId){
                    this->phenotypeList[currIndex]->SetType("C");
                }
                
                for(itr = results.begin(); itr != results.end(); itr++){
                    long resultIndex = *itr;
                    
                    if(this->phenotypeList[resultIndex]->GetStatus() == false){
                        //cout<<"Seed size before push:"<<seeds.size()<<",("<<resultIndex<<")"<<endl;
                        seeds.push_back(resultIndex);
                        
                        if(this->setUniqueId){
                            this->phenotypeList[resultIndex]->SetUniqueId(clusterID);
                            this->phenotypeList[resultIndex]->SetStatus(true);
                        }else{
                            this->phenotypeList[resultIndex]->AddClusterID(clusterID);
                        }
                    }
                    
                    
                }
            }
            
            seeds.remove(currIndex);
            results.clear();
            
            //cout<<"Seed size:"<<seeds.size()<<endl;
        }
        
    }else{
        for(itr = seeds.begin(); itr != seeds.end(); itr++){
            long id = *itr;
            
            if(this->setUniqueId){
                this->phenotypeList[id]->SetUniqueId(clusterID);
                this->phenotypeList[id]->SetStatus(true);
            }else{
                this->phenotypeList[id]->AddClusterID(clusterID);
                this->phenotypeList[id]->SetType("P");
            }
        }
    }
    
    this->lastClusterId = clusterID;
    
    //return true;
}

void DBScan::GetSeeds(list<long> *phIndexList, long index){
    float value = this->phenotypeList[index]->GetValue();
    
    if(!SORT_BY_PHENOTYPE){
        value = this->phenotypeList[index]->getDAP();
    }
    
    long prevIndex = -1;
    if(index > 0) prevIndex = index-1;
    
    long nextIndex = index;
    
    float prevDiff=0, nextDiff=0;
    
    while (prevDiff <= this->Eps) {
        
        if(prevIndex >= 0){
            prevDiff = this->phenotypeList[prevIndex]->GetValue()-value;
            
            if(!SORT_BY_PHENOTYPE){
                prevDiff = this->phenotypeList[prevIndex]->getDAP()-value;
            }
            
            if(prevDiff<0) prevDiff *= -1;
            if(prevDiff <= this->Eps){
                phIndexList->push_front(prevIndex);
                prevIndex--;
            }else{
                break;
            }
        }else{
            break;
        }
    }
    
    while (nextDiff <= this->Eps) {
        
        if(nextIndex < this->size){
            nextDiff = this->phenotypeList[nextIndex]->GetValue()-value;
            
            if(!SORT_BY_PHENOTYPE){
                nextDiff = this->phenotypeList[nextIndex]->getDAP()-value;
            }
            
            if(nextDiff<0) nextDiff *= -1;
            if(nextDiff <= this->Eps){
                phIndexList->push_back(nextIndex);
                nextIndex++;
            }else{
                break;
            }
        }else{
            break;
        }
    }
}

#pragma For_gen_loc
int DBScan::ExpandCluster(long index, long clusterID, int gen_loc_weight){
    list<long> seeds;
    list<long>::iterator itr;
    int totalItem = 0;
    
    this->GetSeeds(&seeds, index, gen_loc_weight);
    totalItem = (int)seeds.size();
    
    if(seeds.size() >= this->minPoints){
        
        if(!this->setUniqueId){
            this->phenotypeList[index]->SetType("C");
        }
        
        for(itr = seeds.begin(); itr != seeds.end(); itr++){
            long id = *itr;
            
            if(this->setUniqueId){
                this->phenotypeList[id]->SetUniqueId(clusterID);
                this->phenotypeList[id]->SetStatus(true);
            }else{
                this->phenotypeList[id]->AddClusterID(clusterID);
            }
        }
        
        //cout<<"Seed size:"<<seeds.size()<<endl;
        
        seeds.remove(index);
        
        long currIndex = 0;
        list<long> results;
        
        while (seeds.size() > 0) {
            itr = seeds.begin();
            
            currIndex = *itr;
            
            /*if(this->phenotypeList[currIndex]->GetValue() > 85){
             this->GetSeeds(&results, currIndex);
             }else
             this->GetSeeds(&results, currIndex);*/
            
            this->GetSeeds(&results, currIndex, gen_loc_weight);
            
            if(results.size() >= this->minPoints){
                if(!this->setUniqueId){
                    this->phenotypeList[currIndex]->SetType("C");
                }
                
                for(itr = results.begin(); itr != results.end(); itr++){
                    long resultIndex = *itr;
                    
                    if(this->phenotypeList[resultIndex]->GetStatus() == false){
                        //cout<<"Seed size before push:"<<seeds.size()<<",("<<resultIndex<<")"<<endl;
                        seeds.push_back(resultIndex);
                        
                        if(this->setUniqueId){
                            this->phenotypeList[resultIndex]->SetUniqueId(clusterID);
                            this->phenotypeList[resultIndex]->SetStatus(true);
                        }else{
                            this->phenotypeList[resultIndex]->AddClusterID(clusterID);
                        }
                    }
                    
                    
                }
            }else{
                if(!this->setUniqueId){
                    this->phenotypeList[currIndex]->SetType("P");
                }
            }
            
            seeds.remove(currIndex);
            results.clear();
            
            //cout<<"Seed size:"<<seeds.size()<<endl;
        }
        
    }else{
        /*for(itr = seeds.begin(); itr != seeds.end(); itr++){
         long id = *itr;
         
         this->phenotypeList[id]->AddClusterID(clusterID);
         //node->AddPhenotypeToNode(this->phenotypeList[id]);
         }*/
        
        if(this->setUniqueId){
            this->phenotypeList[index]->SetUniqueId(clusterID);
            this->phenotypeList[index]->SetStatus(true);
        }else{
            this->phenotypeList[index]->AddClusterID(clusterID);
            this->phenotypeList[index]->SetType("P");
        }
    }
    
    this->lastClusterId = clusterID;
    
    return totalItem;
}

void DBScan::GetSeeds(list<long> *phIndexList, long index, int gen_loc_weight){
    float value = this->phenotypeList[index]->GetValue();
    
    if(!SORT_BY_PHENOTYPE){
        value = this->phenotypeList[index]->getDAP();
    }
    
    long prevIndex = -1;
    if(index > 0) prevIndex = index-1;
    
    long nextIndex = index;
    
    float prevDiff=0, nextDiff=0;
    
    while (prevDiff <= this->Eps) {
        
        if(prevIndex >= 0){
            prevDiff = this->phenotypeList[prevIndex]->GetValue()-value;
            
            if(!SORT_BY_PHENOTYPE){
                prevDiff = this->phenotypeList[prevIndex]->getDAP()-value;
            }
            
            if(prevDiff<0) prevDiff *= -1;
            if(prevDiff <= this->Eps){
                int tmpGenLocWeight = this->phenotypeList[prevIndex]->getGenLocWeight();
                
                if(tmpGenLocWeight==gen_loc_weight)
                    phIndexList->push_front(prevIndex);
                
                prevIndex--;
            }else{
                break;
            }
        }else{
            break;
        }
    }
    
    while (nextDiff <= this->Eps) {
        
        if(nextIndex < this->size){
            nextDiff = this->phenotypeList[nextIndex]->GetValue()-value;
            
            if(!SORT_BY_PHENOTYPE){
                nextDiff = this->phenotypeList[nextIndex]->getDAP()-value;
            }
            
            if(nextDiff<0) nextDiff *= -1;
            if(nextDiff <= this->Eps){
                int tmpGenLocWeight = this->phenotypeList[nextIndex]->getGenLocWeight();
                
                if(tmpGenLocWeight==gen_loc_weight)
                    phIndexList->push_front(nextIndex);
                
                nextIndex++;
            }else{
                break;
            }
        }else{
            break;
        }
    }
}

#pragma For_hamming_distance

void DBScan::DBSCAN_HD(){
    long clusterID = 0;
    
    for(long i=0; i<this->size; i++){
        
        if(this->dataPointList[i]->GetStatus() == false){
            
            clusterID = Cluster::GetNextID();
            
            this->ExpandCluster_HD(i, clusterID);
        }
    }
}

void DBScan::ExpandCluster_HD(long index, long clusterID){
    
    list<long> seeds;
    list<long>::iterator itr;
    //Node* node = new Node(clusterID);
    
    this->GetSeeds_HD(&seeds, index);
    
    if(this->lastClusterId != clusterID || seeds.size() >= this->minPoints){
        
        if(!this->setUniqueId){
            this->dataPointList[index]->SetType("C");
        }
        
        for(itr = seeds.begin(); itr != seeds.end(); itr++){
            long id = *itr;
            
            if(this->setUniqueId){
                this->dataPointList[id]->SetUniqueId(clusterID);
                this->dataPointList[id]->SetStatus(true);
            }else{
                this->dataPointList[id]->AddClusterID(clusterID);
            }
        }
        
        //cout<<"Seed size:"<<seeds.size()<<endl;
        
        seeds.remove(index);
        
        long currIndex = 0;
        list<long> results;
        
        while (seeds.size() > 0) {
            itr = seeds.begin();
            
            currIndex = *itr;
            
            /*if(this->phenotypeList[currIndex]->GetValue() > 85){
             this->GetSeeds(&results, currIndex);
             }else
             this->GetSeeds(&results, currIndex);*/
            
            this->GetSeeds_HD(&results, currIndex);
            
            if(this->lastClusterId != clusterID || results.size() >= this->minPoints){
                if(!this->setUniqueId){
                    this->dataPointList[currIndex]->SetType("C");
                }
                
                for(itr = results.begin(); itr != results.end(); itr++){
                    long resultIndex = *itr;
                    
                    if(this->dataPointList[resultIndex]->GetStatus() == false){
                        //cout<<"Seed size before push:"<<seeds.size()<<",("<<resultIndex<<")"<<endl;
                        seeds.push_back(resultIndex);
                        
                        if(this->setUniqueId){
                            this->dataPointList[resultIndex]->SetUniqueId(clusterID);
                            this->dataPointList[resultIndex]->SetStatus(true);
                        }else{
                            this->dataPointList[resultIndex]->AddClusterID(clusterID);
                        }
                    }
                    
                    
                }
            }else{
                if(!this->setUniqueId){
                    this->dataPointList[currIndex]->SetType("P");
                }
            }
            
            seeds.remove(currIndex);
            results.clear();
            
            //cout<<"Seed size:"<<seeds.size()<<endl;
        }
        
    }else{
        /*for(itr = seeds.begin(); itr != seeds.end(); itr++){
         long id = *itr;
         
         this->phenotypeList[id]->AddClusterID(clusterID);
         //node->AddPhenotypeToNode(this->phenotypeList[id]);
         }*/
        
        if(this->setUniqueId){
            this->dataPointList[index]->SetUniqueId(clusterID);
            this->dataPointList[index]->SetStatus(true);
        }else{
            this->dataPointList[index]->AddClusterID(clusterID);
            this->dataPointList[index]->SetType("P");
        }
    }
    
    this->lastClusterId = clusterID;
    
    //return true;
}

void DBScan::GetSeeds_HD(list<long> *phIndexList, long index){
    //float value = this->dataPointList[index]->GetValue();
    DataPoint* dp1 = this->dataPointList[index];
    
    long prevIndex = -1;
    if(index > 0) prevIndex = index-1;
    
    long nextIndex = index;
    
    float prevDiff=0, nextDiff=0;
    
    while (fabsf(prevDiff) <= this->Eps) {
        
        if(prevIndex >= 0){
            DataPoint* dp2 = this->dataPointList[prevIndex];
            prevDiff = dp1->HammingDistance(dp2);
            //if(prevDiff<0) prevDiff *= -1;
            if(fabsf(prevDiff) <= this->Eps){
                phIndexList->push_front(prevIndex);
                prevIndex--;
            }//else{
            // break;
            //}
        }else{
            break;
        }
    }
    
    while (fabsf(nextDiff) <= this->Eps) {
        
        if(nextIndex < this->size){
            DataPoint* dp2 = this->dataPointList[nextIndex];
            nextDiff = dp1->HammingDistance(dp2);
            //if(nextDiff<0) nextDiff *= -1;
            if(fabsf(nextDiff) <= this->Eps){
                phIndexList->push_back(nextIndex);
                nextIndex++;
            }//else{
            // break;
            //}
        }else{
            break;
        }
    }
}


