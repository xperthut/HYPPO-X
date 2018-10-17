/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: Graph.h
 Objective: Class to generate graph
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              02/16/2017      Initial version
 Md. Kamruzzaman              06/20/2017      Added logic to color connected components, either all components under same coloring scheme or separate coloring scheme for each connect component, controlled by a flag
 Md. Kamruzzaman              01/06/2018      Code revision, fixing obscure logic issue
 **************************************************************************************************/

#ifndef graph_h
#define graph_h

#include "dbScan.h"
#include "RGBColor.h"
#include <bitset>
#include <vector>
#include <list>
#include <algorithm>
#include "customCollection.h"

enum STAGE {INT_PATH=0, FLARE};

//// Definition of global methods ///////
const std::string fixPrecision(float value, int precision){
    std::string pValue = std::to_string(value);
    size_t pos = 0;
    
    if((pos = pValue.find(".")) != std::string::npos){
        
        if(pValue.length()-pos-1 > precision){
            if(precision>0) return pValue.substr(0, pos+1+precision);
            return pValue.substr(0, pos);
        }
    }
    
    return pValue;
}

const std::string getHTMLColor(size_t index){
    std::vector<std::string> s={"#ff0000", "#3cb44b", "#ffe119", "#4363d8", "#f58231", "#911eb4", "#46f0f0", "#f032e6", "#bcf60c", "#fabebe", "#008080", "#e6beff", "#9a6324", "#fffac8", "#800000", "#aaffc3", "#808000", "#ffd8b1", "#000075", "#808080"};
    return s[index%s.size()];
}

namespace hyppox {
    namespace graph{
        namespace path{
            class InterestingPath{
                private:
                CustomHashSet<std::string> path;
                float score;
                size_t rank;
                std::string color;
                
                public:
                InterestingPath();
                ~InterestingPath()=default;
                
                void setScore(float s);
                float getScore();
                void setRank(size_t r);
                size_t getRank();
                void addToPath(std::string edge);
                std::unordered_set<std::string>& getPath();
                void setColor(std::string c);
                std::string getColor();
                bool PathContainsEdge(std::string eID);
                static bool comparePaths(InterestingPath& p1, InterestingPath& p2){
                    return (p1.getScore()>p2.getScore());
                }
            };
            
            InterestingPath::InterestingPath(){
                this->rank = 0;
                this->score = 0.0;
            }
            
            void InterestingPath::setScore(float s){this->score = s;}
            float InterestingPath::getScore(){return this->score;}
            void InterestingPath::setRank(size_t r){this->rank = r;}
            size_t InterestingPath::getRank(){return this->rank;}
            void InterestingPath::addToPath(std::string edge){this->path.addToSet(edge);}
            std::unordered_set<std::string>& InterestingPath::getPath(){return this->path.getSet();}
            void InterestingPath::setColor(std::string c){this->color = c;}
            std::string InterestingPath::getColor(){return this->color;}
            bool InterestingPath::PathContainsEdge(std::string eID){return this->path.hasItem(eID);}
        }
        
        namespace flare{
            class CoverageBranch{
                public:
                int totalCovOutDegree;
                int coveredOutDegree;
                int totalCovInDegree;
                float score;
                bool visited;
                
                CoverageBranch():totalCovOutDegree(0),score(0.0),coveredOutDegree(0),totalCovInDegree(0),visited(false){}
                ~CoverageBranch()=default;
            };
            
            class Coverage{
                private:
                size_t rank;
                CustomHashSet<std::string> edgeList;
                CustomHashSet<size_t> terminalNodes;
                CustomHashMap<size_t, CoverageBranch> branchNodeScore;
                float score;
                size_t sourceNode;
                std::string color;
                
                public:
                Coverage(size_t sID);
                ~Coverage();
                void addCoverageEdge(std::string eID);
                void addTerminalNode(size_t tID);
                void addScore(float w);
                void setColor(std::string c);
                void setRank(size_t r);
                
                bool hasCoverageEdge(std::string eID);
                float getScore();
                std::string getColor();
                size_t getRank();
                size_t getSourceNode();
                void clearEdgeCoverage();
                std::unordered_set<size_t> getTerminalNodes();
                std::unordered_set<std::string> getEdgeList();
                //void copyBranchNodeScore(CustomHashMap<size_t, CoverageBranch> *brNodeScore);
                CustomHashMap<size_t, CoverageBranch>& getCoverageBranchNodeScore();
                static bool compareFlares(Coverage& c1, Coverage& c2){
                    return (c1.getScore()>c2.getScore());
                }
            };
            
            Coverage::Coverage(size_t sID){
                this->score=0.0;
                this->rank=0;
                this->edgeList.clearHashSet();
                this->terminalNodes.clearHashSet();
                this->sourceNode = sID;
                this->color = "";
            }
            
            Coverage::~Coverage(){
                this->edgeList.clearHashSet();
                this->terminalNodes.clearHashSet();
            }
            
            void Coverage::addCoverageEdge(std::string eID){this->edgeList.addToSet(eID);}
            void Coverage::addScore(float w){this->score += w;}
            void Coverage::addTerminalNode(size_t tID){this->terminalNodes.addToSet(tID);}
            void Coverage::setColor(std::string c){this->color = c;}
            void Coverage::setRank(size_t r){this->rank = r;}
            
            bool Coverage::hasCoverageEdge(std::string eID){return this->edgeList.hasItem(eID);}
            float Coverage::getScore(){return this->score;}
            std::string Coverage::getColor(){return this->color;}
            size_t Coverage::getRank(){return this->rank;}
            void Coverage::clearEdgeCoverage(){this->edgeList.clearHashSet();}
            size_t Coverage::getSourceNode(){return this->sourceNode;}
            std::unordered_set<size_t> Coverage::getTerminalNodes(){return this->terminalNodes.getSet();}
            /*void Coverage::copyBranchNodeScore(CustomHashMap<size_t, CoverageBranch> *brNodeScore){
                this->branchNodeScore.copyHashMap(brNodeScore);
            }*/
            CustomHashMap<size_t, CoverageBranch>& Coverage::getCoverageBranchNodeScore(){
                return this->branchNodeScore;
            }
            
            std::unordered_set<std::string> Coverage::getEdgeList(){return this->edgeList.getSet();}
        }
        
        namespace cell{
            class CostTableCell{
                private:
                float score;
                CostTableCell* maxPred;
                std::string edgeID;
                
                public:
                CostTableCell();
                ~CostTableCell()=default;
                void setScore(float s);
                float getScore();
                void setCell(CostTableCell* mp);
                CostTableCell* getCell();
                std::string getEdgeID();
                void setEdgeID(std::string eID);
            };
            
            CostTableCell::CostTableCell():score(0),maxPred(nullptr),edgeID(""){}
            
            void CostTableCell::setScore(float s){this->score = s;}
            void CostTableCell::setCell(CostTableCell *mp){this->maxPred = mp;}
            float CostTableCell::getScore(){return this->score;}
            CostTableCell* CostTableCell::getCell(){return this->maxPred;}
            void CostTableCell::setEdgeID(std::string eID){this->edgeID = eID;}
            std::string CostTableCell::getEdgeID(){return this->edgeID;}
        }
        
        namespace graph_element{
            class Property{
                private:
                std::vector<float> weight;
                size_t depth;
                
                public:
                Property();
                ~Property()=default;
                float getWeight(short index);
                void setWeight(float value, short index);
                size_t getDepth();
                void setDepth(size_t d);
            };
            
            Property::Property():depth(0){
                this->weight.assign(hyppox::Config::FILTER+hyppox::Config::CLUSTER, 0.0);
            }
            
            float Property::getWeight(short index){return this->weight[index];}
            void Property::setWeight(float value, short index){this->weight[index]=value;}
            size_t Property::getDepth(){return this->depth;}
            void Property::setDepth(size_t d){this->depth=d;}
            
            template<typename PerfType>
            class rNode:public Property{
                public:
                //using RNType = rNode<PerfType>;
                
                rNode(size_t _id);
                ~rNode();
                
                size_t getID();
                size_t getSize();
                size_t getTotalInDegree();
                size_t getTotalOutDegree();
                bool IsThisNodeASource();
                rNode<PerfType>* getNextNode(size_t index);
                rNode<PerfType>* getPrevNode(size_t index);
                void removeNextNode(rNode<PerfType>* n);
                void removePrevNode(rNode<PerfType>* n);
                std::set<size_t> getIndvCoverage();
                float getWeightOfIndividuals(std::set<size_t> indv, std::unordered_map<std::string, size_t>*& _individualIndexMap);
                std::string getPieChart();
                
                //void setSize(float size);
                void addInDegree();
                void addOutDegree();
                void setSource(bool source);
                
                void createLinkNodes();
                void addNextNode(rNode<PerfType>* nextNode);
                void addPrevNode(rNode<PerfType>* prevNode);
                void resetLinkNodes();
                void addPoint(PerfType* ph, std::unordered_map<std::string, size_t>*& _individualIndexMap);
                std::string getPhListForD3();
                std::string getPhIdListForD3();
                void writeNodeInfo();
                std::string getPointDetails();
                static bool compareByFilter(rNode<PerfType>* n1, rNode<PerfType>* n2){
                    return (n1->getWeight(hyppox::Config::CLUSTER+hyppox::Config::SORT_FILTER_INDEX)<n2->getWeight(hyppox::Config::CLUSTER+hyppox::Config::SORT_FILTER_INDEX));
                }
                
                private:
                size_t _id;
                size_t in_degree;
                size_t out_degree;
                //float size;
                bool isSource;
                bool isMarked;
                std::vector<rNode<PerfType>*> nextNodes;
                std::vector<rNode<PerfType>*> prevNodes;
                std::unordered_map<size_t, size_t> indv_cov;
                std::set<size_t> indv_cov_set;
                CustomHashMap<size_t, PerfType*> points;
                std::unordered_set<std::string> corePoints;
            };
            
            template<typename PerfType>
            rNode<PerfType>::rNode(size_t _id){
                this->_id = _id;
                //this->size = 0.0;
                this->in_degree = this->out_degree = 0;
                this->isSource = false;
                this->isMarked = false;
                this->nextNodes.clear();
                this->prevNodes.clear();
                for(short i=0; i<(hyppox::Config::CLUSTER+hyppox::Config::FILTER); i++){
                    this->setWeight(0.0, i);
                }
                this->corePoints.clear();
            }
            
            template<typename PerfType>
            rNode<PerfType>::~rNode(){
                this->nextNodes.clear();
                this->prevNodes.clear();
            }
            
            // Getter
            template<typename PerfType>
            size_t rNode<PerfType>::getID(){return _id;}
            
            template<typename PerfType>
            size_t rNode<PerfType>::getSize(){return this->corePoints.size();}
            
            template<typename PerfType>
            size_t rNode<PerfType>::getTotalInDegree(){return in_degree;}
            
            template<typename PerfType>
            size_t rNode<PerfType>::getTotalOutDegree(){return out_degree;}
            
            template<typename PerfType>
            bool rNode<PerfType>::IsThisNodeASource(){return isSource;}
            
            // Setter
            //void rNode::setSize(float size){this->size += size;}
            template<typename PerfType>
            void rNode<PerfType>::addInDegree(){this->in_degree++;}
            
            template<typename PerfType>
            void rNode<PerfType>::addOutDegree(){this->out_degree++;}
            
            template<typename PerfType>
            void rNode<PerfType>::setSource(bool source){this->isSource = source;}
            
            template<typename PerfType>
            void rNode<PerfType>::createLinkNodes(){
                if(this->out_degree>0){
                    this->nextNodes.assign(this->out_degree, nullptr);
                }
                
                if(this->in_degree>0){
                    this->prevNodes.assign(this->in_degree, nullptr);
                }
            }
            
            template<typename PerfType>
            void rNode<PerfType>::addNextNode(rNode<PerfType> *nextNode){
                for(int i=0; i<this->out_degree; i++){
                    if(this->nextNodes[i] == nullptr){
                        this->nextNodes[i] = nextNode;
                        break;
                    }
                }
            }
            
            template<typename PerfType>
            void rNode<PerfType>::addPrevNode(rNode<PerfType> *prevNode){
                for(int i=0; i<this->in_degree; i++){
                    if(this->prevNodes[i] == nullptr){
                        this->prevNodes[i] = prevNode;
                        break;
                    }
                }
            }
            
            template<typename PerfType>
            rNode<PerfType>* rNode<PerfType>::getNextNode(size_t index){
                return nextNodes.at(index);
            }
            
            template<typename PerfType>
            rNode<PerfType>* rNode<PerfType>::getPrevNode(size_t index){
                return prevNodes.at(index);
            }
            
