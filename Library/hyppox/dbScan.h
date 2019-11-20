
/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: dbScan.h
 Objective: Implementation of density based clustering DBSCAN algo
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              03/12/2016      Initial version
 
 **************************************************************************************************/

#ifndef dbScan_h
#define dbScan_h

#include "cluster.h"
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <stdlib.h>
#include <fstream>

#define MIN2(A, B) (A>B?B:A)
#define NOISE -1

template<typename PerfType>
bool comparePID(PerfType* p1, PerfType* P2){
    return p1->getID()<P2->getID();
}

template<typename PerfType>
bool comparePerf(PerfType* p1, PerfType* P2){
    return p1->getClusterValue(0)<P2->getClusterValue(0);
}

namespace hyppox {
    namespace cluster{
        template<typename PerfType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        class DBScan:public Cluster<PerfType,ClusterIDType>{
            public:
            DBScan(float Eps, float minPoints, bool setUniqueId){
                this->Eps = Eps;
                this->minPoints = minPoints;
                this->setUniqueId = setUniqueId;
                this->lastClusterId = 0;
                this->phIndexMap.clear();
                this->densityMatrix.clear();
                this->phSeedMap.clear();
                std::map<float, std::set<RowIDType>> _tmpMap;
                this->pIDMap.assign(hyppox::Config::CLUSTER, _tmpMap);
            }
            
            ~DBScan()=default;
            void doClustering(std::list<PerfType*>*& phList);
            
            private:
            float Eps;
            float minPoints;
            std::vector<PerfType*> performanceList;
            std::unordered_map<size_t, std::unordered_set<RowIDType>> densityMatrix;
            std::unordered_map<RowIDType, RowIDType> phIndexMap;
            std::vector<std::map<float, std::set<RowIDType> > > pIDMap;
            std::unordered_map<RowIDType, std::pair<RowIDType, RowIDType> > phSeedMap;
            RowIDType size;
            bool setUniqueId;
            RowIDType lastClusterId;
            
            std::set<RowIDType> getAllPerformances(const std::vector<float>& value, short dim);
            void getAllPoints(std::vector<RowIDType>& vPoints, RowIDType index);
            void prepareDistanceBasedDensityMatrix();
            float getDistance(std::vector<float>* p1, std::vector<float>* p2, bool L1);
            void run();
            void FormatSeeds(std::unordered_set<RowIDType>& seeds);
            void ExpandCluster(RowIDType index, ClusterIDType clusterID);
            void computeSeeds();
            void computeSeedsForTesting(std::ofstream& fp);
            void getSeeds(std::set<RowIDType> *seed, RowIDType index, RowIDType& left, RowIDType& right, float& cover);
        };
        
        template<typename PerfType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        std::set<RowIDType> DBScan<PerfType,ClusterIDType,RowIDType,ClusterType>::getAllPerformances(const std::vector<float>& value, short dim){
            std::set<RowIDType> s;
            bool f[] = {true,true};
            
            auto itr1=this->pIDMap[dim].find(value[dim]), itr2=this->pIDMap[dim].find(value[dim]);
            
            while (f[0]==true||f[1]==true) {
                if(f[1] && itr2!=this->pIDMap[dim].end()){
                    if(abs(itr2->first-value[dim])<=this->Eps){
                        s.insert(itr2->second.begin(), itr2->second.end());
                        
                        itr2++;
                    }else{
                        f[1] = false;
                    }
                }else{
                    f[1] = false;
                }
                
                if(f[0]){
                    if(itr1==this->pIDMap[dim].begin()){
                        if(abs(itr1->first-value[dim])<=this->Eps){
                            s.insert(itr1->second.begin(), itr1->second.end());
                            
                        }
                        f[0] = false;
                    }else{
                        if(abs(itr1->first-value[dim])<=this->Eps){
                            s.insert(itr1->second.begin(), itr1->second.end());
                            
                            
                            itr1--;
                        }else{
                            f[0] = false;
                        }
                    }
                }
            }
            
            return s;
        }
        
