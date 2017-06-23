
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: SimplicialComplex.cpp
 Objective: Implement member functions
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              08/17/2016      Initial version
 Md. Kamruzzaman              04/11/2017      Implement bit logic to create simplex
 
 **************************************************************************************************/

#include "SimplicialComplex.h"

SimplicialComplex::SimplicialComplex(){}

SimplicialComplex::~SimplicialComplex(){}

void SimplicialComplex::AddSimplicialComplex(Phenotype *ph, float &filtration){
    if(ph!=NULL){
        string indv = ph->getIndividualId();
        
        // New individual
        if(indvTracker.find(indv)==indvTracker.end()){
            long idList[TOTAL_CLUSTER_IDS]={0};
            
            ph->GetIDList(idList);
            list<long> ids;
            
            for(short i=0; i<TOTAL_CLUSTER_IDS; i++){
                if(idList[i]>0){
                    ids.push_back(idList[i]);
                }
            }
            
            CreateSimplex(&ids, filtration);
        }
    }
}

void SimplicialComplex::CreateSimplex(list<long> *idList, float &filtration){
    long k = idList->size();
    
    if(k>0){
        // Sort in ascending order
        idList->sort();
        
        bitset<BIT_SET_SIZE> idBitSet;
        long max = pow(2, k)-1;
        unordered_map<string, float>::iterator sItr;
        
        for(long i=1; i<=max; i++){
            idBitSet = i;
            
            
            // If has at least one 1
            if(idBitSet.count()>0){
                
                // Made key with all 1 position ids
                string key = "";
                for(long j=0;j<k; j++){
                    
                    // Go to the bit position, similar to the id position in the list
                    if(idBitSet[j]==1){
                        
                        long tmp=0;
                        for(list<long>::iterator itr=idList->begin(); itr!=idList->end(); itr++){
                            
                            // Match the position of list to the bit set
                            if(j == tmp){
                                if(key.length()>0) key += ",";
                                key += to_string(*itr);
                                
                                break;
                            }
                            
                            tmp++;
                        }
                    }
                }
                
                if((sItr=simplexFiltrationMap.find(key)) == simplexFiltrationMap.end()){
                    simplexFiltrationMap.insert(pair<string, float>(key, filtration));
                    AddToSimplexMap(key, filtration);
                }else{
                    if(sItr->second == 0.00 && filtration > 0){
                        sItr->second = filtration;
                        AddToSimplexMap(key, filtration);
                    }
                }
                
                
                
            }
        }
    }
}

void SimplicialComplex::AddToSimplexMap(string key, float &filtration){
    string simplexText = "";
    map<float, SimplexList>::iterator sitr;
    
    if(key.find(",")==string::npos){
        if(filtration==0)
            simplexText = "stream.addVertex(" + key + ", " + to_string(filtration).substr(0,5) + ");";
    }else{
        simplexText = "stream.addElement(new int[]{" + key + "},  " + to_string(filtration).substr(0,5) + ");";
    }
    
    if(keyTracker.find(simplexText) == keyTracker.end()){
        keyTracker.insert(simplexText);
        
        if((sitr=this->simplexMap.find(filtration)) == this->simplexMap.end()){
            SimplexList sl;
            sl.slist.push_back(simplexText);
            
            this->simplexMap.insert(pair<float, SimplexList>(filtration, sl));
        }else{
            sitr->second.slist.push_back(simplexText);
        }
    }
}

void SimplicialComplex::clearTracker(){indvTracker.clear();}

string SimplicialComplex::PrintSimplex(){
    string simplex = "";
    
    
    
    for(map<float, SimplexList>::iterator sitr = this->simplexMap.begin(); sitr != this->simplexMap.end(); sitr++){
        if(simplex.length()>0) simplex += "\n";
        
        simplex += sitr->second.GetAllData();
    }
    
    simplex += "\n";
    
    
    return simplex;
}


//#pragma DEPRICATED_METHODS

void SimplicialComplex::AddZeroSimplex(long node, float filtrationValue){
    
    unordered_map<long, float>::iterator itr;
    
    if((itr = this->zeroSimplex.find(node)) == this->zeroSimplex.end()){
        this->zeroSimplex.insert(pair<long, float>(node, filtrationValue));
    }else{
        if(itr->second == 0.00 && filtrationValue > 0){
            itr->second = filtrationValue;
        }
    }
}