            template<typename PerfType>
            void rNode<PerfType>::removeNextNode(rNode<PerfType>* n){
                for(auto itr = this->nextNodes.begin(); itr!=this->nextNodes.end(); itr++){
                    rNode<PerfType>* tmpNode = *itr;
                    
                    if(n->getID()==tmpNode->getID()){
                        this->nextNodes.erase(itr);
                        this->out_degree--;
                        break;
                    }
                }
            }
            
            template<typename PerfType>
            void rNode<PerfType>::removePrevNode(rNode<PerfType>* n){
                for(auto itr = this->prevNodes.begin(); itr!=this->prevNodes.end(); itr++){
                    rNode<PerfType>* tmpNode = *itr;
                    
                    if(n->getID()==tmpNode->getID()){
                        this->prevNodes.erase(itr);
                        this->in_degree--;
                        break;
                    }
                }
            }
            
            template<typename PerfType>
            std::set<size_t> rNode<PerfType>::getIndvCoverage(){
                if(this->indv_cov_set.size()==0){
                    for(auto itr:this->indv_cov){
                        this->indv_cov_set.insert(itr.first);
                    }
                }
                return this->indv_cov_set;
            }
            
            template<typename PerfType>
            void rNode<PerfType>::resetLinkNodes(){
                
                this->nextNodes.clear();
                this->prevNodes.clear();
                
                this->in_degree = this->out_degree = 0;
            }
            
            template<typename PerfType>
            void rNode<PerfType>::addPoint(PerfType *ph, std::unordered_map<std::string, size_t>*& _individualIndexMap){
                size_t totalPoints = this->points.getSize();
                
                if(!points.hasItem(ph->getID())){
                    size_t i=0;
                    for(;i<hyppox::Config::CLUSTER; i++){
                        float avg = this->getWeight(i);
                        float tw = (avg*totalPoints)+ph->getClusterValue(i);
                        this->setWeight(tw/(totalPoints+1), i);
                    }
                    
                    for(size_t j=0; j<hyppox::Config::FILTER; j++,i++){
                        float avg = this->getWeight(i);
                        float tw = (avg*totalPoints)+ph->getFilter(j);
                        this->setWeight(tw/(totalPoints+1), i);
                    }
                    
                    points.addToMap(ph->getID(), ph);
                    
                    this->corePoints.insert(ph->getPoint());
                    
                    //this->setSize(ph->GetWeight(this->getID())==1?1:0);
                    
                    if(hyppox::Config::COL_INDIVIDUAL.size()>0){
                        auto itr = _individualIndexMap->find(ph->getIndividualId());
                        this->indv_cov_set.insert(itr->second);
                        if(itr!=_individualIndexMap->end()){
                            auto _citr=this->indv_cov.find(itr->second);
                            if(_citr==this->indv_cov.end()){
                                this->indv_cov.insert(std::make_pair(itr->second, 1));
                            }else{
                                _citr->second++;
                            }
                        }
                    }
                }
            }
            
            template<typename PerfType>
            float rNode<PerfType>::getWeightOfIndividuals(std::set<size_t> indv, std::unordered_map<std::string, size_t> *&_individualIndexMap){
                float avgW = 0.0;
                size_t c = 0;
                auto _pts = this->points.getMap();
    
                for(auto itr = _pts.begin(); itr!=_pts.end(); itr++){
                    PerfType* ph = itr->second;
                    
                    auto itr1=_individualIndexMap->find(ph->getIndividualId());
                    if(itr1 != _individualIndexMap->end()){
                        if(indv.find(itr1->second)!=indv.end()){
                            avgW += ph->getClusterValue(hyppox::Config::REFERENCE_PH_INDEX);
                            c++;
                        }
                    }
                }
                
                return (c>1?avgW/c:avgW);
            }
            
            template<typename PerfType>
            std::string rNode<PerfType>::getPhListForD3(){
                std::string phList = "[";
                auto phMap = this->points.getMap();
                
                for(auto itr=phMap.begin(); itr!=phMap.end(); itr++){
                    PerfType* ph = itr->second;
                    
                    if(phList.length()>1) phList += ",";
                    
                    std::string weight = "["+fixPrecision(ph->getClusterValue(hyppox::Config::REFERENCE_PH_INDEX), 2);
                    for(short i=0; i<hyppox::Config::FILTER; i++){
                        if(weight.length()>1) weight +=",";
                        
                        weight += fixPrecision(ph->getFilter(i), 2);
                    }
                    weight += "]";
                    
                    phList += "{\"Gen\":\"" + ph->getGenotype() + "\",\"Loc\":\"" + ph->getLocation() + "\",\"DT\":\"" + ph->getDateTime() +
                    "\",\"Weight\":" + weight + "}";
                }
                phList += "]";
                
                return phList;
            }
            
            template<typename PerfType>
            std::string rNode<PerfType>::getPhIdListForD3(){
                std::string phList = "[";
                auto phMap = this->points.getMap();
                std::set<size_t> phIDSet;
                
                for(auto itr=phMap.begin(); itr!=phMap.end(); itr++){
                    phIDSet.insert(itr->first);
                }
                
                size_t _lastID=0;
                std::string tmp = "";
                for(size_t _id:phIDSet){
                    
                    //if(phList.length()>1) phList += ",";
                    //phList += to_string(_id);
                    
                    if(_lastID==0){
                        _lastID = _id;
                        tmp = std::to_string(_id);
                        
                    }else if(_lastID+1!=_id){
                        if(tmp.length()>0) tmp += ",";
                        tmp += std::to_string(_lastID);
                        if(tmp.length()>0){
                            if(phList.length()>1) phList += ",";
                            
                            phList += "[" + tmp + "]";
                        }
                        
                        _lastID = _id;
                        tmp = std::to_string(_id);
                        
                    }else if(_lastID+1==_id){
                        
                        _lastID = _id;
                    }
                }
                
                if(tmp.length()>0) tmp += ",";
                tmp += std::to_string(_lastID);
                if(tmp.length()>0){
                    if(phList.length()>1) phList += ",";
                    
                    phList += "[" + tmp + "]";
                }
                
                phList += "]";
                
                return phList;
            }
            
            template<typename PerfType>
            std::string rNode<PerfType>::getPieChart(){
                std::string pie = "";
                
                size_t total = 0;
                for(auto itr:this->indv_cov){
                    total += itr.second;
                }
                
                if(total>0){
                    for(auto itr:this->indv_cov){
                        size_t covID = itr.first;
                        float covPer = ((float)itr.second/total)*100.0;
                        
                        if(pie.length()>0) pie += ",";
                        pie += "[" + std::to_string(covID) + "," + fixPrecision(covPer, 2) + "]";
                    }
                }
                
                return "[" + pie + "]";
            }
            
            template<typename PerfType>
            void rNode<PerfType>::writeNodeInfo(){
                std::string data = "";
                
                auto phMap = points.getMap();
                std::vector<std::string> _loc;
                
                for(auto itr:phMap){
                    if(data.length()>0) data += "\n";
                    
                    data += to_string(this->getID()) + ",";
                    data += itr.second->getGenotype();
                    itr.second->getLocation(&_loc);
                    
                    for(std::string _l:_loc){
                        try {
                            data  += "," + std::to_string(stol(_l)) + "";
                        } catch (std::exception& e) {
                            data  += ",\"" + _l + "\"";
                        }
                    }
                }
                
                try {
                    
                    std::ofstream output(hyppox::Config::WRITE_DIR+"node_info.csv", std::ios::app);
                    
                    output<<data<<std::endl;
                    
                    output.close();
                    
                } catch (std::exception& e) {
                    std::cout<<"Error: "<<e.what()<<std::endl;
                }
            }
            
            template<typename PerfType>
            std::string rNode<PerfType>::getPointDetails(){
                std::string data = "";
                
                auto phMap = points.getMap();
                std::vector<std::string> _loc;
                
                for(auto itr:phMap){
                    if(data.length()>0) data += "\n";
                    
                    data += std::to_string(this->getID()) + ",";
                    data += std::to_string(itr.second->getID()) + ",";
                    data += itr.second->getClusterValue() + ",";
                    data += std::to_string(itr.second->getFilter((short)0)) + ",";
                    data += "\"" + itr.second->GetClusterIdList() + "\"";
                }
                
                return data;
            }
            
            ///// Edge methods ///////////////
            template<typename RNType>
            class rEdge:public Property{
                public:
                //using REType = rEdge<RNType>;
                rEdge(RNType* node1, RNType* node2);
                ~rEdge();
                
                std::string getID();
                RNType* getFromNode();
                RNType* getToNode();
                bool getMark();
                void getSignature(size_t &sig);
                void getSignature(std::string &sig);
                bool getSignatureAtIndex(short index);
                rEdge<RNType>* getRefEdge();
                size_t getRank(short index);
                void setRank(short index, size_t r);
                void setColor(short index, std::string c);
                std::string getColor(short index);
                void setDirection(bool dir);
                bool getDirection();
                
                void setMark(bool mark);
                void setSignatue(bool value, short index);
                void setRefEdge(rEdge<RNType>* e);
                
                void swapNodes();
                bool matchSignature(rEdge<RNType>* e);
                bool strictMatchSignature(rEdge<RNType>* e);
                void assignScore(rEdge<RNType> *eMax);
                
                void createCells();
                void destroyCells();
                float getScoreOfCell(size_t index);
                void setScoreOfCell(float score, size_t index);
                cell::CostTableCell* getCostTableCellOfIndex(size_t index);
                void setCostTableCellOfIndex(cell::CostTableCell* cell, size_t index);
                
                private:
                RNType* fromNode;
                RNType* toNode;
                rEdge<RNType>* refEdge;
                std::bitset<BIT_SET_SIZE> signature;
                bool direction;
                cell::CostTableCell* cells;
                bool isMarked;
                std::string _id;
                std::vector<size_t> rank;
                std::vector<std::string> color;
            };
            
            template<typename RNType>
            rEdge<RNType>::rEdge(RNType* node1, RNType* node2){
                if(node1->getWeight(hyppox::Config::REFERENCE_PH_INDEX) > node2->getWeight(hyppox::Config::REFERENCE_PH_INDEX)){
                    this->fromNode = node2;
                    this->toNode = node1;
                }else{
                    this->fromNode = node1;
                    this->toNode = node2;
                }
                
                this->_id = std::to_string(this->fromNode->getID()) + "#" + std::to_string(this->toNode->getID());
                
                // 10->11->10
                /*if(this->id.compare("106#81")==0||this->id.compare("81#107")==0||this->id.compare("107#82")==0){
                 cout<<"";
                 }*/
                
                this->signature.reset();
                this->isMarked = false;
                this->refEdge = nullptr;
                this->rank.assign(2, 0);
                this->color.assign(2, hyppox::Config::EDGE_COLOR);
                this->direction = true;
                
                float w1,w2,w;
                
                // Edge weight based on phenotypic value
                size_t i=0;
                for(;i<hyppox::Config::CLUSTER;i++){
                    w1 = this->fromNode->getWeight(i);
                    w2 = this->toNode->getWeight(i);
                    w = ABS(w2-w1)>0?ABS(w2-w1):hyppox::Config::NEGLIGIBLE_WEIGHT;
                    
                    this->setWeight(w, i);
                }
                
                for(size_t j=0; j<hyppox::Config::FILTER; j++,i++){
                    w1 = this->fromNode->getWeight(i);
                    w2 = this->toNode->getWeight(i);
                    w = ABS(w2-w1)>0?ABS(w2-w1):hyppox::Config::NEGLIGIBLE_WEIGHT;
                    
                    this->setWeight(w, i);
                    
                    w = w2-w1;
                    
                    if(w<0){
                        this->setSignatue(false, j+1);
                    }else{
                        this->setSignatue(true, j+1);
                    }
                }
                
                this->cells = nullptr;
                
                
                // 10->11->10
                /*if(this->id.compare("106#81")==0||this->id.compare("81#107")==0||this->id.compare("107#82")==0){
                 size_t tmp;
                 this->getSignature(tmp);
                 
                 cout<<tmp<<endl;
                 }*/
            }
            
            template<typename RNType>
            rEdge<RNType>::~rEdge(){
                if(this->cells!=nullptr) delete [] this->cells;
            }
            
            // Getter
            template<typename RNType>
            std::string rEdge<RNType>::getID(){return this->_id;}
            
            template<typename RNType>
            RNType* rEdge<RNType>::getFromNode(){return this->fromNode;}
            
            template<typename RNType>
            RNType* rEdge<RNType>::getToNode(){return this->toNode;}
            
            template<typename RNType>
            bool rEdge<RNType>::getMark(){return this->isMarked;}
            
            template<typename RNType>
            void rEdge<RNType>::getSignature(size_t& sig){sig = signature.to_ulong();}
            
