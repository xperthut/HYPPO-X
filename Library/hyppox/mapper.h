/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: mapper.h
 Objective: Create mapper from high dimensional data
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              01/03/2018      Initial version
 
 **************************************************************************************************/

#ifndef mapper_h
#define mapper_h

#include "unionFindWithPathCompression.h"
#include "simplicialComplex.h"
#include "graph.h"
#include <vector>

namespace hyppox {
    namespace mapper{
        
        template<typename PerfType, typename DPType>
        class ManageCluster{
            public:
            std::list<PerfType*> phList;
            std::list<DPType*> dpList;
            
            ManageCluster()=default;
            ~ManageCluster()=default;
            
            void AddToCluster(PerfType* ph){this->phList.push_back(ph);}
            void GetPerfTypeListOfThisCluster(std::list<PerfType*> *ph){ph->insert(ph->begin(), this->phList.begin(), this->phList.end());}
            void AddToCluster(DPType* dp){this->dpList.push_back(dp);}
            void GetPerfTypeListOfThisCluster(std::list<DPType*> *dp){dp->insert(dp->begin(), this->dpList.begin(), this->dpList.end());}
        };
        
        
        template<typename ClusterIDType>
        class Grid{
            public:
            std::vector<float> center;
            ClusterIDType _id;
            std::unordered_set<ClusterIDType> clusterIdList;
            
            Grid(){
                this->clusterIdList.clear();
                this->center.assign(hyppox::Config::FILTER, 0);
            }
            
            ~Grid(){
                this->center.clear();
            }
            
            void GetClusterID(std::unordered_set<ClusterIDType>* mainClsIDSet){
                ClusterIDType ide =0;
                
                for(auto itr=clusterIdList.begin(); itr!=clusterIdList.end(); itr++){
                    ide = *itr;
                    
                    mainClsIDSet->insert(ide);
                }
            }
        };
        
        template<typename NodeType>
        class AnEdge{
            private:
            NodeType node1;
            NodeType node2;
            std::string _id;
            
            public:
            AnEdge(NodeType n1, NodeType n2){
                this->node1 = n1;
                this->node2 = n2;
                
                // An undirected edge from low cluster id to high cluster id
                _id = (n1<n2)?std::to_string(n1)+"#"+std::to_string(n2):std::to_string(n2)+"#"+std::to_string(n1);
            }
            
            ~AnEdge()=default;
            NodeType getNode1(){return node1;}
            NodeType getNode2(){return node2;}
        };
        
        template<typename PerfType, typename DPType, typename QTType, typename GType, typename FHType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        class Mapper{
            public:
            using GRType = Grid<ClusterIDType>;
            using EType = AnEdge<ClusterIDType>;
            using MCType = ManageCluster<PerfType,DPType>;
            using DBType = cluster::DBScan<PerfType,ClusterIDType,RowIDType,ClusterType>;
            using SCType = SimplicialComplex<PerfType,ClusterIDType>;
            using ClsType = cluster::Cluster<PerfType,ClusterIDType>;
            //using MType = Mapper<PerfType,DPType,QTType,GType,ClusterIDType,RowIDType>;
            
            Mapper();
            ~Mapper();
            std::string createMapper(GType* graph);
            
            private:
            std::vector<std::vector<GRType>> mesh;
            QTType treeRoot; // To store data for nD (n>1) filter
            
            std::vector<float> minPos;
            std::vector<float> maxPos;
            std::vector<float> gridLength;
            std::list<EType*> edgeList;
            std::unordered_map<size_t,GRType*> gridMap;
            std::unordered_map<long, long> connectedComponents;
            
            int storeData(std::unordered_map<std::string, size_t>& memMap, std::unordered_map<std::string, size_t>& pieMap);
            int createFilter(std::unordered_map<std::string, size_t>& memMap, std::unordered_map<std::string, size_t>& pieMap);
            void clearClusterInformation();
            void createCluster(std::list<PerfType*> *pList, bool setUniqueId);
            void assignUniqueClusterIds();
            void createUniqueClusters();
            bool isEligibleForClustering(std::vector<float> &min, std::vector<float> &max, size_t boxID);
            bool isValidCluster(std::list<PerfType *> *pList, std::unordered_set<size_t>* mainClsID);
            void manageCluster(std::list<PerfType *> *pList, size_t minClsId, size_t maxClsId, std::unordered_set<size_t>* mainClsID);
            void assignClusterIdsForOverlapingRegion(std::vector<float> overlap);
            void assignClusterIdsForOverlapingRegion(std::list<PerfType*>* overlappedPhList, std::vector<float> overlap);
            void setOverlappedIndv(std::unordered_set<size_t> *indvSet, std::list<PerfType *> *clPhList, std::list<PerfType *> *overlappedPhList, std::unordered_set<std::string>* edgeSet);
            void generateEdgeListFromOverlappingClusters(std::list<PerfType*>* overlappedPhList, std::vector<float> overlap);
            void addSimplicialComplex(SCType *sc, float overlap);
            std::string generateSimplicesForPersistentHomology();
            void computeConnectedComponents(size_t &ccSize);
            void constructGraphComponents(GType*& graph);
            void constructGraph(GType*& graph);
            
            void testRange(std::vector<float>& min, std::vector<float>& max, std::list<PerfType*>& clPhList);
            
        };
        
