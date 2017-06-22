//
//  ValidationCheck.hpp
//  FastCompressedQuadTree
//
//  Created by Md Kamruzzaman on 5/8/16.
//  Copyright © 2016 WSU. All rights reserved.
//

#ifndef ValidationCheck_hpp
#define ValidationCheck_hpp

#include "FileHandler.hpp"
#include "ClusteringRegion.hpp"
#include "RandomPermutation.hpp"
#include <time.h>
#include <string>

enum ErrorType{
    ARITHMATIC_MEAN=0,
    GEOMETRIC_MEAN,
    BIGGEST_CLUSTER,
    KNN,
    RANDOM,
    BIGGEST_CLUSTER_KNN,
    KNN_BIGGEST_CLUSTER,
    MEAN_MEAN,
    BIGGEST_MEAN
};

class PhenotypeList;

class ValidationCheck{
private:
    string fileName;
    float windowX;
    float windowY;
    float overlapX;
    float overlapY;
    float Eps;
    float minPoints;
    long recordPerIteration;
    int foldCount;
    
    QuadTree *treeRoot;
    ClusteringRegion* clusterRegion;
    
    list<DataPoint*> dataPointList;
    list<DataPoint*> crossValidationDataPointList;
    
    list<list<DataPoint*>> crossValidationList;
    
    void BuildQuadTree(int foldCount);
    float GetPredictedValue(list<DataPoint*> *trainList, list<DataPoint*> *testList, float radius, int errorType, int k, int N, bool isArithmaticMean);
    float GetRMSValueFromArithmaticMean(DataPoint* point, list<Phenotype*> *phList);
    float GetRMSValueFromGeometricMean(DataPoint* point, list<Phenotype*> *phList);
    float GetRMSValueByRandomSelection(DataPoint* point, list<Phenotype*> *phList);
    float GetRMSValueFromBiggest_K_Cluster(DataPoint* point, list<Phenotype*> *phList, int k, bool isArithmaticMean);
    float GetRMSValueFrom_N_Neighbors(DataPoint* point, list<Phenotype*> *phList, int N, bool isArithmaticMean);
    float GetRMSValueFromBiggest_K_Cluster_N_Neighbors(DataPoint* point, list<Phenotype*> *phList, int k, int N, bool isArithmaticMean);
    float GetRMSValueFrom_N_Neighbors_Biggest_K_Cluster(DataPoint* point, list<Phenotype*> *phList, int k, int N, bool isArithmaticMean);
    float L2Distance(float x1, float x2, float y1, float y2);
    void GetPhenotypeFromBiggest_K_Clusters(DataPoint* point, list<Phenotype*> *phList, int K, list<Phenotype*> *finalList);
    void GetPhenotypeFrom_N_Neighbors(DataPoint* point, list<Phenotype*> *phList, int N, list<Phenotype*> *finalList);
    float GetMeanOfAllClusters(DataPoint* point, list<Phenotype *> *phList, map<size_t, PhenotypeList*>* hashMap, bool isAll, bool isArithmaticMean);
    
public:
    ValidationCheck(string fileName, float windowX, float windowY, float overlapX, float overlapY, float radius, float minPoints, int foldCount);
    ~ValidationCheck();
    void CreateClusterAndGraph();
    void CrossValidation(float searchRadius, int type, int k, int N, bool isArithmaticMean);
    
};

class PhenotypeList{
public:
    list<Phenotype*> pList;
    
    PhenotypeList(){
    
    }
    
    void AddPhenotype(Phenotype* ph){
        pList.push_back(ph);
    }
    
    size_t GetSize(){
        return pList.size();
    }
    
    void CopyList(list<Phenotype*> *pl){
        list<Phenotype*>::iterator itr;
        
        for(itr = pList.begin(); itr!=pList.end(); itr++){
            pl->push_back(*itr);
        }
    }
    
    float GetAvgValue(){
        float avgValue=0.0;
        size_t total = pList.size();
        
        list<Phenotype*>::iterator itr;
        
        for(itr = pList.begin(); itr!=pList.end(); itr++){
            Phenotype* ph = *itr;
            avgValue += ph->GetValue();
        }
        
        return (avgValue/total);
    }
    
    float GetGeometricAvgValue(){
        float avgValue=0.0;
        size_t total = pList.size();
        
        list<Phenotype*>::iterator itr;
        
        for(itr = pList.begin(); itr!=pList.end(); itr++){
            Phenotype* ph = *itr;
            avgValue += logf(ph->GetValue());
        }
        
        return expf(avgValue/total);
    }
    
};

class ClusterIdList{
public:
    set<size_t> clsIdList;
    
    ClusterIdList(){
        
    }
    
    void AddClusterId(size_t id){
        clsIdList.insert(id);
    }
    
    size_t GetSize(){
        return clsIdList.size();
    }
    
    bool findID(size_t id){
        return (clsIdList.find(id) != clsIdList.end());
    }
    
};

#endif /* ValidationCheck_hpp */
