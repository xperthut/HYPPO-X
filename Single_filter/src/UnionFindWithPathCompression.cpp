
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: UnionFindWithPathCompression.cpp
 Objective: Impmementation of member functions
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              07/16/2016      Initial version
 
 **************************************************************************************************/

#include "UnionFindWithPathCompression.h"
#include <stdlib.h>
#include <set>

UnionFindWithPathCompression::UnionFindWithPathCompression(long totalIds){
    this->totalIds = totalIds+1;
    
    this->s = (long*)malloc(sizeof(long)*this->totalIds);
    
    for(long i=0; i<this->totalIds; i++){
        this->s[i] = -1;
    }
}

UnionFindWithPathCompression::~UnionFindWithPathCompression(){
    free(this->s);
}

void UnionFindWithPathCompression::Union(long idA, long idB){
    this->UnionSets(this->Find(idA), this->Find(idB));
}

void UnionFindWithPathCompression::PrintArray(){
    for(long i=0; i<this->totalIds; i++){
        if(s[i] != -1)
            cout<<i<<"=>"<<this->s[i]<<endl;
    }
}

void UnionFindWithPathCompression::PrintForest(){
    
    this->Adjust();
    
    set<long> treeRoots;
    set<long>::iterator itr;
    
    for(long i=0; i<this->totalIds; i++){
        if(s[i] < -1)
            treeRoots.insert(i);
    }
    
    for(itr = treeRoots.begin(); itr != treeRoots.end(); itr++){
        long ide = *itr;
        
        cout<<"Root: "<<ide<<"\n";
        
        for(long i=0; i<this->totalIds; i++){
            if(s[i] == ide)
                cout<<i<<",";
        }
        
        cout<<"\n";
    }
    
    cout<<"\n";
}

unordered_map<long, list<long> > UnionFindWithPathCompression::GetAllConnectedComponents(){
    
    this->Adjust();
    
    unordered_map<long, list<long> > scc;
    long index = 0;
    
    set<long> treeRoots;
    set<long>::iterator itr;
    
    for(long i=0; i<this->totalIds; i++){
        if(s[i] < -1)
            treeRoots.insert(i);
    }
    
    for(itr = treeRoots.begin(); itr != treeRoots.end(); itr++){
        long ide = *itr;
        
        list<long> subGraphs;
        subGraphs.push_front(ide);
        
        for(long i=0; i<this->totalIds; i++){
            if(s[i] == ide)
                subGraphs.push_front(i);
        }
        
        scc.insert(pair<long, list<long> >(index, subGraphs));
        index++;
    }
    
    return scc;
}

//#pragma Private_Methods
void UnionFindWithPathCompression::UnionSets(long rootA, long rootB){
    if(rootA != rootB){
        if(this->s[rootB]<this->s[rootA]){
            this->s[rootA] = rootB;
        }else{
            if(this->s[rootB] == this->s[rootA]){
                this->s[rootA]--;
            }
            
            this->s[rootB] = rootA;
        }
    }
}

long UnionFindWithPathCompression::Find(long ide){
    if(this->s[ide]<0){
        return ide;
    }
    else{
        return (this->s[ide] = this->Find(this->s[ide]));
    }
}

void UnionFindWithPathCompression::Adjust(){
    for(long i=0; i<this->totalIds; i++){
        if(this->s[i] != -1)
            this->Find(i);
    }
    
}
