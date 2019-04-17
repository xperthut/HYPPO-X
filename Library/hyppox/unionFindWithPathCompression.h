
/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: unionFindWithPathCompression.h
 Objective: Class to find connected components in a graph
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              07/16/2016      Initial version
 
 **************************************************************************************************/

#ifndef unionFindWithPathCompression_h
#define unionFindWithPathCompression_h

#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>

namespace hyppox{
    namespace graph{
        // Here template class IDType will be a signed data type
        template <typename IDType>
        class UnionFindWithPathCompression{
            private:
            IDType totalIds;
            std::vector<IDType> s;
            
            void UnionSets(IDType rootA, IDType rootB);
            IDType Find(IDType _id);
            void Adjust();
            
            public:
            using ccSetMap = std::unordered_map<IDType, std::unordered_set<IDType> >;
            using ccMap = std::unordered_map<IDType, IDType>;
            
            UnionFindWithPathCompression(IDType totalIds);
            ~UnionFindWithPathCompression();
            void Union(IDType idA, IDType idB);
            
            void PrintArray();
            void PrintForest();
            ccSetMap GetAllConnectedComponents(){
                this->Adjust();
                
                ccSetMap scc;
                IDType index = 0;
                
                std::unordered_set<IDType> treeRoots;
                
                for(IDType i=0; i<this->totalIds; i++){
                    if(s[i] < -1)
                    treeRoots.insert(i);
                }
                
                for(auto itr = treeRoots.begin(); itr != treeRoots.end(); itr++){
                    IDType ide = *itr;
                    
                    std::unordered_set<IDType> subGraphs;
                    subGraphs.insert(ide);
                    
                    for(IDType i=0; i<this->totalIds; i++){
                        if(s[i] == ide)
                        subGraphs.insert(i);
                    }
                    
                    scc.insert(make_pair(index, subGraphs));
                    index++;
                }
                
                return scc;
            }
            
            ccMap getConnectedComponents(size_t &ccSize){
                this->Adjust();
                
                ccMap scc;
                size_t index = 0;
                
                std::unordered_set<IDType> treeRoots;
                
                for(IDType i=0; i<this->totalIds; i++){
                    if(s[i] < -1)
                    treeRoots.insert(i);
                }
                
                for(auto itr = treeRoots.begin(); itr != treeRoots.end(); itr++){
                    IDType ide = *itr;
                    
                    scc.insert(std::make_pair(ide, index));
                    
                    for(IDType i=0; i<this->totalIds; i++){
                        if(s[i] == ide){
                            scc.insert(std::make_pair(i, index));
                        }
                    }
                    
                    index++;
                }
                
                ccSize = index;
                
                return scc;
            }
        };
        
        template <typename IDType>
        UnionFindWithPathCompression<IDType>::UnionFindWithPathCompression(IDType totalIds){
            this->totalIds = totalIds+1;
            
            this->s.assign(this->totalIds, -1);
            
            for(IDType i=0; i<this->totalIds; i++){
                this->s[i] = -1;
            }
        }
        
        template <typename IDType>
        UnionFindWithPathCompression<IDType>::~UnionFindWithPathCompression(){}
        
        template <typename IDType>
        void UnionFindWithPathCompression<IDType>::Union(IDType idA, IDType idB){
            this->UnionSets(this->Find(idA), this->Find(idB));
        }
        
        template <typename IDType>
        void UnionFindWithPathCompression<IDType>::PrintArray(){
            for(IDType i=0; i<this->totalIds; i++){
                if(s[i] != -1)
                std::cout<<i<<"=>"<<this->s[i]<<std::endl;
            }
        }
        
        template <typename IDType>
        void UnionFindWithPathCompression<IDType>::PrintForest(){
            
            this->Adjust();
            
            std::unordered_set<IDType> treeRoots;
            
            for(IDType i=0; i<this->totalIds; i++){
                if(s[i] < -1)
                treeRoots.insert(i);
            }
            
            for(auto itr = treeRoots.begin(); itr != treeRoots.end(); itr++){
                IDType ide = *itr;
                
                std::cout<<"Root: "<<ide<<"\n";
                
                for(IDType i=0; i<this->totalIds; i++){
                    if(s[i] == ide)
                    std::cout<<i<<",";
                }
                
                std::cout<<"\n";
            }
            
            std::cout<<"\n";
        }
        
        //#pragma Private_Methods
        template <typename IDType>
        void UnionFindWithPathCompression<IDType>::UnionSets(IDType rootA, IDType rootB){
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
        
        template <typename IDType>
        IDType UnionFindWithPathCompression<IDType>::Find(IDType ide){
            if(this->s[ide]<0){
                return ide;
            }
            else{
                return (this->s[ide] = this->Find(this->s[ide]));
            }
        }
        
        template <typename IDType>
        void UnionFindWithPathCompression<IDType>::Adjust(){
            for(IDType i=0; i<this->totalIds; i++){
                if(this->s[i] != -1)
                this->Find(i);
            }
            
        }
    }
}

#endif /* UnionFindWithPathCompression_h */
