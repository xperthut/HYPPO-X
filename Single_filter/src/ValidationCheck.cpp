//
//  ValidationCheck.cpp
//  FastCompressedQuadTree
//
//  Created by Md Kamruzzaman on 5/8/16.
//  Copyright © 2016 WSU. All rights reserved.
//

#include "ValidationCheck.hpp"
#include <algorithm>
#include <math.h>

ValidationCheck::ValidationCheck(string fileName, float windowX, float windowY, float overlapX, float overlapY, float radius, float minPoints, int foldCount){
    this->fileName = fileName;
    this->windowX = windowX;
    this->windowY = windowY;
    this->overlapX = overlapX;
    this->overlapY = overlapY;
    this->Eps = radius;
    this->minPoints = minPoints;
    this->foldCount = foldCount;
    
    this->BuildQuadTree(foldCount);
}

ValidationCheck::~ValidationCheck(){
    delete this->clusterRegion;
    delete this->treeRoot;
}

void ValidationCheck::BuildQuadTree(int foldCount){
    list<Phenotype*> phynList;
    
    FileHandler fileHandler(this->fileName);
    fileHandler.ReadFileData(&this->dataPointList);
    
    DataPoint* tmpDP;
    
    list<DataPoint*>::iterator itr;
    list<Phenotype*>::iterator ptr;
    
    float minX=0, minY=0, maxX=0, maxY=0;
    bool first = true;
    long totalRecords = 0;
    
    for (itr = dataPointList.begin(); itr != dataPointList.end(); itr++)
    {
        tmpDP = *itr;
        
        this->crossValidationDataPointList.push_back(tmpDP);
        
        if(first){
            minX = tmpDP->positionX;
            
            maxX = tmpDP->positionX;
            
            first = false;
        }
        
        if(minX > tmpDP->positionX){minX = tmpDP->positionX;}
        
        if(maxX < tmpDP->positionX){maxX = tmpDP->positionX;}
        
        totalRecords++;
    }
    
    this->recordPerIteration = (long)(totalRecords/foldCount);
        
    cout<<minX<<endl<<minY<<endl<<maxX<<endl<<maxY<<endl<<totalRecords<<endl;
    
    clock_t t1, t2;
    
    t1 = clock();
    this->treeRoot = new QuadTree(minX, (maxX-minX));
    
    for (itr = dataPointList.begin(); itr != dataPointList.end(); itr++)
    {
        tmpDP = *itr;
        
        treeRoot->AddDataToNode(tmpDP);
    }
    t2 = clock();
    
    cout<<"\nQuad tree construction completed..."<<(t2-t1)/CLK_TCK<<endl;
    
    this->clusterRegion = new ClusteringRegion(treeRoot, minX, minY, maxX, maxY, windowX, windowY, overlapX, overlapY);
    
}

void ValidationCheck::CreateClusterAndGraph(){
    clock_t t1, t2;
    
    t1 = clock();
    clusterRegion->SearchRectangle();
    t2 = clock();
    
    //cout<<"Clustering has completed: "<<(t2-t1)/CLOCKS_PER_SEC<<"(s)"<<endl;
    
    this->clusterRegion->GenerateGraphUsingOverlappingClusters();
    //string str = this->clusterRegion->PrintGraph();
    //cout<<str<<endl;
}

