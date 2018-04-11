/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: mapper.cpp
 Objective: Implementation of functions defined in header file
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              01/05/2018      Initial version
 
 **************************************************************************************************/

#include "mapper.h"

////////////////// Public methods ////////////////////

Mapper::Mapper(){
    if(Config::WINDOW_X>0 && Config::WINDOW_Y>0){
        this->mesh = (Grid**)malloc(sizeof(Grid*)*(Config::WINDOW_X));
        
        for(int i=0; i<Config::WINDOW_X; i++){
            this->mesh[i] = new Grid[Config::WINDOW_Y];
        }
    }
    
    this->minPos = (float*)malloc(sizeof(float)*Config::FILTER);
    this->maxPos = (float*)malloc(sizeof(float)*Config::FILTER);
    this->gridLength = (float*)malloc(sizeof(float)*Config::FILTER);
    
    for(int i=0; i<Config::FILTER; i++){
        this->minPos[i] = 0;
        this->maxPos[i] = 0;
    }
    
    this->graph = NULL;
}

Mapper::~Mapper(){
    if(Config::WINDOW_X>0 && Config::WINDOW_Y>0){
        for(int i=0; i<Config::WINDOW_X; i++){
            delete[] this->mesh[i];
        }
        
        free(this->mesh);
    }
    
    if(this->graph!=NULL) delete this->graph;
}

void Mapper::createMapper(){
    this->createFilter();
    
    this->createUniqueClusters();
    
    if(Config::PRINT_BARCODE){
        this->generateSimplicesForPersistentHomology();
    }else{
        this->visualization();
    }
}

////////////////// Private methods /////////////////////////
void Mapper::storeData(){
    list<Phenotype*> phynList;
    
    FileHandler fileHandler(Config::DATA_FILE_NAME);
    fileHandler.ReadFileData(&this->dataPointList);
    
    if(this->dataPointList.size()==0){
        cout<<"Can't read file."<<"\n";
        exit(0);
    }
    
    DataPoint* tmpDP;
    
    list<DataPoint*>::iterator itr;
    list<Phenotype*>::iterator ptr;
    
    bool first = true;
    
    for (itr = dataPointList.begin(); itr != dataPointList.end(); itr++)
    {
        tmpDP = *itr;
        
        //cout<<"("<<tmpDP->positionX<<","<<tmpDP->positionY<<")"<<endl;
        
        if(first){
            for(int i=0; i<Config::FILTER; i++){
                this->minPos[i] = tmpDP->getPosition(i);
                this->maxPos[i] = tmpDP->getPosition(i);
            }
            
            first = false;
        }
        
        for(int i=0; i<Config::FILTER; i++){
            if(this->minPos[i] > tmpDP->getPosition(i)) this->minPos[i] = tmpDP->getPosition(i);
            if(this->maxPos[i] < tmpDP->getPosition(i)) this->maxPos[i] = tmpDP->getPosition(i);
        }
    }
    
    // Increase search reason to encapsulate terminal points
    for(int i=0; i<Config::FILTER; i++){
        this->minPos[i] -= 0.1;
        this->maxPos[i] += 0.1;
    }
    
    // Quad tree for more than 1 filters
    if(Config::FILTER>1){
        clock_t t1, t2;
        
        t1 = clock();
        this->treeRoot = new QuadTree(this->minPos[0], this->minPos[1], (this->maxPos[0]-this->minPos[0]), (this->maxPos[1]-this->minPos[1]));
        
        long c = 0;
        
        for (itr = dataPointList.begin(); itr != dataPointList.end(); itr++)
        {
            tmpDP = *itr;
            
            c++;
            
            this->treeRoot->AddDataToNode(tmpDP);
            
        }
        t2 = clock();
        
        //cout<<"\nTotal data:"<<c<<":"<<QuadNode::totalChildren;
        cout<<"\nQuad tree construction completed in: "<<(t2-t1)/CLOCKS_PER_SEC<<"(ms)"<<endl;
    }
}