            template<typename RNType>
            void rEdge<RNType>::getSignature(std::string &sig){sig = signature.to_string().substr(BIT_SET_SIZE-hyppox::Config::FILTER, hyppox::Config::FILTER);}
            
            template<typename RNType>
            rEdge<RNType>* rEdge<RNType>::getRefEdge(){return this->refEdge;}
            
            template<typename RNType>
            size_t rEdge<RNType>::getRank(short index){return this->rank[index];}
            
            template<typename RNType>
            std::string rEdge<RNType>::getColor(short index){return this->color[index];}
            
            template<typename RNType>
            bool rEdge<RNType>::getDirection(){return this->direction;}
            
            // Setter
            template<typename RNType>
            void rEdge<RNType>::setMark(bool mark){isMarked = mark;}
            
            template<typename RNType>
            void rEdge<RNType>::setSignatue(bool value, short index){
                if(index>0 && index<=hyppox::Config::FILTER){
                    signature.set(hyppox::Config::FILTER-index,value);
                }
            }
            
            template<typename RNType>
            void rEdge<RNType>::setRefEdge(rEdge<RNType> *e){refEdge=e;}
            
            template<typename RNType>
            void rEdge<RNType>::setRank(short index, size_t r){if(this->rank[index]==0)this->rank[index]=r;}
            
            template<typename RNType>
            void rEdge<RNType>::setColor(short index, std::string c){if(this->color[index].compare(hyppox::Config::EDGE_COLOR)==0)this->color[index] = c;}
            
            template<typename RNType>
            void rEdge<RNType>::setDirection(bool dir){
                this->direction = dir;
            }
            
            template<typename RNType>
            void rEdge<RNType>::swapNodes(){
                RNType* tmp = this->fromNode;
                this->fromNode = this->toNode;
                this->toNode = tmp;
                
                this->_id = std::to_string(this->fromNode->getID()) + "#" + std::to_string(this->toNode->getID());
            }
            
            template<typename RNType>
            bool rEdge<RNType>::matchSignature(rEdge<RNType>* e){
                
                size_t s, sig;
                this->getSignature(s);
                e->getSignature(sig);
                
                if(hyppox::Config::INCREASE_DAP) return (this->getSignatureAtIndex(1) == e->getSignatureAtIndex(1) && this->getSignatureAtIndex(1)==1);
                
                if(!hyppox::Config::MATCH_SIGNATURE) return true;
                
                if(hyppox::Config::DELTA_CHANGE==0){
                    if(hyppox::Config::FILTER_SIGNATURE_MATCHING==0) return (s==sig);
                    
                    return ((s&hyppox::Config::FILTER_SIGNATURE_MATCHING)==(sig&hyppox::Config::FILTER_SIGNATURE_MATCHING));
                }else{
                    if(s==sig) return true;
                    else{
                        bool mismatch = true;
                        float wDiff = 0;
                        
                        for(int index=hyppox::Config::CLUSTER; index<(hyppox::Config::CLUSTER+hyppox::Config::FILTER); index++){
                            
                            // Check delta threshold
                            if(this->getSignatureAtIndex(hyppox::Config::CLUSTER+hyppox::Config::FILTER-index-1) != e->getSignatureAtIndex(hyppox::Config::CLUSTER+hyppox::Config::FILTER-index-1)){
                                
                                if(hyppox::Config::REFERENCE_PERFORMANCE){
                                    //wDiff = ABS(getNormalizedWeight(index+1)-e->getNormalizedWeight(index+1))*100;
                                    wDiff = (ABS(this->getWeight(index)-e->getWeight(index))*100)/((this->getWeight(index)-e->getWeight(index))>0?this->getWeight(index+1):e->getWeight(index+1));
                                    
                                    mismatch = mismatch && (wDiff<=hyppox::Config::DELTA_CHANGE);
                                }else{
                                    // For performance
                                    if(index==hyppox::Config::CLUSTER){
                                        //wDiff = ABS(getNormalizedWeight(index)-e->getNormalizedWeight(index))*100;
                                        wDiff = (ABS(this->getWeight(index-1)-e->getWeight(index-1))*100)/((this->getWeight(index-1)-e->getWeight(index-1))>0?this->getWeight(index-1):e->getWeight(index-1));
                                        
                                        mismatch = mismatch && (wDiff<=hyppox::Config::DELTA_CHANGE);
                                    }
                                    
                                    // For environmental attributes
                                    if(index != hyppox::Config::REFERENCE_ENV_INDEX){
                                        
                                        //wDiff = ABS(getNormalizedWeight(index+1)-e->getNormalizedWeight(index+1))*100;
                                        wDiff = (ABS(this->getWeight(index)-e->getWeight(index))*100)/((this->getWeight(index)-e->getWeight(index))>0?this->getWeight(index):e->getWeight(index));
                                        
                                        mismatch = mismatch && (wDiff<=hyppox::Config::DELTA_CHANGE);
                                    }
                                }
                            }
                        }
                        
                        return mismatch;
                    }
                }
                
                return false;
            }
            
            template<typename RNType>
            bool rEdge<RNType>::strictMatchSignature(rEdge<RNType> *e){
                
                size_t s, sig;
                getSignature(s);
                e->getSignature(sig);
                
                return (s==sig);
            }
            
            template<typename RNType>
            bool rEdge<RNType>::getSignatureAtIndex(short index){
                if(index>0 && index<=hyppox::Config::FILTER){
                    return this->signature[hyppox::Config::FILTER-index];
                }
                
                return false;
            }
            
            template<typename RNType>
            void rEdge<RNType>::assignScore(rEdge<RNType> *eMax){
                
                if(eMax!=nullptr){
                    this->setDepth(eMax->getDepth()+1);
                }
            }
            
            template<typename RNType>
            void rEdge<RNType>::createCells(){
                size_t depth = this->getDepth();
                
                this->cells = new cell::CostTableCell[depth];
                for(size_t i=0;i<depth;i++){
                    this->cells[i].setEdgeID(this->_id);
                }
            }
            
            template<typename RNType>
            void rEdge<RNType>::destroyCells(){
                if(this->cells!=nullptr)delete [] this->cells;
                this->cells = nullptr;
            }
            
            template<typename RNType>
            float rEdge<RNType>::getScoreOfCell(size_t index){
                if(index<this->getDepth()){
                    return this->cells[index].getScore();
                }
                
                return -1.0;
            }
            
            template<typename RNType>
            void rEdge<RNType>::setScoreOfCell(float score, size_t index){
                if(index<this->getDepth()){
                    this->cells[index].setScore(score);
                }
            }
            
            template<typename RNType>
            void rEdge<RNType>::setCostTableCellOfIndex(cell::CostTableCell *cell, size_t index){
                if(index<this->getDepth()){
                    this->cells[index].setCell(cell);
                }
            }
            
            template<typename RNType>
            cell::CostTableCell* rEdge<RNType>::getCostTableCellOfIndex(size_t index){
                if(index<this->getDepth()){
                    return &this->cells[index];
                }
                
                return nullptr;
            }
            
        }
        
        //////// Subgraph methods ///////////////
        template<typename PerfType, typename RNType, typename REType>
        class IsolatedSubGraph{
            public:
            //using IGType = IsolatedSubGraph<PerfType,RNType,REType>;
            IsolatedSubGraph(std::unordered_map<std::string, size_t>* _individualIndexMap);
            ~IsolatedSubGraph()=default;
            
            bool hasNode(size_t id);
            RNType* getNodeById(size_t id);
            void addPointToNode(size_t id, PerfType* ph);
            void addEdge(size_t id1, size_t id2);
            void setupLinks();
            std::string getGMLNodes();
            std::string getGMLEdges(CustomList<path::InterestingPath>* IPs);
            void getNodeSizeAndWeightRange(std::vector<float>& size, std::vector<std::vector<float> >& weight);
            void setGlobalSizeRange(float size, short index);
            void setGlobalWeightRange(std::vector<float> size, short index);
            
            void resetLinks(bool cov);
            
            void computeInterestingPaths(CustomList<path::InterestingPath>* IPs);
            void recoverNodeLinks();
            bool addRankColorToEdge(std::string edgeID, std::string color, size_t rank, short index);
            
            void computeFlares(std::vector<flare::Coverage>* _cov);
            void computeFlareScore(std::vector<flare::Coverage>* _cov);
            
            void getD3Nodes(CustomList<std::string>* nodeListJS, short cci);
            void getD3Edges(CustomList<std::string>* edgeListJS, CustomList<path::InterestingPath>* IPs, short cci);
            
            private:
            CustomHashMap<size_t, RNType*> nodes;
            CustomHashMap<std::string, REType*> edges;
            CustomHashSet<size_t> sourceNodeIdSet;
            CustomHashSet<size_t> covSourceNodeIdSet;
            CustomHashSet<size_t> covIntSourceNodeIdSet;
            CustomHashSet<size_t> exploreSet;
            CustomHashSet<std::string> terminalEdgeSet;
            
            unsigned int ccNumber;
            std::vector<float> nodeSizeRange;
            std::vector<std::vector<float> > nodeWeightRange;
            std::vector<std::string> envName;
            size_t maxDepth;
            cell::CostTableCell* maxScoredCell;
            
            std::vector<float> globalNodeSizeRange;
            std::vector<std::vector<float> > globalNodeWeightRange;
            
            std::vector<flare::Coverage> coverageList;
            
            std::unordered_map<std::string, size_t>* individualIndexMap;
            
            void addNode(size_t _id);
            void setRange();
            REType* getMaxPredecessor(REType* e);
            bool needMoreExplore(REType* e);
            bool checkTerminalEdge(REType* e);
            void exploreNode();
            void resetDepth();
            void identifySourceNodes(bool _internal);
            void runBFS();
            size_t computeMaxLengthPath();
            void computeCostTableForColumn(size_t index);
            void computeCostTable();
            void createEdgeCostList();
            void printTable();
            void getMaximumInterestingPath(CustomList<path::InterestingPath>* IPs);
            bool EdgeBelongsInPathList(std::string eID, CustomList<path::InterestingPath>* IPs);
            std::string getEdgeColor(std::string eID, CustomList<path::InterestingPath>* IPs);
            float adjustSize(float size);
            int exploreStem(size_t sNode, size_t &brNode, std::set<size_t>* _indv_cov, flare::Coverage* _coverage);
            int exploreBranch(size_t sNode, size_t brNode, std::set<size_t>* _br_cov,  std::set<size_t>* _stem_cov, flare::Coverage* _coverage);
            void individualCoverage(std::set<size_t>* s1, std::set<size_t>* s2);
            bool checkIndividualCoverage(std::set<size_t>* s1, std::set<size_t>* s2);
            void getCoverageInOutDegree(size_t aNode, flare::Coverage*& _cov, CustomHashMap<size_t, flare::CoverageBranch>*& _brNodeCov, short& inD, short& outD);
            void computeScoreForAFlare(flare::Coverage* _cov);
            void computeBranchScore(flare::Coverage*& _cov, float& score, size_t tNode);
            void computeBranchScore(flare::Coverage*& _cov, float& score, float brScore, size_t tNode);
            void setFlareEdgeColor(std::vector<flare::Coverage>* _cov);
        };
        
        template<typename PerfType, typename RNType, typename REType>
        IsolatedSubGraph<PerfType,RNType,REType>::IsolatedSubGraph(std::unordered_map<std::string, size_t>* _individualIndexMap){
            this->nodeSizeRange.assign(2,0.0);
            this->globalNodeSizeRange.assign(2,0.0);
            std::vector<float> _v(2, 0.0);
            this->nodeWeightRange.assign(hyppox::Config::FILTER+hyppox::Config::CLUSTER, _v);
            this->globalNodeWeightRange.assign(hyppox::Config::FILTER+hyppox::Config::CLUSTER, _v);
            this->envName.assign(hyppox::Config::FILTER, "");
            this->maxDepth = 0;
            this->individualIndexMap = _individualIndexMap;
        }
        
        template<typename PerfType, typename RNType, typename REType>
        bool IsolatedSubGraph<PerfType,RNType,REType>::hasNode(size_t _id){
            return nodes.hasItem(_id);
        }
        