void ValidationCheck::CrossValidation(float searchRadius, int type, int k, int N, bool isArithmaticMean){
    
    list<list<long>> foldList;
    
    RandomPermutation rm(1,this->dataPointList.size());
    rm.GetValue(this->foldCount, &foldList);
    
    cout<<this->crossValidationList.size()<<endl;
    
    list<DataPoint*> trainList;
    list<DataPoint*> testList;
    
    list<list<DataPoint*>>::iterator itr;
    list<DataPoint*>::iterator ditr;
    
    long testCount = 1;
    int runningFoldCount = 1;
    long totalTestRecord = 1;
    
    list<list<long>>::iterator fitr;
    list<long>::iterator litr;
    float overallRMSerror = 0.0;
    
    for(fitr = foldList.begin(); fitr!=foldList.end(); fitr++){
        list<long> tmpList = *fitr;
        testCount = 1;
        
        for(ditr=this->crossValidationDataPointList.begin(); ditr!=this->crossValidationDataPointList.end(); ditr++){
            if(find(tmpList.begin(), tmpList.end(), testCount) == tmpList.end()){
                trainList.push_back(*ditr);
            }else{
                testList.push_back(*ditr);
                
                cout<<testCount<<",";
            }
            
            testCount++;
            
        }
        
        float rmsError = this->GetPredictedValue(&trainList, &testList, searchRadius, type, k, N, isArithmaticMean);
        overallRMSerror += rmsError;
        totalTestRecord += testList.size();
        
        cout<<"\n\nValidation check for fold:"<<runningFoldCount<<"\ntrain count:"<<trainList.size()<<"\ntest count:"<<testList.size()<<"\nSquare error:"<<rmsError<<endl<<endl;
        
        trainList.clear();
        testList.clear();
        runningFoldCount++;
        
        for(ditr=this->crossValidationDataPointList.begin(); ditr!=this->crossValidationDataPointList.end(); ditr++){
            
            DataPoint* dp = *ditr;
            
            list<Phenotype*>::iterator pitr;
            
            for(pitr=dp->phenotypeList.begin(); pitr!=dp->phenotypeList.end(); pitr++){
                Phenotype* ph = *pitr;
                ph->ResetClusterInformation();
            }
            
        }
    }
    
    overallRMSerror = overallRMSerror/totalTestRecord;
    
    cout<<"\n\nOverall RMS Error: "<<sqrtf(overallRMSerror)<<endl<<totalTestRecord<<endl;
    
}

float ValidationCheck::GetPredictedValue(list<DataPoint *> *trainList, list<DataPoint *> *testList, float radius, int errorType, int k, int N, bool isArithmaticMean){
    float RMSvalue = 0.0;
    DataPoint* tmpDP;
    
    list<DataPoint*>::iterator itr;
    list<Phenotype*>::iterator ptr;
    
    float minX=0, minY=0, maxX=0, maxY=0;
    bool first = true;
    long totalRecords = 0;
    
    for (itr = trainList->begin(); itr != trainList->end(); itr++)
    {
        tmpDP = *itr;
        
        if(first){
            minX = tmpDP->positionX;
            maxX = tmpDP->positionX;
            
            first = false;
        }
        
        if(minX > tmpDP->positionX){minX = tmpDP->positionX;}
        if(maxX < tmpDP->positionX){maxX = tmpDP->positionX;}
        
        totalRecords++;
    }
    
    cout<<minX<<endl<<minY<<endl<<maxX<<endl<<maxY<<endl<<totalRecords<<endl;
    
    clock_t t1, t2;
    
    t1 = clock();
    QuadTree root(minX, (maxX-minX));
    
    for (itr = trainList->begin(); itr != trainList->end(); itr++)
    {
        tmpDP = *itr;
        
        root.AddDataToNode(tmpDP);
    }
    t2 = clock();
    
    cout<<"\nQuad tree construction completed..."<<(t2-t1)/CLK_TCK<<endl;
    
    ClusteringRegion clsRegion(&root, minX, minY, maxX, maxY, windowX, windowY, overlapX, overlapY);
    
    t1 = clock();
    clsRegion.SearchRectangle();
    t2 = clock();
    
    map<size_t, PhenotypeList*> hashMap;
    
    if(errorType == MEAN_MEAN || errorType == BIGGEST_MEAN){
        map<size_t, PhenotypeList*>::iterator hitr;
        list<Phenotype*> clsPhList;
        list<Phenotype*>::iterator clsItr;
        
        root.SearchSurface(minX-.5, maxX+.5, &clsPhList, NULL, 0);
        
        long clsID[4];
        for(clsItr=clsPhList.begin(); clsItr != clsPhList.end(); clsItr++){
            Phenotype* ph = *clsItr;
            ph->GetIDList(clsID);
            
            for(short i=0; i<4; i++){
                if(clsID[i] > 0){
                    if((hitr = hashMap.find(clsID[i])) == hashMap.end()){
                        hashMap.insert(pair<size_t, PhenotypeList*>(clsID[i], new PhenotypeList()));
                        
                        hitr = hashMap.find(clsID[i]);
                    }
                    
                    hitr->second->AddPhenotype(ph);
                }
            }
        }
        
    }
    
    list<Phenotype*> phList;
    float x,y;
    float rms = 0.0;

    for (itr = testList->begin(); itr != testList->end(); itr++)
    {
        tmpDP = *itr;
        
        x = tmpDP->positionX;
        
        phList.clear();
        
        root.SearchSurface(x-radius, x+radius, &phList, NULL, 0);
        
        if(phList.size() > 0){
            if(errorType == MEAN_MEAN){
            
                rms += this->GetMeanOfAllClusters(tmpDP, &phList, &hashMap, true, isArithmaticMean);
                
            }if(errorType == BIGGEST_MEAN){
                
                rms += this->GetMeanOfAllClusters(tmpDP, &phList, &hashMap, false, isArithmaticMean);
                
            }else if(errorType == ARITHMATIC_MEAN){
            
                rms += this->GetRMSValueFromArithmaticMean(tmpDP, &phList);
                
            }else if(errorType == GEOMETRIC_MEAN){
                
                rms += this->GetRMSValueFromGeometricMean(tmpDP, &phList);
                
            }else if(errorType == BIGGEST_CLUSTER){
            
                rms += this->GetRMSValueFromBiggest_K_Cluster(tmpDP, &phList, k, isArithmaticMean);
                
            }else if(errorType == KNN){
                
                rms += this->GetRMSValueFrom_N_Neighbors(tmpDP, &phList, N, isArithmaticMean);
                
            }else if(errorType == RANDOM){
                
                rms += this->GetRMSValueByRandomSelection(tmpDP, &phList);
            }
        }
    }
    
    RMSvalue = rms;
    
    return RMSvalue;
}