void SimplicialComplex::AddOneSimplex(long node1, long node2, float filtrationValue){
    list<long> nodeList;
    nodeList.push_back(node1);
    nodeList.push_back(node2);
    
    nodeList.sort();
    list<long>::iterator nitr;
    
    string nList = "";
    for(nitr = nodeList.begin(); nitr != nodeList.end(); nitr++){
        long id = *nitr;
        
        if(nList.length() > 0) nList += ",";
        
        nList += to_string(id);
    }
    
    this->AddToOneOrMoreSimplex(nList, filtrationValue);
}

void SimplicialComplex::AddTwoSimplex(long node1, long node2, long node3, float filtrationValue){
    list<long> nodeList;
    nodeList.push_back(node1);
    nodeList.push_back(node2);
    nodeList.push_back(node3);
    
    nodeList.sort();
    list<long>::iterator nitr;
    
    string nList = "";
    for(nitr = nodeList.begin(); nitr != nodeList.end(); nitr++){
        long id = *nitr;
        
        if(nList.length() > 0) nList += ",";
        
        nList += to_string(id);
    }
    
    this->AddToOneOrMoreSimplex(nList, filtrationValue);
}

void SimplicialComplex::AddThreeSimplex(long node1, long node2, long node3, long node4, float filtrationValue){
    list<long> nodeList;
    nodeList.push_back(node1);
    nodeList.push_back(node2);
    nodeList.push_back(node3);
    nodeList.push_back(node4);
    
    nodeList.sort();
    list<long>::iterator nitr;
    
    string nList = "";
    for(nitr = nodeList.begin(); nitr != nodeList.end(); nitr++){
        long id = *nitr;
        
        if(nList.length() > 0) nList += ",";
        
        nList += to_string(id);
    }
    
    this->AddToOneOrMoreSimplex(nList, filtrationValue);
}

string SimplicialComplex::PrintSimplex_depricated(){
    string simplex = "";
    
    unordered_map<long, float>::iterator zeroItr;
    unordered_map<string, float>::iterator itr;
    map<float, SimplexList>::iterator sitr;
    
    for(zeroItr=this->zeroSimplex.begin(); zeroItr != this->zeroSimplex.end(); zeroItr++){
        //if(simplex.length()>0) simplex += "\n";
        
        //simplex += "stream.addVertex(" + to_string(zeroItr->first) + ", " + to_string(zeroItr->second).substr(0,5) + ");";
        
        string str = "stream.addVertex(" + to_string(zeroItr->first) + ", " + to_string(zeroItr->second).substr(0,5) + ");";
        
        sitr = this->simplexMap.find(zeroItr->second);
        if(sitr == this->simplexMap.end()){
            SimplexList sl;
            sl.slist.push_back(str);
            
            this->simplexMap.insert(pair<float, SimplexList>(zeroItr->second, sl));
        }else{
            sitr->second.slist.push_back(str);
        }
    }
    
    for(itr = this->oneOrMoreSimplex.begin(); itr != this->oneOrMoreSimplex.end(); itr++){
        //if(simplex.length()>0) simplex += "\n";
        
        //simplex += "stream.addElement(new int[]{" + itr->first + "},  " + to_string(itr->second).substr(0,5) + ");";
        
        string str = "stream.addElement(new int[]{" + itr->first + "},  " + to_string(itr->second).substr(0,5) + ");";
        
        sitr = this->simplexMap.find(itr->second);
        if(sitr == this->simplexMap.end()){
            SimplexList sl;
            sl.slist.push_back(str);
            
            this->simplexMap.insert(pair<float, SimplexList>(itr->second, sl));
        }else{
            sitr->second.slist.push_back(str);
        }
    }
    
    for(sitr = this->simplexMap.begin(); sitr != this->simplexMap.end(); sitr++){
        if(simplex.length()>0) simplex += "\n";
        
        simplex += sitr->second.GetAllData();
    }
    
    simplex += "\n";
    
    
    return simplex;
}


//#pragma Private_Methods
void SimplicialComplex::AddToOneOrMoreSimplex(string nodeList, float filtrationValue){
    unordered_map<string, float>::iterator itr;
    
    if((itr = this->oneOrMoreSimplex.find(nodeList)) == this->oneOrMoreSimplex.end()){
        this->oneOrMoreSimplex.insert(pair<string, float>(nodeList, filtrationValue));
    }else{
        if(itr->second == 0.00 && filtrationValue > 0){
            itr->second = filtrationValue;
        }
    }
}
