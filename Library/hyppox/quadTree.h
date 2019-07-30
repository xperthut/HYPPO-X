
/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: quadTree.h
 Objective: Class holds data of a quad tree
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              02/21/2016      Initial version
 
 **************************************************************************************************/

#ifndef quadTree_h
#define quadTree_h

#include <iostream>
#include "quadNode.h"
#include <unordered_set>
#include <vector>

namespace hyppox {
    namespace quadtree{
        template<typename PerfType, typename PosType, typename NodePosType>
        class QuadTree{
            public:
            //using mapper::DataPoint<PerfType,PosType> = mapper::DataPoint<PerfType,PosType>;
            //using QuadNode<mapper::DataPoint<PerfType,PosType>,NodePosType> = QuadNode<mapper::DataPoint<PerfType,PosType>,NodePosType>;
            //using QTType = QuadTree<PerfType,PosType,NodePosType>;
            
            QuadTree();
            QuadTree(std::vector<NodePosType> _start, std::vector<NodePosType> _len);
            ~QuadTree();
            QuadNode<mapper::DataPoint<PerfType,PosType>,NodePosType>* GetRoot();
            void setRoot(std::vector<NodePosType> _start, std::vector<NodePosType> _len);
            void AddDataToNode(mapper::DataPoint<PerfType,PosType>* data);
            void PrintQuadTree();
            void SearchSurface(std::vector<NodePosType> min, std::vector<NodePosType> max, std::list<PerfType> *phList, std::list<mapper::DataPoint<PerfType,PosType>*> *dpList, size_t boxID);
            void SearchDataPoints(std::vector<NodePosType> min, std::vector<NodePosType> max, std::unordered_set<size_t> *boxIdList);
            void SearchAndGetDataPointOfBoxId(std::vector<NodePosType> min, std::vector<NodePosType> max, size_t boxID);
            void ResetClusterInformation();
            
            private:
            QuadNode<mapper::DataPoint<PerfType,PosType>,NodePosType>* root;
            
            int PrintNode(QuadNode<mapper::DataPoint<PerfType,PosType>,NodePosType>* node);
            // Has error here, do not use for now
            int SearchSurfaceInNode(QuadNode<mapper::DataPoint<PerfType,PosType>,NodePosType> *node, std::vector<NodePosType> min, std::vector<NodePosType> max, std::list<PerfType> *phList, std::list<mapper::DataPoint<PerfType,PosType>*> *dpList, size_t boxID);
            // Use this
            int SearchDataInSerface(QuadNode<mapper::DataPoint<PerfType,PosType>,NodePosType> *node, std::vector<NodePosType> min, std::vector<NodePosType> max, std::list<PerfType> *phList, std::list<mapper::DataPoint<PerfType,PosType>*> *dpList, size_t boxID, std::unordered_set<size_t>*& pointSet);
            int SearchDataPoint(QuadNode<mapper::DataPoint<PerfType,PosType>,NodePosType> *node, std::vector<NodePosType> min, std::vector<NodePosType> max, std::unordered_set<size_t> *boxIdList);
            int ResetClusterInformation(QuadNode<mapper::DataPoint<PerfType,PosType>,NodePosType>* node);
            std::vector<bool> CheckMembership(QuadNode<mapper::DataPoint<PerfType,PosType>,NodePosType> *node, std::vector<NodePosType> min, std::vector<NodePosType> max);
            
        };
        
        template<typename PerfType, typename PosType, typename NodePosType>
        QuadTree<PerfType,PosType,NodePosType>::QuadTree(){
            this->root = nullptr;
        }
        
        template<typename PerfType, typename PosType, typename NodePosType>
        QuadTree<PerfType,PosType,NodePosType>::QuadTree(std::vector<NodePosType> _start, std::vector<NodePosType> _len){
            this->root = new QuadNode<mapper::DataPoint<PerfType,PosType>,NodePosType>(_start, _len);
        }
        
        template<typename PerfType, typename PosType, typename NodePosType>
        QuadTree<PerfType,PosType,NodePosType>::~QuadTree(){
            delete this->root;
        }
        
        //#pragma Public_Methods
        template<typename PerfType, typename PosType, typename NodePosType>
        void QuadTree<PerfType,PosType,NodePosType>::AddDataToNode(mapper::DataPoint<PerfType,PosType>* data){
            this->root->AddDataToNode(data);
        }
        
        template<typename PerfType, typename PosType, typename NodePosType>
        void QuadTree<PerfType,PosType,NodePosType>::PrintQuadTree(){
            this->PrintNode(this->root);
        }
        
        template<typename PerfType, typename PosType, typename NodePosType>
        void QuadTree<PerfType,PosType,NodePosType>::ResetClusterInformation(){
            this->ResetClusterInformation(this->root);
        }
        