float ValidationCheck::GetMeanOfAllClusters(DataPoint *point, list<Phenotype *> *phList, map<size_t, PhenotypeList *> *hashMap, bool isAll, bool isArithmaticMean){
    float squareError = 0.0;
    
    set<size_t> clsIDList;
    long clsID[4];
    
    list<Phenotype *>::iterator pitr;
    
    for(pitr = phList->begin(); pitr != phList->end(); pitr++){
        Phenotype* ph = *pitr;
        
        ph->GetIDList(clsID);
        
        for(short i=0; i<4; i++){
            size_t id = clsID[i];
            
            if(id > 0){
                clsIDList.insert(id);
            }
        }
    }
    
    map<size_t, PhenotypeList *>::iterator hitr;
    set<size_t>::iterator sItr;
    size_t total = clsIDList.size();
    float totalPhValue = 0.0;
    float predictedAvgValue;
    
    if(isAll){
        for(sItr=clsIDList.begin(); sItr!=clsIDList.end(); sItr++){
            size_t id = *sItr;
            
            hitr = hashMap->find(id);
            
            if(hitr != hashMap->end()){
                if(isArithmaticMean)
                    totalPhValue += hitr->second->GetAvgValue();
                else
                   totalPhValue += logf(hitr->second->GetGeometricAvgValue());
            }
        }
        
        predictedAvgValue = totalPhValue/total;
        
        if(!isArithmaticMean)
            predictedAvgValue = expf(predictedAvgValue);
        
    }else{
        
        map<size_t, ClusterIdList*>sizeList;
        map<size_t, ClusterIdList*>::iterator sitr;
        list<size_t>clsSizeList;
    
        for(hitr=hashMap->begin(); hitr!=hashMap->end(); hitr++){
            
            size_t size = hitr->second->GetSize();
            
            if((sitr=sizeList.find(size)) == sizeList.end()){
                sizeList.insert(pair<size_t, ClusterIdList*>(size, new ClusterIdList()));
                
                sitr=sizeList.find(size);
                clsSizeList.push_back(size);
            }
            
            sitr->second->AddClusterId(hitr->first);
        }
        
        clsSizeList.sort();
        clsSizeList.reverse();
        size_t biggestID = 0;
        
        list<size_t>::iterator citr;
        for(citr=clsSizeList.begin(); citr!=clsSizeList.end(); citr++){
            size_t size = *citr;
            
            sitr = sizeList.find(size);
            
            for(sItr=clsIDList.begin(); sItr!=clsIDList.end(); sItr++){
                size_t id = *sItr;
                
                if(sitr->second->findID(id)){
                    biggestID = id;
                    
                    break;
                }
            }
            
            if(biggestID>0) break;
        }
        
        if(biggestID>0){
            hitr=hashMap->find(biggestID);
            
            predictedAvgValue = hitr->second->GetAvgValue();
        }
    }
    
    float actualAvgValue = point->GetAveragePhenotypeData();
    
    squareError = powf((actualAvgValue-predictedAvgValue), 2);
    
    return squareError;
}

