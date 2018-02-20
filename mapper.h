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

#include "quadTree.h"
#include "dbScan.h"
#include "unionFindWithPathCompression.h"
#include "simplicialComplex.h"
#include "graph.h"
#include <vector>

class Grid{
public:
    float *center;
    unsigned long id;
    unordered_set<unsigned long> clusterIdList;
    
    Grid(){
        this->clusterIdList.clear();
        this->center = (float*)malloc(sizeof(float)*Config::FILTER);
    }
    
    ~Grid(){
        free(this->center);
    }
    
    void GetClusterID(unordered_set<unsigned long>* mainClsIDSet){
        unsigned long ide =0;
        
        for(unordered_set<unsigned long>::iterator itr=clusterIdList.begin(); itr!=clusterIdList.end(); itr++){
            ide = *itr;
            
            mainClsIDSet->insert(ide);
        }
    }
};

class AnEdge{
private:
    long node1;
    long node2;
    string id;
public:
    AnEdge(long n1, long n2){
        this->node1 = n1;
        this->node2 = n2;
        
        // An undirected edge from low cluster id to high cluster id
        id = (n1<n2)?to_string(n1)+"#"+to_string(n2):to_string(n2)+"#"+to_string(n1);
    }
    
    ~AnEdge(){}
    long getNode1(){return node1;}
    long getNode2(){return node2;}
};

class Mapper{
private:
    Grid **mesh;
    QuadTree *treeRoot; // To store data for nD (n>1) filter
    Graph* graph;
    list<DataPoint*> dataPointList;
    float *minPos;
    float *maxPos;
    float *gridLength;
    list<AnEdge*> edgeList;
    unordered_map<unsigned long,Grid*> gridMap;
    unordered_map<long, unordered_set<long>> connectedComponents;
    
    void storeData();
    void createFilter();
    void clearClusterInformation();
    void createCluster(list<Phenotype*> *pList, bool setUniqueId);
    void assignUniqueClusterIds();
    void createUniqueClusters();
    bool isEligibleForClustering(float *min, float* max, unsigned long boxID);
    bool isValidCluster(list<Phenotype *> *pList, unordered_set<unsigned long>* mainClsID);
    void manageCluster(list<Phenotype *> *pList, unsigned long minClsId, unsigned long maxClsId, unordered_set<unsigned long>* mainClsID);
    void assignClusterIdsForOverlapingRegion(float overlap);
    void getIndividualsOfRegion(list<Phenotype *> *clPhList, float *min, float *max, long* clsRange);
    void setOverlappedIndv(unordered_set<string> *indvSet, list<Phenotype *> *clPhList, list<Phenotype *> *overlappedPhList, unordered_set<string>* edgeSet);
    void generateEdgeListFromOverlappingClusters(list<Phenotype*>* overlappedPhList, float overlap);
    void addSimplicialComplex(SimplicialComplex *sc, float overlap);
    void generateSimplicesForPersistentHomology();
    void computeConnectedComponents();
    void constructGraphComponents();
    void constructGraph();
    void visualization();
public:
    Mapper();
    ~Mapper();
    void createMapper();
};

#endif /* mapper_hp */