        template<typename PerfType, typename PosType, typename NodePosType>
        QuadNode<mapper::DataPoint<PerfType,PosType>,NodePosType>* QuadTree<PerfType,PosType,NodePosType>::GetRoot(){return this->root;}
        
        template<typename PerfType, typename PosType, typename NodePosType>
        void QuadTree<PerfType,PosType,NodePosType>::setRoot(std::vector<NodePosType> _start, std::vector<NodePosType> _len){
            this->root = new QuadNode<mapper::DataPoint<PerfType,PosType>,NodePosType>(_start, _len);
        }
        
        template<typename PerfType, typename PosType, typename NodePosType>
        void QuadTree<PerfType,PosType,NodePosType>::SearchSurface(std::vector<NodePosType> min, std::vector<NodePosType> max, std::list<PerfType> *phList, std::list<mapper::DataPoint<PerfType,PosType>*> *dpList, size_t boxID){
            
            //this->SearchSurfaceInNode(this->root, min, max, phList, dpList, boxID);
            
            std::unordered_set<size_t> pointSet, *ptSet = &pointSet;
            this->SearchDataInSerface(this->root, min, max, phList, dpList, boxID, ptSet);
        }
        
        template<typename PerfType, typename PosType, typename NodePosType>
        void QuadTree<PerfType,PosType,NodePosType>::SearchDataPoints(std::vector<NodePosType> min, std::vector<NodePosType> max, std::unordered_set<size_t> *boxIdList){
            
            this->SearchDataPoint(this->root, min, max, boxIdList);
            
        }
        
        //#pragma Private_Methods
        template<typename PerfType, typename PosType, typename NodePosType>
        int QuadTree<PerfType,PosType,NodePosType>::PrintNode(QuadNode<mapper::DataPoint<PerfType,PosType>,NodePosType> *node){
            if(node == nullptr) return 0;
            
            mapper::DataPoint<PerfType,PosType>* d = node->GetNodeData();
            
            if(d==nullptr){
                for(int i=0; i<hyppox::Config::QUAD_TREE_CHILDREN; i++){
                    this->PrintNode(node->GetChild(i));
                }
            }else {
                std::cout<<"["<<d->getPosition(0)<<","<<d->getPosition(1)<<"]=>"<<d->getBoxId()<<"\n";
            }
            
            return 0;
        }
        
        template<typename PerfType, typename PosType, typename NodePosType>
        int QuadTree<PerfType,PosType,NodePosType>::ResetClusterInformation(QuadNode<mapper::DataPoint<PerfType,PosType>,NodePosType> *node){
            if(node == nullptr) return 0;
            
            mapper::DataPoint<PerfType,PosType>* d = node->GetNodeData();
            
            if(d==nullptr){
                for(int i=0; i<hyppox::Config::QUAD_TREE_CHILDREN; i++){
                    this->ResetClusterInformation(node->GetChild(i));
                }
            }else {
                std::list<PerfType>* pL = &d->getPhList();
                
                for (auto itr = pL->begin(); itr != pL->end(); itr++)
                {
                    (*itr)->ResetClusterInformation();
                }
            }
            
            return 0;
        }
        
        // Check whether any corner of a cube is inside the search region or
        // Any corner of the search cube is inside the node cube
        // Also check whether all the corners of a node inside the search cube or not
        template<typename PerfType, typename PosType, typename NodePosType>
        std::vector<bool> QuadTree<PerfType,PosType,NodePosType>::CheckMembership(QuadNode<mapper::DataPoint<PerfType,PosType>,NodePosType> *node, std::vector<NodePosType> min, std::vector<NodePosType> max){
            
            std::vector<NodePosType> _v(hyppox::Config::FILTER, 0.0);
            bool allCorner = true, anyCorner=false;
            std::vector<NodePosType> _start(hyppox::Config::FILTER, 0.0), _end(hyppox::Config::FILTER, 0.0);
            
            for(short j=0;j<hyppox::Config::FILTER; j++){
                _start[j] = node->GetNodePosition(j);
                _end[j] = _start[j] + node->GetLength(j);
            }
            
            for(unsigned int i=0; i<hyppox::Config::QUAD_TREE_CHILDREN; i++){
                unsigned int k = i;
                bool nodeCorner = true, searchCorner = true;
                // For each position identify the bit 1 position, make that position
                for(short j=0;j<hyppox::Config::FILTER; j++,k>>=1){
                    if(k&1){
                        nodeCorner &= min[j]<=_end[j] && _end[j]<=max[j];
                        searchCorner &= _start[j]<=max[j]&&max[j]<=_end[j];
                    }else{
                        nodeCorner &= min[j]<=_start[j] && _start[j]<=max[j];
                        searchCorner &= _start[j]<=min[j]&&min[j]<=_end[j];
                    }
                }
                
                anyCorner |= nodeCorner||searchCorner;
                allCorner &= nodeCorner;
                
                if(anyCorner && !allCorner) break;
            }
            
            return {anyCorner, allCorner};
        }
        