        template<typename PerfType, typename RNType, typename REType>
        RNType* IsolatedSubGraph<PerfType,RNType,REType>::getNodeById(size_t _id){
            if(nodes.hasItem(_id)) return nodes.getValue(_id);
            return nullptr;
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::addNode(size_t _id){
            if(!nodes.hasItem(_id)){
                RNType* aNode = new RNType(_id);
                nodes.addToMap(_id, aNode);
            }
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::setRange(){
            auto nodeMap = this->nodes.getMap();
            bool first = true;
            
            for(auto itr=nodeMap.begin(); itr!=nodeMap.end(); itr++){
                RNType* n = itr->second;
                
                if(first){
                    this->nodeSizeRange[0] = this->nodeSizeRange[1] = n->getSize();
                    
                    for(short i=0; i<hyppox::Config::FILTER+hyppox::Config::CLUSTER; i++){
                        this->nodeWeightRange[i][0] = this->nodeWeightRange[i][1] = n->getWeight(i);
                    }
                    
                    first = false;
                }else{
                    float w = n->getSize();
                    
                    if(w<this->nodeSizeRange[0]) this->nodeSizeRange[0] = w;
                    if(w>this->nodeSizeRange[1]) this->nodeSizeRange[1] = w;
                    
                    for(short i=0; i<hyppox::Config::FILTER+hyppox::Config::CLUSTER; i++){
                        w = n->getWeight(i);
                        if(w<this->nodeWeightRange[i][0]) this->nodeWeightRange[i][0] = w;
                        if(w>this->nodeWeightRange[i][1]) this->nodeWeightRange[i][1] = w;
                    }
                }
            }
        }
        
        template<typename PerfType, typename RNType, typename REType>
        REType* IsolatedSubGraph<PerfType,RNType,REType>::getMaxPredecessor(REType* e){
            RNType* toNode = e->getFromNode();
            REType* eMax = nullptr;
            
            // Predicessor edges
            for(int i=0; i<toNode->getTotalInDegree(); i++){
                size_t maxDepth = 0;
                RNType* fNode = toNode->getPrevNode(i);
                
                std::string key = std::to_string(fNode->getID())+"#"+std::to_string(toNode->getID());
                REType* e1 = this->edges.getValue(key);
                
                if(e1->getDepth()>0){
                    
                    REType* refEdge = e1->getRefEdge();
                    if(refEdge == nullptr) refEdge=e1;
                    
                    if(refEdge->matchSignature(e)){
                        // Changes is here
                        // Compute which predecessor maximizes my score
                        if(e->getDepth() < e1->getDepth()+1){
                            e->setDepth(e1->getDepth()+1);
                        }
                        
                        if(maxDepth<e1->getDepth()+1){
                            maxDepth = e1->getDepth();
                            eMax = e1;
                        }
                    }
                }
            }
            
            return eMax;
        }
        
        template<typename PerfType, typename RNType, typename REType>
        bool IsolatedSubGraph<PerfType,RNType,REType>::checkTerminalEdge(REType *e){
            RNType* fromNode = e->getToNode();
            
            for(int i=0; i<fromNode->getTotalOutDegree(); i++){
                RNType* toNode = fromNode->getNextNode(i);
                REType* edge = this->edges.getValue(std::to_string(fromNode->getID())+"#"+std::to_string(toNode->getID()));
                
                REType* refEdge = e->getRefEdge();
                if(refEdge==nullptr) refEdge=e;
                
                if(refEdge->matchSignature(edge)){
                    return false;
                }
            }
            
            return true;
        }
        
        template<typename PerfType, typename RNType, typename REType>
        bool IsolatedSubGraph<PerfType,RNType,REType>::needMoreExplore(REType* e){
            
            auto toNode = e->getFromNode();
            
            // Predicessor edges
            for(int i=0; i<toNode->getTotalInDegree(); i++){
                auto fNode = toNode->getPrevNode(i);
                
                std::string key = std::to_string(fNode->getID())+"#"+std::to_string(toNode->getID());
                auto edge = this->edges.getValue(key);
                
                if(edge->getDepth()==0){
                    return true;
                }
            }
            
            return false;
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::exploreNode(){
            
            auto _id = this->exploreSet.getElement();
            this->exploreSet.removeItem(_id);
            
            auto fromNode = this->nodes.getValue(_id);
            
            for(int i=0; i<fromNode->getTotalOutDegree(); i++){
                auto toNode = fromNode->getNextNode(i);
                auto key = std::to_string(fromNode->getID())+"#"+std::to_string(toNode->getID());
                auto e = this->edges.getValue(key);
                
                auto eMax = this->getMaxPredecessor(e);
                
                if(eMax == nullptr){
                    if(!fromNode->IsThisNodeASource() && !this->needMoreExplore(e)){
                        fromNode->setSource(true);
                        e->setDepth(1);
                    }
                }else if(hyppox::Config::DELTA_CHANGE>0){
                    
                    // First signature mismatch
                    if(eMax->getRefEdge()==nullptr){
                        if(!eMax->strictMatchSignature(e))
                        e->setRefEdge(eMax);
                        else{
                            e->setRefEdge(nullptr);
                        }
                    }
                    // Next edges after first mismatch
                    else {
                        if(!eMax->strictMatchSignature(e)){
                            e->setRefEdge(eMax->getRefEdge());
                        }else{
                            e->setRefEdge(nullptr);
                        }
                    }
                }
                
                if(!this->terminalEdgeSet.hasItem(e->getID()) && this->checkTerminalEdge(e)){
                    this->terminalEdgeSet.addToSet(e->getID());
                }
            }
            
            // Add to explore list
            for(int i=0; i<fromNode->getTotalOutDegree(); i++){
                auto toNode = fromNode->getNextNode(i);
                
                this->exploreSet.addToSet(toNode->getID());
            }
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::identifySourceNodes(bool _internal){
            this->terminalEdgeSet.clearHashSet();
            this->exploreSet.clearHashSet();
            this->sourceNodeIdSet.clearHashSet();
            this->covSourceNodeIdSet.clearHashSet();
            
            auto nList = this->nodes.getMap();
            auto eList = this->edges.getMap();
            
            for(auto nitr=nList.begin(); nitr!=nList.end(); nitr++){
                auto n = nitr->second;
                
                if(n->getTotalInDegree() == 0){
                    n->setSource(true);
                    this->sourceNodeIdSet.addToSet(n->getID());
                    this->covSourceNodeIdSet.addToSet(n->getID());
                }else{
                    n->setSource(false);
                }
                
                // For flare case: If a node coverage is not a subset of any of its predecessor nodes
                // then this node is a source node
                if(!_internal){
                    short inD = 0;
                    auto n_cov = n->getIndvCoverage();
                    for(short i=0; i<n->getTotalInDegree(); i++){
                        auto fNode = n->getPrevNode(i);
                        auto fn_cov = fNode->getIndvCoverage();
                        
                        if(this->checkIndividualCoverage(&n_cov, &fn_cov)){
                            inD++;
                        }
                    }
                    
                    if(inD==0) this->covSourceNodeIdSet.addToSet(n->getID());
                }
            }
            
            // Internal sources
            if(_internal){
                for(auto eitr=eList.begin(); eitr!=eList.end(); eitr++){
                    auto e = eitr->second;
                    
                    if(!e->getMark()){
                        auto toNode = e->getFromNode();
        
                        
                        if(!toNode->IsThisNodeASource()){
                            bool match = false;
                            // Predicessor edges
                            for(int i=0; i<toNode->getTotalInDegree(); i++){
                                auto fNode = toNode->getPrevNode(i);
                                auto key = std::to_string(fNode->getID())+"#"+std::to_string(toNode->getID());
                                auto e1 = this->edges.getValue(key);
                                
                                auto refEdge = e1->getRefEdge();
                                if(refEdge == nullptr) refEdge=e1;
                                
                                match |= refEdge->matchSignature(e);
                            }
                            
                            if(!match) this->sourceNodeIdSet.addToSet(toNode->getID());
                        }
                    }
                }
            }
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::resetDepth(){
            auto edgeMap = this->edges.getMap();
            
            for(auto itr=edgeMap.begin(); itr!=edgeMap.end(); itr++){
                auto e = itr->second;
                if(!e->getMark()){
                    e->setDepth(0);
                }
            }
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::runBFS(){
            this->resetDepth();
            this->identifySourceNodes(true);
            
            auto sourceIds = this->sourceNodeIdSet.getSet();
            
            for(auto itr=sourceIds.begin(); itr!=sourceIds.end(); itr++){
                auto fromId = *itr;
                auto fromNode = this->nodes.getValue(fromId);
                
                for(int i=0; i<fromNode->getTotalOutDegree(); i++){
                    auto toNode = fromNode->getNextNode(i);
                    auto e = this->edges.getValue(std::to_string(fromNode->getID())+"#"+std::to_string(toNode->getID()));
                    
                    e->setDepth(1);
                    
                    this->exploreSet.removeItem(e->getToNode()->getID());
                    this->exploreSet.addToSet(e->getToNode()->getID());
                }
            }
            
            while(this->exploreSet.getSize()>0){
                this->exploreNode();
            }
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::computeCostTableForColumn(size_t index){
            auto eList = this->edges.getMap();
            
            if(index<=0){
                for(auto itr=eList.begin(); itr!=eList.end(); itr++){
                    auto e = itr->second;
                    
                    if(!e->getMark())
                    e->setScoreOfCell(e->getWeight(hyppox::Config::REFERENCE_PH_INDEX), index);
                }
            }else{
                for(auto itr=eList.begin(); itr!=eList.end(); itr++){
                    auto e = itr->second;
                    
                    if(!e->getMark() && index<e->getDepth()){
                        auto score = e->getWeight(hyppox::Config::REFERENCE_PH_INDEX)*log(index+2);
                        auto toNode = e->getFromNode();
                        float maxScore = 0.0;
                        bool first = true;
                        REType* eMax = nullptr;
                        
                        // Compute max predecessor
                        for(size_t i=0; i<toNode->getTotalInDegree(); i++){
                            auto fNode = toNode->getPrevNode(i);
                            
                            auto key = std::to_string(fNode->getID())+"#"+std::to_string(toNode->getID());
                            auto predEdge = this->edges.getValue(key);
                            
                            auto refEdge = predEdge->getRefEdge();
                            if(refEdge == nullptr) refEdge=predEdge;
                            
                            if(refEdge->matchSignature(e)){
                                auto tmpScore = predEdge->getScoreOfCell(index-1) + score;
                                
                                if(first){ maxScore = tmpScore; first=false; eMax = predEdge;}
                                else if(maxScore<tmpScore){ maxScore = tmpScore; eMax = predEdge;}
                            }
                        }
                        
                        e->setScoreOfCell(maxScore, index);
                        e->setCostTableCellOfIndex(eMax->getCostTableCellOfIndex(index-1), index);
                        
                        if(index+1>=hyppox::Config::MINIMUM_PATH_LENGTH){
                            if(this->maxScoredCell == nullptr)this->maxScoredCell = e->getCostTableCellOfIndex(index);
                            else if(this->maxScoredCell->getScore()<maxScore) this->maxScoredCell = e->getCostTableCellOfIndex(index);
                        }
                    }
                }
            }
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::computeCostTable(){
            this->maxScoredCell = nullptr;
            
            for(size_t i=0;i<this->maxDepth; i++){
                this->computeCostTableForColumn(i);
            }
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::createEdgeCostList(){
            auto eList = this->edges.getMap();
            
            for(auto eitr=eList.begin(); eitr!=eList.end(); eitr++){
                auto e = eitr->second;
                
                e->destroyCells();
                
                if(!e->getMark()){
                    e->createCells();
                }
            }
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::printTable(){
            auto eList = this->edges.getMap();
            
            std::cout<<std::endl<<"Cost table:\n";
            for(auto eitr=eList.begin(); eitr!=eList.end(); eitr++){
                auto e = eitr->second;
                
                if(!e->getMark()){
                    std::cout<<e->getID()<<": ";
                    for(size_t i=0; i<e->getDepth(); i++){
                        if(i>0)std::cout<<", ";
                        std::cout<<e->getCostTableCellOfIndex(i)->getScore();
                    }
                    std::cout<<std::endl;
                }
            }
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::getMaximumInterestingPath(CustomList<path::InterestingPath>* IPs){
            //printTable();
            
            path::InterestingPath path;
            auto cell = this->maxScoredCell;
            size_t pathLength = 0;
            
            // Assign max score of this interesting path
            if(cell->getCell()!=nullptr)path.setScore(cell->getScore());
            
            // Recover the interesting path
            while(cell->getCell()!=nullptr){
                path.addToPath(cell->getEdgeID());
                auto e = this->edges.getValue(cell->getEdgeID());
                e->setMark(true);
                
                cell = cell->getCell();
                pathLength++;
            }
            
            path.addToPath(cell->getEdgeID());
            auto e = this->edges.getValue(cell->getEdgeID());
            e->setMark(true);
            pathLength++;
            
            if(pathLength>=hyppox::Config::MINIMUM_PATH_LENGTH) IPs->addToList(path);
            
        }
        
        template<typename PerfType, typename RNType, typename REType>
        bool IsolatedSubGraph<PerfType,RNType,REType>::EdgeBelongsInPathList(std::string eID, CustomList<path::InterestingPath>* IPs){
            auto IPList = &IPs->getList();
            
            for(auto itr=IPList->begin(); itr!=IPList->end(); itr++){
                auto p = &(*itr);
                if(p->PathContainsEdge(eID)/*&&(p->getRank()==1||p->getRank()==2)*/) return true;
            }
            
            return false;
        }
        
        template<typename PerfType, typename RNType, typename REType>
        std::string IsolatedSubGraph<PerfType,RNType,REType>::getEdgeColor(std::string eID, CustomList<path::InterestingPath>* IPs){
            auto IPList = &IPs->getList();
            
            for(auto itr=IPList->begin(); itr!=IPList->end(); itr++){
                auto p = &(*itr);
                if(p->PathContainsEdge(eID) /*&& (p->getRank()==1||p->getRank()==2)*/)return p->getColor();
            }
            
            return "";
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::addPointToNode(size_t _id, PerfType *ph){
            this->addNode(_id);
            auto aNode = this->getNodeById(_id);
            aNode->addPoint(ph, this->individualIndexMap);
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::addEdge(size_t id1, size_t id2){
            if(this->nodes.hasItem(id1) && this->nodes.hasItem(id2)){
                auto e = new REType(this->getNodeById(id1), this->getNodeById(id2));
                auto n1 = std::to_string(e->getFromNode()->getID());
                auto n2 = std::to_string(e->getToNode()->getID());
                auto key = n1+"#"+n2;
                
                this->edges.addToMap(key, e);
            }
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::setupLinks(){
            auto nList = this->nodes.getMap();
            auto eList = this->edges.getMap();
            
            for(auto nitr=nList.begin(); nitr!=nList.end(); nitr++){
                auto n = nitr->second;
                n->resetLinkNodes();
            }
            
            for(auto eitr=eList.begin(); eitr!=eList.end(); eitr++){
                auto e = eitr->second;
                
                e->getFromNode()->addOutDegree();
                e->getToNode()->addInDegree();
            }
            
            for(auto nitr=nList.begin(); nitr!=nList.end(); nitr++){
                auto n = nitr->second;
                
                n->createLinkNodes();
            }
            
            for(auto eitr=eList.begin(); eitr!=eList.end(); eitr++){
                auto e = eitr->second;
                
                e->getFromNode()->addNextNode(e->getToNode());
                e->getToNode()->addPrevNode(e->getFromNode());
            }
            
            // Adjust size and weight range
            this->setRange();
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::resetLinks(bool cov){
            auto eList = this->edges.getMap();
            std::unordered_set<std::string> swapSet;
            
            for(auto eitr=eList.begin(); eitr!=eList.end(); eitr++){
                auto e = eitr->second;
                
                float fw, tw;
                
                if(cov){
                    std::set<size_t> fIndv = e->getFromNode()->getIndvCoverage();
                    std::set<size_t> tIndv = e->getToNode()->getIndvCoverage();
                    
                    this->individualCoverage(&fIndv, &tIndv);
                    
                    fw = e->getFromNode()->getWeightOfIndividuals(tIndv, this->individualIndexMap);
                    tw = e->getToNode()->getWeightOfIndividuals(tIndv, this->individualIndexMap);
                    
                }else{
                    fw = e->getFromNode()->getWeight(hyppox::Config::REFERENCE_PH_INDEX);
                    tw = e->getToNode()->getWeight(hyppox::Config::REFERENCE_PH_INDEX);
                    
                }
                
                if(tw<fw){
                    swapSet.insert(e->getID());
                    
                    // For coverage, store the direction
                    if(cov) e->setDirection(false);
                    else e->setDirection(true);
                }
            }
            
            for(std::string s:swapSet){
                auto e = this->edges.getValue(s);
                e->swapNodes();
                this->edges.removeFromMap(s);
                this->edges.addToMap(e->getID(), e);
            }
        }
        
        template<typename PerfType, typename RNType, typename REType>
        size_t IsolatedSubGraph<PerfType,RNType,REType>::computeMaxLengthPath(){
            this->runBFS();
            
            size_t length = 0;
            auto tSet = this->terminalEdgeSet.getSet();
            
            for(auto itr=tSet.begin(); itr!=tSet.end(); itr++){
                std::string eID = *itr;
                auto e = this->edges.getValue(eID);
                
                if(length==0 || length<e->getDepth()) length = e->getDepth();
            }
            
            this->maxDepth = length;
            return length;
        }
        
        template<typename PerfType, typename RNType, typename REType>
        std::string IsolatedSubGraph<PerfType,RNType,REType>::getGMLNodes(){
            std::string nodes = "";
            auto nodeMap = this->nodes.getMap();
            
            RGBColor rgbColor;
            rgbColor.SetGamma(0.1);
            rgbColor.SetRange(this->globalNodeWeightRange[0][0], this->globalNodeWeightRange[0][1]);
            
            for(auto itr=nodeMap.begin(); itr!=nodeMap.end(); itr++){
                auto n = itr->second;
                
                std::string color = rgbColor.GetColor(n->getWeight(hyppox::Config::REFERENCE_PH_INDEX));
                std::string label = fixPrecision(n->getWeight(hyppox::Config::REFERENCE_PH_INDEX), 2);
                
                nodes += "node\n[\n id " + to_string(n->getID()) + "\n label \"" + to_string(n->getID()) + "\" \n size " + std::to_string(20) + "\n color \"" + "#ffffff" + "\" \n shape \"" + "circle" + "\" \n]\n";
            }
            
            return nodes;
        }
        
        template<typename PerfType, typename RNType, typename REType>
        std::string IsolatedSubGraph<PerfType,RNType,REType>::getGMLEdges(CustomList<path::InterestingPath>* IPs){
            std::string edges = "";
            auto edgeMap = this->edges.getMap();
            
            for(auto itr=edgeMap.begin(); itr!=edgeMap.end(); itr++){
                auto e = itr->second;
                std::string sig;
                e->getSignature(sig);
                std::string label = sig;
                
                if(hyppox::Config::FILTER==1){
                    edges += "edge\n[\n source " + to_string(e->getFromNode()->getID()) + "\n target " + to_string(e->getToNode()->getID()) + "\n width " + std::to_string(2) + "\n label \"" + "" + "\" \n color \"" + hyppox::Config::EDGE_COLOR + "\"\n]\n";
                }else{
                    std::string color = this->getEdgeColor(e->getID(), IPs);
                    bool edgeInAPath = this->EdgeBelongsInPathList(e->getID(), IPs);
                    
                    if(edgeInAPath){
                        edges += "edge\n[\n source " + to_string(e->getFromNode()->getID()) + "\n target " + to_string(e->getToNode()->getID()) + "\n width " + std::to_string(hyppox::Config::EDGE_WIDTH) + "\n label \"" + label + "\" \n color \"" + color + "\"\n]\n";
                    }else{
                        edges += "edge\n[\n source " + to_string(e->getFromNode()->getID()) + "\n target " + to_string(e->getToNode()->getID()) + "\n width " + std::to_string(2) + "\n label \"" + label + "\" \n color \"" + hyppox::Config::EDGE_COLOR + "\"\n]\n";
                    }
                }
            }
            
            return edges;
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::getNodeSizeAndWeightRange(std::vector<float>& size, std::vector<std::vector<float> >& weight){
            size[0]=this->nodeSizeRange[0];
            size[1]=this->nodeSizeRange[1];
            
            for(int i=0;i<hyppox::Config::FILTER+hyppox::Config::CLUSTER; i++){
                weight[i][0] = this->nodeWeightRange[i][0];
                weight[i][1] = this->nodeWeightRange[i][1];
            }
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::setGlobalSizeRange(float size, short index){
            this->globalNodeSizeRange[index] = size;
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::setGlobalWeightRange(std::vector<float> size, short index){
            this->globalNodeWeightRange[index][0] = size[0];
            this->globalNodeWeightRange[index][1] = size[1];
        }
        
        /****** Cost table related methods *******/
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::computeInterestingPaths(CustomList<path::InterestingPath>* IPs){
            
            auto eList = this->edges.getMap();
            
            while(true){
                for(auto eitr=eList.begin(); eitr!=eList.end(); eitr++){
                    auto e = eitr->second;
                    
                    if(e->getMark()){
                        auto n1 = e->getFromNode();
                        auto n2 = e->getToNode();
                        
                        n1->removeNextNode(n2);
                        n2->removePrevNode(n1);
                    }
                }
                
                // Compute the max length path
                size_t pathLength = this->computeMaxLengthPath();
                
                if(pathLength<hyppox::Config::MINIMUM_PATH_LENGTH) break;
                
                // Create cost table
                this->createEdgeCostList();
                
                // Compute score in cost table
                this->computeCostTable();
                
                // Generate max-IP
                this->getMaximumInterestingPath(IPs);
            }
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::recoverNodeLinks(){
            auto eList = this->edges.getMap();
            
            for(auto itr=eList.begin(); itr!=eList.end(); itr++){
                auto e = itr->second;
                
                if(e->getMark()){
                    auto fn = e->getFromNode();
                    auto tn = e->getToNode();
                    
                    fn->addOutDegree();
                    fn->addNextNode(tn);
                    
                    tn->addInDegree();
                    tn->addPrevNode(fn);
                }
            }
        }
        
        template<typename PerfType, typename RNType, typename REType>
        bool IsolatedSubGraph<PerfType,RNType,REType>::addRankColorToEdge(std::string edgeID, std::string color, size_t rank, short index){
            if(this->edges.hasItem(edgeID)){
                auto e = this->edges.getValue(edgeID);
                
                e->setColor(index, color);
                e->setRank(index, rank);
                
                return true;
            }
            
            return false;
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::computeFlares(std::vector<flare::Coverage> *_allCov){
            this->resetDepth();
            this->identifySourceNodes(false);
            
            CustomHashSet<size_t> tmp_covSourceNodeIdSet;
            std::vector<size_t> cs;
            
            tmp_covSourceNodeIdSet.copyHashSet(this->covSourceNodeIdSet);
            
            while(tmp_covSourceNodeIdSet.getSize()>0){
                if(cs.size()>0) cs.clear();
                cs.insert(cs.begin(),tmp_covSourceNodeIdSet.getSet().begin(), tmp_covSourceNodeIdSet.getSet().end());
                
                for(size_t i=0; i<cs.size(); i++){
                    flare::Coverage _cov(cs[i]);
                    
                    size_t brNode;
                    std::set<size_t> _indv_cov;
                    if(exploreStem(cs[i], brNode, &_indv_cov, &_cov)==2){
                        std::set<size_t> br_cov;
                        exploreBranch(brNode, 0, &br_cov, &_indv_cov, &_cov);
                        
                        _allCov->push_back(_cov);
                    }
                }
                
                tmp_covSourceNodeIdSet.clearHashSet();
                if(this->covIntSourceNodeIdSet.getSize()>0){
                    tmp_covSourceNodeIdSet.copyHashSet(this->covIntSourceNodeIdSet);
                    this->covIntSourceNodeIdSet.clearHashSet();
                }
                
                auto ts = tmp_covSourceNodeIdSet.getSet();
                for(size_t _d:ts){
                    this->covSourceNodeIdSet.addToSet(_d);
                }
            }
        }
        
        template<typename PerfType, typename RNType, typename REType>
        int IsolatedSubGraph<PerfType,RNType,REType>::exploreBranch(size_t sNode, size_t brNode, std::set<size_t>* _br_cov,  std::set<size_t>* _stem_cov, flare::Coverage *_coverage){
            auto sourceNode = this->nodes.getValue(sNode);
            if(sourceNode->getTotalOutDegree()==0){
                _coverage->addTerminalNode(sourceNode->getID());
            }
            // For single linked branch
            else if(sourceNode->getTotalOutDegree()==1&&sourceNode->getTotalInDegree()==1){
                std::set<size_t> ic = sourceNode->getIndvCoverage();
                _br_cov->clear();
                for(size_t s:ic){
                    _br_cov->insert(s);
                }
                
                auto tNode = sourceNode->getNextNode(0);
                
                while(tNode->getTotalOutDegree()<=1&&tNode->getTotalInDegree()>=1){
                    
                    std::set<size_t> nCov = tNode->getIndvCoverage();
                    individualCoverage(&nCov, _br_cov);
                    
                    // No common individuals between a node and its adjacent node along a branch
                    if(_br_cov->size()==0){
                        _coverage->addTerminalNode(sourceNode->getID());
                        if(tNode->getTotalOutDegree()>0)this->covIntSourceNodeIdSet.addToSet(tNode->getID());
                        return 1;
                    }
                    
                    // Branch node contains individuals of stem individual as well as branch individual
                    if(checkIndividualCoverage(&nCov, _stem_cov)&&checkIndividualCoverage(&nCov, _br_cov)){
                        _coverage->addCoverageEdge(std::to_string(sourceNode->getID())+"#"+std::to_string(tNode->getID()));
                        sourceNode = tNode;
                        if(sourceNode->getTotalOutDegree()==0){
                            _coverage->addTerminalNode(sourceNode->getID());
                            return 0;
                        }
                        tNode = sourceNode->getNextNode(0);
                    }else{
                        _coverage->addTerminalNode(sourceNode->getID());
                        if(tNode->getTotalOutDegree()>0)this->covIntSourceNodeIdSet.addToSet(tNode->getID());
                        return 1;
                    }
                }
                
                // Check whether this node of stem is the terminal node of the coverage or not
                // Then branching node will be the internediate source node
                if(tNode->getTotalOutDegree()>1){
                    
                    std::set<size_t> nCov = tNode->getIndvCoverage();
                    individualCoverage(&nCov, _br_cov);
                    
                    // No common individuals between a node and its adjacent node along a branch
                    if(_br_cov->size()==0){
                        _coverage->addTerminalNode(sourceNode->getID());
                        this->covIntSourceNodeIdSet.addToSet(tNode->getID());
                        return 1;
                    }
                    
                    // Branch node contains individuals of stem individual as well as branch individual
                    if(!checkIndividualCoverage(&nCov, _stem_cov) || !checkIndividualCoverage(&nCov, _br_cov)){
                        _coverage->addTerminalNode(sourceNode->getID());
                        this->covIntSourceNodeIdSet.addToSet(tNode->getID());
                        return 1;
                    }
                    
                    bool allCh = false;
                    std::set<size_t> sc = sourceNode->getIndvCoverage();
                    
                    for(int i=0; i<tNode->getTotalOutDegree(); i++){
                        auto cNode = tNode->getNextNode(i);
                        std::set<size_t> chC = cNode->getIndvCoverage();
                        
                        if(checkIndividualCoverage(&chC, &sc)) allCh = true;
                    }
                    
                    if(!allCh){
                        _coverage->addTerminalNode(sourceNode->getID());
                        if(tNode->getTotalOutDegree()>0)this->covIntSourceNodeIdSet.addToSet(tNode->getID());
                        return 1;
                    }
                    
                    if(checkIndividualCoverage(&nCov, _stem_cov)&&checkIndividualCoverage(&nCov, _br_cov)){
                        _coverage->addCoverageEdge(std::to_string(sourceNode->getID())+"#"+std::to_string(tNode->getID()));
                    }
                }
                
                exploreBranch(tNode->getID(), 0, _br_cov, _stem_cov, _coverage);
                
            }else if(sourceNode->getTotalOutDegree()>1&&sourceNode->getTotalInDegree()<=1){
                if(_br_cov->size()>0){
                    _stem_cov->clear();
                    for(auto itr=_br_cov->begin(); itr!=_br_cov->end(); itr++){
                        _stem_cov->insert(*itr);
                    }
                    _br_cov->clear();
                }
                
                std::set<size_t> br_stem_cov;
                for(auto itr=_stem_cov->begin(); itr!=_stem_cov->end();itr++){
                    br_stem_cov.insert(*itr);
                }
                
                for(int i=0;i<sourceNode->getTotalOutDegree();i++){
                    auto tNode = sourceNode->getNextNode(i);
                    auto node_cov = tNode->getIndvCoverage();
                    
                    if(checkIndividualCoverage(&node_cov, _stem_cov)){
                        _coverage->addCoverageEdge(std::to_string(sourceNode->getID())+"#"+std::to_string(tNode->getID()));
                        exploreBranch(tNode->getID(), sourceNode->getID(), &node_cov, _stem_cov, _coverage);
                    }
                    
                    _stem_cov->clear();
                    for(size_t __sc:br_stem_cov){
                        _stem_cov->insert(__sc);
                    }
                }
            }else if(sourceNode->getTotalOutDegree()>=1&&sourceNode->getTotalInDegree()>1){
                if(_br_cov->size()>0){
                    _stem_cov->clear();
                    for(auto itr=_br_cov->begin(); itr!=_br_cov->end(); itr++){
                        _stem_cov->insert(*itr);
                    }
                    _br_cov->clear();
                }
                
                std::set<size_t> br_stem_cov;
                for(auto itr=_stem_cov->begin(); itr!=_stem_cov->end();itr++){
                    br_stem_cov.insert(*itr);
                }
                
                for(int i=0;i<sourceNode->getTotalOutDegree();i++){
                    auto tNode = sourceNode->getNextNode(i);
                    auto node_cov = tNode->getIndvCoverage();
                    
                    if(checkIndividualCoverage(&node_cov, _stem_cov)){
                        _coverage->addCoverageEdge(std::to_string(sourceNode->getID())+"#"+std::to_string(tNode->getID()));
                        exploreBranch(tNode->getID(), sourceNode->getID(), &node_cov, _stem_cov, _coverage);
                    }
                    
                    _stem_cov->clear();
                    for(size_t __sc:br_stem_cov){
                        _stem_cov->insert(__sc);
                    }
                }
            }
            
            return 0;
        }
        
        template<typename PerfType, typename RNType, typename REType>
        int IsolatedSubGraph<PerfType,RNType,REType>::exploreStem(size_t sNode, size_t &brNode, std::set<size_t> *_indv_cov, flare::Coverage* _coverage){
            auto sourceNode = this->nodes.getValue(sNode);
            if(sourceNode->getTotalOutDegree()>1){
                brNode = sNode;
                auto ic = sourceNode->getIndvCoverage();
                if(_indv_cov->size()==0){
                    for(size_t s:ic){
                        _indv_cov->insert(s);
                    }
                }else{
                    individualCoverage(&ic, _indv_cov);
                    
                    // If individual coverage upto branching node contains different set of individuals
                    if(_indv_cov->size()==0){
                        _coverage->clearEdgeCoverage();
                        for(size_t s:ic){
                            _indv_cov->insert(s);
                        }
                    }
                }
                
                return 2;
            }else if(sourceNode->getTotalOutDegree()==1){
                auto ic = sourceNode->getIndvCoverage();
                if(_indv_cov->size()==0){
                    for(size_t s:ic){
                        _indv_cov->insert(s);
                    }
                }else{
                    individualCoverage(&ic, _indv_cov);
                }
                
                auto toNode = sourceNode->getNextNode(0);
                auto nId = toNode->getID();
                ic = toNode->getIndvCoverage();
                individualCoverage(&ic, _indv_cov);
                
                // If individual coverage upto this node contains different set of individuals
                if(_indv_cov->size()==0){
                    _coverage->clearEdgeCoverage();
                }else{
                    _coverage->addCoverageEdge(std::to_string(sourceNode->getID()) + "#" + std::to_string(toNode->getID()));
                }
                
                return exploreStem(nId, brNode, _indv_cov, _coverage);
            }
            
            return 0;
        }
        
        // Set s2 = Set s1 interset Set s2
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::individualCoverage(std::set<size_t> *s1, std::set<size_t> *s2){
            std::vector<size_t> v1;
            std::vector<size_t> v2;
            v1.insert(v1.begin(), s1->begin(), s1->end());
            v2.insert(v2.begin(), s2->begin(), s2->end());
            std::sort(v1.begin(), v1.end());
            std::sort(v2.begin(), v2.end());
            std::vector<size_t> v((v1.size()<v2.size())?v1.size():v2.size(), 0);
            std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), v.begin());
            s2->clear();
            for(size_t s:v){
                if(s>0) s2->insert(s);
            }
        }
        
        // Set x = set s1 minus set s2
        // Return true if x is empty
        // Otherwise return false
        template<typename PerfType, typename RNType, typename REType>
        bool IsolatedSubGraph<PerfType,RNType,REType>::checkIndividualCoverage(std::set<size_t> *s1, std::set<size_t> *s2){
            std::vector<size_t> v1;
            std::vector<size_t> v2;
            v1.insert(v1.begin(), s1->begin(), s1->end());
            v2.insert(v2.begin(), s2->begin(), s2->end());
            std::sort(v1.begin(), v1.end());
            std::sort(v2.begin(), v2.end());
            std::vector<size_t> v((v1.size()>v2.size())?v1.size():v2.size(), 0);
            std::set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), v.begin());
            
            for(size_t s:v){
                if(s>0) return false;
            }
            
            return true;
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::computeFlareScore(std::vector<flare::Coverage> *_cov){
            
            for(size_t i=0; i<_cov->size(); i++){
                this->computeScoreForAFlare(&_cov->at(i));
            }
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::getCoverageInOutDegree(size_t aNode, flare::Coverage *&_cov, CustomHashMap<size_t, flare::CoverageBranch>*& _brNodeCov, short& inD, short& outD){
            auto _node = this->nodes.getValue(aNode);
            auto brNodeCov = &_brNodeCov->getMap();
            
            if(_node!=nullptr){
                inD = 0;
                for(short i=0; i<_node->getTotalInDegree(); i++){
                    auto fNode = _node->getPrevNode(i);
                    if(fNode!=nullptr && _cov->hasCoverageEdge(std::to_string(fNode->getID())+"#"+std::to_string(aNode))){
                        inD++;
                    }
                }
                
                outD = 0;
                for(short i=0; i<_node->getTotalOutDegree();i++){
                    auto toNode = _node->getNextNode(i);
                    if(toNode!=nullptr && _cov->hasCoverageEdge(std::to_string(aNode)+"#"+std::to_string(toNode->getID()))){
                        outD++;
                    }
                }
            }
            
            if(outD>1){
                if(_brNodeCov->hasItem(aNode)){
                    auto itr = brNodeCov->find(aNode);
                    if(itr->second.totalCovOutDegree>itr->second.coveredOutDegree)itr->second.coveredOutDegree++;
                }else{
                    flare::CoverageBranch cb;
                    cb.totalCovOutDegree = outD;
                    cb.totalCovInDegree = inD;
                    cb.coveredOutDegree++;
                    
                    _brNodeCov->addToMap(aNode, cb);
                }
            }else if(inD>1){
                if(!_brNodeCov->hasItem(aNode)){
                    flare::CoverageBranch cb;
                    cb.totalCovOutDegree = outD;
                    cb.totalCovInDegree = inD;
                    cb.coveredOutDegree++;
                    
                    _brNodeCov->addToMap(aNode, cb);
                }
            }
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::computeScoreForAFlare(flare::Coverage *_cov){
            auto tNodes = _cov->getTerminalNodes();
            bool inThisSubgraph = false;
            
            //Get first landing branching nodes with score
            //for(unordered_set<size_t>::iterator itr=tNodes.begin(); itr!=tNodes.end(); itr++){
            for(size_t tNode:tNodes){
                
                if(this->nodes.hasItem(tNode)){
                    inThisSubgraph = true;
                    // Check whether tNode is a branching node or not
                    auto toNode = this->nodes.getValue(tNode);
                    
                    for(short i=0; i<toNode->getTotalInDegree();i++){
                        auto frNode = toNode->getPrevNode(i);
                        
                        if(frNode!=nullptr){
                            auto _edge = this->edges.getValue(std::to_string(frNode->getID()) + "#" + std::to_string(toNode->getID()));
                            
                            if(_edge!=nullptr){
                                if(_cov->hasCoverageEdge(_edge->getID())){
                                    float score = _edge->getWeight(hyppox::Config::REFERENCE_PH_INDEX);
                                    this->computeBranchScore(_cov, score, frNode->getID());
                                }
                            }
                        }
                    }
                }
            }
            
            if(!inThisSubgraph) return;
            
            auto brNodeScore = &_cov->getCoverageBranchNodeScore();
            auto branchNodeScore = &brNodeScore->getMap();
            
            //float cov_score = 0.0;
            bool visited = false;
            
            while (!visited) {
                
                visited = true;
                for(auto itr = branchNodeScore->begin(); itr!=branchNodeScore->end(); itr++){
                    if(itr->second.visited==false && (itr->second.totalCovOutDegree-itr->second.coveredOutDegree)==0){
                        visited = false;
                        float brScore = (itr->second.totalCovInDegree>0)?itr->second.score/itr->second.totalCovInDegree:itr->second.score;
                        
                        if(this->nodes.hasItem(itr->first)){
                            auto toNode = this->nodes.getValue(itr->first);
                            
                            // Branching node has no indegree
                            if(toNode->getTotalInDegree()==0){
                                _cov->addScore(brScore);
                            }else{
                                for(int i=0; i<toNode->getTotalInDegree(); i++){
                                    auto frNode = toNode->getPrevNode(i);
                                    
                                    if(frNode!=nullptr){
                                        
                                        if(this->edges.hasItem(std::to_string(frNode->getID()) + "#" + std::to_string(toNode->getID()))){
                                            auto _edge = this->edges.getValue(std::to_string(frNode->getID()) + "#" + std::to_string(toNode->getID()));
                                            
                                            if(_cov->hasCoverageEdge(_edge->getID())){
                                                float score = _edge->getWeight(hyppox::Config::REFERENCE_PH_INDEX);
                                                this->computeBranchScore(_cov, score, brScore, frNode->getID());
                                            }else{
                                                _cov->addScore(brScore);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        itr->second.visited=true;
                    }
                }
            }
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::computeBranchScore(flare::Coverage *&_cov, float &score, float brScore, size_t tNode){
            auto aNode = this->nodes.getValue(tNode);
            auto brNodeScore = &_cov->getCoverageBranchNodeScore();
            
            short tin=0, tout=0;
            this->getCoverageInOutDegree(tNode, _cov, brNodeScore, tin, tout);
            
            // Single linkage branch
            if(tin<=1&&tout<=1){
                if(tin==0){
                    _cov->addScore(brScore);
                }else if(aNode!=nullptr){
                    for(short i=0; i<aNode->getTotalInDegree();i++){
                        auto frNode = aNode->getPrevNode(i);
                        
                        if(frNode!=nullptr){
                            auto _edge = this->edges.getValue(std::to_string(frNode->getID()) + "#" + std::to_string(aNode->getID()));
                            
                            if(_edge!=nullptr){
                                if(_cov->hasCoverageEdge(_edge->getID())){
                                    score += _edge->getWeight(hyppox::Config::REFERENCE_PH_INDEX);
                                    
                                    this->computeBranchScore(_cov, score, brScore, frNode->getID());
                                }
                            }
                        }
                    }
                }
            }else if(tout>1){
                auto brNodeCov = &brNodeScore->getMap();
                auto itr = brNodeCov->find(tNode);
                if(itr!=brNodeCov->end()){
                    if(itr->second.totalCovOutDegree==1){
                        itr->second.score = brScore;
                    }else{
                        itr->second.score += brScore+score;
                    }
                }
            }
            
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::computeBranchScore(flare::Coverage *&_cov, float& score, size_t tNode){
            auto aNode = this->nodes.getValue(tNode);
            auto brNodeScore = &_cov->getCoverageBranchNodeScore();
            
            short tin=0, tout=0;
            this->getCoverageInOutDegree(tNode, _cov, brNodeScore, tin, tout);
            
            // Single linkage branch
            if(tin<=1&&tout<=1){
                if(aNode!=nullptr){
                    for(short i=0; i<aNode->getTotalInDegree();i++){
                        auto frNode = aNode->getPrevNode(i);
                        
                        if(frNode!=nullptr){
                            auto _edge = this->edges.getValue(std::to_string(frNode->getID()) + "#" + std::to_string(aNode->getID()));
                            
                            if(_edge!=nullptr){
                                if(_cov->hasCoverageEdge(_edge->getID())){
                                    score += _edge->getWeight(hyppox::Config::REFERENCE_PH_INDEX);
                                    
                                    this->computeBranchScore(_cov, score, frNode->getID());
                                }
                            }
                        }
                    }
                }
            }else if(tout>1){
                auto brNodeCov = &brNodeScore->getMap();
                auto itr = brNodeCov->find(tNode);
                if(itr!=brNodeCov->end()){
                    itr->second.score += score;
                }
            }
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::getD3Nodes(CustomList<std::string> *nodeListJS, short cci){
            auto nodeMap = this->nodes.getMap();
            
            std::vector<RNType*> nodeV;
            for(auto itr=nodeMap.begin(); itr!=nodeMap.end(); itr++){
                nodeV.push_back(itr->second);
            }
            
            std::sort(nodeV.begin(), nodeV.end(), RNType::compareByFilter);
            
            RGBColor* rgbColor = new RGBColor[hyppox::Config::FILTER+hyppox::Config::CLUSTER];
            
            //string filter_name = "[\"performance\"";
            for(short i=0; i<hyppox::Config::FILTER+hyppox::Config::CLUSTER; i++){
                rgbColor[i].SetGamma(0.1);
                
                if(hyppox::Config::HEATMAP_EACH_CC){
                    rgbColor[i].SetRange(this->nodeWeightRange[i][0], this->nodeWeightRange[i][1]);
                }else{
                    rgbColor[i].SetRange(this->globalNodeWeightRange[i][0], this->globalNodeWeightRange[i][1]);
                }
                
                //if(i>0)filter_name += ",\"" + hyppox::Config::FILTER_NAMES[i-1] + "\"";
            }
            //filter_name += "]";
            
            // Print only those nodes which are part of an interesting path
            /*unordered_map<string, rEdge*> edgeMap = this->edges.getMap();
             size_t paths[]={1,2,3,4,5,6};
             unordered_set<size_t> allowPaths(begin(paths), end(paths));
             unordered_set<size_t> allowNode;
             
             for(unordered_map<string, rEdge*>::iterator itr=edgeMap.begin(); itr!=edgeMap.end(); itr++){
             rEdge* e = itr->second;
             
             if(allowPaths.find(e->getIPRank())!=allowPaths.end()){
             
             allowNode.insert(e->getFromNode()->getID());
             allowNode.insert(e->getToNode()->getID());
             }
             }*/
            
            
            //for(unordered_map<size_t, rNode*>::iterator itr=nodeMap.begin(); itr!=nodeMap.end(); itr++){
            for(RNType* n:nodeV){
                //rNode* n = itr->second;
                //n->writeNodeInfo();
                
                //if(n->getID()==18){
                //cout<<n->getPointDetails();
                //}
                
                std::string color = "[", label = "[";
                for(short i=0; i<hyppox::Config::FILTER+hyppox::Config::CLUSTER; i++){
                    if(color.length()>1) color += ",";
                    if(label.length()>1) label += ",";
                    
                    color += "\"" + rgbColor[i].GetColor(n->getWeight(i)) + "\"";
                    label += "\"" + fixPrecision(n->getWeight(i), 2) + "\"";
                }
                color += "]";
                label += "]";
                
                std::string _nodeSize = fixPrecision(adjustSize(n->getSize()), 2);
                //cout<<n->getID()<<","<<n->getSize()<<","<<_nodeSize<<endl;
                std::string node = "{\"Id\":" + std::to_string(n->getID()) + ",\"Size\":" +
                    //((hyppox::Config::FILTER==1)?"15":fixPrecision(adjustSize(n->getSize()), 2)) +
                    _nodeSize + ",\"NP\":" + fixPrecision(n->getSize(), 2) +
                    ",\"Color\":" + color + ",\"pie\":" + n->getPieChart()+ ",\"Label\":" + label + ",\"Ph\":" +
                    n->getPhIdListForD3() +
                    "}";
                //n->getPhListForD3() + "}";
                
                nodeListJS->addToList(node);
            }
            
            delete [] rgbColor;
        }
        
        template<typename PerfType, typename RNType, typename REType>
        void IsolatedSubGraph<PerfType,RNType,REType>::getD3Edges(CustomList<std::string> *edgeListJS, CustomList<path::InterestingPath>* IPs, short cci){
            auto edgeMap = this->edges.getMap();
            
            for(auto itr=edgeMap.begin(); itr!=edgeMap.end(); itr++){
                auto e = itr->second;
                
                std::string label;
                e->getSignature(label);
                std::string edges = "";
                
                short wINT = (e->getRank(INT_PATH)>0)?hyppox::Config::EDGE_WIDTH:2;
                short wCOV = (e->getRank(FLARE)>0)?hyppox::Config::EDGE_WIDTH:2;
                
                size_t fID = e->getFromNode()->getID();
                size_t tID = e->getToNode()->getID();
                
                if(!e->getDirection()){
                    size_t tmpID = fID;
                    fID = tID;
                    tID = tmpID;
                }
                
                edges = "{\"source\": "+std::to_string(fID)+",\"target\":" + std::to_string(tID)+
                    ",\"L\":\"" + label + "\",\"C\":\"" + e->getColor(INT_PATH) + "\",\"FC\":\"" + e->getColor(FLARE) + "\",\"W\":" + std::to_string(wINT) +
                ", \"FW\":" + std::to_string(wCOV) + ",\"ED\":" + (e->getDirection()?"1":"0") + ",\"R\":" + std::to_string(e->getRank(INT_PATH)) +
                ",\"FR\":" + std::to_string(e->getRank(FLARE)) + ",\"CC\":" + std::to_string(cci) + "}";
                
                /*if(hyppox::Config::FILTER==1){
                 edges = "{\"source\": "+to_string(e->getFromNode()->getID())+", \"target\":" + to_string(e->getToNode()->getID())+
                 ",\"L\":\"" + label + "\", \"C\":\"" + hyppox::Config::EDGE_COLOR + "\",\"W\":2,\"R\":0,\"CC\":" + to_string(cci) + "}";
                 }else{
                 string color[] = {e->getColor(INT_PATH), e->getColor(FLARE)};
                 size_t rank[] = {e->getRank(INT_PATH), e->getRank(FLARE)};
                 
                 if(rank[]>0){
                 edges = "{\"source\": "+to_string(e->getFromNode()->getID())+", \"target\":" + to_string(e->getToNode()->getID())+
                 ",\"L\":\"" + label + "\", \"C\":\"" + color + "\",\"W\":" + to_string(hyppox::Config::EDGE_WIDTH) + ",\"R\":" + to_string(rank) +
                 ",\"CC\":" + to_string(cci) + "}";
                 }
                 // Remove all edges those not part of an interesting path
                 else{
                 edges = "{\"source\": "+to_string(e->getFromNode()->getID())+", \"target\":" + to_string(e->getToNode()->getID())+
                 ",\"L\":\"" + label + "\", \"C\":\"" + hyppox::Config::EDGE_COLOR + "\",\"W\":2,\"R\":0,\"CC\":" + to_string(cci) + "}";
                 }
                 }*/
                
                if(edges.length()>0)
                edgeListJS->addToList(edges);
            }
        }
        
        template<typename PerfType, typename RNType, typename REType>
        float IsolatedSubGraph<PerfType,RNType,REType>::adjustSize(float size){
            float max = hyppox::Config::NODE_SIZE_RANGE[1], min = hyppox::Config::NODE_SIZE_RANGE[0],
            Rmin = this->nodeSizeRange[0], Rmax = this->nodeSizeRange[1];
            
            if(Rmax==Rmin) return (min+max)/2;
            
            float k = (max-min)/(Rmax-Rmin),
            alpha = min + (k*(size-Rmin));
            
            return alpha;
        }
        
        ///////// Graph ///////////////
        template<typename PerfType, typename FHType>
        class Graph{
            public:
            using RNType = graph_element::rNode<PerfType>;
            using REType = graph_element::rEdge<RNType>;
            
            std::unordered_map<std::string, size_t> individualIndexMap;
            std::vector<std::string> indv_color;
            
            Graph()=default;
            ~Graph()=default;
            
            void initGraph();
            void setConnectedComponents(short totalCC);
            auto getConnectedComponent(short index);
            void printGMLGraph();
            void setGlobalNodeSizeAndWeightRange(std::vector<float> size, std::vector<std::vector<float> > weight);
            void generateInterestingPaths();
            void generateFlares();
            std::string printD3Graph();
            
            private:
            std::vector<IsolatedSubGraph<PerfType,RNType,REType>> connectedComponent;
            short totalCC;
            std::vector<float> globalNodeSizeRange;
            std::vector<std::vector<float> > globalNodeWeightRange;
            CustomList<path::InterestingPath> IPs;
            std::vector<flare::Coverage> _coverages;
            bool isCoverage;
            
            std::vector<RGB> generateColors(size_t totalColors);
            void generatePathColor();
            void generateFlareColor();
            void resetLinks(bool coverage);
            void setIndvColor();
        };
        
        template<typename PerfType, typename FHType>
        void Graph<PerfType,FHType>::initGraph(){
            this->totalCC = 0;
            this->globalNodeSizeRange.assign(2, 0.0);
            std::vector<float> _v(2, 0.0);
            this->globalNodeWeightRange.assign(hyppox::Config::FILTER+hyppox::Config::CLUSTER, _v);
            this->isCoverage = false;
        }
        
        template<typename PerfType, typename FHType>
        void Graph<PerfType,FHType>::setConnectedComponents(short totalCC){
            if(totalCC>0){
                this->totalCC = totalCC;
                IsolatedSubGraph<PerfType,RNType,REType> _sg(&this->individualIndexMap);
                this->connectedComponent.assign(this->totalCC, _sg);
            }
        }
        
        template<typename PerfType, typename FHType>
        auto Graph<PerfType,FHType>::getConnectedComponent(short index){
            return &this->connectedComponent[index];
        }
        
        template<typename PerfType, typename FHType>
        std::vector<RGB> Graph<PerfType,FHType>::generateColors(size_t totalColors){
            
            if(totalColors<1) totalColors=1;
            
            std::vector<RGB> rgb(totalColors);
            RGB ra(255,0,0), rb(255,255,0), rc(0,255,0), rd(0,255,255), re(0,0,255);
            
            if(totalColors<=5){
                switch (totalColors) {
                    case 1:
                    rgb[0] = ra;
                    break;
                    
                    case 2:
                    rgb[0] = ra;
                    rgb[1] = re;
                    break;
                    
                    case 3:
                    rgb[0] = ra;
                    rgb[1] = rc;
                    rgb[2] = re;
                    break;
                    
                    case 4:
                    rgb[0] = ra;
                    rgb[1] = rb;
                    rgb[2] = rd;
                    rgb[3] = re;
                    break;
                    
                    case 5:
                    rgb[0] = ra;
                    rgb[1] = rb;
                    rgb[2] = rc;
                    rgb[3] = rd;
                    rgb[4] = re;
                    break;
                    
                    default:
                    break;
                }
            }else{
                size_t a = 3*ceil(totalColors/4);
                size_t b = 3*floor(totalColors/4);
                size_t interval = 0;
                
                if(totalColors-a > 4 && totalColors-b>4){
                    interval = ceil(totalColors/4);
                }else{
                    interval = floor(totalColors/4);
                }
                
                rgb[0] = ra;
                rgb[interval] = rb;
                rgb[2*interval] = rc;
                rgb[3*interval] = rd;
                rgb[totalColors-1] = re;
                
                size_t counter = 0;
                float ta=0, tb=0, tc=0, t1=0, t2=0;
                for(size_t i=1; i<=totalColors; i++,counter++){
                    
                    if(counter==0){
                        ta = i;
                    }else if(counter == interval){
                        t1 = i;
                    }else if(counter == (2*interval)){
                        tb = i;
                    }else if(counter == (3*interval)){
                        t2 = i;
                    }else if(counter == totalColors-1){
                        tc = i;
                    }
                }
                
                counter=0;
                for(size_t i=1; i<=totalColors; i++,counter++){
                    
                    if(counter>0 && counter<interval){
                        
                        float d1 = ABS(ta-t1);
                        float d2 = ABS(ta-i);
                        short gray = ceil((255*d2)/d1);
                        
                        RGB r(255,gray,0);
                        rgb[counter] = r;
                        
                    }else if(counter>interval && counter<(2*interval)){
                        
                        float d1 = ABS(t1-tb);
                        float d2 = ABS(tb-i);
                        short red = ceil((255*d2)/d1);
                        
                        RGB r(red,255,0);
                        rgb[counter] = r;
                    }else if(counter>(2*interval) && counter<(3*interval)){
                        
                        float d1 = ABS(t2-tb);
                        float d2 = ABS(tb-i);
                        short blue = ceil((255*d2)/d1);
                        
                        RGB r(0,255,blue);
                        rgb[counter] = r;
                    }else if(counter>(3*interval) && counter<totalColors-1){
                        
                        float d1 = ABS(t2-tc);
                        float d2 = ABS(tc-i);
                        short grey = ceil((255*d2)/d1);
                        
                        RGB r(0,grey,255);
                        rgb[counter] = r;
                    }
                }
            }
            
            return rgb;
        }
        
        template<typename PerfType, typename FHType>
        void Graph<PerfType,FHType>::generatePathColor(){
            std::list<path::InterestingPath>* IPList = &this->IPs.getList();
            IPList->sort(path::InterestingPath::comparePaths);
            
            size_t r = 1;
            for(auto itr=IPList->begin(); itr!=IPList->end(); itr++, r++){
                auto ip = &(*itr);
                ip->setRank(r);
            }
            
            //size_t totalPaths=IPList->size();
            
            //std::vector<RGB> rgb = this->generateColors(totalPaths);
            
            unsigned int pathCounter=0;
            //RGBColor c;
            
            for(auto itr=IPList->begin(); itr!=IPList->end(); itr++, pathCounter++){
                //RGB r = rgb[pathCounter];
                //short R=r.getR(),G=r.getG(), B=r.getB();
                
                auto p = &(*itr);
                p->setColor(getHTMLColor(p->getRank()-1));
                
                auto edgeInAPath = &p->getPath();
                for(auto epItr=edgeInAPath->begin(); epItr!=edgeInAPath->end(); epItr++){
                    std::string key = *epItr;
                    
                    for(short i=0;i<this->totalCC;i++){
                        if(this->connectedComponent[i].addRankColorToEdge(key, p->getColor(), p->getRank(), INT_PATH)) break;
                    }
                }
            }
        }
        
        template<typename PerfType, typename FHType>
        void Graph<PerfType,FHType>::printGMLGraph(){
            this->generatePathColor();
            
            std::string nodes = "", edges = "";
            std::string gmlData = "graph\n[\ndirected 1\nweighted 1\n";
            
            for(short i=0;i<this->totalCC;i++){
                nodes += this->connectedComponent[i].getGMLNodes();
                edges += this->connectedComponent[i].getGMLEdges(&this->IPs);
            }
            
            gmlData += nodes + "\n" + edges + "]";
            
            FHType* fileHandler = new FHType("");
            fileHandler->WriteDataToFile("GML_PathLength", "gml", gmlData, false);
            delete fileHandler;
        }
        
        template<typename PerfType, typename FHType>
        void Graph<PerfType,FHType>::setGlobalNodeSizeAndWeightRange(std::vector<float> size, std::vector<std::vector<float> > weight){
            this->globalNodeSizeRange[0] = size[0];
            this->globalNodeSizeRange[1] = size[1];
            
            for(short i=0;i<hyppox::Config::FILTER+hyppox::Config::CLUSTER; i++){
                this->globalNodeWeightRange[i][0] = weight[i][0];
                this->globalNodeWeightRange[i][1] = weight[i][1];
            }
        }
        
        template<typename PerfType, typename FHType>
        void Graph<PerfType,FHType>::generateInterestingPaths(){
            for(short i=0;i<this->totalCC;i++){
                this->connectedComponent[i].computeInterestingPaths(&this->IPs);
            }
        }
        
        template<typename PerfType, typename FHType>
        void Graph<PerfType,FHType>::resetLinks(bool coverage){
            for(short i=0;i<this->totalCC;i++){
                this->connectedComponent[i].resetLinks(coverage);
                this->connectedComponent[i].setupLinks();
            }
        }
        
        template<typename PerfType, typename FHType>
        void Graph<PerfType,FHType>::generateFlares(){
            this->resetLinks(true);
            
            for(short i=0;i<this->totalCC;i++){
                this->connectedComponent[i].computeFlares(&this->_coverages);
            }
            
            
            this->generateFlareColor();
        }
        
        template<typename PerfType, typename FHType>
        void Graph<PerfType,FHType>::generateFlareColor(){
            for(short i=0;i<this->totalCC;i++){
                this->connectedComponent[i].computeFlareScore(&this->_coverages);
            }
            
            std::sort(this->_coverages.begin(), this->_coverages.end(), flare::Coverage::compareFlares);
            
            //std::vector<RGB> rgb = this->generateColors(this->_coverages.size());
            //RGBColor c;
            
            for(size_t i=0;i<this->_coverages.size(); i++){
                //RGB r = rgb[i];
                //short R=r.getR(),G=r.getG(), B=r.getB();
                
                this->_coverages[i].setRank(i+1);
                this->_coverages[i].setColor(getHTMLColor(this->_coverages[i].getRank()-1));
                
                auto cEdges = this->_coverages[i].getEdgeList();
                for(auto epItr=cEdges.begin(); epItr!=cEdges.end(); epItr++){
                    std::string key = *epItr;
                    
                    for(short j=0;j<this->totalCC;j++){
                        if(this->connectedComponent[j].addRankColorToEdge(key, this->_coverages[i].getColor(), this->_coverages[i].getRank(), FLARE)) break;
                    }
                }
            }
        }
        
        template<typename PerfType, typename FHType>
        void Graph<PerfType,FHType>::setIndvColor(){
            this->indv_color.clear();
            this->indv_color.assign(this->individualIndexMap.size(), "");
            
            //std::vector<RGB> rgb = this->generateColors(this->individualIndexMap.size());
            //RGBColor c;
            
            for(size_t i=0; i<this->indv_color.size(); i++){
                //RGB r = rgb[i];
                //short R=r.getR(),G=r.getG(), B=r.getB();
                
                this->indv_color[i] = getHTMLColor(i);
            }
        }
        
        template<typename PerfType, typename FHType>
        std::string Graph<PerfType,FHType>::printD3Graph(){
            
            this->setIndvColor();
            
            this->generatePathColor();
            
            std::string indv_UIC = "\"color\": [";
            std::string indv_NAME = "\"indv\":[";
            std::string buttons = "\"btn\": [";
            std::string d3Data = "";
            std::string d3Nodes = "\"nodes\": [\n\t";
            std::vector<std::string> btn(hyppox::Config::CLUSTER_NAMES);
            btn.insert(btn.end(),hyppox::Config::FILTER_NAMES.begin(), hyppox::Config::FILTER_NAMES.end());
            
            // All individual colors
            for(size_t i=0; i<this->indv_color.size(); i++){
                indv_UIC += "\"" + this->indv_color[i] + "\"";
                if(i<this->indv_color.size()-1) indv_UIC += ",";
            }
            indv_UIC += "]";
            
            // All individuals
            std::vector<std::string>indvName(this->indv_color.size(), "");
            for(auto itr:this->individualIndexMap){
                indvName[itr.second-1] = itr.first;
            }
            
            for(size_t i=0; i<indvName.size(); i++){
                indv_NAME += "\"" + indvName[i] + "\"";
                if(i<indvName.size()-1) indv_NAME += ",";
            }
            indv_NAME += "]";
            indvName.clear();
            
            // Column names of both filter and cluster
            for(size_t i=0; i<btn.size(); i++){
                buttons += "\"" + btn[i] + "\"";
                if(i<btn.size()-1) buttons += ",";
            }
            buttons += "]";
            
            CustomList<std::string> nodeListJS, edgeListJS;
            
            for(short i=0;i<this->totalCC;i++){
                this->connectedComponent[i].getD3Nodes(&nodeListJS, i+1);
                this->connectedComponent[i].getD3Edges(&edgeListJS, &this->IPs, i+1);
            }
            
            std::string _filePath = "";
            
            if(nodeListJS.getSize()>0 && edgeListJS.getSize()>0){
                std::string d3Data = "";
                std::string d3Nodes = "\"nodes\": [\n\t";
                
                auto d3NodeList = nodeListJS.getList();
                
                for(auto d3Itr=d3NodeList.begin(); d3Itr!=d3NodeList.end(); d3Itr++){
                    if(d3Nodes.find("{")!=std::string::npos) d3Nodes += ",\n\t";
                    
                    d3Nodes += *d3Itr;
                }
                
                d3Nodes += "]";
                
                std::string d3Edges = "\"links\": [\n\t";
                
                auto d3EdgeList = edgeListJS.getList();
                
                for(auto d3Itr=d3EdgeList.begin(); d3Itr!=d3EdgeList.end(); d3Itr++){
                    if(d3Edges.find("{")!=std::string::npos) d3Edges += ",\n\t";
                    
                    d3Edges += *d3Itr;
                }
                
                d3Edges += "]";
                
                d3Data = "{" + indv_UIC + ",\n" + indv_NAME + ",\n" + buttons + ",\n" + d3Nodes + ",\n" + d3Edges + "}";
                
                std::string fName = "";
                for(int i=0; i<hyppox::Config::FILTER; i++){
                    if(fName.length()>0) fName += "_";
                    fName += hyppox::Config::FILTER_NAMES[i];
                }
                
                for(int i=0;i<hyppox::Config::FILTER;i++)
                fName += "_" + std::to_string(hyppox::Config::WINDOWS[i]);
                
                for(int i=0;i<hyppox::Config::GAIN.size();i++)
                fName += "_" + fixPrecision((hyppox::Config::GAIN[i]<0.98)?(hyppox::Config::GAIN[i]/2)*100:50, 2);
                
                fName += "_" + fixPrecision(hyppox::Config::CLUSTER_PARAM[0], 2);
                fName += "_" + fixPrecision(hyppox::Config::CLUSTER_PARAM[1], 0);
                
                
                //if(hyppox::Config::CLUSTER>1){
                fName += "_" + hyppox::Config::CLUSTER_NAMES[hyppox::Config::REFERENCE_PH_INDEX];
                //}
                
                FHType* writeToFile = new FHType("");
                _filePath = writeToFile->WriteDataToFile(hyppox::Config::WRITE_DIR+fName, ".json", d3Data, false);
                delete writeToFile;
            }
            
            return _filePath;
        }
    }
}

#endif /* graph_h */