        ////////////////// Public methods ////////////////////
        template<typename PerfType, typename DPType, typename QTType, typename GType, typename FHType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        Mapper<PerfType,DPType,QTType,GType,FHType,ClusterIDType,RowIDType,ClusterType>::Mapper(){
            GRType g;
            std::vector<GRType> v(hyppox::Config::WINDOWS[1], g);
            this->mesh.assign(hyppox::Config::WINDOWS[0], v);
            
            this->minPos.assign(hyppox::Config::FILTER, 0.0);
            this->maxPos.assign(hyppox::Config::FILTER, 0.0);
            this->gridLength.assign(hyppox::Config::FILTER, 0.0);
        }
        
        template<typename PerfType, typename DPType, typename QTType, typename GType, typename FHType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        Mapper<PerfType,DPType,QTType,GType,FHType,ClusterIDType,RowIDType,ClusterType>::~Mapper(){
            this->mesh.clear();
            this->minPos.clear();
            this->maxPos.clear();
        }
        
        template<typename PerfType, typename DPType, typename QTType, typename GType, typename FHType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        std::string Mapper<PerfType,DPType,QTType,GType,FHType,ClusterIDType,RowIDType,ClusterType>::createMapper(GType* graph){
            int a = this->createFilter(graph->memIndexMap, graph->pieIndexMap);
            
            if(a==-1){
                return "File dataformat error. Please check data type in the column based on instruction.";
            }else if(a==-2){
                return "Can not read data file. Please check file path and permission.";
            }
            
            clock_t t1, t2;
            
            t1 = clock();
            this->createUniqueClusters();
            
            std::string r="";
            if(hyppox::Config::PRINT_BARCODE){
                r = this->generateSimplicesForPersistentHomology();
            }else{
                this->constructGraph(graph);
                t2 = clock();
                std::vector<std::string> _time = getTime((float)(t2-t1)/CLOCKS_PER_SEC);
                std::cout<<"Time to complete mapper (filter, cluster, graph construction): "<<_time[0]<<_time[1]<<std::endl;
            }
            
            return r;
        }
        
        ////////////////// Private methods /////////////////////////
        template<typename PerfType, typename DPType, typename QTType, typename GType, typename FHType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        int Mapper<PerfType,DPType,QTType,GType,FHType,ClusterIDType,RowIDType,ClusterType>::storeData(std::unordered_map<std::string, size_t>& memMap, std::unordered_map<std::string, size_t>& pieMap){
            clock_t t1, t2;
            
            t1 = clock();
            FHType fileHandler(hyppox::Config::DATA_FILE_NAME);
            int status;
            if((status=fileHandler.ReadFileData(&this->treeRoot, this->minPos, this->maxPos, memMap, pieMap))!=0){
                return status;
            }
            
            t2 = clock();
            
            std::vector<std::string> _time = getTime((float)(t2-t1)/CLOCKS_PER_SEC);
            
            //cout<<"Total time:"<<_time[0]<<_time[1]<<endl;
            
            //this->treeRoot.PrintQuadTree();
            //cout<<"\nTotal data:"<<c<<":"<<QuadNode::totalChildren;
            std::cout<<"\nData file read and store in memory time: "<<_time[0]<<_time[1]<<std::endl;
            
            return 0;
        }
        
        template<typename PerfType, typename DPType, typename QTType, typename GType, typename FHType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        int Mapper<PerfType,DPType,QTType,GType,FHType,ClusterIDType,RowIDType,ClusterType>::createFilter(std::unordered_map<std::string, size_t>& memMap, std::unordered_map<std::string, size_t>& pieMap){
            int a = this->storeData(memMap, pieMap);
            if(a<0){
                return a;
            }
            
            for(short i=0; i<hyppox::Config::FILTER; i++){
                this->gridLength[i] = (hyppox::Config::WINDOWS[i]>0)?(this->maxPos[i] - this->minPos[i])/hyppox::Config::WINDOWS[i]:(this->maxPos[i] - this->minPos[i]);
            }
            
            float start_x = this->minPos[0], start_y = 0;
            
            for(int i=0; i<hyppox::Config::WINDOWS[0]; i++){
                
                if(hyppox::Config::FILTER>1) start_y = this->minPos[1];
                else start_y = 0;
                
                for(int j=0; j<hyppox::Config::WINDOWS[1]; j++){
                    this->mesh[i][j].center[0] = start_x + (this->gridLength[0]/2);
                    if(hyppox::Config::FILTER>1) this->mesh[i][j].center[1] = start_y + (this->gridLength[1]/2);
                    else this->mesh[i][j].center[1] = start_y;
                    
                    if(hyppox::Config::FILTER>1) start_y += this->gridLength[1];
                }
                
                start_x += this->gridLength[0];
            }
            
            return 0;
        }
        
        template<typename PerfType, typename DPType, typename QTType, typename GType, typename FHType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        void Mapper<PerfType,DPType,QTType,GType,FHType,ClusterIDType,RowIDType,ClusterType>::clearClusterInformation(){
            
            std::list<PerfType*> clPhList;
            std::list<DPType*> dpList;
            
            clPhList.clear();
            this->treeRoot.ResetClusterInformation();
            
            ClsType::SetClusterID(0);
        }
        
        template<typename PerfType, typename DPType, typename QTType, typename GType, typename FHType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        void Mapper<PerfType,DPType,QTType,GType,FHType,ClusterIDType,RowIDType,ClusterType>::createCluster(std::list<PerfType*> *pList, bool setUniqueId){
            //clock_t t1, t2;
            //t1 = clock();
            ClsType *cluster = new DBType(hyppox::Config::CLUSTER_PARAM[0], hyppox::Config::CLUSTER_PARAM[1], setUniqueId);
            cluster->doClustering(pList);
            //t2 = clock();
            
            //std::cout<<"\nClustering completed..."<<(t2-t1)/CLOCKS_PER_SEC<<endl;
            delete cluster;
        }
        