        template<typename PerfType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        void DBScan<PerfType,ClusterIDType,RowIDType,ClusterType>::getAllPoints(std::vector<RowIDType> &vPoints, RowIDType index){
            std::vector<ClusterType> value;
            this->performanceList[index]->getClusterValue(value);
            
            std::vector<std::set<RowIDType> > s(hyppox::Config::CLUSTER);
            
            // Get all points those are in L1 phenotypic distance of the point of index
            for(short i=0; i<hyppox::Config::CLUSTER; i++){
                s[i] = this->getAllPerformances(value, i);
            }
            
            std::vector<RowIDType> v[hyppox::Config::CLUSTER];
            RowIDType max = 0;
            
            // Copy the points to a vector for set operation
            for(short i=0; i<hyppox::Config::CLUSTER; i++){
                if(s[i].size()>0){
                    v[i].insert(v[i].begin(), s[i].begin(), s[i].end());
                    //sort(v[i].begin(), v[i].end());
                    if(max<s[i].size()) max = s[i].size();
                    s[i].clear();
                }
            }
            
            // Perform set intersection to find the common set of points
            vPoints.clear();
            
            if(hyppox::Config::CLUSTER==1){
                vPoints.insert(vPoints.begin(), v[0].begin(), v[0].end());
            }else{
                vPoints.assign(max, 0);
                for(int i=0;i<hyppox::Config::CLUSTER; i++){
                    if(v[i].size()>0){
                        std::vector<RowIDType> tv(v[i]);
                        
                        for(int j=i+1;j<hyppox::Config::CLUSTER;j++){
                            if(v[j].size()>0){
                                std::set_intersection(tv.begin(), tv.end(), v[j].begin(), v[j].end(), vPoints.begin());
                                
                                tv = vPoints;
                            }
                        }
                        
                        break;
                    }
                }
            }
            
            // Remove all item whose value is 0
            vPoints.erase(remove(vPoints.begin(), vPoints.end(), 0), vPoints.end());
        }
        
        template<typename PerfType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        float DBScan<PerfType,ClusterIDType,RowIDType,ClusterType>::getDistance(std::vector<float>* p1, std::vector<float>* p2, bool L1){
            float d = 0.0;
            
            if(p1->size()!=p2->size()) return -1;
            
            auto itr1 = p1->begin(), itr2 = p2->begin();
            while(itr1!=p1->end()){
                float x = *itr1, y = *itr2;
                
                if(L1){
                    d += abs(x-y);
                }else{
                    d += ((x-y)*(x-y));
                }
                
                itr1++;
                itr2++;
            }
            
            if(L1){
                return d;
            }
            
            return sqrt(d);
        }
        