void Mapper::createFilter(){
    this->storeData();
    
    gridLength[0] = (Config::WINDOW_X>0)?(this->maxPos[0] - this->minPos[0])/Config::WINDOW_X:(this->maxPos[0] - this->minPos[0]);
    
    if(Config::FILTER>1)
        gridLength[1] = (Config::WINDOW_Y>0)?(this->maxPos[1] - this->minPos[1])/Config::WINDOW_Y:(this->maxPos[1] - this->minPos[1]);
    else
        gridLength[1] = 0;
    
    float start_x = this->minPos[0], start_y = 0;
    
    for(int i=0; i<Config::WINDOW_X; i++){
        
        if(Config::FILTER>1) start_y = this->minPos[1];
        else start_y = 0;
        
        for(int j=0; j<Config::WINDOW_Y; j++){
            this->mesh[i][j].center[0] = start_x + (this->gridLength[0]/2);
            if(Config::FILTER>1) this->mesh[i][j].center[1] = start_y + (this->gridLength[1]/2);
            else this->mesh[i][j].center[1] = start_y;
            
            if(Config::FILTER>1) start_y += this->gridLength[1];
        }
        
        start_x += this->gridLength[0];
    }
}

void Mapper::clearClusterInformation(){
    
    if(Config::FILTER==1){
        list<DataPoint*> dpList = this->dataPointList;
        
        for(list<DataPoint*>::iterator ditr = dpList.begin(); ditr != dpList.end(); ditr++){
            DataPoint* dp = *ditr;
            list<Phenotype*> clPhList = dp->getPhList();
            
            dp->ResetClusterInformation();
            
            for(list<Phenotype*>::iterator itr = clPhList.begin(); itr != clPhList.end(); itr++){
                Phenotype* ph = *itr;
                
                ph->ResetClusterInformation();
            }
        }
    }else{
        list<Phenotype*> clPhList;
        list<DataPoint*> dpList;
        
        clPhList.clear();
        
        this->treeRoot->SearchSurface(this->minPos[0], this->minPos[1], this->maxPos[0], this->maxPos[1], &clPhList, &dpList, 0);
        
        list<Phenotype*>::iterator itr;
        //set<int> varIdSet;
        
        for(itr = clPhList.begin(); itr != clPhList.end(); itr++){
            Phenotype* ph = *itr;
            
            ph->ResetClusterInformation();
            //varIdSet.insert(ph->GetVarietyID());
        }
        
        list<DataPoint*>::iterator ditr;
        
        for(ditr = dpList.begin(); ditr != dpList.end(); ditr++){
            DataPoint* dp = *ditr;
            
            dp->ResetClusterInformation();
        }
    }
    
    Cluster::SetClusterID(0);
}

void Mapper::createCluster(list<Phenotype *> *pList, bool setUniqueId){
    clock_t t1, t2;
    t1 = clock();
    DBScan *cluster = new DBScan(Config::CLUSTER_RADIUS, Config::CLUSTER_DENSITY, setUniqueId);
    cluster->MakeCluster(pList);
    t2 = clock();
    
    //std::cout<<"\nClustering completed..."<<(t2-t1)/CLOCKS_PER_SEC<<endl;
    delete cluster;
}