        template<typename PerfType, typename DPType, typename QTType, typename GType, typename FHType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        void Mapper<PerfType,DPType,QTType,GType,FHType,ClusterIDType,RowIDType,ClusterType>::assignUniqueClusterIds(){
            std::cout<<"\nCompleted filtering step"<<std::endl<<"Started clustering...\n";
            
            std::vector<float> min(hyppox::Config::FILTER, 0.0), max(hyppox::Config::FILTER, 0.0);
            std::list<PerfType*> clPhList;
            std::list<DPType*> dpList;
            size_t boxID = 1, lastID, thisID;
            
            if(this->mesh.size()>0){
                for(int j=0; j<hyppox::Config::WINDOWS[1];j++){
                    for(int i=0; i<hyppox::Config::WINDOWS[0]; i++){
                        for(short f=0; f<hyppox::Config::FILTER; f++){
                            float c = this->mesh[i][j].center[f];
                            float l = this->gridLength[f]/2;
                            min[f] = c - l;
                            max[f] = c + l;
                            
                            if(min[f] < this->minPos[f]) min[f] = this->minPos[f];
                            if(max[f] > this->maxPos[f]) max[f] = this->maxPos[f];
                        }
                        
                        clPhList.clear();
                        dpList.clear();
                        
                        this->treeRoot.SearchSurface(min, max, &clPhList, &dpList, boxID);
                        
                        //std::cout<<"("<<i<<","<<j<<")={("<<min[0]<<","<<min[1]<<"),("<<max[0]<<","<<max[1]<<")}: ";
                        
                        if(clPhList.size() > 0){
                            
                            //testRange(min, max, clPhList);
                            
                            // The following commented code is just to test whether same point contians different PerfTypes or not
                            /*map<size_t, list<vector<float>>> _tmpCV;
                             map<size_t, list<vector<float>>>::iterator _tItr;
                             
                             for(PerfType* ph:clPhList){
                             vector<float> _clsVal;
                             ph->getClusterValue(&_clsVal);
                             
                             if((_tItr=_tmpCV.find(ph->getID()))==_tmpCV.end()){
                             list<vector<float>> _lv;
                             _lv.push_back(_clsVal);
                             
                             _tmpCV.insert(make_pair(ph->getID(), _lv));
                             }else{
                             _tItr->second.push_back(_clsVal);
                             }
                             }
                             
                             cout<<"Total:"<<_tmpCV.size()<<endl;
                             for(_tItr=_tmpCV.begin(); _tItr!=_tmpCV.end(); _tItr++){
                             cout<<_tItr->first<<"=>{";
                             for(vector<float> _clsVal:_tItr->second){
                             cout<<"(";
                             for(float _cv:_clsVal){
                             cout<<_cv<<",";
                             }
                             cout<<"),";
                             }
                             cout<<"}\n";
                             }
                             cout<<endl<<"-------------------\n";*/
                            
                            lastID = ClsType::GetClusterID();
                            
                            //std::cout<<"Data points:"<<dpList.size()<<", clusterPts:"<<clPhList.size();
                            time_t _t1 = clock();
                            
                            this->createCluster(&clPhList, true);
                            
                            time_t _t2 = clock();
                             
                             float _time = (float)(_t2-_t1)/CLOCKS_PER_SEC;
                             std::string unit = "(sec)";
                             
                             if(_time<1){
                             _time = ((float)(_t2-_t1)*1000)/CLOCKS_PER_SEC;
                             unit = "(ms)";
                             }
                             
                             //std::cout<<" Total time to cluster:"<<_time<<unit;
                            
                            thisID = ClsType::GetClusterID();
                            
                            //std::cout<<" ID:"<<thisID<<std::endl;
                            
                            GRType* gr = &this->mesh[i][j];
                            gr->_id = boxID;
                            
                            if(thisID-lastID>1){
                                for(ClusterIDType i=lastID+1; i<=thisID; i++){
                                    gr->clusterIdList.insert(i);
                                }
                            }else{
                                gr->clusterIdList.insert(thisID);
                            }
                            
                            this->gridMap.insert(std::make_pair(boxID, gr));
                            
                        }
                        //std::cout<<":"<<ClsType::GetClusterID()<<std::endl;
                        boxID++;
                        
                    }
                }
            }
            
        }
        
        template<typename PerfType, typename DPType, typename QTType, typename GType, typename FHType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        void Mapper<PerfType,DPType,QTType,GType,FHType,ClusterIDType,RowIDType,ClusterType>::createUniqueClusters(){
            this->clearClusterInformation();
            this->assignUniqueClusterIds();
            ClsType::SetClusterID(0);
        }
        
        template<typename PerfType, typename DPType, typename QTType, typename GType, typename FHType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        bool Mapper<PerfType,DPType,QTType,GType,FHType,ClusterIDType,RowIDType,ClusterType>::isEligibleForClustering(std::vector<float> &min, std::vector<float> &max, size_t boxID){
            
            std::unordered_set<size_t> bxIdList;
            
            this->treeRoot.SearchDataPoints(min, max, &bxIdList);
            
            if(bxIdList.size()>0){
                if(bxIdList.find(boxID) != bxIdList.end()){
                    return true;
                }
            }
            
            return false;
        }
        