        template<typename PerfType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        void DBScan<PerfType,ClusterIDType,RowIDType,ClusterType>::prepareDistanceBasedDensityMatrix(){
            std::vector<ClusterType> value1, value2;
            
            // Create an one to one index map between point id and index
            for(RowIDType i = 0; i<this->size; i++){
                this->performanceList[i]->SetStatus(false);
                this->phIndexMap.insert(std::make_pair(this->performanceList[i]->getID(), i));
            }
            
            for(RowIDType i = 0; i<this->size; i++){
                this->performanceList[i]->getClusterValue(value1);
                auto itr=this->densityMatrix.find(this->performanceList[i]->getID());
                
                // Either point does not exist or the density is low
                if(itr==this->densityMatrix.end() || itr->second.size()<this->minPoints){
                    std::vector<RowIDType> vPoints;
                    // Collect all points at L1 distance
                    this->getAllPoints(vPoints, i);
                    
                    for(RowIDType pt:vPoints){
                        itr=this->densityMatrix.find(pt);
                        
                        // Either this point does not exist or distance with this point did not calculated earlier
                        if(itr==this->densityMatrix.end() || itr->second.find(this->performanceList[i]->getID())==itr->second.end()){
                            auto pItr = this->phIndexMap.find(pt);
                            this->performanceList[pItr->second]->getClusterValue(value2);
                            
                            float d = getDistance(&value1, &value2, (hyppox::Config::CLUSTER==1));
                            if(d>=0 && d<=this->Eps){
                                if((itr=this->densityMatrix.find(this->performanceList[i]->getID()))==this->densityMatrix.end()){
                                    std::unordered_set<size_t> _s;
                                    _s.insert(this->performanceList[pItr->second]->getID());
                                    this->densityMatrix.insert(make_pair(this->performanceList[i]->getID(), _s));
                                }else{
                                    itr->second.insert(this->performanceList[pItr->second]->getID());
                                }
                                
                                if(pItr->second!=i){
                                    if((itr=this->densityMatrix.find(this->performanceList[pItr->second]->getID()))==this->densityMatrix.end()){
                                        std::unordered_set<RowIDType> _s;
                                        _s.insert(this->performanceList[i]->getID());
                                        this->densityMatrix.insert(make_pair(this->performanceList[pItr->second]->getID(), _s));
                                    }else{
                                        itr->second.insert(this->performanceList[i]->getID());
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        template<typename PerfType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        void DBScan<PerfType,ClusterIDType,RowIDType,ClusterType>::doClustering(std::list<PerfType *>*& phList){
            this->size = phList->size();
            
            if(this->size > 0){
                this->performanceList.clear();
                this->performanceList.insert(this->performanceList.begin(), phList->begin(), phList->end());
                
                if(hyppox::Config::CLUSTER<=1){
                    std::sort(this->performanceList.begin(), this->performanceList.end(), comparePerf<PerfType>);
                    
                    /*for(RowIDType i=0; i<this->performanceList.size(); i++){
                        if(this->performanceList[i]->getID()==207){
                            std::cout<<"";
                            break;
                        }
                    }*/
                    //std::ofstream fp;
                    //fp.open("/Users/methun/Sites/Data/tmp/clstst.csv", std::ios::out | std::ios::app);
                    //this->computeSeedsForTesting(fp);
                    //fp.close();
                    
                    this->computeSeeds();
                }else{
                    std::sort(this->performanceList.begin(), this->performanceList.end(), comparePID<PerfType>);
                    
                    /// This block for version 1
                    /// Starts here
                    for(short i=0;i<hyppox::Config::CLUSTER;i++){
                        this->pIDMap[i].clear();
                        
                        for(size_t j=0; j<this->size; j++){
                            
                            float value = this->performanceList[j]->getClusterValue(i);
                            RowIDType _id = this->performanceList[j]->getID();
                            
                            auto itr=this->pIDMap[i].find(value);
                            if(itr==this->pIDMap[i].end()){
                                std::set<RowIDType> s;
                                s.insert(_id);
                                
                                this->pIDMap[i].insert(std::make_pair(value, s));
                            }else{
                                itr->second.insert(_id);
                            }
                        }
                    }
                    
                    this->prepareDistanceBasedDensityMatrix();
                }
                /// Ends here
                
                // This block for version 2
                //this->prepareDensityMatrix_version_2();
                
                /*cout<<endl;
                 for(auto itr=this->densityMatrix.begin(); itr!=this->densityMatrix.end(); itr++){
                 cout<<itr->first<<"("<<itr->second.size()<<") ";
                 }
                 cout<<endl;*/
                
                phList->clear();
                
                // Place clusting code here
                this->run();
                
                // Reassign the clustering performances
                phList->insert(phList->begin(), this->performanceList.begin(), this->performanceList.end());
                
                //std::cout<<"Cluster ID:"<<Cluster<PerfType,ClusterIDType>::GetClusterID()<<std::endl;
            }
        }
        
        //#pragma Private_Methods
        template<typename PerfType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        void DBScan<PerfType,ClusterIDType,RowIDType,ClusterType>::run(){
            RowIDType clusterID = 0;
            
            for(RowIDType i=0; i<this->size; i++){
                
                if(this->performanceList[i]->GetStatus() == false){
                    
                    clusterID = Cluster<PerfType,ClusterIDType>::GetNextID();
                    
                    this->ExpandCluster(i, clusterID);
                    
                }
            }
            
            /*if(!this->setUniqueId){
                cout<<endl;
                for(size_t i=0; i<this->size; i++){
                    
                    vector<long> tmpIL;
                    this->performanceList[i]->GetIDList(tmpIL);
                    
                    for(long _id:tmpIL){
                        if(_id==77){
                            cout<<this->performanceList[i]->getID()<<",";
                        }
                    }
                }
                cout<<endl;
            }*/
        }
        
        // This method removes all points those are already clustered
        template<typename PerfType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        void DBScan<PerfType,ClusterIDType,RowIDType,ClusterType>::FormatSeeds(std::unordered_set<RowIDType> &seeds){
            std::unordered_set<RowIDType> pidList;
            
            for(auto sItr = seeds.begin(); sItr!=seeds.end(); sItr++){
                auto pItr = this->phIndexMap.find(*sItr);
                RowIDType currIndex = pItr->second;
                
                if(this->performanceList[currIndex]->GetStatus()==true){
                    pidList.insert(*sItr);
                }
            }
            
            for(RowIDType _pid:pidList){
                seeds.erase(_pid);
            }
        }
        
        template<typename PerfType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        void DBScan<PerfType,ClusterIDType,RowIDType,ClusterType>::ExpandCluster(RowIDType index, ClusterIDType clusterID){
            
            if(hyppox::Config::CLUSTER<=1){
                float cover=0;
                std::set<RowIDType> seeds;
                
                auto itr = this->phSeedMap.find(index);
                
                cover = itr->second.second-itr->second.first+1;
                RowIDType low=itr->second.first, high=itr->second.second;
                
                for(RowIDType i=itr->second.first; i<=itr->second.second; i++){
                    seeds.insert(i);
                }
                
                if(cover<this->minPoints){
                    if(this->setUniqueId){
                        this->performanceList[index]->SetUniqueId(clusterID);
                        this->performanceList[index]->SetStatus(true);
                    }else{
                        this->performanceList[index]->AddClusterID(clusterID);
                        this->performanceList[index]->SetType("P");
                    }
                }else{
                    for(auto itr = seeds.begin(); itr != seeds.end(); itr++){
                        long tmpIndex = *itr;
                        
                        if(this->setUniqueId){
                            this->performanceList[tmpIndex]->SetUniqueId(clusterID);
                            this->performanceList[tmpIndex]->SetStatus(true);
                        }else{
                            this->performanceList[tmpIndex]->AddClusterID(clusterID);
                        }
                    }
                    
                    if(!this->setUniqueId){
                        this->performanceList[index]->SetType("C");
                    }
                    
                    /*if(this->setUniqueId){
                        this->performanceList[index]->SetUniqueId(clusterID);
                        this->performanceList[index]->SetStatus(true);
                    }else{
                        this->performanceList[index]->AddClusterID(clusterID);
                        this->performanceList[index]->SetType("C");
                    }*/
                    
                    seeds.erase(index);
                    
                    while (seeds.size() > 0) {
                        auto sItr = seeds.begin();
                        RowIDType currIndex = *sItr;
                        
                        //std::set<RowIDType> results;
                        
                        auto itr1 = this->phSeedMap.find(currIndex);
                        
                        cover = itr1->second.second-itr1->second.first+1;
                        RowIDType tLow=itr1->second.first, tHigh=itr1->second.second;
                        
                        /*for(RowIDType i=itr1->second.first; i<=itr1->second.second; i++){
                            results.insert(i);
                        }*/
                        
                        if(cover >= this->minPoints){
                            //if(results.size() > 0){
                                //RowIDType thisIndex;
                                //for(sItr = results.begin(); sItr != results.end(); sItr++){
                            
                            // Need to add selected item which are outside of seed boundary
                            if(tLow<low){
                                for(RowIDType thisIndex=tLow; thisIndex<low; thisIndex++){
                                    //thisIndex = *sItr;
                                    if(this->performanceList[thisIndex]->GetStatus() == false){
                                        seeds.insert(thisIndex);
                                        
                                        if(this->setUniqueId){
                                            this->performanceList[thisIndex]->SetUniqueId(clusterID);
                                            this->performanceList[thisIndex]->SetStatus(true);
                                        }else{
                                            this->performanceList[thisIndex]->AddClusterID(clusterID);
                                        }
                                    }
                                }
                                
                                low=tLow;
                            }
                            
                            if(tHigh>high){
                                for(RowIDType thisIndex=high+1; thisIndex<=tHigh; thisIndex++){
                                    //thisIndex = *sItr;
                                    if(this->performanceList[thisIndex]->GetStatus() == false){
                                        seeds.insert(thisIndex);
                                        
                                        if(this->setUniqueId){
                                            this->performanceList[thisIndex]->SetUniqueId(clusterID);
                                            this->performanceList[thisIndex]->SetStatus(true);
                                        }else{
                                            this->performanceList[thisIndex]->AddClusterID(clusterID);
                                        }
                                    }
                                }
                                
                                high=tHigh;
                            }
                            
                            
                            
                                /*for(RowIDType thisIndex=itr1->second.first; thisIndex<=itr1->second.second; thisIndex++){
                                    //thisIndex = *sItr;
                                    if(this->performanceList[thisIndex]->GetStatus() == false){
                                        seeds.insert(thisIndex);
                                        
                                        if(this->setUniqueId){
                                            this->performanceList[thisIndex]->SetUniqueId(clusterID);
                                            this->performanceList[thisIndex]->SetStatus(true);
                                        }else{
                                            this->performanceList[thisIndex]->AddClusterID(clusterID);
                                        }
                                    }
                                }*/
                            //}
                            
                            if(!this->setUniqueId){
                                this->performanceList[currIndex]->SetType("C");
                            }
                        }else{
                            /*if(this->performanceList[currIndex]->getID()==176274){
                             cout<<"";
                             }*/
                            if(!this->setUniqueId){
                                this->performanceList[currIndex]->SetType("P");
                            }
                        }
                        
                        seeds.erase(currIndex);
                        
                        //results.clear();
                    }
                }
            }else{
                auto itr = this->densityMatrix.find(this->performanceList[index]->getID());
                
                std::unordered_set<RowIDType> seeds(itr->second.begin(), itr->second.end());
                
                //std::unordered_set<size_t>::iterator sItr;
                //unordered_map<size_t, size_t>::iterator pItr;
                
                this->FormatSeeds(seeds);
                
                if(seeds.size()<this->minPoints){
                    if(this->setUniqueId){
                        this->performanceList[index]->SetUniqueId(clusterID);
                        this->performanceList[index]->SetStatus(true);
                    }else{
                        this->performanceList[index]->AddClusterID(clusterID);
                        this->performanceList[index]->SetType("P");
                    }
                }else{
                    if(this->setUniqueId){
                        this->performanceList[index]->SetUniqueId(clusterID);
                        this->performanceList[index]->SetStatus(true);
                    }else{
                        this->performanceList[index]->AddClusterID(clusterID);
                        this->performanceList[index]->SetType("C");
                    }
                    
                    seeds.erase(this->performanceList[index]->getID());
                    
                    while (seeds.size() > 0) {
                        auto sItr = seeds.begin();
                        auto pItr = this->phIndexMap.find(*sItr);
                        RowIDType currIndex = pItr->second;
                        
                        if(this->setUniqueId){
                            this->performanceList[currIndex]->SetUniqueId(clusterID);
                            this->performanceList[currIndex]->SetStatus(true);
                        }else{
                            this->performanceList[currIndex]->AddClusterID(clusterID);
                        }
                        
                        itr = this->densityMatrix.find(this->performanceList[currIndex]->getID());
                        std::unordered_set<RowIDType> results(itr->second.begin(), itr->second.end());
                        
                        this->FormatSeeds(results);
                        
                        if(results.size() >= this->minPoints){
                            
                            if(!this->setUniqueId){
                                this->performanceList[currIndex]->SetType("C");
                            }
                            
                            for(sItr = results.begin(); sItr != results.end(); sItr++){
                                pItr = this->phIndexMap.find(*sItr);
                                if(this->performanceList[pItr->second]->GetStatus() == false){
                                    seeds.insert(this->performanceList[pItr->second]->getID());
                                }
                            }
                        }else{
                            /*if(this->performanceList[currIndex]->getID()==176274){
                             cout<<"";
                             }*/
                            if(!this->setUniqueId){
                                this->performanceList[currIndex]->SetType("P");
                            }
                        }
                        
                        seeds.erase(this->performanceList[currIndex]->getID());
                        
                        results.clear();
                    }
                }
            }
            
            this->lastClusterId = clusterID;
        }
    
        template<typename PerfType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        void DBScan<PerfType,ClusterIDType,RowIDType,ClusterType>::computeSeedsForTesting(std::ofstream& fp){
            this->phSeedMap.clear();
            
            RowIDType tmpLeft = 0, tmpRight = 0, rl=0,rr=0;
            float ptValue=0.0, dist=0.0, lastPt=0.0;
            
            //time_t t1=clock(),t2,t3,t4;
            for(RowIDType i=0; i<this->size; i++){
                ptValue = this->performanceList[i]->getClusterValue(0);
                
                // Starting point in the ordered sequence
                // Compute right index for it only becasue left index = 0
                if(i==0){ // Left corner case
                    rl = i;
                    
                    // Checking right limit
                    while(rr<this->size){
                        dist = this->performanceList[rr]->getClusterValue(0)-ptValue;
                        if(dist>this->Eps)break;
                        rr++;
                    }
                    
                    // Loop will exit either out of boundary or out of reach point
                    // In both cases, exclude the current index and consider the previous index
                    rr--;
                    
                }else if(i==this->size-1){ // Right corner case
                    rr = i;
                    
                    if(lastPt != ptValue){
                        dist = ptValue -this->performanceList[rl]->getClusterValue(0);
                        while(dist>this->Eps && rl<rr){
                            rl++;
                            
                            // Pointing to the ptValue
                            if(rl==rr) break;
                            
                            dist = ptValue -this->performanceList[rl]->getClusterValue(0);
                        }
                    }
                }else{ // Intermediate case
                    if(lastPt != ptValue){
                        // Checking left limit
                        dist = ptValue -this->performanceList[rl]->getClusterValue(0);
                        while(dist>this->Eps && rl<i){
                            rl++;
                            
                            // Pointing to the ptValue
                            if(rl==i) break;
                            
                            dist = ptValue -this->performanceList[rl]->getClusterValue(0);
                        }
                        
                        // Checking right limit
                        while(rr<this->size){
                            dist = this->performanceList[rr]->getClusterValue(0)-ptValue;
                            if(dist>this->Eps)break;
                            rr++;
                        }
                        
                        // Loop will exit either out of boundary or out of reach point
                        // In both cases, exclude the current index and consider the previous index
                        rr--;
                    }
                }
                
                // store this value as last value
                lastPt = ptValue;
                
                //this->phSeedMap.insert(std::make_pair(i, std::make_pair(tmpLeft, tmpRight)));
                
                ptValue = this->performanceList[i]->getClusterValue(0);
                
                /////////////////////////////////
                tmpLeft = i; tmpRight = i;
                while(tmpLeft>0){
                    dist = ptValue -this->performanceList[tmpLeft]->getClusterValue(0);
                    if(dist>this->Eps){tmpLeft++; break;}
                    tmpLeft--;
                    
                    // Special case to handle non-signed number
                    if(tmpLeft==0){
                        dist = ptValue -this->performanceList[tmpLeft]->getClusterValue(0);
                        if(dist>this->Eps) tmpLeft++;
                        
                        break;
                    }
                }
                
                // Checking right limit
                while(tmpRight<this->size){
                    dist = this->performanceList[tmpRight]->getClusterValue(0)-ptValue;
                    if(dist>this->Eps)break;
                    tmpRight++;
                }
                tmpRight--;
                ////////////////////////
                
                if(rl != tmpLeft || rr!=tmpRight)
                    fp<<i<<","<<ptValue<<","<<rl<<","<<rr<<","<<tmpLeft<<","<<tmpRight<<std::endl;
                
                this->phSeedMap.insert(std::make_pair(i, std::make_pair(tmpLeft, tmpRight)));
            }
            
        }
        
        // This method is used only for 1D cluster analysis
        template<typename PerfType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        void DBScan<PerfType,ClusterIDType,RowIDType,ClusterType>::computeSeeds(){
            this->phSeedMap.clear();
            
            RowIDType tmpLeft = 0, tmpRight = 0;
            float ptValue=0.0, dist=0.0, lastPt=0.0;
            
            for(RowIDType i=0; i<this->size; i++){
                ptValue = this->performanceList[i]->getClusterValue(0);
                
                // Starting point in the ordered sequence
                // Compute right index for it only becasue left index = 0
                if(i==0){ // Left corner case
                    tmpLeft = i;
                    
                    // Checking right limit
                    while(tmpRight<this->size){
                        dist = this->performanceList[tmpRight]->getClusterValue(0)-ptValue;
                        if(dist>this->Eps)break;
                        tmpRight++;
                    }
                    
                    // Loop will exit either out of boundary or out of reach point
                    // In both cases, exclude the current index and consider the previous index
                    tmpRight--;
                    
                }else if(i==this->size-1){ // Right corner case
                    tmpRight = i;
                    
                    if(lastPt != ptValue){
                        dist = ptValue -this->performanceList[tmpLeft]->getClusterValue(0);
                        while(dist>this->Eps && tmpLeft<tmpRight){
                            tmpLeft++;
                            
                            // Pointing to the ptValue
                            if(tmpLeft==tmpRight) break;
                            
                            dist = ptValue -this->performanceList[tmpLeft]->getClusterValue(0);
                        }
                    }
                }else{ // Intermediate case
                    if(lastPt != ptValue){
                        // Checking left limit
                        dist = ptValue -this->performanceList[tmpLeft]->getClusterValue(0);
                        while(dist>this->Eps && tmpLeft<i){
                            tmpLeft++;
                            
                            // Pointing to the ptValue
                            if(tmpLeft==i) break;
                            
                            dist = ptValue -this->performanceList[tmpLeft]->getClusterValue(0);
                        }
                        
                        // Checking right limit
                        while(tmpRight<this->size){
                            dist = this->performanceList[tmpRight]->getClusterValue(0)-ptValue;
                            if(dist>this->Eps)break;
                            tmpRight++;
                        }
                        
                        // Loop will exit either out of boundary or out of reach point
                        // In both cases, exclude the current index and consider the previous index
                        tmpRight--;
                    }
                }
                
                // store this value as last value
                lastPt = ptValue;
                
                this->phSeedMap.insert(std::make_pair(i, std::make_pair(tmpLeft, tmpRight)));
            }
            
            /*for(RowIDType i=0; i<this->size; i++){
                ptValue = this->performanceList[i]->getClusterValue(0);
                
                /////////////////////////////////
                tmpLeft = i; tmpRight = i;
                while(tmpLeft>0){
                    dist = ptValue -this->performanceList[tmpLeft]->getClusterValue(0);
                    if(dist>this->Eps){tmpLeft++; break;}
                    tmpLeft--;
                    
                    // Special case to handle non-signed number
                    if(tmpLeft==0){
                        dist = ptValue -this->performanceList[tmpLeft]->getClusterValue(0);
                        if(dist>this->Eps) tmpLeft++;
                        
                        break;
                    }
                }
                
                // Checking right limit
                while(tmpRight<this->size){
                    dist = this->performanceList[tmpRight]->getClusterValue(0)-ptValue;
                    if(dist>this->Eps)break;
                    tmpRight++;
                }
                tmpRight--;
                ////////////////////////
                
                this->phSeedMap.insert(std::make_pair(i, std::make_pair(tmpLeft, tmpRight)));
            }*/
        }
        
        template<typename PerfType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        void DBScan<PerfType,ClusterIDType,RowIDType,ClusterType>::getSeeds(std::set<RowIDType> *seed, RowIDType index, RowIDType& left, RowIDType& right, float& cover){
            
            RowIDType tmpLeft = left, tmpRight = right;
            
            // Clear the container
            seed->clear();
            float ptValue = this->performanceList[index]->getClusterValue(0), dist=0.0;
            
            // increment left index until perf[index]-perf[left]<=EPS
            if(tmpLeft == index && tmpLeft > 0){
                do{
                    tmpLeft--;
                    dist = ptValue -this->performanceList[tmpLeft]->getClusterValue(0);
                    if(dist>this->Eps){tmpLeft++; break;}
                }while(tmpLeft>0);
                
            }else while(tmpLeft<index){
                dist = ptValue -this->performanceList[tmpLeft]->getClusterValue(0);
                if(dist<=this->Eps) break;
                tmpLeft++;
            }
            //tmpLeft--;
            
            // increment right index until perf[right]-perf[index]<=EPS
            while(tmpRight<this->performanceList.size()){
                dist = this->performanceList[tmpRight]->getClusterValue(0)-ptValue;
                if(dist>this->Eps) break;
                tmpRight++;
            }
            tmpRight--;
            
            cover = tmpRight-tmpLeft+1;
            
            // Copy index to container
            // Copy new items on the right side
            if(right<tmpRight){
                for(RowIDType i=right; i<=tmpRight; i++){
                     if(this->performanceList[i]->GetStatus() == false) seed->insert(i);
                }
            }
            
            // Copy new items from left side
            if(tmpLeft<left){
                for(RowIDType i=tmpLeft; i<=left; i++){
                    if(this->performanceList[i]->GetStatus() == false) seed->insert(i);
                }
            }
            
            left = tmpLeft;
            right = tmpRight;
        }
    }
}


#endif /* DBScan_h */