        // Need to check, has error
        template<typename PerfType, typename PosType, typename NodePosType>
        int QuadTree<PerfType,PosType,NodePosType>::SearchSurfaceInNode(QuadNode<mapper::DataPoint<PerfType,PosType>,NodePosType> *node, std::vector<NodePosType> min, std::vector<NodePosType> max, std::list<PerfType> *phList, std::list<mapper::DataPoint<PerfType,PosType>*> *dpList, size_t boxID){
            if(node == nullptr) return 0;
            if(node->GetNodeData()!=nullptr){
                mapper::DataPoint<PerfType,PosType>* d = node->GetNodeData();
                
                bool _m = true;
                for(short i=0;i<hyppox::Config::FILTER;i++){
                    _m &= min[i]<=d->getPosition(i) && d->getPosition(i)<=max[i];
                }
                
                if(_m){
                    if(boxID > 0 && d->getBoxId()==0){
                        d->setBoxId(boxID);
                    }
                    
                    dpList->push_back(d);
                    std::list<PerfType> pL = d->getPhList();
                    
                    if(pL.size()>0){
                        phList->insert(phList->end(), pL.begin(), pL.end());
                    }
                }
                
                return 0;
            }
            
            std::vector<bool> status = this->CheckMembership(node, min, max);
            
            // If all corners of a node is inside the search cube,
            // Collect all data belongs in the node at its subsequent child
            if(status[1]){
                return this->SearchDataInSerface(node, min, max, phList, dpList, boxID);
            }
            // If any corner belongs in the search cube then search for each child
            else if(status[0]){
                for(unsigned int i=0; i<hyppox::Config::QUAD_TREE_CHILDREN; i++){
                    this->SearchSurfaceInNode(node->GetChild(i), min, max, phList, dpList, boxID);
                }
            }
            
            return 0;
        }
        
        template<typename PerfType, typename PosType, typename NodePosType>
        int QuadTree<PerfType,PosType,NodePosType>::SearchDataInSerface(QuadNode<mapper::DataPoint<PerfType,PosType>,NodePosType> *node, std::vector<NodePosType> min, std::vector<NodePosType> max, std::list<PerfType> *phList, std::list<mapper::DataPoint<PerfType,PosType>*> *dpList, size_t boxID, std::unordered_set<size_t>*& pointSet){
            if(node == nullptr) return 0;
            
            if(node->GetNodeData()!=nullptr){
                mapper::DataPoint<PerfType,PosType>* d = node->GetNodeData();
                
                bool _m = true;
                for(short i=0;i<hyppox::Config::FILTER;i++){
                    _m &= min[i]<=d->getPosition(i) && d->getPosition(i)<=max[i];
                }
                
                if(_m){
                    if(boxID > 0 && d->getBoxId()==0){
                        d->setBoxId(boxID);
                    }
                    
                    dpList->push_back(d);
                    std::list<PerfType> pL = d->getPhList();
                    
                    if(pL.size()>0){
                        for(auto itr = pL.begin(); itr!=pL.end(); itr++){
                            PerfType ph = *itr;
                            if(pointSet->find(ph->getID()) == pointSet->end()){
                                // Added this after investigating that during clustering step if any point clusterd earlier then is not being considered for reclustering
                                (*itr)->SetStatus(false);
                                /// end here
                                pointSet->insert(ph->getID());
                                phList->push_back(*itr);
                            }
                        }
                        //phList->insert(phList->end(), pL.begin(), pL.end());
                    }
                }
                
                return 0;
            }
            
            for(unsigned int i=0; i<hyppox::Config::QUAD_TREE_CHILDREN; i++){
                this->SearchDataInSerface(node->GetChild(i), min, max, phList, dpList, boxID, pointSet);
            }
            
            return 0;
        }
        
        template<typename PerfType, typename PosType, typename NodePosType>
        int QuadTree<PerfType,PosType,NodePosType>::SearchDataPoint(QuadNode<mapper::DataPoint<PerfType,PosType>,NodePosType> *node, std::vector<NodePosType> min, std::vector<NodePosType> max, std::unordered_set<size_t> *boxIdList){
            if(node == nullptr) return 0;
            
            for(unsigned int i=0; i<hyppox::Config::QUAD_TREE_CHILDREN; i++){
                this->SearchDataPoint(node->GetChild(i), min, max, boxIdList);
            }
            
            mapper::DataPoint<PerfType,PosType>* d = node->GetNodeData();
            
            if(d != nullptr){
                
                bool m = true;
                for(short i=0;i<hyppox::Config::FILTER;i++){
                    m &= min[i]<=d->getPosition(i) && d->getPosition(i)<=max[i];
                }
                
                if(m){
                    boxIdList->insert(d->getBoxId());
                }
            }
            
            return 0;
            
        }
    }
}


#endif /* QuadTree_h */