        // A valid cluster contains points of both in box and out of box
        // Means the points comes from both adjacent boxes
        // Invalid cluster contains either in box points or out box points
        template<typename PerfType, typename DPType, typename QTType, typename GType, typename FHType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        bool Mapper<PerfType,DPType,QTType,GType,FHType,ClusterIDType,RowIDType,ClusterType>::isValidCluster(std::list<PerfType *> *pList, std::unordered_set<size_t> *mainClsID){
            bool alienPoints = false, knownPoints = false;
            
            for(auto itr=pList->begin(); itr!=pList->end(); itr++){
                PerfType* ph = *itr;
                
                if(mainClsID->find(ph->GetUniqueID())==mainClsID->end()){
                    alienPoints = true;
                }else{
                    knownPoints = true;
                }
            }
            
            return (alienPoints&&knownPoints);
        }
        
        /*
         When increasing the box sides, there could be few cases:
         Case 1: Overlapping area does not contains any new points
         Case 2: Overlapping area contains points
         
         When we are doing clustering in a box, for case 1, all points of a box are clustered again and it is redundent
         For case 2, when we are clustering a box, it could happen that only new points create a cluster.
         
         This function checks both cases and avoid clustering points at both scenarios.
         */
        template<typename PerfType, typename DPType, typename QTType, typename GType, typename FHType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        void Mapper<PerfType,DPType,QTType,GType,FHType,ClusterIDType,RowIDType,ClusterType>::manageCluster(std::list<PerfType *> *pList, size_t minClsId, size_t maxClsId, std::unordered_set<size_t>* mainClsID){
            // Create bucket based on newly created cluster ids
            MCType* cluster = new MCType[maxClsId-minClsId+1];
            std::unordered_set<size_t> clsRange;
            
            for(size_t i=minClsId; i<=maxClsId; i++){
                clsRange.insert(i);
            }
            
            // assign points at each bucket based on cluster id
            for(auto itr=pList->begin(); itr!=pList->end(); itr++){
                PerfType* ph = *itr;
                
                std::vector<ClusterIDType> clsIDList(hyppox::Config::TOTAL_CLUSTER_IDS, 0);
                
                ph->GetIDList(clsIDList);
                
                for(short i=0; i<hyppox::Config::TOTAL_CLUSTER_IDS; i++){
                    if(clsIDList[i] > 0 && clsRange.find(clsIDList[i]) != clsRange.end()){
                        cluster[clsIDList[i]-minClsId].AddToCluster(ph);
                    }
                }
            }
            
            clsRange.clear();
            std::list<PerfType *> cphList;
            cphList.insert(cphList.begin(), pList->begin(), pList->end());
            
            pList->clear();
            
            std::list<PerfType *> phList;
            std::unordered_set<size_t> phListTrack;
            
            for(size_t i=minClsId; i<=maxClsId; i++){
                cluster[i-minClsId].GetPerfTypeListOfThisCluster(&phList);
                
                if(!isValidCluster(&phList, mainClsID)){
                    for(auto itr=phList.begin(); itr!=phList.end(); itr++){
                        PerfType* ph = *itr;
                        
                        std::vector<ClusterIDType> idList;
                        ph->GetIDList(idList);
                        
                        for(short j=0; j<hyppox::Config::TOTAL_CLUSTER_IDS; j++){
                            if(idList[j] == i){
                                ph->DecreaseClusterIndex(j);
                                
                                break;
                            }
                        }
                    }
                }
                
                for(auto itr=phList.begin(); itr!=phList.end(); itr++){
                    PerfType* ph = *itr;
                    
                    if(phListTrack.find(ph->getID())==phListTrack.end()){
                        phListTrack.insert(ph->getID());
                        pList->push_back(ph);
                    }
                }
                
                phList.clear();
                clsRange.clear();
            }
            
            //ClsType::SetClusterID(minClsId-1);
            
            delete []cluster;
        }
        
        template<typename PerfType, typename DPType, typename QTType, typename GType, typename FHType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        void Mapper<PerfType,DPType,QTType,GType,FHType,ClusterIDType,RowIDType,ClusterType>::assignClusterIdsForOverlapingRegion(std::vector<float> overlap){
            std::vector<float> min(hyppox::Config::FILTER, 0.0), max(hyppox::Config::FILTER, 0.0);
            std::list<PerfType*> clPhList;
            std::list<DPType*> dpList;
            size_t boxID = 1, prevClsId, thisClsId;
            std::unordered_set<size_t> boxIdSet;
            std::unordered_set<size_t> mainClsIDSet;
            std::unordered_set<std::string> phListTrack;
            
            if(this->mesh.size()>0){
                for(int j=0; j<hyppox::Config::WINDOWS[1];j++){
                    for(int i=0; i<hyppox::Config::WINDOWS[0]; i++){
                        for(short f=0; f<hyppox::Config::FILTER; f++){
                            float c = this->mesh[i][j].center[f];
                            float l = (this->gridLength[f]*(1+overlap[f]))/2;
                            min[f] = c - l;
                            max[f] = c + l;
                            
                            if(min[f] < this->minPos[f]) min[f] = this->minPos[f];
                            if(max[f] > this->maxPos[f]) max[f] = this->maxPos[f];
                        }
                        
                        clPhList.clear();
                        dpList.clear();
                        phListTrack.clear();
                        
                        if(isEligibleForClustering(min, max, boxID)){
                            this->treeRoot.SearchSurface(min, max, &clPhList, &dpList, boxID);
                        }
                        
                        //std::cout<<"~~("<<i<<","<<j<<")={("<<min[0]<<","<<min[1]<<"),("<<max[0]<<","<<max[1]<<")}: ";
                        
                        if(clPhList.size() > 0){
                            prevClsId = ClsType::GetClusterID();
                            
                            //std::cout<<"Data points:"<<dpList.size()<<", clusterPts:"<<clPhList.size();
                            
                            this->createCluster(&clPhList, false);
                            
                            thisClsId = ClsType::GetClusterID();
                            
                            if(thisClsId-prevClsId > 1){
                                auto itrBRM = this->gridMap.find(boxID);
                                itrBRM->second->GetClusterID(&mainClsIDSet);
                                
                                this->manageCluster(&clPhList, prevClsId+1, thisClsId, &mainClsIDSet);
                                
                                //this->createCluster(&clPhList, false);
                            }
                        }
                        
                        /*cout<<"\nx1="<<min[0]<<", x2="<<max[0]<<"=>";
                         for(list<PerfType*>::iterator itr=clPhList.begin(); itr!=clPhList.end(); itr++){
                         PerfType* ph = *itr;
                         cout<<"\n"<<ph->getIndividualId()<<":"<<ph->GetClusterIdList();
                         }
                         cout<<"\n";*/
                        
                        boxID++;
                        mainClsIDSet.clear();
                        
                    }
                }
            }
            
        }
        