float ValidationCheck::GetRMSValueFromArithmaticMean(DataPoint *point, list<Phenotype *> *phList){
    float squareError = 0.0;
    float totalPhValue = 0.0;
    
    list<Phenotype *>::iterator itr;
    
    for(itr = phList->begin(); itr!=phList->end(); itr++){
        Phenotype* ph = *itr;
        
        totalPhValue += ph->GetValue();
    }
    
    float predictedAvgValue = totalPhValue/phList->size();
    float actualAvgValue = point->GetAveragePhenotypeData();
    
    squareError = powf((actualAvgValue-predictedAvgValue), 2);
    
    return squareError;
}

float ValidationCheck::GetRMSValueFromGeometricMean(DataPoint *point, list<Phenotype *> *phList){
    float squareError = 0.0;
    float totalPhValue = 0.0;
    
    list<Phenotype *>::iterator itr;
    
    for(itr = phList->begin(); itr!=phList->end(); itr++){
        Phenotype* ph = *itr;
        
        totalPhValue += logf(ph->GetValue());
    }
    
    float predictedAvgValue = totalPhValue/phList->size();
    predictedAvgValue = expf(predictedAvgValue);
    
    float actualAvgValue = point->GetAveragePhenotypeData();
    
    squareError = powf((actualAvgValue-predictedAvgValue), 2);
    
    return squareError;
}

float ValidationCheck::GetRMSValueByRandomSelection(DataPoint *point, list<Phenotype *> *phList){

    float squareError = 0.0;
    size_t size = phList->size();
    
    unsigned short seed = time(0) + (size*drand48());
    seed48(&seed);
    
    long randomPos = (long)(size*drand48());
    
    //cout<<"Random position:"<<randomPos<<endl;
    
    long iteration = 1;
    
    float predictedAvgValue = 0.0;
    
    list<Phenotype *>::iterator itr;
    
    for(itr = phList->begin(); itr!=phList->end(); itr++){
        
        if(iteration == randomPos){
        
            Phenotype* ph = *itr;
        
            predictedAvgValue = ph->GetValue();
            
            break;
        }
        
        iteration++;
    }
    
    float actualAvgValue = point->GetAveragePhenotypeData();
    
    //cout<<"Actual:"<<actualAvgValue<<endl<<"Pred:"<<predictedAvgValue<<endl;
    
    squareError = powf((actualAvgValue-predictedAvgValue), 2);
    
    return squareError;

}

void ValidationCheck::GetPhenotypeFromBiggest_K_Clusters(DataPoint *point, list<Phenotype *> *phList, int K, list<Phenotype *> *finalList){
    map<long,long> clsPhID;
    map<long,long>::iterator citr;
    long clusterID[4]={0,0,0,0};
    list<Phenotype *>::iterator itr;
    list<long> idList;
    
    map<size_t, PhenotypeList*> hash;
    map<size_t, PhenotypeList*>::iterator hitr;
    
    for(itr = phList->begin(); itr!=phList->end(); itr++){
        Phenotype* ph = *itr;
        ph->GetIDList(clusterID);
        
        for(short i=0; i<4; i++){
            if(clusterID[i] > 0){
                if((hitr = hash.find(clusterID[i])) == hash.end()){
                    
                    hash.insert(pair<size_t, PhenotypeList*>(clusterID[i], new PhenotypeList()));
                    
                    hitr = hash.find(clusterID[i]);
                    
                }
                
                hitr->second->AddPhenotype(ph);
                
                if((citr = clsPhID.find(clusterID[i])) == clsPhID.end()){
                    clsPhID.insert(pair<long, long>(clusterID[i], 1));
                    
                }else{
                    citr->second = citr->second+1;
                    
                }
                
            }
        }
    }
    
    for(citr=clsPhID.begin(); citr!=clsPhID.end(); citr++){
        long size = citr->second;
        idList.push_back(size);
    }
    
    idList.sort();
    idList.reverse();
    
    int bCls=1;
    list<long>::iterator idItr;
    
    
    // Find all points belongs biggest k clusters
    for(idItr=idList.begin(); idItr!=idList.end(); idItr++){
        size_t phCount = *idItr;
        
        for(hitr=hash.begin(); hitr!=hash.end(); hitr++){
            PhenotypeList* pl = hitr->second;
            size_t size = pl->GetSize();
            
            if(size == phCount){
                pl->CopyList(finalList);
            }
        }
        
        bCls++;
        if(bCls>K) break;
    }

}