void Mapper::assignUniqueClusterIds(){
    float *min, *max;
    list<Phenotype*> clPhList;
    list<DataPoint*> dpList;
    unsigned long boxID = 1, lastID, thisID;
    
    min = (float*)malloc(sizeof(float)*Config::FILTER);
    max = (float*)malloc(sizeof(float)*Config::FILTER);
    
    if(mesh!=NULL){
        for(int j=0; j<Config::WINDOW_Y;j++){
            for(int i=0; i<Config::WINDOW_X; i++){
                for(short f=0; f<Config::FILTER; f++){
                    float c = this->mesh[i][j].center[f];
                    float l = this->gridLength[f]/2;
                    min[f] = c - l;
                    max[f] = c + l;
                    
                    if(min[f] < this->minPos[f]) min[f] = this->minPos[f];
                    if(max[f] > this->maxPos[f]) max[f] = this->maxPos[f];
                }
                
                clPhList.clear();
                if(Config::FILTER==1){
                    for(list<DataPoint*>::iterator dItr=dataPointList.begin(); dItr!=dataPointList.end();dItr++){
                        DataPoint* dp = *dItr;
                        float x = dp->getPosition(0);
                        
                        if(min[0]<=x&&x<=max[0]){
                            dp->setBoxId(boxID);
                            
                            list<Phenotype*> phList = dp->getPhList();
                            
                            for(list<Phenotype*>::iterator pItr = phList.begin(); pItr!=phList.end(); pItr++){
                                Phenotype* ph = *pItr;
                                clPhList.push_back(ph);
                            }
                        }
                    }
                }else{
                    dpList.clear();
                    this->treeRoot->SearchSurface(min[0], min[1], max[0], max[1], &clPhList, &dpList, boxID);
                }
                
                if(clPhList.size() > 0){
                    
                    lastID = Cluster::GetClusterID();
                    
                    this->createCluster(&clPhList, true);
                    
                    thisID = Cluster::GetClusterID();
                    
                    Grid* gr = &this->mesh[i][j];
                    gr->id = boxID;
                    
                    if(thisID-lastID>1){
                        for(long i=lastID+1; i<=thisID; i++){
                            gr->clusterIdList.insert(i);
                        }
                    }else{
                        gr->clusterIdList.insert(thisID);
                    }
                    
                    this->gridMap.insert(pair<unsigned long, Grid*>(boxID, gr));
                    
                }
                
                boxID++;
                
            }
        }
    }
    
    free(min);
    free(max);
}

void Mapper::createUniqueClusters(){
    this->clearClusterInformation();
    this->assignUniqueClusterIds();
    Cluster::SetClusterID(0);
}