        template<typename PerfType, typename DPType, typename QTType, typename GType, typename FHType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        void Mapper<PerfType,DPType,QTType,GType,FHType,ClusterIDType,RowIDType,ClusterType>::testRange(std::vector<float> &min, std::vector<float> &max, std::list<PerfType *> &clPhList){
            std::cout<<"\nTesting:"<<std::endl;
            for(auto itr=clPhList.begin(); itr!=clPhList.end(); itr++){
                PerfType* ph = *itr;
                
                for(size_t i=0; i<min.size(); i++){
                    if(max[i]>min[i]){
                        auto f = ph->getFilter(i);
                        
                        if(f<min[i] || f>max[i]){
                            std::cout<<ph->getID()<<":"<<f<<",";
                        }
                    }
                }
            }
        }
        
        template<typename PerfType, typename DPType, typename QTType, typename GType, typename FHType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        void Mapper<PerfType,DPType,QTType,GType,FHType,ClusterIDType,RowIDType,ClusterType>::assignClusterIdsForOverlapingRegion(std::list<PerfType*>* overlappedPhList, std::vector<float> overlap){
            std::cout<<"Still clustering, your patience is appreciating...\n";
            
            std::vector<float> min(hyppox::Config::FILTER, 0.0), max(hyppox::Config::FILTER, 0.0);
            std::list<PerfType*> clPhList;
            std::list<DPType*> dpList;
            size_t boxID = 1, prevClsId, thisClsId;
            std::unordered_set<size_t> boxIdSet;
            std::unordered_set<size_t> mainClsIDSet;
            std::unordered_set<std::string> phListTrack;
            
            std::unordered_set<size_t> indvSet;
            std::unordered_set<std::string> edgeSet;
            
            //cout<<"\n\n";
            
            if(this->mesh.size()>0){
                for(int j=0; j<hyppox::Config::WINDOWS[1];j++){
                    for(int i=0; i<hyppox::Config::WINDOWS[0]; i++){
                        for(short f=0; f<hyppox::Config::FILTER; f++){
                            float c = this->mesh[i][j].center[f];
                            float l = (this->gridLength[f]*(1+overlap[f]))/2;
                            min[f] = c - l;
                            max[f] = c + l;
                            
                            if(min[f] < this->minPos[f]) min[f] = this->minPos[f];
                            if(max[f] > this->maxPos[f]) max[f] = this->maxPos[f];
                        }
                        
                        clPhList.clear();
                        dpList.clear();
                        phListTrack.clear();
                        
                        if(isEligibleForClustering(min, max, boxID)){
                            this->treeRoot.SearchSurface(min, max, &clPhList, &dpList, boxID);
                        }
                        
                        //std::cout<<"("<<i<<","<<j<<")={("<<min[0]<<","<<min[1]<<"),("<<max[0]<<","<<max[1]<<")}: ";
                        
                        if(clPhList.size() > 0){
                            /*if(i==6){
                             testRange(min, max, clPhList);
                             }*/
                            
                            prevClsId = ClsType::GetClusterID();
                            
                            //std::cout<<"Data points:"<<dpList.size()<<", clusterPts:"<<clPhList.size();
                            time_t _t1 = clock();
                            
                            this->createCluster(&clPhList, false);
                            
                            time_t _t2 = clock();
                             
                             float _time = (float)(_t2-_t1)/CLOCKS_PER_SEC;
                             std::string unit = "(sec)";
                             
                             if(_time<1){
                             _time = ((float)(_t2-_t1)*1000)/CLOCKS_PER_SEC;
                             unit = "(ms)";
                             }
                             
                             //std::cout<<" Total time for clustering:"<<_time<<unit;
                            
                            thisClsId = ClsType::GetClusterID();
                            
                            //std::cout<<" ID:"<<thisClsId<<std::endl;
                            
                            if(thisClsId-prevClsId > 1){
                                auto itrBRM = this->gridMap.find(boxID);
                                itrBRM->second->GetClusterID(&mainClsIDSet);
                                
                                this->manageCluster(&clPhList, prevClsId+1, thisClsId, &mainClsIDSet);
                                
                                //this->createCluster(&clPhList, false);
                            }
                        }
                        
                        /*cout<<"\nx1="<<min[0]<<", x2="<<max[0]<<"=>";
                         for(list<PerfType*>::iterator itr=clPhList.begin(); itr!=clPhList.end(); itr++){
                         PerfType* ph = *itr;
                         cout<<"\n"<<ph->getIndividualId()<<":"<<ph->GetClusterIdList();
                         }
                         cout<<"\n";*/
                        
                        boxID++;
                        mainClsIDSet.clear();
                        
                        setOverlappedIndv(&indvSet, &clPhList, overlappedPhList, &edgeSet);
                        
                    }
                }
            }
            
        }
        