float ValidationCheck::GetRMSValueFromBiggest_K_Cluster(DataPoint *point, list<Phenotype *> *phList, int k, bool isArithmaticMean){
    float squareError = 0.0;
    list<Phenotype*> bgPhList;
    
    this->GetPhenotypeFromBiggest_K_Clusters(point, phList, k, &bgPhList);
    
    if(isArithmaticMean)
        squareError = this->GetRMSValueFromArithmaticMean(point, &bgPhList);
    else
        squareError = this->GetRMSValueFromGeometricMean(point, &bgPhList);
    
    return squareError;
}

void ValidationCheck::GetPhenotypeFrom_N_Neighbors(DataPoint *point, list<Phenotype *> *phList, int N, list<Phenotype *> *finalList){
    // Find points belongs to N nearest neighbors using L2 distance
    int neignbors = 1;
    float testX = point->positionX;
    
    list<Phenotype*>::iterator phItr;
    map<float, PhenotypeList*> l2HashMap;
    map<float, PhenotypeList*>::iterator l2Itr;
    list<float> l2DistList;
    
    for(phItr=phList->begin(); phItr!=phList->end(); phItr++){
        Phenotype* ph = *phItr;
        
        float trainX = ph->getCoOrdinate(0);
        float trainY = ph->getCoOrdinate(1);
        
        float l2 = this->L2Distance(trainX, testX, trainY, 0);
        
        if((l2Itr=l2HashMap.find(l2)) == l2HashMap.end()){
            
            l2HashMap.insert(pair<float, PhenotypeList*>(l2, new PhenotypeList()));
            
            l2Itr = l2HashMap.find(l2);
            
            l2DistList.push_back(l2);
        }
        
        l2Itr->second->AddPhenotype(ph);
    }
    
    l2DistList.sort();
    
    list<float>::iterator fItr;
    
    for(fItr=l2DistList.begin(); fItr!=l2DistList.end(); fItr++){
        float l2d = *fItr;
        
        // Find all points that belongs in K cluster
        if((l2Itr=l2HashMap.find(l2d)) != l2HashMap.end()){
            PhenotypeList* pl = l2Itr->second;
            pl->CopyList(finalList);
        }
        
        neignbors++;
        if(neignbors>N) break;
    }
}

float ValidationCheck::GetRMSValueFrom_N_Neighbors(DataPoint *point, list<Phenotype *> *phList, int N, bool isArithmaticMean){
    float squareError = 0.0;
    list<Phenotype*> pnnList;
    
    this->GetPhenotypeFrom_N_Neighbors(point, phList, N, &pnnList);
    
    if(isArithmaticMean)
        squareError = this->GetRMSValueFromArithmaticMean(point, &pnnList);
    else
        squareError = this->GetRMSValueFromGeometricMean(point, &pnnList);
    
    
    return squareError;
}


float ValidationCheck::GetRMSValueFromBiggest_K_Cluster_N_Neighbors(DataPoint *point, list<Phenotype *> *phList, int k, int N, bool isArithmaticMean){
    float squareError = 0.0;
    
    list<Phenotype*> clsList;
    this->GetPhenotypeFromBiggest_K_Clusters(point, phList, k, &clsList);
    
    list<Phenotype*> finalList;
    this->GetPhenotypeFrom_N_Neighbors(point, &clsList, N, &finalList);
    
    if(isArithmaticMean)
        squareError = this->GetRMSValueFromArithmaticMean(point, &finalList);
    else
        squareError = this->GetRMSValueFromGeometricMean(point, &finalList);
    
    return squareError;
}

float ValidationCheck::L2Distance(float x1, float x2, float y1, float y2){
    float distance = 0.0;
    
    distance = sqrtf(powf((x1-x2), 2) + powf((y1-y2), 2));
    
    return distance;
}