bool Mapper::isEligibleForClustering(float *min, float* max, unsigned long boxID){
    
    unordered_set<unsigned long> bxIdList;
    
    this->treeRoot->SearchDataPoints(min[0], min[1], max[0], max[1], &bxIdList);
    
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
bool Mapper::isValidCluster(list<Phenotype *> *pList, unordered_set<unsigned long> *mainClsID){
    bool alienPoints = false, knownPoints = false;
    
    for(list<Phenotype *>::iterator itr=pList->begin(); itr!=pList->end(); itr++){
        Phenotype* ph = *itr;
        
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
void Mapper::manageCluster(list<Phenotype *> *pList, unsigned long minClsId, unsigned long maxClsId, unordered_set<unsigned long>* mainClsID){
    // Create bucket based on newly created cluster ids
    Cluster* cluster = new Cluster[maxClsId-minClsId+1];
    unordered_set<unsigned long> clsRange;
    
    for(unsigned long i=minClsId; i<=maxClsId; i++){
        clsRange.insert(i);
    }
    
    list<Phenotype *>::iterator itr;
    
    // assign points at each bucket based on cluster id
    for(itr=pList->begin(); itr!=pList->end(); itr++){
        Phenotype* ph = *itr;
        
        unsigned long *clsIDList = (unsigned long*)malloc(sizeof(unsigned long)*Config::TOTAL_CLUSTER_IDS);
        
        ph->GetIDList(clsIDList);
        
        for(short i=0; i<Config::TOTAL_CLUSTER_IDS; i++){
            if(clsIDList[i] > 0 && clsRange.find(clsIDList[i]) != clsRange.end()){
                cluster[clsIDList[i]-minClsId].AddToCluster(ph);
            }
        }
        
        free(clsIDList);
    }
    
    clsRange.clear();
    list<Phenotype *> cphList;
    
    for(itr=pList->begin(); itr!=pList->end(); itr++){
        Phenotype* ph = *itr;
        
        cphList.push_back(ph);
    }
    
    pList->clear();
    
    list<Phenotype *> phList;
    unordered_set<unsigned long>::iterator sitr;
    unordered_set<string> phListTrack;
    
    for(unsigned long i=minClsId; i<=maxClsId; i++){
        cluster[i-minClsId].GetPhenotypeListOfThisCluster(&phList);
        
        if(!isValidCluster(&phList, mainClsID)){
            for(itr=phList.begin(); itr!=phList.end(); itr++){
                Phenotype* ph = *itr;
                
                unsigned long *idList = (unsigned long*)malloc(sizeof(unsigned long)*Config::TOTAL_CLUSTER_IDS);
                ph->GetIDList(idList);
                
                for(short j=0; j<Config::TOTAL_CLUSTER_IDS; j++){
                    if(idList[j] == i){
                        ph->UpdateClusterID(j, 0);
                        ph->UpdateType(j, "");
                        ph->DecreaseClusterIndex();
                        
                        break;
                    }
                }
                
                free(idList);
            }
        }
        
        for(itr=phList.begin(); itr!=phList.end(); itr++){
            Phenotype* ph = *itr;
            
            if(phListTrack.find(ph->getIndividualId())==phListTrack.end()){
                phListTrack.insert(ph->getIndividualId());
                pList->push_back(ph);
            }
        }
        
        phList.clear();
        clsRange.clear();
    }
    
    //Cluster::SetClusterID(minClsId-1);
    
    delete []cluster;
}

void Mapper::assignClusterIdsForOverlapingRegion(float overlap){
    float *min, *max;
    list<Phenotype*> clPhList;
    list<DataPoint*> dpList;
    unsigned long boxID = 1, prevClsId, thisClsId;
    unordered_set<unsigned long> boxIdSet;
    unordered_set<unsigned long> mainClsIDSet;
    unordered_set<string> phListTrack;
    
    min = (float*)malloc(sizeof(float)*Config::FILTER);
    max = (float*)malloc(sizeof(float)*Config::FILTER);
    
    if(mesh!=NULL){
        for(int j=0; j<Config::WINDOW_Y;j++){
            for(int i=0; i<Config::WINDOW_X; i++){
                for(short f=0; f<Config::FILTER; f++){
                    float c = this->mesh[i][j].center[f];
                    float l = (this->gridLength[f]*(1+overlap))/2;
                    min[f] = c - l;
                    max[f] = c + l;
                    
                    if(min[f] < this->minPos[f]) min[f] = this->minPos[f];
                    if(max[f] > this->maxPos[f]) max[f] = this->maxPos[f];
                }
                
                clPhList.clear();
                dpList.clear();
                phListTrack.clear();
                
                if(Config::FILTER==1){
                    // Implementation of is eligible for clustering
                    boxIdSet.clear();
                    
                    for(list<DataPoint*>::iterator dItr=dataPointList.begin(); dItr!=dataPointList.end();dItr++){
                        DataPoint* dp = *dItr;
                        float x = dp->getPosition(0);
                        
                        if(min[0]<=x&&x<=max[0]){
                            boxIdSet.insert(dp->getBoxId());
                            
                            list<Phenotype*> phList = dp->getPhList();
                            
                            for(list<Phenotype*>::iterator pItr = phList.begin(); pItr!=phList.end(); pItr++){
                                Phenotype* ph = *pItr;
                                if(ph->getIndividualId().compare("A#N#63")==0){
                                    cout<<"";
                                }
                                if(phListTrack.find(ph->getIndividualId())==phListTrack.end()){
                                    phListTrack.insert(ph->getIndividualId());
                                    clPhList.push_back(ph);
                                }
                            }
                        }
                    }
                    
                    // At least one point belongs in this box
                    if(boxIdSet.size()==0 || boxIdSet.find(boxID)==boxIdSet.end()){
                        clPhList.clear();
                    }
                }else{
                    if(isEligibleForClustering(min, max, boxID)){
                        
                        this->treeRoot->SearchSurface(min[0], min[1], max[0], max[1], &clPhList, &dpList, boxID);
                    }
                }
                
                if(clPhList.size() > 0){
                    prevClsId = Cluster::GetClusterID();
                    
                    this->createCluster(&clPhList, false);
                    
                    thisClsId = Cluster::GetClusterID();
                    
                    if(thisClsId-prevClsId > 1){
                       unordered_map<unsigned long, Grid*>::iterator itrBRM = this->gridMap.find(boxID);
                        itrBRM->second->GetClusterID(&mainClsIDSet);
                        
                        this->manageCluster(&clPhList, prevClsId+1, thisClsId, &mainClsIDSet);
                        
                        //this->createCluster(&clPhList, false);
                    }
                }
                
                /*cout<<"\nx1="<<min[0]<<", x2="<<max[0]<<"=>";
                for(list<Phenotype*>::iterator itr=clPhList.begin(); itr!=clPhList.end(); itr++){
                    Phenotype* ph = *itr;
                    cout<<"\n"<<ph->getIndividualId()<<":"<<ph->GetClusterIdList();
                }
                cout<<"\n";*/
                
                boxID++;
                mainClsIDSet.clear();
                
            }
        }
    }
    
    free(min);
    free(max);
    
}

void Mapper::getIndividualsOfRegion(list<Phenotype *> *clPhList, float *min, float *max, long* clsRange){
    clPhList->clear();
    
    for(list<DataPoint*>::iterator dItr=dataPointList.begin(); dItr!=dataPointList.end();dItr++){
        DataPoint* dp = *dItr;
        float x = dp->getPosition(0);
        
        if(min[0]<=x&&x<=max[0]){
            list<Phenotype*> phList = dp->getPhList();
            
            for(list<Phenotype*>::iterator pItr = phList.begin(); pItr!=phList.end(); pItr++){
                Phenotype* ph = *pItr;
                
                unsigned long *idList = (unsigned long*)calloc(Config::TOTAL_CLUSTER_IDS, sizeof(unsigned long));
                ph->GetIDList(idList);
                
                short totalOL = 0;
                
                for(short i=0; i<Config::TOTAL_CLUSTER_IDS; i++){
                    if(idList[i] > 0) totalOL++;
                }
                
                free(idList);
                
                if(clsRange[1]<totalOL) clsRange[1]=totalOL;
                if(clsRange[0]>totalOL) clsRange[0]=totalOL;
                
                clPhList->push_back(ph);
            }
        }
    }
}

void Mapper::setOverlappedIndv(unordered_set<string> *indvSet, list<Phenotype *> *clPhList, list<Phenotype *> *overlappedPhList, unordered_set<string>* edgeSet){
    
    for(list<Phenotype *>::iterator itr=clPhList->begin(); itr!=clPhList->end(); itr++){
        Phenotype* ph = *itr;
        
        string key = ph->getGenotype() + "#" + ph->GetSite() + "#" + to_string(ph->getDAP());
        
        if(indvSet->find(key)==indvSet->end()){
            //cout<<"\n"<<key<<":"<<ph->GetClusterIdList();
            indvSet->insert(key);
            overlappedPhList->push_back(ph);
            
            unsigned long *idList = (unsigned long*)malloc(sizeof(unsigned long)*Config::TOTAL_CLUSTER_IDS);
            ph->GetIDList(idList);
            vector<long> idVector;
            
            for(int i=0;i<Config::TOTAL_CLUSTER_IDS;i++){
                if(idList[i]>0){
                    idVector.push_back(idList[i]);
                }
            }
            free(idList);
            
            for(int i=0;i<idVector.size(); i++){
                for(int j=i+1; j<idVector.size(); j++){
                    
                    string k = (idVector[i]<idVector[j])?(to_string(idVector[i]) + "#" + to_string(idVector[j])):(to_string(idVector[j]) + "#" + to_string(idVector[i]));
                    
                    if(edgeSet->find(k)==edgeSet->end()){
                        edgeSet->insert(k);
                        AnEdge* e = new AnEdge(idVector[i],idVector[j]);
                        this->edgeList.push_back(e);
                    }
                }
            }
            
        }
    }
    
}
void Mapper::generateEdgeListFromOverlappingClusters(list<Phenotype*>* overlappedPhList, float overlap){
    
    float* min, *max;
    list<Phenotype*> clPhList;
    list<DataPoint*> dpList;
    unordered_set<string> indvSet;
    unordered_set<string> edgeSet;
    long boxID = 1;
    long clsRange[]={Config::TOTAL_CLUSTER_IDS+4, 0};
    
    min = (float*)malloc(sizeof(float)*Config::FILTER);
    max = (float*)malloc(sizeof(float)*Config::FILTER);
    
    if(Config::FILTER==1){
        for(int i=1; i<Config::WINDOW_X; i++){
            float c2 = this->mesh[i][0].center[0];
            float c1 = this->mesh[i-1][0].center[0];
            
            float l = (this->gridLength[0]*(1+overlap))/2;
            min[0] = c2 - l;
            max[0] = c1 + l;
            
            this->getIndividualsOfRegion(&clPhList, min, max, clsRange);
            
            setOverlappedIndv(&indvSet, &clPhList, overlappedPhList, &edgeSet);
        }
    }else{
        for(int j=0; j<Config::WINDOW_Y;j++){
            for(int i=0; i<Config::WINDOW_X; i++){
                
                clPhList.clear();
                dpList.clear();
                
                if(i>0){
                    for(short f=0; f<Config::FILTER; f++){
                        float c2 = this->mesh[i][j].center[f];
                        float c1 = this->mesh[i-1][j].center[f];
                        
                        float l = (this->gridLength[f]*(1+overlap))/2;
                        min[f] = c2 - l;
                        max[f] = c1 + l;
                        
                        if(min[f] < this->minPos[f]) min[f] = this->minPos[f];
                        if(max[f] > this->maxPos[f]) max[f] = this->maxPos[f];
                    }
                    
                    this->treeRoot->SearchSurface(min[0], min[1], max[0], max[1], &clPhList, &dpList, boxID);
                }
                
                if(j>0){
                    for(short f=0; f<Config::FILTER; f++){
                        float c2 = this->mesh[i][j].center[f];
                        float c1 = this->mesh[i][j-1].center[f];
                        
                        float l = (this->gridLength[f]*(1+overlap))/2;
                        min[f] = c2 - l;
                        max[f] = c1 + l;
                        
                        if(min[f] < this->minPos[f]) min[f] = this->minPos[f];
                        if(max[f] > this->maxPos[f]) max[f] = this->maxPos[f];
                    }
                    
                    this->treeRoot->SearchSurface(min[0], min[1], max[0], max[1], &clPhList, &dpList, boxID);
                }
                
                boxID++;
                
                setOverlappedIndv(&indvSet, &clPhList, overlappedPhList, &edgeSet);
            }
        }
    }
    
    free(min);
    free(max);
}

void Mapper::addSimplicialComplex(SimplicialComplex *sc, float overlap){
    float* min, *max;
    
    // Make it 50%
    overlap *=50;
    
    min = (float*)malloc(sizeof(float)*Config::FILTER);
    max = (float*)malloc(sizeof(float)*Config::FILTER);
    
    if(mesh!=NULL){
        sc->clearTracker();
        
        if(Config::FILTER==1){
            for(int i=0; i<Config::WINDOW_X; i++){
                float c = this->mesh[i][0].center[0];
                
                float l = this->gridLength[0]/2;
                min[0] = c - l;
                max[0] = c + l;
                
                if(min[0] < this->minPos[0]) min[0] = this->minPos[0];
                if(max[0] > this->maxPos[0]) max[0] = this->maxPos[0];
                
                for(list<DataPoint*>::iterator dItr=dataPointList.begin(); dItr!=dataPointList.end();dItr++){
                    DataPoint* dp = *dItr;
                    float x = dp->getPosition(0);
                    
                    if(min[0]<=x&&x<=max[0]){
                        list<Phenotype*> phList = dp->getPhList();
                        
                        for(list<Phenotype*>::iterator pItr = phList.begin(); pItr!=phList.end(); pItr++){
                            Phenotype* ph = *pItr;
                            
                            sc->AddSimplicialComplex(ph, overlap);
                        }
                    }
                }
            }
        }else{
            list<Phenotype*> clPhList;
            list<DataPoint*> dpList;
            long boxID = 1;
            
            for(int j=0; j<Config::WINDOW_Y;j++){
                for(int i=0; i<Config::WINDOW_X; i++){
                    
                    clPhList.clear();
                    dpList.clear();
                    
                    for(short f=0; f<Config::FILTER; f++){
                        float c = this->mesh[i][j].center[f];
                        float l = this->gridLength[f]/2;
                        min[f] = c - l;
                        max[f] = c + l;
                        
                        if(min[f] < this->minPos[f]) min[f] = this->minPos[f];
                        if(max[f] > this->maxPos[f]) max[f] = this->maxPos[f];
                    }
                
                    this->treeRoot->SearchSurface(min[0], min[1], max[0], max[1], &clPhList, &dpList, boxID);
                    
                    for(list<Phenotype*>::iterator pItr = clPhList.begin(); pItr!=clPhList.end(); pItr++){
                        Phenotype* ph = *pItr;
                        
                        sc->AddSimplicialComplex(ph, overlap);
                    }
                    
                    boxID++;
                }
            }
        }
    }
    
    free(min);
    free(max);
}

void Mapper::generateSimplicesForPersistentHomology(){
    SimplicialComplex sc;
    
    for(float overLap=0.00; overLap<=1.00; overLap+=0.05){
        this->clearClusterInformation();
        this->assignClusterIdsForOverlapingRegion(overLap);
        
        this->addSimplicialComplex(&sc, overLap);
        
        //cout<<"\nFiltration:"<<overLap*50<<"\n"<<sc.PrintSimplex()<<endl;
    }
    
    if(Config::PH_JAVA_PLEX)
        cout<<"\nDump the following codes to Java file to generate barcode.\n\n/************ Start from here *************/\n"<<sc.PrintSimplex()<<"/************ End here *************/\n\n";
    else
        cout<<"\n Persistent overlap: "<<sc.getPersistentOverlap()<<"\n\n";
}

void Mapper::computeConnectedComponents(){
    UnionFindWithPathCompression ufpc(Cluster::GetClusterID());
    
    for(list<AnEdge *>::iterator itr = this->edgeList.begin(); itr!=this->edgeList.end(); itr++){
        
        AnEdge* e = *itr;
        
        long node_1 = e->getNode1();
        long node_2 = e->getNode2();
        //cout<<"\n"<<node_1<<","<<node_2;
        ufpc.Union(node_1, node_2);
    }
    
    this->connectedComponents = ufpc.GetAllConnectedComponents();
}

void Mapper::constructGraphComponents(){
    short size = this->connectedComponents.size();
    float* min, *max;
    
    this->graph = new Graph(size);
    
    if(Config::FILTER==1){
        for(list<DataPoint*>::iterator dItr=dataPointList.begin(); dItr!=dataPointList.end();dItr++){
            
            list<Phenotype*> phList = (*dItr)->getPhList();
            
            for(list<Phenotype*>::iterator pItr = phList.begin(); pItr!=phList.end(); pItr++){
                Phenotype* ph = *pItr;
                
                unsigned long *idList = (unsigned long*)malloc(sizeof(unsigned long)*Config::TOTAL_CLUSTER_IDS);
                ph->GetIDList(idList);
                
                for(short i=0;i<Config::TOTAL_CLUSTER_IDS;i++){
                    if(idList[i]>0){
                        short index=0;
                        for(unordered_map<long, unordered_set<long> >::iterator itr=this->connectedComponents.begin(); itr!=this->connectedComponents.end(); itr++){
                            unordered_set<long> nodeSet = itr->second;
                            
                            if(nodeSet.find(idList[i])!=nodeSet.end()){
                                IsolatedSubGraph* subGraph = this->graph->getConnectedComponent(index);
                                subGraph->addPointToNode(idList[i], ph);
                                break;
                            }
                            
                            index++;
                        }
                    }
                }
                free(idList);
            }
        }
    }else{
        min = (float*)malloc(sizeof(float)*Config::FILTER);
        max = (float*)malloc(sizeof(float)*Config::FILTER);
        
        if(mesh!=NULL){
            list<Phenotype*> clPhList;
            list<DataPoint*> dpList;
            long boxID = 1;
            
            for(int j=0; j<Config::WINDOW_Y;j++){
                for(int i=0; i<Config::WINDOW_X; i++){
                    
                    clPhList.clear();
                    dpList.clear();
                    
                    for(short f=0; f<Config::FILTER; f++){
                        float c = this->mesh[i][j].center[f];
                        float l = this->gridLength[f]/2;
                        min[f] = c - l;
                        max[f] = c + l;
                        
                        if(min[f] < this->minPos[f]) min[f] = this->minPos[f];
                        if(max[f] > this->maxPos[f]) max[f] = this->maxPos[f];
                    }
                    
                    this->treeRoot->SearchSurface(min[0], min[1], max[0], max[1], &clPhList, &dpList, boxID);
                    
                    for(list<Phenotype*>::iterator pItr = clPhList.begin(); pItr!=clPhList.end(); pItr++){
                        Phenotype* ph = *pItr;
                        
                        unsigned long *idList = (unsigned long*)malloc(sizeof(unsigned long)*Config::TOTAL_CLUSTER_IDS);
                        ph->GetIDList(idList);
                        
                        for(short i=0;i<Config::TOTAL_CLUSTER_IDS;i++){
                            if(idList[i]>0){
                                short index=0;
                                for(unordered_map<long, unordered_set<long> >::iterator itr=this->connectedComponents.begin(); itr!=this->connectedComponents.end(); itr++){
                                    unordered_set<long> nodeSet = itr->second;
                                    
                                    if(nodeSet.find(idList[i])!=nodeSet.end()){
                                        IsolatedSubGraph* subGraph = this->graph->getConnectedComponent(index);
                                        subGraph->addPointToNode(idList[i], ph);
                                        break;
                                    }
                                    
                                    index++;
                                }
                            }
                        }
                        free(idList);
                    }
                    
                    boxID++;
                }
            }
        }
        
        free(min);
        free(max);
    }
    
    for(list<AnEdge *>::iterator itr = this->edgeList.begin(); itr!=this->edgeList.end(); itr++){
        
        AnEdge* e = *itr;
        
        unsigned long node_1 = e->getNode1();
        unsigned long node_2 = e->getNode2();
        
        for(short i=0; i<size; i++){
            IsolatedSubGraph* subGraph = this->graph->getConnectedComponent(i);
            
            if(subGraph->hasNode(node_1) && subGraph->hasNode(node_2)){
                subGraph->addEdge(node_1, node_2);
            }
        }
        
    }
    
    float sizeRange[2]={0.0}, **weightRange=(float**)malloc(sizeof(float*)*(Config::FILTER+1));
    for(short i=0;i<=Config::FILTER;i++){
        weightRange[i] = (float*)malloc(sizeof(float)*2);
        weightRange[i][0] = 0.0;
        weightRange[i][1] = 0.0;
    }
    
    for(short i=0; i<size; i++){
        IsolatedSubGraph* subGraph = this->graph->getConnectedComponent(i);
    
        subGraph->setupLinks();
        subGraph->getNodeSizeAndWeightRange(sizeRange, weightRange);
    }
    
    this->graph->setGlobalNodeSizeAndWeightRange(sizeRange, weightRange);
    
    for(short i=0; i<size; i++){
        IsolatedSubGraph* subGraph = this->graph->getConnectedComponent(i);
        
        for(int i=0;i<2;i++) subGraph->setGlobalSizeRange(sizeRange[i], i);
        for(short i=0;i<=Config::FILTER;i++) subGraph->setGlobalWeightRange(weightRange[i], i);
    }
    
    for(short i=0;i<=Config::FILTER;i++){
        free(weightRange[i]);
    }
    free(weightRange);
}

void Mapper::constructGraph(){
    list<Phenotype*> overlappedIndiv;
    
    this->clearClusterInformation();
    this->assignClusterIdsForOverlapingRegion(Config::OVERLAP);
    
    overlappedIndiv.clear();
    edgeList.clear();
    
    this->generateEdgeListFromOverlappingClusters(&overlappedIndiv, Config::OVERLAP);
    this->computeConnectedComponents();
    this->constructGraphComponents();
}

void Mapper::visualization(){
    this->constructGraph();
    if(Config::FILTER>1)this->graph->generateInterestingPaths();
    this->graph->printD3Graph();
}