        template<typename PerfType, typename DPType, typename QTType, typename GType, typename FHType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        void Mapper<PerfType,DPType,QTType,GType,FHType,ClusterIDType,RowIDType,ClusterType>::setOverlappedIndv(std::unordered_set<size_t> *indvSet, std::list<PerfType *> *clPhList, std::list<PerfType *> *overlappedPhList, std::unordered_set<std::string>* edgeSet){
            
            for(auto itr=clPhList->begin(); itr!=clPhList->end(); itr++){
                PerfType* ph = *itr;
                
                //if(indvSet->find(ph->getID())==indvSet->end()){
                //cout<<"\n"<<key<<":"<<ph->GetClusterIdList();
                indvSet->insert(ph->getID());
                overlappedPhList->push_back(ph);
                
                std::vector<ClusterIDType> idList(hyppox::Config::TOTAL_CLUSTER_IDS, 0);
                std::vector<std::string> typeList(hyppox::Config::TOTAL_CLUSTER_IDS, "");
                
                ph->GetIDList(idList);
                ph->GetTypeList(typeList);
                
                std::vector<ClusterIDType> idVector;
                std::vector<std::string> typeVector;
                
                for(int i=0;i<hyppox::Config::TOTAL_CLUSTER_IDS;i++){
                    if(idList[i]>0){
                        idVector.push_back(idList[i]);
                        typeVector.push_back(typeList[i]);
                    }
                }
                
                for(ClusterIDType i=0;i<idVector.size(); i++){
                    for(ClusterIDType j=i+1; j<idVector.size(); j++){
                        // If this point is not a noise for both clusters
                        //if(typeVector[i].compare("P")!=0 || typeVector[j].compare("P")!=0){
                        std::string k = (idVector[i]<idVector[j])?(std::to_string(idVector[i]) + "#" + std::to_string(idVector[j])):(std::to_string(idVector[j]) + "#" + std::to_string(idVector[i]));
                        
                        if(edgeSet->find(k)==edgeSet->end()){
                            edgeSet->insert(k);
                            EType* e = new EType(idVector[i],idVector[j]);
                            this->edgeList.push_back(e);
                        }
                        //}
                    }
                }
                
                //}
            }
            
        }
        
        template<typename PerfType, typename DPType, typename QTType, typename GType, typename FHType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        void Mapper<PerfType,DPType,QTType,GType,FHType,ClusterIDType,RowIDType,ClusterType>::generateEdgeListFromOverlappingClusters(std::list<PerfType*>* overlappedPhList, std::vector<float> overlap){
            
            std::vector<float> min(hyppox::Config::FILTER,0.0), max(hyppox::Config::FILTER,0.0);
            std::list<PerfType*> clPhList;
            std::list<DPType*> dpList;
            std::unordered_set<size_t> indvSet;
            std::unordered_set<std::string> edgeSet;
            ClusterIDType boxID = 1;
            std::vector<ClusterIDType> clsRange(hyppox::Config::TOTAL_CLUSTER_IDS+4, 0);
            
            for(int j=0; j<hyppox::Config::WINDOWS[1];j++){
                for(int i=0; i<hyppox::Config::WINDOWS[0]; i++){
                    
                    clPhList.clear();
                    dpList.clear();
                    
                    if(i>0){
                        for(short f=0; f<hyppox::Config::FILTER; f++){
                            float c2 = this->mesh[i][j].center[f];
                            float c1 = this->mesh[i-1][j].center[f];
                            
                            float l = (this->gridLength[f]*(1+overlap[f]))/2;
                            min[f] = c2 - l;
                            max[f] = c1 + l;
                            
                            if(min[f] < this->minPos[f]) min[f] = this->minPos[f];
                            if(max[f] > this->maxPos[f]) max[f] = this->maxPos[f];
                        }
                        
                        this->treeRoot.SearchSurface(min, max, &clPhList, &dpList, boxID);
                    }
                    
                    if(j>0){
                        for(short f=0; f<hyppox::Config::FILTER; f++){
                            float c2 = this->mesh[i][j].center[f];
                            float c1 = this->mesh[i][j-1].center[f];
                            
                            float l = (this->gridLength[f]*(1+overlap[f]))/2;
                            min[f] = c2 - l;
                            max[f] = c1 + l;
                            
                            if(min[f] < this->minPos[f]) min[f] = this->minPos[f];
                            if(max[f] > this->maxPos[f]) max[f] = this->maxPos[f];
                        }
                        
                        this->treeRoot.SearchSurface(min, max, &clPhList, &dpList, boxID);
                    }
                    
                    boxID++;
                    
                    setOverlappedIndv(&indvSet, &clPhList, overlappedPhList, &edgeSet);
                }
            }
            
        }
        
        template<typename PerfType, typename DPType, typename QTType, typename GType, typename FHType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        void Mapper<PerfType,DPType,QTType,GType,FHType,ClusterIDType,RowIDType,ClusterType>::addSimplicialComplex(SCType *sc, float overlap){
            std::vector<float> min(hyppox::Config::FILTER, 0.0), max(hyppox::Config::FILTER, 0.0);
            
            // Make it 50%
            overlap *=50;
            
            if(this->mesh.size()>0){
                sc->clearTracker();
                
                std::list<PerfType*> clPhList;
                std::list<DPType*> dpList;
                ClusterIDType boxID = 1;
                
                for(int j=0; j<hyppox::Config::WINDOWS[1];j++){
                    for(int i=0; i<hyppox::Config::WINDOWS[0]; i++){
                        
                        clPhList.clear();
                        dpList.clear();
                        
                        for(short f=0; f<hyppox::Config::FILTER; f++){
                            float c = this->mesh[i][j].center[f];
                            float l = this->gridLength[f]/2;
                            min[f] = c - l;
                            max[f] = c + l;
                            
                            if(min[f] < this->minPos[f]) min[f] = this->minPos[f];
                            if(max[f] > this->maxPos[f]) max[f] = this->maxPos[f];
                        }
                        
                        this->treeRoot.SearchSurface(min, max, &clPhList, &dpList, boxID);
                        
                        for(auto pItr = clPhList.begin(); pItr!=clPhList.end(); pItr++){
                            PerfType* ph = *pItr;
                            
                            sc->AddSimplicialComplex(ph, overlap);
                        }
                        
                        boxID++;
                    }
                }
            }
            
        }
        
        template<typename PerfType, typename DPType, typename QTType, typename GType, typename FHType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        std::string Mapper<PerfType,DPType,QTType,GType,FHType,ClusterIDType,RowIDType,ClusterType>::generateSimplicesForPersistentHomology(){
            SCType sc;
            
            std::vector<float> oV(hyppox::Config::FILTER);
            for(float Ovx=0.00; Ovx<=1.00; Ovx+=0.05){
                for(short i=0; i<hyppox::Config::FILTER; i++){
                    oV[i] = Ovx;
                }
                
                this->clearClusterInformation();
                this->assignClusterIdsForOverlapingRegion(oV);
                
                float overlap = Ovx;
                this->addSimplicialComplex(&sc, overlap);
                
            }
            
            std::string r = "";
            
            if(hyppox::Config::PH_JAVA_PLEX){
                FHType* writeToFile = new FHType("");
                r = writeToFile->WriteDataToFile(hyppox::Config::WRITE_DIR+"Simplex_for_Javaplex", ".txt", sc.PrintSimplex(), false);
                delete writeToFile;
                
                std::cout<<"\nDump the code of file: \""<<r<<"\" to Java file to generate barcode.";
            }else{
                std::vector<float> v = sc.getPersistentOverlap();
                r = "\n Persistent overlap: ";
                for(size_t i=0; i<v.size(); i++){
                    r += "\nDimension-" + std::to_string(i) + ", Overlap: " + std::to_string(v[i]) + "%";
                }
                //std::cout<<"\n Persistent overlap: "<<r<<"\n\n";
            }
            
            return r;
        }
        
        template<typename PerfType, typename DPType, typename QTType, typename GType, typename FHType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        void Mapper<PerfType,DPType,QTType,GType,FHType,ClusterIDType,RowIDType,ClusterType>::computeConnectedComponents(size_t &ccSize){
            graph::UnionFindWithPathCompression<long> ufpc(ClsType::GetClusterID());
            
            for(auto itr = this->edgeList.begin(); itr!=this->edgeList.end(); itr++){
                
                EType* e = *itr;
                
                long node_1 = e->getNode1();
                long node_2 = e->getNode2();
                //cout<<"\n"<<node_1<<","<<node_2;
                ufpc.Union(node_1, node_2);
            }
            
            this->connectedComponents = ufpc.getConnectedComponents(ccSize);
        }
        
        template<typename PerfType, typename DPType, typename QTType, typename GType, typename FHType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        void Mapper<PerfType,DPType,QTType,GType,FHType,ClusterIDType,RowIDType,ClusterType>::constructGraphComponents(GType*& graph){
            std::cout<<"Started constructing graph...\n";
            
            size_t size;
            this->computeConnectedComponents(size);
            
            std::cout<<"Total subgraphs:"<<size<<std::endl;
            std::cout<<"Total Nodes:"<<this->connectedComponents.size()<<std::endl;
            std::cout<<"Total Edges:"<<this->edgeList.size()<<std::endl;
            
            std::vector<float> min(hyppox::Config::FILTER, 0.0), max(hyppox::Config::FILTER,0.0);
            
            if(graph==nullptr) graph = new GType();
            graph->setConnectedComponents(size);
            
            if(this->mesh.size()>0){
                std::list<PerfType*> clPhList;
                std::list<DPType*> dpList;
                ClusterIDType boxID = 1;
                
                for(int j=0; j<hyppox::Config::WINDOWS[1];j++){
                    for(int i=0; i<hyppox::Config::WINDOWS[0]; i++){
                        
                        clPhList.clear();
                        dpList.clear();
                        
                        for(short f=0; f<hyppox::Config::FILTER; f++){
                            float c = this->mesh[i][j].center[f];
                            float l = this->gridLength[f]/2;
                            min[f] = c - l;
                            max[f] = c + l;
                            
                            if(min[f] < this->minPos[f]) min[f] = this->minPos[f];
                            if(max[f] > this->maxPos[f]) max[f] = this->maxPos[f];
                        }
                        
                        this->treeRoot.SearchSurface(min, max, &clPhList, &dpList, boxID);
                        
                        for(auto pItr = clPhList.begin(); pItr!=clPhList.end(); pItr++){
                            PerfType* ph = *pItr;
                            
                            std::vector<ClusterIDType> idList(hyppox::Config::TOTAL_CLUSTER_IDS, 0);
                            ph->GetIDList(idList);
                            
                            /*if(ph->getID()==1805944 || ph->getID()==2149840 || ph->getID()==2100947){
                             cout<<"";
                             }*/
                            
                            idList.erase(remove(idList.begin(), idList.end(), 0), idList.end());
                            
                            for(ClusterIDType _id:idList){
                                auto ccItr=this->connectedComponents.find(_id);
                                if(ccItr!=this->connectedComponents.end()){
                                    short index = (short)ccItr->second;
                                    auto subGraph = graph->getConnectedComponent(index);
                                    subGraph->addPointToNode(_id, ph);
                                }
                            }
                        }
                        
                        boxID++;
                    }
                }
            }
            
            short index_1, index_2;
            for(auto itr = this->edgeList.begin(); itr!=this->edgeList.end(); itr++){
                
                EType* e = *itr;
                
                size_t node_1 = e->getNode1();
                size_t node_2 = e->getNode2();
                
                if(this->connectedComponents.find(node_1)!=this->connectedComponents.end() && this->connectedComponents.find(node_2)!=this->connectedComponents.end()){
                    
                    auto ccItr = this->connectedComponents.find(node_1);
                    index_1 = (short)ccItr->second;
                    
                    ccItr = this->connectedComponents.find(node_2);
                    index_2 = (short)ccItr->second;
                    
                    if(index_1==index_2){
                        auto subGraph = graph->getConnectedComponent(index_1);
                        subGraph->addEdge(node_1, node_2);
                    }
                }
            }
            
            std::vector<float> sizeRange(2,0.0), _v(2, 0.0);
            std::vector<std::vector<float> > weightRange(hyppox::Config::FILTER+hyppox::Config::CLUSTER, _v);
            
            for(short i=0;i<hyppox::Config::FILTER+hyppox::Config::CLUSTER;i++){
                weightRange[i][0] = 0.0;
                weightRange[i][1] = 0.0;
            }
            
            for(size_t i=0; i<size; i++){
                std::vector<float> _sizeRange(2,0.0);
                std::vector<std::vector<float> > _weightRange(hyppox::Config::FILTER+hyppox::Config::CLUSTER, _v);
                
                auto subGraph = graph->getConnectedComponent(i);
                
                subGraph->setupLinks();
                subGraph->getNodeSizeAndWeightRange(_sizeRange, _weightRange);
                
                if(i==0){
                    sizeRange[0] = _sizeRange[0];
                    sizeRange[1] = _sizeRange[1];
                    
                    for(short j=0;j<hyppox::Config::FILTER+hyppox::Config::CLUSTER;j++){
                        weightRange[j][0] = _weightRange[j][0];
                        weightRange[j][1] = _weightRange[j][1];
                    }
                }else{
                    if(_sizeRange[0]<sizeRange[0]) sizeRange[0] = _sizeRange[0];
                    if(_sizeRange[1]>sizeRange[1]) sizeRange[1] = _sizeRange[1];
                    
                    for(short j=0;j<hyppox::Config::FILTER+hyppox::Config::CLUSTER;j++){
                        if(_weightRange[j][0]<weightRange[j][0]) weightRange[j][0] = _weightRange[j][0];
                        if(_weightRange[j][1]>weightRange[j][1]) weightRange[j][1] = _weightRange[j][1];
                    }
                }
                
            }
            
            graph->setGlobalNodeSizeAndWeightRange(sizeRange, weightRange);
            
            for(size_t i=0; i<size; i++){
                auto subGraph = graph->getConnectedComponent(i);
                
                for(int j=0;j<2;j++) subGraph->setGlobalSizeRange(sizeRange[j], j);
                for(short j=0;j<hyppox::Config::FILTER+hyppox::Config::CLUSTER;j++) subGraph->setGlobalWeightRange(weightRange[j], j);
            }
        }
        
        template<typename PerfType, typename DPType, typename QTType, typename GType, typename FHType, typename ClusterIDType, typename RowIDType, typename ClusterType>
        void Mapper<PerfType,DPType,QTType,GType,FHType,ClusterIDType,RowIDType,ClusterType>::constructGraph(GType*& graph){
            
            std::list<PerfType*> overlappedIndiv;
            
            overlappedIndiv.clear();
            edgeList.clear();
            
            time_t t1 = clock();
            this->clearClusterInformation();
            this->assignClusterIdsForOverlapingRegion(&overlappedIndiv, hyppox::Config::GAIN);
            
            time_t t2 = clock();
            
            std::vector<std::string> _time = getTime((float)(t2-t1)/CLOCKS_PER_SEC);
            std::cout<<"Clusteing completed...\nTotal time for clustering:"<<_time[0]<<_time[1]<<std::endl;
            
            //cout<<"Total time:"<<_time<<unit<<endl;
            
            //overlappedIndiv.clear();
            //edgeList.clear();
            
            //this->generateEdgeListFromOverlappingClusters(&overlappedIndiv, hyppox::Config::GAIN);
            t1 = clock();
            this->constructGraphComponents(graph);
            t2 = clock();
            
            _time = getTime((float)(t2-t1)/CLOCKS_PER_SEC);
            
            std::cout<<"Graph construction completed...\nTotal time to construct graph:"<<_time[0]<<_time[1]<<std::endl;
        }
    }
}


#endif /* mapper_hp */
