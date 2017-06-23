
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: ClusteringRegion.cpp
 Objective: Implementation of member functions
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              03/02/2016      Initial version
 
 **************************************************************************************************/

#include "ClusteringRegion.h"
#include <iostream>
#include <time.h>
#include <set>

ClusteringRegion::ClusteringRegion(list<DataPoint*>* dataPointList, float xMin, float yMin, float xMax, float yMax, float wSizeX, float wSizeY, float overlapX, float overlapY){
    this->dataPointList = dataPointList;
    this->xMin = xMin;
    this->xMax = xMax;
    this->yMin = yMin;
    this->yMax = yMax;
    this->windowSizeX = wSizeX;
    this->windowSizeY = wSizeY;
    this->overlapX = overlapX;
    this->overlapY = overlapY;
    this->Eps = CLUSTER_RADIUS;
    this->minPoints = CLUSTER_DENSITY;
    this->isVarietyMapHasCreated = false;
    this->isDp = HAMMING_DISTANCE;
    
    //cout<<"\nEPS:"<<Eps<<"\nMinpoints:"<<minPoints<<endl;
}

ClusteringRegion::~ClusteringRegion(){
    delete xPositions;
    delete yPositions;
}

void ClusteringRegion::SearchAndInitializeRectangle(){

}

void ClusteringRegion::SearchAndInitializeRectangle(Grid **mesh){
    float x1, x2;
    list<Phenotype*> clPhList;
    long boxID = 1, lastID, thisID;
    string s = "";
    
    if(mesh!=NULL){
        for(int i=0; i<this->totalSplitX; i++){
            float cx = mesh[i][0].center_x;
            
            float lx = this->windowSizeX/2;
            x1 = cx - lx;
            x2 = cx + lx;
            
            if(x1 < this->xMin) x1 = this->xMin;
            if(x2 > this->xMax) x2 = this->xMax;
            
            clPhList.clear();
            
            for(list<DataPoint*>::iterator dItr=dataPointList->begin(); dItr!=dataPointList->end();dItr++){
                DataPoint* dp = *dItr;
                float x = dp->getPosition(0);
                
                if(x1<=x&&x<=x2){
                    dp->setBoxId(boxID);
                    
                    list<Phenotype*> phList = dp->getPhList();
                    
                    for(list<Phenotype*>::iterator pItr = phList.begin(); pItr!=phList.end(); pItr++){
                        Phenotype* ph = *pItr;
                        clPhList.push_back(ph);
                    }
                }
            }
            
            if(clPhList.size() > 0){
                
                lastID = Cluster::GetClusterID();
                
                this->CreateCluster(&clPhList, true);
                
                thisID = Cluster::GetClusterID();
                
                BoxRectangle br;
                br.id = boxID;
                
                if(thisID-lastID>1){
                    for(long i=lastID+1; i<=thisID; i++){
                        br.clusterIdList.insert(i);
                    }
                }else{
                    br.clusterIdList.insert(thisID);
                }
                
                this->boxRectangleMap.insert(pair<long, BoxRectangle>(boxID, br));
                
                s += "BoxID:" + to_string(boxID) + "(" + to_string(x1) + "," + to_string(x2) + ")=>" + to_string(Cluster::GetClusterID()) + "\n";
                
            }
            
            boxID++;
            
        }
    }
    
    totalClusters = Cluster::GetClusterID();
    
    //cout<<s<<"\nTotal cluster IDs:"<<totalClusters<<endl;
    
}

void ClusteringRegion::SearchRectangle(){
    
}

void ClusteringRegion::SearchRectangle(Grid **mesh){
    float x1, x2;
    list<Phenotype*> clPhList;
    list<DataPoint*> dpList;
    long boxID = 1, prevClsId, thisClsId;
    set<long> boxIdSet;
    set<long> mainClsIDSet;
    unordered_set<string> phListTrack;
    
    if(mesh!=NULL){
        
        for(int i=0; i<this->totalSplitX; i++){
            float cx = mesh[i][0].center_x;
            
            float lx = (this->windowSizeX*(1+this->overlapX))/2;
            x1 = cx - lx;
            x2 = cx + lx;
            
            if(x1 < this->xMin) x1 = this->xMin;
            if(x2 > this->xMax) x2 = this->xMax;
            
            
            clPhList.clear();
            
            // Implementation of is eligible for clustering
            dpList.clear();
            boxIdSet.clear();
            
            for(list<DataPoint*>::iterator dItr=dataPointList->begin(); dItr!=dataPointList->end();dItr++){
                DataPoint* dp = *dItr;
                float x = dp->getPosition(0);
                
                if(x1<=x&&x<=x2){
                    boxIdSet.insert(dp->getBoxId());
                    
                    list<Phenotype*> phList = dp->getPhList();
                    
                    for(list<Phenotype*>::iterator pItr = phList.begin(); pItr!=phList.end(); pItr++){
                        Phenotype* ph = *pItr;
                        clPhList.push_back(ph);
                    }
                }
            }
            
            // At least one point belongs in this box
            if(boxIdSet.size()==0 || boxIdSet.find(boxID)==boxIdSet.end()){
                clPhList.clear();
            }
            
            if(clPhList.size() > 0){
                
                /*for(list<Phenotype*>::iterator ppitr=clPhList.begin(); ppitr!=clPhList.end(); ppitr++){
                 Phenotype* p = *ppitr;
                 
                 if(p->GetValue() > 9.93){
                 cout<<"";
                 }
                 }*/
                
                //cout<<"\nRect"<<"("<<x1<<","<<y1<<","<<x2<<","<<y2<<")\n";
                
                
                prevClsId = Cluster::GetClusterID();
                
                
                this->CreateCluster(&clPhList, false);
                
                thisClsId = Cluster::GetClusterID();
                
                if(thisClsId-prevClsId > 1){
                    map<long, BoxRectangle>::iterator itrBRM = this->boxRectangleMap.find(boxID);
                    itrBRM->second.GetClusterID(&mainClsIDSet);
                    
                    this->ManageCluster(&clPhList, prevClsId+1, thisClsId, &mainClsIDSet);
                    
                    this->CreateCluster(&clPhList, false);
                }
                
                /*for(list<Phenotype*>::iterator citr=clPhList.begin(); citr!=clPhList.end(); citr++){
                    Phenotype* p = *citr;
                    long idL[TOTAL_CLUSTER_IDS]={0};
                    p->GetIDList(idL);
                    string s="";
                    for(int i=0; i<TOTAL_CLUSTER_IDS; i++){
                        if(idL[i]>0){
                            if(s.length()>0) s+=",";
                            
                            s += to_string(idL[i]);
                        }
                    }
                    
                    if(s.length()>0)
                        cout<<prevClsId<<":["<<p->getIndividualId()<<"]=>"<<s<<"\n";
                }*/
                
                //cout<<"BoxID:"<<boxID<<"("<<x1<<","<<x2<<")=>"<<to_string(Cluster::GetClusterID())<<"\n";
            }
            
            
            boxID++;
            mainClsIDSet.clear();
            
        }
    }
    
    totalClusters = Cluster::GetClusterID();
    
    //cout<<"\nTotal cluster IDs:"<<totalClusters<<endl;
}

void ClusteringRegion::ClearCluster(){
    
    list<DataPoint*> *dpList=dataPointList;
    
    for(list<DataPoint*>::iterator ditr = dpList->begin(); ditr != dpList->end(); ditr++){
        DataPoint* dp = *ditr;
        list<Phenotype*> clPhList = dp->getPhList();
        
        dp->ResetClusterInformation();
        
        for(list<Phenotype*>::iterator itr = clPhList.begin(); itr != clPhList.end(); itr++){
            Phenotype* ph = *itr;
            
            ph->ResetClusterInformation();
        }
    }
    
    this->graph.node.nodeList.clear();
    this->graph.node.nodeListDP.clear();
    this->graph.edge.edgeList.clear();
    this->graph.edgeList.clear();
    this->gmlGraph.gmlEdgeList.clear();
    this->gmlGraph.gmlNodeList.clear();
    this->tries.clear();
    this->tetras.clear();
    this->ctries.clear();
    this->ctetras.clear();
    
    Cluster::SetClusterID(0);
    
}

// Depricated
void ClusteringRegion::SearchRegion(){
    
    
}

string ClusteringRegion::GetAllClusteredPhenotype(){
    list<Phenotype*>::iterator itr;
    string str = "";
    
    for(itr = this->clusterPhList.begin(); itr != this->clusterPhList.end(); itr++){
        Phenotype* ph = *itr;
        
        str += ph->toString() + "\n";
    }
    
    return str;
}

string ClusteringRegion::GetAllPhenotypes(){
    
    return "";
}

void ClusteringRegion::GenerateGraphUsingOverlappingClusters(){
    
    
}

void ClusteringRegion::getIndividualsOfRegion(list<Phenotype *> *clPhList, float &x1, float &x2, float &min, float &max){
    clPhList->clear();
    
    for(list<DataPoint*>::iterator dItr=dataPointList->begin(); dItr!=dataPointList->end();dItr++){
        DataPoint* dp = *dItr;
        float x = dp->getPosition(0);
        
        if(x1<=x&&x<=x2){
            list<Phenotype*> phList = dp->getPhList();
            
            for(list<Phenotype*>::iterator pItr = phList.begin(); pItr!=phList.end(); pItr++){
                Phenotype* ph = *pItr;
                
                long idList[TOTAL_CLUSTER_IDS]={0};
                ph->GetIDList(idList);
                
                short totalOL = 0;
                
                for(short i=0; i<TOTAL_CLUSTER_IDS; i++){
                    if(idList[i] > 0) totalOL++;
                }
                
                if(max<totalOL) max=totalOL;
                if(min>totalOL) min=totalOL;
                
                clPhList->push_back(ph);
            }
        }
    }
}

void ClusteringRegion::setOverlappedIndv(unordered_set<string> *indvSet, list<Phenotype *> *clPhList, list<Phenotype *> *overlappedPhList, list<AnEdge*>* edgeList, unordered_set<string>* edgeSet){
    
    for(list<Phenotype *>::iterator itr=clPhList->begin(); itr!=clPhList->end(); itr++){
        Phenotype* ph = *itr;
        
        string key = ph->getGenotype() + "#" + ph->GetSite() + "#" + to_string(ph->getDAP());
        
        if(indvSet->find(key)==indvSet->end()){
            indvSet->insert(key);
            overlappedPhList->push_back(ph);
            
            long idList[TOTAL_CLUSTER_IDS];
            ph->GetIDList(idList);
            vector<long> idVector;
            
            for(int i=0;i<TOTAL_CLUSTER_IDS;i++){
                if(idList[i]>0){
                    idVector.push_back(idList[i]);
                }
            }
            
            for(int i=0;i<idVector.size(); i++){
                for(int j=i+1; j<idVector.size(); j++){
                    
                    string k = (idVector[i]<idVector[j])?(to_string(idVector[i]) + "#" + to_string(idVector[j])):(to_string(idVector[j]) + "#" + to_string(idVector[i]));
                    
                    if(edgeSet->find(k)==edgeSet->end()){
                        edgeSet->insert(k);
                        AnEdge* e = new AnEdge(idVector[i],idVector[j]);
                        edgeList->push_back(e);
                    }
                }
            }
            
        }
    }

}

void ClusteringRegion::GenerateGraphUsingOverlappingClusters(Grid **mesh, list<Phenotype*>* overlappedPhList, list<AnEdge*>* edgeList){
    string str;
    float x1, x2;
    list<Phenotype*> clPhList;
    float max=0,min=6;
    unordered_set<string> indvSet;
    unordered_set<string> edgeSet;
    
    for(int i=1; i<totalSplitX; i++){
        float cx2 = mesh[i][0].center_x;
        float cx1 = mesh[i-1][0].center_x;
        
        float lx = (this->windowSizeX*(1+this->overlapX))/2;
        x1 = cx2 - lx;
        x2 = cx1 + lx;
        
        getIndividualsOfRegion(&clPhList, x1, x2, min, max);
        
        setOverlappedIndv(&indvSet, &clPhList, overlappedPhList, edgeList, &edgeSet);
        
        /*cout<<"\n("<<x1<<","<<x2<<")=>";
        for(list<AnEdge*>::iterator eitr=edgeList->begin(); eitr!=edgeList->end(); eitr++){
            AnEdge* e = *eitr;
            cout<<"("<<e->getNode1()<<"->"<<e->getNode2()<<")";
        }*/
    }
    //cout<<"\n";
}

string ClusteringRegion::PrintGraph(list<Phenotype *> *overlappedPhList, list<AnEdge *> *edgeList){

    
    string envName[]=ENV_NAMES;
    string fileName_suffix = to_string(this->overlapX).substr(0,4) + "_" + "R" + to_string(this->Eps).substr(0,4) + "_" + SUFFIX;
    
    
    // Print isolated clusters
    /*long totalClusters = Cluster::GetClusterID();
    for(long nnid=1; nnid <= totalClusters; nnid++){
        sc->AddZeroSimplex(nnid, 0.00);
    }*/
    
    UnionFindWithPathCompression ufpc(Cluster::GetClusterID());
    
    for(list<AnEdge *>::iterator itr = edgeList->begin(); itr!=edgeList->end(); itr++){
        
        AnEdge* e = *itr;
        
        long node_1 = e->getNode1();
        long node_2 = e->getNode2();
        
        ufpc.Union(node_1, node_2);
    }
    
    unordered_map<long, list<long> > scc = ufpc.GetAllConnectedComponents();
    //long maxSize = 0;
    list<long> maxCC;
    
    /*cout<<"Connected components are:\n";
    for(unordered_map<long, list<long> >::iterator itr = scc.begin(); itr!=scc.end(); itr++){
        long index = itr->first;
        list<long> lst = itr->second;
        
        if(lst.size() > maxSize){
            maxSize = lst.size();
            maxCC = lst;
        }
        
        cout<<index<<":"<<lst.size()<<" ";
        
        for(list<long>::iterator litr = lst.begin(); litr!=lst.end(); litr++){
            long ide = *litr;
            
            cout<<ide<<",";
        }
        
        cout<<"\n\n";
    }*/
    
    unordered_map<long, unordered_map<string, Phenotype*> > nodeList;
    unordered_map<long, unordered_map<string, Phenotype*> >::iterator nodeItr;
    
    for(list<Phenotype *>::iterator itr=overlappedPhList->begin(); itr!=overlappedPhList->end(); itr++){
        Phenotype* ph = *itr;
        
        long idList[TOTAL_CLUSTER_IDS]={0};
        ph->GetIDList(idList);
        
        string key = ph->getIndividualId();
        
        for(short i=0; i<TOTAL_CLUSTER_IDS; i++){
            if(idList[i]>0){
                long id = idList[i];
                
                if((nodeItr=nodeList.find(id)) == nodeList.end()){
                    
                    unordered_map<string, Phenotype*> phMap;
                    phMap.insert(pair<string, Phenotype*>(key,ph));
                    
                    nodeList.insert(pair<long, unordered_map<string, Phenotype*> >(id, phMap));
                }else{
                    unordered_map<string, Phenotype*>::iterator phItr = nodeItr->second.find(key);
                    
                    if(phItr==nodeItr->second.end()){
                        nodeItr->second.insert(pair<string, Phenotype*>(key,ph));
                    }
                }
                
            }
        }
    }
    
    int total_CC = (int)scc.size();
    MainGraph* mainGraph = new MainGraph(total_CC);
    mainGraph->setEnvName(envName);
    int ccIndex = 0;
    
    for(unordered_map<long, list<long> >::iterator itr = scc.begin(); itr!=scc.end(); itr++){
        
        CompositeGraph* rGraph =  mainGraph->getCC(ccIndex);
        ccIndex++;
        
        //if(maxCC.size()>0){
        list<long> lstNode = itr->second;//maxCC;
        
        float range[FILTER+2][2]={{0.0}};
        bool first = true;
        bool mean_first = true;
        
        for(list<long>::iterator nItr = lstNode.begin(); nItr!=lstNode.end(); nItr++){
            long nodeID = *nItr;
            
            // Create node in a CC graph
            rNode *rn = new rNode(nodeID, 0.0, 0.0);
            
            float size = 0.0;
            float sum[FILTER+1]={0.0};
            float meanValues[FILTER+1]={0.0};
            
            unordered_map<long, unordered_map<string, Phenotype*> >::iterator nodeItr = nodeList.find(nodeID);
            
            if(nodeItr!=nodeList.end()){
                for(unordered_map<string, Phenotype*>::iterator phItr=nodeItr->second.begin(); phItr!=nodeItr->second.end(); phItr++){
                    
                    Phenotype* ph = phItr->second;
                    
                    if(PLANT_ID_LIST){
                        list<long> pidList = ph->getPlantIdList();
                        
                        for(list<long>::iterator piditr = pidList.begin(); piditr!=pidList.end(); piditr++){
                            rn->setPhenotypeSummary(ph->getGenotype(), ph->GetSite(), *piditr, ph->getDAP(), ph->getCoOrdinate(0), ph->GetValue());
                        }
                    }else{
                        rn->setPhenotypeSummary(ph->getGenotype(), ph->GetSite(), ph->GetVariety(), ph->getDAP(), ph->getCoOrdinate(0), ph->GetValue());
                    }
                    
                    size += ph->GetWeight(nodeID);
                    sum[0] += ph->GetValue();
                    
                    for(short e=0;e<FILTER;e++){
                        sum[e+1] += ph->getCoOrdinate(e);
                    }
                }
            }
            
            if(first){
                first=false;
                
                // Phenotype: min, max
                range[0][0] = size;
                range[0][1] = size;
                
            }else{
                // Phenotype: min, max
                if(range[0][0]>size) range[0][0]=size;
                if(range[0][1]<size) range[0][1]=size;
                
            }
            
            for(short e=0; e<=FILTER;e++){
                meanValues[e] = sum[e]/nodeItr->second.size();
                
                if(e==0){
                    if(mean_first){
                        mean_first = false;
                        
                        range[FILTER+1][0]=meanValues[e];
                        range[FILTER+1][1]=meanValues[e];
                    }else{
                        if(range[FILTER+1][0]>meanValues[e]) range[FILTER+1][0]=meanValues[e];
                        if(range[FILTER+1][1]<meanValues[e]) range[FILTER+1][1]=meanValues[e];
                    }
                }
                
            }
            
            rn->setSize(size);
            rn->setWeight(meanValues[0]);
            
            for(int e=0;e<FILTER;e++){
                rn->setEnvWeight(meanValues[e+1], e);
            }
            rn->setVariations(nodeItr->second.size());
            
            rGraph->addNode(rn);
        }
        
        /*cout<<"\n\nRanges:";
        for(short e=0;e<FILTER;e++){
            cout<<"\n\t"<<envName[0]<<":(" + to_string(range[e+1][0]) + "," + to_string(range[e+1][1]) + ")";
        }
        cout<<"\n\tGR:(" + to_string(range[0][0]) + "," + to_string(range[0][1]) + ")";
        cout<<"\n\tAvg.GR:(" + to_string(range[FILTER+1][0]) + "," + to_string(range[FILTER+1][1]) + ")\n";*/
        
        float nw[] = {range[FILTER+1][0],range[FILTER+1][1]};
        float ns[] = {range[0][0],range[0][1]};
        float ew[FILTER][2]={{0}};
        for(int e=0;e<FILTER;e++){
            ew[e][0] = range[e+1][0];
            ew[e][1]=range[e+1][1];
        }
        rGraph->storeRange(ns, nw, ew, envName);
    }
    
    for(list<AnEdge *>::iterator itr = edgeList->begin(); itr!=edgeList->end(); itr++){
        
        AnEdge* e = *itr;
        
        long node_1 = e->getNode1();
        long node_2 = e->getNode2();
        
        //sc->AddOneSimplex(node_1, node_2, this->overlapX*50);
        
        for(int i=0; i<total_CC; i++){
            CompositeGraph* rGraph = mainGraph->getCC(i);
            
            if(rGraph->hasNode(node_1, node_2)){
                rGraph->addEdge(node_1, node_2);
            }
        }
        
    }
    
    if(!PRINT_BARCODE)
        mainGraph->printAllGraphs(fileName_suffix);
    
    if(!PRINT_BARCODE&&false)
        menu(mainGraph, &this->graph, fileName_suffix, total_CC);
    
    delete mainGraph;
    
    return "";
}

void ClusteringRegion::AddSimplicialComplex(SimplicialComplex *sc, Grid **mesh, float overlap){
    float x1, x2;
    overlap = overlap*50;
    
    if(mesh!=NULL){
        sc->clearTracker();
        
        for(int i=0; i<this->totalSplitX; i++){
            float cx = mesh[i][0].center_x;
            
            float lx = this->windowSizeX/2;
            x1 = cx - lx;
            x2 = cx + lx;
            
            if(x1 < this->xMin) x1 = this->xMin;
            if(x2 > this->xMax) x2 = this->xMax;
            
            for(list<DataPoint*>::iterator dItr=dataPointList->begin(); dItr!=dataPointList->end();dItr++){
                DataPoint* dp = *dItr;
                float x = dp->getPosition(0);
                
                if(x1<=x&&x<=x2){
                    list<Phenotype*> phList = dp->getPhList();
                    
                    for(list<Phenotype*>::iterator pItr = phList.begin(); pItr!=phList.end(); pItr++){
                        Phenotype* ph = *pItr;
                        
                        sc->AddSimplicialComplex(ph, overlap);
                    }
                }
            }
            
            
        }
    }
}

string ClusteringRegion::PrintGraph(SimplicialComplex* sc){
    
    return "";
}

void ClusteringRegion::menu(MainGraph* mGraph, Graph *graph, string suffix, int totalCC){
    short choice=0;
    
    do{
        printf("Please select one:\n\t1.Print statistics of all paths.\n\t2. Print statistics of list of nodes.\n\t3. Print minimize path statistics.\n\t4. Exit.\nEnter your choice: ");
        scanf("%hd",&choice);
        
        switch (choice) {
            case 1:
                
                for(int i=0;i<totalCC;i++){
                    CompositeGraph* rGraph = mGraph->getCC(i);
                    rGraph->analyzePaths(1, 2, true, graph, suffix+"_CC_"+to_string(i+1));
                }
                
                break;
            case 2:
                
                mGraph->PrintAllNodeStatistics(suffix);
                
                break;
            case 3:
                
                for(int i=0;i<totalCC;i++){
                    CompositeGraph* rGraph = mGraph->getCC(i);
                    rGraph->analyzeAndSummarizePaths(suffix+"_CC_"+to_string(i+1));
                }
                
                break;
            default:
                break;
        }
    }while(choice!=4);
}

string ClusteringRegion::PrintGraphForDataPoint(SimplicialComplex* sc){
    
    return "";
}

string ClusteringRegion::GetSimplexInformation(){
    string str = "";
    map<long, string>::iterator mitr;
    map<string, long>::iterator cmitr;
    
    if(this->tries.size() > 0){
        str += "Tries:" + to_string(this->tries.size()) + "\n";
        for(mitr = this->tries.begin(); mitr != this->tries.end(); mitr++){
            string val = mitr->second;
            cmitr = this->ctries.find(val);
            
            
            str += "(" + val + ")=" + to_string(cmitr->second) + ", ";
        }
        
        str += "\n";
    }
    
    if(this->tetras.size() > 0){
        str += "Tetras:" + to_string(this->tetras.size()) + "\n";
        for(mitr = this->tetras.begin(); mitr != this->tetras.end(); mitr++){
            string val = mitr->second;
            cmitr = this->ctetras.find(val);
            
            str += "(" + val + ")=" + to_string(cmitr->second) + ", ";
        }
        
        str += "\n";
    }
    
    return str;
}

void ClusteringRegion::ShuffleClusters(){
    
}

//#pragma Private_Methods

void ClusteringRegion::SplitRegion(){
    float x1 = this->xMin, y1=this->yMin;
    
    this->xPositions[0] = x1;
    this->yPositions[0] = y1;
    
    this->xPositions[this->maxBlockX-1] = this->xMax;
    this->yPositions[this->maxBlockY-1] = this->yMax;
    
    for(int i=1; i<this->maxBlockX-1; i+=2){
        if(i-2>=0){
            x1 = this->xPositions[i-2];
        }else{
            x1 = this->xPositions[0];
        }
        
        this->xPositions[i] = x1 + (this->windowSizeX*(1-this->overlapX));
        this->xPositions[i+1] = x1 + this->windowSizeX;
        
        
        /*if((i+1)%2 == 0){
            this->xPositions[i] = x1 + (this->windowSizeX*(1-this->overlapX));
        }else{
            this->xPositions[i] = x1 + this->windowSizeX;
        }*/
    }
    
    for(int i=1; i<this->maxBlockY-1; i+=2){
        if(i-2>=0){
            y1 = this->yPositions[i-2];
        }else{
            y1 = this->yPositions[0];
        }
        
        this->yPositions[i] = y1 + (this->windowSizeY*(1-this->overlapY));
        this->yPositions[i+1] = y1 + this->windowSizeY;
        
        /*if((i+1)%2 == 0){
            this->yPositions[i] = y1 + (this->windowSizeY*(1-this->overlapY));
        }else{
            this->yPositions[i] = y1 + this->windowSizeY;
        }*/
    }
    
    /*std::cout<<"X points:(";
    for(int i=0; i<this->maxBlockX; i++){
        std::cout<<this->xPositions[i]<<",";
    }
    std::cout<<")"<<endl;
    
    std::cout<<"Y points:(";
    for(int i=0; i<this->maxBlockY; i++){
        std::cout<<this->yPositions[i]<<",";
    }
    std::cout<<")"<<endl;*/
}

void ClusteringRegion::AssignPoints(){
    //cout<<((xMax-xMin))<<endl<<(1-overlap)*wSizeX<<endl<<((yMax-yMin))<<endl<<(1-overlap)*wSizeY<<endl;
    //cout<<((xMax-xMin)/((1-overlap)*wSizeX))<<endl<<((yMax-yMin)/((1-overlap)*wSizeY))<<endl;
    
    float xRange = this->xMax-this->xMin;
    float yRange = this->yMax-this->yMin;
    float o1 = this->overlapX*this->windowSizeX;
    float o2 = (1-this->overlapX)*this->windowSizeX;
    float remX = xRange - (o2*floor(xRange/o2));
    int n = xRange/o2;
    
    this->maxBlockX = 2*n;
    
    if(remX == o1){
        this->maxBlockX++;
    }else if(remX > o1){
        this->maxBlockX += 2;
    }
    
    o1 = this->overlapY*this->windowSizeY;
    o2 = (1-this->overlapY)*this->windowSizeY;
    float remY = yRange - (o2*floor(yRange/o2));
    n = yRange/o2;
    
    this->maxBlockY = 2*n;
    
    if(remY == o1){
        this->maxBlockY++;
    }else if(remY > o1){
        this->maxBlockY += 2;
    }
    
    this->xPositions = new float[this->maxBlockX];
    this->yPositions = new float[this->maxBlockY];
    
    this->SplitRegion();
}

void ClusteringRegion::SplitRegion(list<float> *x, list<float> *y){
    list<float>::iterator itr;
    int index = 0;
    
    for(itr = x->begin(); itr != x->end(); itr++){
        float val = *itr;
        
        this->xPositions[index] = val;
        index++;
    }
    
    index = 0;
    
    for(itr = y->begin(); itr != y->end(); itr++){
        float val = *itr;
        
        this->yPositions[index] = val;
        index++;
    }
    
}

void ClusteringRegion::AssignPoints(int totalBloc_x, int totalBlock_y){
    this->maxBlockX = 2*totalBloc_x;
    this->maxBlockY = 2*totalBlock_y;
    
    this->xPositions = new float[this->maxBlockX];
    this->yPositions = new float[this->maxBlockY];
    
}

void ClusteringRegion::setOverlapX(float ox){this->overlapX = ox;}
void ClusteringRegion::setOverlapY(float oy){this->overlapY = oy;}

void ClusteringRegion::setSplit(int &x, int &y){
    this->totalSplitX = x;
    this->totalSplitY = y;
}

void ClusteringRegion::SetShift(float sx, float sy){
    this->shiftX = sx;
    this->shiftY = sy;
}

void ClusteringRegion::CreateCluster(list<Phenotype*> *pList, bool setUniqueId){
    clock_t t1, t2;
    t1 = clock();
    DBScan *cluster = new DBScan(this->Eps, this->minPoints, setUniqueId);
    cluster->MakeCluster(pList);
    t2 = clock();
    
    //std::cout<<"\nClustering completed..."<<(t2-t1)/CLK_TCK<<endl;
    delete cluster;
}

void ClusteringRegion::CreateCluster(list<DataPoint*> *dpList){
    clock_t t1, t2;
    t1 = clock();
    DBScan *cluster = new DBScan(this->Eps, this->minPoints, (this->overlapX==0.0 && this->overlapY==0.0));
    cluster->MakeCluster(dpList);
    t2 = clock();
    
    //std::cout<<"\nClustering completed..."<<(t2-t1)/CLK_TCK<<endl;
    delete cluster;
}

void ClusteringRegion::CopyCluster(list<Phenotype *> cpList){
    list<Phenotype*>::iterator itr;
    
    for(itr = cpList.begin(); itr != cpList.end(); itr++){
        Phenotype* ph = *itr;
        
        this->clusterPhList.push_back(ph);
    }
}

void ClusteringRegion::CreateGraph(list<Phenotype*> cpList, short* clusterIndex, bool isHorizontalOverlap, float *BoxCoOrdinate, int *positionCoOrdinate, bool isTwoWayOverlap){
    
    long idList[TOTAL_CLUSTER_IDS]={-1};
    int xMaxBox = this->maxBlockX/2;
    float minX = BoxCoOrdinate[0], minY = BoxCoOrdinate[1], maxX = BoxCoOrdinate[2], maxY = BoxCoOrdinate[3];
    int i = positionCoOrdinate[0], j = positionCoOrdinate[1];
    list<Phenotype*>::iterator itr;
    string key = "";
    string points = "";
    float faceWeight = 0.0;
    short boxID[]={0,0,0,0};
    
    if(!isTwoWayOverlap){
        boxID[0] = j+1+(i*xMaxBox);
        boxID[1] = j+2+(i*xMaxBox);
        boxID[2] = j+1+((i+1)*xMaxBox);
        boxID[3] = j+2+((i+1)*xMaxBox);
    }else{
        boxID[0] = j+1+(i*xMaxBox);
        
        if(isHorizontalOverlap){
            boxID[1] = j+2+(i*xMaxBox);
        }else{
            boxID[1] = j+1+((i+1)*xMaxBox);
        }
    }
    
    if(cpList.size() > 0){
        
        for(itr = cpList.begin(); itr != cpList.end(); itr++){
            Phenotype* ph = *itr;
            
            ph->GetIDList(idList);
            
            short totalOL = 0;
            for(short i=0; i<TOTAL_CLUSTER_IDS; i++){
                if(idList[i] > 0) totalOL++;
            }
            
            string ids = "";
            long fid=0;
            bool first = true;
            
            for(short i=0; i<TOTAL_CLUSTER_IDS; i++){
                if(idList[i] > 0){
                    if(ids.length()>0) ids += ",";
                    ids += to_string(idList[i]);
                    
                    if(first){
                        fid = idList[i]; first=false;
                    }
                }
            }
            
            if(totalOL == 3){
                
                if(this->tries.find(fid) == this->tries.end()){
                    this->tries.insert(pair<long, string>(fid, ids));
                }
                
                if(this->ctries.find(ids) == this->ctries.end()){
                    this->ctries.insert(pair<string, long>(ids, 0));
                }else{
                    map<string, long>::iterator cmitr = this->ctries.find(ids);
                    cmitr->second = cmitr->second+1;
                }
                
            }else if(totalOL == 4){
                
                if(this->tetras.find(fid) == this->tetras.end()){
                    this->tetras.insert(pair<long, string>(fid, ids));
                }
                
                if(this->ctetras.find(ids) == this->ctetras.end()){
                    this->ctetras.insert(pair<string, long>(ids, 0));
                }else{
                    map<string, long>::iterator cmitr = this->ctetras.find(ids);
                    cmitr->second = cmitr->second+1;
                }
            }
            
            string s = ((isHorizontalOverlap)?"horizontal":"vertical");
            
            if(isTwoWayOverlap){
                //cout<<"2 way " + s + " overlap.\n";
                
                unordered_map<string, list<Phenotype*> >::iterator nodeItr;
                
                //if(idList[clusterIndex[0]] == 23){
                    //cout<<"\n";
                //}
                
                if(idList[clusterIndex[0]] > 0){
                    key = "" + to_string(idList[clusterIndex[0]]);
                    if((nodeItr = this->graph.node.nodeList.find(key)) == this->graph.node.nodeList.end()){
                        
                        list<Phenotype*> phenoList;
                        phenoList.push_back(ph);
                        
                        this->graph.node.nodeList.insert(pair<string, list<Phenotype*> >(key, phenoList));
                    }else{
                        
                        list<Phenotype*>::iterator tmpNdItr;
                        bool match = false;
                        
                        for(tmpNdItr = nodeItr->second.begin(); tmpNdItr != nodeItr->second.end(); tmpNdItr++){
                            Phenotype* tph = *tmpNdItr;
                            
                            if(tph->ComparePoint(ph)){
                                match = true;
                                break;
                            }
                        }
                        
                        if(!match)nodeItr->second.push_back(ph);
                    }
                }
                
                if(idList[clusterIndex[1]] > 0){
                    key = "" + to_string(idList[clusterIndex[1]]);
                    if((nodeItr = this->graph.node.nodeList.find(key)) == this->graph.node.nodeList.end()){
                        
                        list<Phenotype*> phenoList;
                        phenoList.push_back(ph);
                        
                        this->graph.node.nodeList.insert(pair<string, list<Phenotype*> >(key, phenoList));
                    }else{
                        list<Phenotype*>::iterator tmpNdItr;
                        bool match = false;
                        
                        for(tmpNdItr = nodeItr->second.begin(); tmpNdItr != nodeItr->second.end(); tmpNdItr++){
                            Phenotype* tph = *tmpNdItr;
                            
                            if(tph->ComparePoint(ph)){
                                match = true;
                                break;
                            }
                        }
                        
                        if(!match)nodeItr->second.push_back(ph);
                    }
                }
                
                if(idList[clusterIndex[0]] > 0 && idList[clusterIndex[1]] > 0){
                    string k1 = to_string(idList[clusterIndex[0]])+"#"+to_string(idList[clusterIndex[1]]);
                    string k2 = to_string(idList[clusterIndex[1]])+"#"+to_string(idList[clusterIndex[0]]);
                    
                    if(this->graph.edgeList.find(k1) == this->graph.edgeList.end() && this->graph.edgeList.find(k2) == this->graph.edgeList.end()){
                        
                        Edges e(idList[clusterIndex[0]], idList[clusterIndex[1]], ph->GetWeight(clusterIndex[0], clusterIndex[1]));
                        this->graph.edgeList.insert(pair<string, Edges>(k1, e));
                        
                    }else{
                        this->graph.edgeItr = this->graph.edgeList.find(k1);
                        
                        this->graph.edgeItr->second.face_weight = this->graph.edgeItr->second.face_weight + ph->GetWeight(clusterIndex[0], clusterIndex[1]);
                    }
                    
                    long clusterIDList[]={-1,-1,-1,-1};
                    clusterIDList[clusterIndex[0]] = idList[clusterIndex[0]];
                    clusterIDList[clusterIndex[1]] = idList[clusterIndex[1]];
                    
                    long keyIDList[]={clusterIDList[clusterIndex[0]], clusterIDList[clusterIndex[1]]};
                    
                    this->SortClusterIDs(keyIDList, 2);
                    short clusterIndexNew[]={clusterIndex[0],clusterIndex[1],-1,-1};
                    
                    key = "" + to_string(keyIDList[0]) + "#" + to_string(keyIDList[1]) + ":" + to_string(clusterIndex[0]) + "#" + to_string(clusterIndex[1]);
                    if(this->graph.edge.edgeList.find(key) == this->graph.edge.edgeList.end()){
                        
                        Box box(boxID[0], boxID[1], boxID[2], boxID[3], minX, maxX, minY, maxY, isHorizontalOverlap, isTwoWayOverlap, clusterIDList, clusterIndexNew);
                        
                        //cout<<"Box:("<<minX<<","<<maxX<<","<<minY<<","<<maxY<<"), ID:("<<boxID[0]<<","<<boxID[1]<<","<<boxID[2]<<","<<boxID[3]<<")\n";
                        
                        box.phenoList.push_back(ph);
                        faceWeight = ph->GetWeight(clusterIndex[0], clusterIndex[1]);
                        
                        this->graph.edge.edgeList.insert(pair<string, pair<float, Box> >(key, pair<float, Box>(faceWeight, box)));
                        
                    }else{
                        unordered_map<string, pair<float, Box> >::iterator eItr;
                        
                        if((eItr = this->graph.edge.edgeList.find(key)) != this->graph.edge.edgeList.end()){
                            
                            faceWeight = ph->GetWeight(clusterIndex[0], clusterIndex[1]) + eItr->second.first;
                            
                            eItr->second.first = faceWeight;
                            eItr->second.second.phenoList.push_back(ph);
                        }
                    }
                }
                
                
                
                //cout<<"key:"<<key<<"\n";
            
            }else{
                //cout<<"4 way " + s + " overlap.\n";
                
                unordered_map<string, list<Phenotype*> >::iterator nodeItr;
                
                if(idList[clusterIndex[0]] > 0){
                    key = "" + to_string(idList[clusterIndex[0]]);
                    if((nodeItr = this->graph.node.nodeList.find(key)) == this->graph.node.nodeList.end()){
                        
                        list<Phenotype*> phenoList;
                        phenoList.push_back(ph);
                        
                        this->graph.node.nodeList.insert(pair<string, list<Phenotype*> >(key, phenoList));
                    }else{
                        list<Phenotype*>::iterator tmpNdItr;
                        bool match = false;
                        
                        for(tmpNdItr = nodeItr->second.begin(); tmpNdItr != nodeItr->second.end(); tmpNdItr++){
                            Phenotype* tph = *tmpNdItr;
                            
                            if(tph->ComparePoint(ph)){
                                match = true;
                                break;
                            }
                        }
                        
                        if(!match)nodeItr->second.push_back(ph);
                    }
                }
                
                if(idList[clusterIndex[1]] > 0){
                    key = "" + to_string(idList[clusterIndex[1]]);
                    if((nodeItr = this->graph.node.nodeList.find(key)) == this->graph.node.nodeList.end()){
                        
                        list<Phenotype*> phenoList;
                        phenoList.push_back(ph);
                        
                        this->graph.node.nodeList.insert(pair<string, list<Phenotype*> >(key, phenoList));
                    }else{
                        list<Phenotype*>::iterator tmpNdItr;
                        bool match = false;
                        
                        for(tmpNdItr = nodeItr->second.begin(); tmpNdItr != nodeItr->second.end(); tmpNdItr++){
                            Phenotype* tph = *tmpNdItr;
                            
                            if(tph->ComparePoint(ph)){
                                match = true;
                                break;
                            }
                        }
                        
                        if(!match)nodeItr->second.push_back(ph);
                    }
                }
                
                if(idList[clusterIndex[2]] > 0){
                    key = "" + to_string(idList[clusterIndex[2]]);
                    if((nodeItr = this->graph.node.nodeList.find(key)) == this->graph.node.nodeList.end()){
                        
                        list<Phenotype*> phenoList;
                        phenoList.push_back(ph);
                        
                        this->graph.node.nodeList.insert(pair<string, list<Phenotype*> >(key, phenoList));
                    }else{
                        list<Phenotype*>::iterator tmpNdItr;
                        bool match = false;
                        
                        for(tmpNdItr = nodeItr->second.begin(); tmpNdItr != nodeItr->second.end(); tmpNdItr++){
                            Phenotype* tph = *tmpNdItr;
                            
                            if(tph->ComparePoint(ph)){
                                match = true;
                                break;
                            }
                        }
                        
                        if(!match)nodeItr->second.push_back(ph);
                    }
                }
                
                if(idList[clusterIndex[3]] > 0){
                    key = "" + to_string(idList[clusterIndex[3]]);
                    if((nodeItr = this->graph.node.nodeList.find(key)) == this->graph.node.nodeList.end()){
                        
                        list<Phenotype*> phenoList;
                        phenoList.push_back(ph);
                        
                        this->graph.node.nodeList.insert(pair<string, list<Phenotype*> >(key, phenoList));
                    }else{
                        list<Phenotype*>::iterator tmpNdItr;
                        bool match = false;
                        
                        for(tmpNdItr = nodeItr->second.begin(); tmpNdItr != nodeItr->second.end(); tmpNdItr++){
                            Phenotype* tph = *tmpNdItr;
                            
                            if(tph->ComparePoint(ph)){
                                match = true;
                                break;
                            }
                        }
                        
                        if(!match)nodeItr->second.push_back(ph);
                    }
                }
                
                if(idList[0] > 0 && idList[1] > 0 && idList[2] > 0){
                    long keyIDList[]={idList[0], idList[1], idList[2], idList[3]};
                    
                    this->SortClusterIDs(keyIDList, 4);
                    
                    short clusterIndexNew[]={0,1,2,3};
                    
                    key = "" + to_string(keyIDList[0]) + "#" + to_string(keyIDList[1]) + "#" + to_string(keyIDList[2]) + "#" + to_string(keyIDList[3]);
                    if(this->graph.edge.edgeList.find(key) == this->graph.edge.edgeList.end()){
                        
                        Box box(boxID[0], boxID[1], boxID[2], boxID[3], minX, maxX, minY, maxY, isHorizontalOverlap, isTwoWayOverlap, idList, clusterIndexNew);
                        
                        //cout<<"Box:("<<minX<<","<<maxX<<","<<minY<<","<<maxY<<"), ID:("<<boxID[0]<<","<<boxID[1]<<","<<boxID[2]<<","<<boxID[3]<<")\n";
                        
                        box.phenoList.push_back(ph);
                        faceWeight = ph->GetWeight();
                        
                        this->graph.edge.edgeList.insert(pair<string, pair<float, Box> >(key, pair<float, Box>(faceWeight, box)));
                        
                    }else{
                        unordered_map<string, pair<float, Box> >::iterator eItr;
                        
                        if((eItr = this->graph.edge.edgeList.find(key)) != this->graph.edge.edgeList.end()){
                            
                            faceWeight = ph->GetWeight() + eItr->second.first;
                            
                            eItr->second.first = faceWeight;
                            eItr->second.second.phenoList.push_back(ph);
                        }
                    }
                }
                
                //cout<<"key:"<<key<<"\n";
            }
        }
        
    }
}

void ClusteringRegion::CreateGraph_new(list<Phenotype*> cpList, short* clusterIndex, bool isHorizontalOverlap, float *BoxCoOrdinate, int *positionCoOrdinate, bool isTwoWayOverlap){
    
    long idList[TOTAL_CLUSTER_IDS]={-1};
    int xMaxBox = this->maxBlockX/2;
    int i = positionCoOrdinate[0], j = positionCoOrdinate[1];
    list<Phenotype*>::iterator itr;
    string key = "";
    string points = "";
    short boxID[]={0,0,0,0};
    
    if(!isTwoWayOverlap){
        boxID[0] = j+1+(i*xMaxBox);
        boxID[1] = j+2+(i*xMaxBox);
        boxID[2] = j+1+((i+1)*xMaxBox);
        boxID[3] = j+2+((i+1)*xMaxBox);
    }else{
        boxID[0] = j+1+(i*xMaxBox);
        
        if(isHorizontalOverlap){
            boxID[1] = j+2+(i*xMaxBox);
        }else{
            boxID[1] = j+1+((i+1)*xMaxBox);
        }
    }
    
    if(cpList.size() > 0){
        
        for(itr = cpList.begin(); itr != cpList.end(); itr++){
            Phenotype* ph = *itr;
            
            ph->GetIDList(idList);
            
            string s = ((isHorizontalOverlap)?"horizontal":"vertical");
            
            if(isTwoWayOverlap){
                //cout<<"2 way " + s + " overlap.\n";
                
                unordered_map<string, list<Phenotype*> >::iterator nodeItr;
                
                //if(idList[clusterIndex[0]] == 23){
                //cout<<"\n";
                //}
                
                if(idList[clusterIndex[0]] > 0){
                    key = "" + to_string(idList[clusterIndex[0]]);
                    
                    long thisID = idList[clusterIndex[0]];
                    
                    if(this->graph.nodeList.find(thisID)==this->graph.nodeList.end()){
                        for(int k=0; k<TOTAL_CLUSTER_IDS; k++){
                            if(idList[k]>0 && thisID!=idList[k]){
                                unordered_map<long, list<Phenotype*> > aMap;
                                list<Phenotype*> aPh;
                                
                                aPh.push_back(ph);
                                aMap.insert(pair<long, list<Phenotype*> >(idList[k], aPh));
                            }
                        }
                    }else{
                        unordered_map<long, unordered_map<long, list<Phenotype*> > >::iterator nlItr = this->graph.nodeList.find(thisID);
                        
                        for(int k=0; k<TOTAL_CLUSTER_IDS; k++){
                            if(idList[k]>0 && thisID!=idList[k]){
                                unordered_map<long, list<Phenotype*> >::iterator nl2Itr = nlItr->second.find(idList[k]);
                                
                                if(nl2Itr == nlItr->second.end()){
                                    list<Phenotype*> aPh;
                                    
                                    aPh.push_back(ph);
                                    nlItr->second.insert(pair<long, list<Phenotype*> >(idList[k], aPh));
                                }else{
                                    nl2Itr->second.push_back(ph);
                                }
                            }
                        }
                    }
                    
                    
                    if((nodeItr = this->graph.node.nodeList.find(key)) == this->graph.node.nodeList.end()){
                        
                        list<Phenotype*> phenoList;
                        phenoList.push_back(ph);
                        
                        this->graph.node.nodeList.insert(pair<string, list<Phenotype*> >(key, phenoList));
                    }else{
                        
                        list<Phenotype*>::iterator tmpNdItr;
                        bool match = false;
                        
                        for(tmpNdItr = nodeItr->second.begin(); tmpNdItr != nodeItr->second.end(); tmpNdItr++){
                            Phenotype* tph = *tmpNdItr;
                            
                            if(tph->ComparePoint(ph)){
                                match = true;
                                break;
                            }
                        }
                        
                        if(!match)nodeItr->second.push_back(ph);
                    }
                }
                
                if(idList[clusterIndex[1]] > 0){
                    key = "" + to_string(idList[clusterIndex[1]]);
                    
                    long thisID = idList[clusterIndex[1]];
                    
                    if(this->graph.nodeList.find(thisID)==this->graph.nodeList.end()){
                        for(int k=0; k<TOTAL_CLUSTER_IDS; k++){
                            if(idList[k]>0 && thisID!=idList[k]){
                                unordered_map<long, list<Phenotype*> > aMap;
                                list<Phenotype*> aPh;
                                
                                aPh.push_back(ph);
                                aMap.insert(pair<long, list<Phenotype*> >(idList[k], aPh));
                            }
                        }
                    }else{
                        unordered_map<long, unordered_map<long, list<Phenotype*> > >::iterator nlItr = this->graph.nodeList.find(thisID);
                        
                        for(int k=0; k<TOTAL_CLUSTER_IDS; k++){
                            if(idList[k]>0 && thisID!=idList[k]){
                                unordered_map<long, list<Phenotype*> >::iterator nl2Itr = nlItr->second.find(idList[k]);
                                
                                if(nl2Itr == nlItr->second.end()){
                                    list<Phenotype*> aPh;
                                    
                                    aPh.push_back(ph);
                                    nlItr->second.insert(pair<long, list<Phenotype*> >(idList[k], aPh));
                                }else{
                                    nl2Itr->second.push_back(ph);
                                }
                            }
                        }
                    }
                    
                    
                    if((nodeItr = this->graph.node.nodeList.find(key)) == this->graph.node.nodeList.end()){
                        
                        list<Phenotype*> phenoList;
                        phenoList.push_back(ph);
                        
                        this->graph.node.nodeList.insert(pair<string, list<Phenotype*> >(key, phenoList));
                    }else{
                        list<Phenotype*>::iterator tmpNdItr;
                        bool match = false;
                        
                        for(tmpNdItr = nodeItr->second.begin(); tmpNdItr != nodeItr->second.end(); tmpNdItr++){
                            Phenotype* tph = *tmpNdItr;
                            
                            if(tph->ComparePoint(ph)){
                                match = true;
                                break;
                            }
                        }
                        
                        if(!match)nodeItr->second.push_back(ph);
                    }
                }
                
                //cout<<"key:"<<key<<"\n";
                
            }else{
                //cout<<"4 way " + s + " overlap.\n";
                
                unordered_map<string, list<Phenotype*> >::iterator nodeItr;
                
                if(idList[clusterIndex[0]] > 0){
                    key = "" + to_string(idList[clusterIndex[0]]);
                    
                    long thisID = idList[clusterIndex[0]];
                    
                    if(this->graph.nodeList.find(thisID)==this->graph.nodeList.end()){
                        for(int k=0; k<TOTAL_CLUSTER_IDS; k++){
                            if(idList[k]>0 && thisID!=idList[k]){
                                unordered_map<long, list<Phenotype*> > aMap;
                                list<Phenotype*> aPh;
                                
                                aPh.push_back(ph);
                                aMap.insert(pair<long, list<Phenotype*> >(idList[k], aPh));
                            }
                        }
                    }else{
                        unordered_map<long, unordered_map<long, list<Phenotype*> > >::iterator nlItr = this->graph.nodeList.find(thisID);
                        
                        for(int k=0; k<TOTAL_CLUSTER_IDS; k++){
                            if(idList[k]>0 && thisID!=idList[k]){
                                unordered_map<long, list<Phenotype*> >::iterator nl2Itr = nlItr->second.find(idList[k]);
                                
                                if(nl2Itr == nlItr->second.end()){
                                    list<Phenotype*> aPh;
                                    
                                    aPh.push_back(ph);
                                    nlItr->second.insert(pair<long, list<Phenotype*> >(idList[k], aPh));
                                }else{
                                    nl2Itr->second.push_back(ph);
                                }
                            }
                        }
                    }
                    
                    if((nodeItr = this->graph.node.nodeList.find(key)) == this->graph.node.nodeList.end()){
                        
                        list<Phenotype*> phenoList;
                        phenoList.push_back(ph);
                        
                        this->graph.node.nodeList.insert(pair<string, list<Phenotype*> >(key, phenoList));
                    }else{
                        list<Phenotype*>::iterator tmpNdItr;
                        bool match = false;
                        
                        for(tmpNdItr = nodeItr->second.begin(); tmpNdItr != nodeItr->second.end(); tmpNdItr++){
                            Phenotype* tph = *tmpNdItr;
                            
                            if(tph->ComparePoint(ph)){
                                match = true;
                                break;
                            }
                        }
                        
                        if(!match)nodeItr->second.push_back(ph);
                    }
                }
                
                if(idList[clusterIndex[1]] > 0){
                    key = "" + to_string(idList[clusterIndex[1]]);
                    
                    long thisID = idList[clusterIndex[1]];
                    
                    if(this->graph.nodeList.find(thisID)==this->graph.nodeList.end()){
                        for(int k=0; k<TOTAL_CLUSTER_IDS; k++){
                            if(idList[k]>0 && thisID!=idList[k]){
                                unordered_map<long, list<Phenotype*> > aMap;
                                list<Phenotype*> aPh;
                                
                                aPh.push_back(ph);
                                aMap.insert(pair<long, list<Phenotype*> >(idList[k], aPh));
                            }
                        }
                    }else{
                        unordered_map<long, unordered_map<long, list<Phenotype*> > >::iterator nlItr = this->graph.nodeList.find(thisID);
                        
                        for(int k=0; k<TOTAL_CLUSTER_IDS; k++){
                            if(idList[k]>0 && thisID!=idList[k]){
                                unordered_map<long, list<Phenotype*> >::iterator nl2Itr = nlItr->second.find(idList[k]);
                                
                                if(nl2Itr == nlItr->second.end()){
                                    list<Phenotype*> aPh;
                                    
                                    aPh.push_back(ph);
                                    nlItr->second.insert(pair<long, list<Phenotype*> >(idList[k], aPh));
                                }else{
                                    nl2Itr->second.push_back(ph);
                                }
                            }
                        }
                    }
                    
                    
                    if((nodeItr = this->graph.node.nodeList.find(key)) == this->graph.node.nodeList.end()){
                        
                        list<Phenotype*> phenoList;
                        phenoList.push_back(ph);
                        
                        this->graph.node.nodeList.insert(pair<string, list<Phenotype*> >(key, phenoList));
                    }else{
                        list<Phenotype*>::iterator tmpNdItr;
                        bool match = false;
                        
                        for(tmpNdItr = nodeItr->second.begin(); tmpNdItr != nodeItr->second.end(); tmpNdItr++){
                            Phenotype* tph = *tmpNdItr;
                            
                            if(tph->ComparePoint(ph)){
                                match = true;
                                break;
                            }
                        }
                        
                        if(!match)nodeItr->second.push_back(ph);
                    }
                }
                
                if(idList[clusterIndex[2]] > 0){
                    key = "" + to_string(idList[clusterIndex[2]]);
                    
                    long thisID = idList[clusterIndex[2]];
                    
                    if(this->graph.nodeList.find(thisID)==this->graph.nodeList.end()){
                        for(int k=0; k<TOTAL_CLUSTER_IDS; k++){
                            if(idList[k]>0 && thisID!=idList[k]){
                                unordered_map<long, list<Phenotype*> > aMap;
                                list<Phenotype*> aPh;
                                
                                aPh.push_back(ph);
                                aMap.insert(pair<long, list<Phenotype*> >(idList[k], aPh));
                            }
                        }
                    }else{
                        unordered_map<long, unordered_map<long, list<Phenotype*> > >::iterator nlItr = this->graph.nodeList.find(thisID);
                        
                        for(int k=0; k<TOTAL_CLUSTER_IDS; k++){
                            if(idList[k]>0 && thisID!=idList[k]){
                                unordered_map<long, list<Phenotype*> >::iterator nl2Itr = nlItr->second.find(idList[k]);
                                
                                if(nl2Itr == nlItr->second.end()){
                                    list<Phenotype*> aPh;
                                    
                                    aPh.push_back(ph);
                                    nlItr->second.insert(pair<long, list<Phenotype*> >(idList[k], aPh));
                                }else{
                                    nl2Itr->second.push_back(ph);
                                }
                            }
                        }
                    }
                    
                    if((nodeItr = this->graph.node.nodeList.find(key)) == this->graph.node.nodeList.end()){
                        
                        list<Phenotype*> phenoList;
                        phenoList.push_back(ph);
                        
                        this->graph.node.nodeList.insert(pair<string, list<Phenotype*> >(key, phenoList));
                    }else{
                        list<Phenotype*>::iterator tmpNdItr;
                        bool match = false;
                        
                        for(tmpNdItr = nodeItr->second.begin(); tmpNdItr != nodeItr->second.end(); tmpNdItr++){
                            Phenotype* tph = *tmpNdItr;
                            
                            if(tph->ComparePoint(ph)){
                                match = true;
                                break;
                            }
                        }
                        
                        if(!match)nodeItr->second.push_back(ph);
                    }
                }
                
                if(idList[clusterIndex[3]] > 0){
                    key = "" + to_string(idList[clusterIndex[3]]);
                    
                    long thisID = idList[clusterIndex[3]];
                    
                    if(this->graph.nodeList.find(thisID)==this->graph.nodeList.end()){
                        for(int k=0; k<TOTAL_CLUSTER_IDS; k++){
                            if(idList[k]>0 && thisID!=idList[k]){
                                unordered_map<long, list<Phenotype*> > aMap;
                                list<Phenotype*> aPh;
                                
                                aPh.push_back(ph);
                                aMap.insert(pair<long, list<Phenotype*> >(idList[k], aPh));
                            }
                        }
                    }else{
                        unordered_map<long, unordered_map<long, list<Phenotype*> > >::iterator nlItr = this->graph.nodeList.find(thisID);
                        
                        for(int k=0; k<TOTAL_CLUSTER_IDS; k++){
                            if(idList[k]>0 && thisID!=idList[k]){
                                unordered_map<long, list<Phenotype*> >::iterator nl2Itr = nlItr->second.find(idList[k]);
                                
                                if(nl2Itr == nlItr->second.end()){
                                    list<Phenotype*> aPh;
                                    
                                    aPh.push_back(ph);
                                    nlItr->second.insert(pair<long, list<Phenotype*> >(idList[k], aPh));
                                }else{
                                    nl2Itr->second.push_back(ph);
                                }
                            }
                        }
                    }
                    
                    if((nodeItr = this->graph.node.nodeList.find(key)) == this->graph.node.nodeList.end()){
                        
                        list<Phenotype*> phenoList;
                        phenoList.push_back(ph);
                        
                        this->graph.node.nodeList.insert(pair<string, list<Phenotype*> >(key, phenoList));
                    }else{
                        list<Phenotype*>::iterator tmpNdItr;
                        bool match = false;
                        
                        for(tmpNdItr = nodeItr->second.begin(); tmpNdItr != nodeItr->second.end(); tmpNdItr++){
                            Phenotype* tph = *tmpNdItr;
                            
                            if(tph->ComparePoint(ph)){
                                match = true;
                                break;
                            }
                        }
                        
                        if(!match)nodeItr->second.push_back(ph);
                    }
                }
                
            
                //cout<<"key:"<<key<<"\n";
            }
        }
        
    }
}

void ClusteringRegion::CreateGraph(list<DataPoint*> dpList, short* clusterIndex, bool isHorizontalOverlap, float *BoxCoOrdinate, int *positionCoOrdinate, bool isTwoWayOverlap){
    
    long idList[TOTAL_CLUSTER_IDS]={-1};
    int xMaxBox = this->maxBlockX/2;
    float minX = BoxCoOrdinate[0], minY = BoxCoOrdinate[1], maxX = BoxCoOrdinate[2], maxY = BoxCoOrdinate[3];
    int i = positionCoOrdinate[0], j = positionCoOrdinate[1];
    list<DataPoint*>::iterator itr;
    string key = "";
    string points = "";
    float faceWeight = 0.0;
    short boxID[]={0,0,0,0};
    
    if(!isTwoWayOverlap){
        boxID[0] = j+1+(i*xMaxBox);
        boxID[1] = j+2+(i*xMaxBox);
        boxID[2] = j+1+((i+1)*xMaxBox);
        boxID[3] = j+2+((i+1)*xMaxBox);
    }else{
        boxID[0] = j+1+(i*xMaxBox);
        
        if(isHorizontalOverlap){
            boxID[1] = j+2+(i*xMaxBox);
        }else{
            boxID[1] = j+1+((i+1)*xMaxBox);
        }
    }
    
    if(dpList.size() > 0){
        
        map<string, int>::iterator mitr;
        
        for(itr = dpList.begin(); itr != dpList.end(); itr++){
            DataPoint* dp = *itr;
            long tmpIdList[TOTAL_CLUSTER_IDS];
            
            dp->GetIDList(tmpIdList);
            this->SortClusterIDs(tmpIdList, TOTAL_CLUSTER_IDS);
            
            string strClsIdList = "";
            
            for(int i=0; i<TOTAL_CLUSTER_IDS; i++){
                if(strClsIdList.length()>0) strClsIdList += ",";
                if(tmpIdList[i]>0) strClsIdList += to_string(tmpIdList[i]);
            }
            
            if((mitr = edgeThresholdMap.find(strClsIdList)) == edgeThresholdMap.end()){
                edgeThresholdMap.insert(pair<string,int>(strClsIdList, 1));
            }else{
                mitr->second++;
            }
        }
        
        
        for(itr = dpList.begin(); itr != dpList.end(); itr++){
            DataPoint* dp = *itr;
            
            long tmpIdList[TOTAL_CLUSTER_IDS];
            
            dp->GetIDList(tmpIdList);
            this->SortClusterIDs(tmpIdList, TOTAL_CLUSTER_IDS);
            
            string strClsIdList = "";
            
            for(int i=0; i<TOTAL_CLUSTER_IDS; i++){
                if(strClsIdList.length()>0) strClsIdList += ",";
                if(tmpIdList[i]>0) strClsIdList += to_string(tmpIdList[i]);
            }
            
            if((mitr = edgeThresholdMap.find(strClsIdList)) != edgeThresholdMap.end()){
                if(mitr->second >= EDGE_THRESHOLD){
                
                    dp->GetIDList(idList);
                    
                    short totalOL = 0;
                    for(short i=0; i<TOTAL_CLUSTER_IDS; i++){
                        if(idList[i] > 0) totalOL++;
                    }
                    
                    string ids = "";
                    long fid=0;
                    bool first = true;
                    
                    for(short i=0; i<TOTAL_CLUSTER_IDS; i++){
                        if(idList[i] > 0){
                            if(ids.length()>0) ids += ",";
                            ids += to_string(idList[i]);
                            
                            if(first){
                                fid = idList[i]; first=false;
                            }
                        }
                    }
                    
                    if(totalOL == 3){
                        
                        if(this->tries.find(fid) == this->tries.end()){
                            this->tries.insert(pair<long, string>(fid, ids));
                        }
                        
                        if(this->ctries.find(ids) == this->ctries.end()){
                            this->ctries.insert(pair<string, long>(ids, 0));
                        }else{
                            map<string, long>::iterator cmitr = this->ctries.find(ids);
                            cmitr->second = cmitr->second+1;
                        }
                        
                    }else if(totalOL == 4){
                        
                        if(this->tetras.find(fid) == this->tetras.end()){
                            this->tetras.insert(pair<long, string>(fid, ids));
                        }
                        
                        if(this->ctetras.find(ids) == this->ctetras.end()){
                            this->ctetras.insert(pair<string, long>(ids, 0));
                        }else{
                            map<string, long>::iterator cmitr = this->ctetras.find(ids);
                            cmitr->second = cmitr->second+1;
                        }
                    }
                    
                    string s = ((isHorizontalOverlap)?"horizontal":"vertical");
                    
                    if(isTwoWayOverlap){
                        //cout<<"2 way " + s + " overlap.\n";
                        
                        unordered_map<string, list<DataPoint*> >::iterator nodeItr;
                        
                        //if(idList[clusterIndex[0]] == 23){
                        //cout<<"\n";
                        //}
                        
                        if(idList[clusterIndex[0]] > 0){
                            key = "" + to_string(idList[clusterIndex[0]]);
                            if((nodeItr = this->graph.node.nodeListDP.find(key)) == this->graph.node.nodeListDP.end()){
                                
                                list<DataPoint*> dataPointList;
                                dataPointList.push_back(dp);
                                
                                this->graph.node.nodeListDP.insert(pair<string, list<DataPoint*> >(key, dataPointList));
                            }else{
                                
                                list<DataPoint*>::iterator tmpNdItr;
                                bool match = false;
                                
                                for(tmpNdItr = nodeItr->second.begin(); tmpNdItr != nodeItr->second.end(); tmpNdItr++){
                                    DataPoint* tph = *tmpNdItr;
                                    
                                    if(tph->ComparePoint(dp)){
                                        match = true;
                                        break;
                                    }
                                }
                                
                                if(!match)nodeItr->second.push_back(dp);
                            }
                        }
                        
                        if(idList[clusterIndex[1]] > 0){
                            key = "" + to_string(idList[clusterIndex[1]]);
                            if((nodeItr = this->graph.node.nodeListDP.find(key)) == this->graph.node.nodeListDP.end()){
                                
                                list<DataPoint*> phenoList;
                                phenoList.push_back(dp);
                                
                                this->graph.node.nodeListDP.insert(pair<string, list<DataPoint*> >(key, phenoList));
                            }else{
                                list<DataPoint*>::iterator tmpNdItr;
                                bool match = false;
                                
                                for(tmpNdItr = nodeItr->second.begin(); tmpNdItr != nodeItr->second.end(); tmpNdItr++){
                                    DataPoint* tph = *tmpNdItr;
                                    
                                    if(tph->ComparePoint(dp)){
                                        match = true;
                                        break;
                                    }
                                }
                                
                                if(!match)nodeItr->second.push_back(dp);
                            }
                        }
                        
                        if(idList[clusterIndex[0]] > 0 && idList[clusterIndex[1]] > 0){
                            string k1 = to_string(idList[clusterIndex[0]])+"#"+to_string(idList[clusterIndex[1]]);
                            string k2 = to_string(idList[clusterIndex[1]])+"#"+to_string(idList[clusterIndex[0]]);
                            
                            if(this->graph.edgeList.find(k1) == this->graph.edgeList.end() && this->graph.edgeList.find(k2) == this->graph.edgeList.end()){
                                
                                Edges e(idList[clusterIndex[0]], idList[clusterIndex[1]], dp->GetWeight(clusterIndex[0], clusterIndex[1]));
                                this->graph.edgeList.insert(pair<string, Edges>(k1, e));
                                
                            }else{
                                this->graph.edgeItr = this->graph.edgeList.find(k1);
                                
                                this->graph.edgeItr->second.face_weight = this->graph.edgeItr->second.face_weight + dp->GetWeight(clusterIndex[0], clusterIndex[1]);
                            }
                            
                            long clusterIDList[]={-1,-1,-1,-1};
                            clusterIDList[clusterIndex[0]] = idList[clusterIndex[0]];
                            clusterIDList[clusterIndex[1]] = idList[clusterIndex[1]];
                            
                            long keyIDList[]={clusterIDList[clusterIndex[0]], clusterIDList[clusterIndex[1]]};
                            
                            this->SortClusterIDs(keyIDList, 2);
                            short clusterIndexNew[]={clusterIndex[0],clusterIndex[1],-1,-1};
                            
                            key = "" + to_string(keyIDList[0]) + "#" + to_string(keyIDList[1]) + ":" + to_string(clusterIndex[0]) + "#" + to_string(clusterIndex[1]);
                            if(this->graph.edge.edgeList.find(key) == this->graph.edge.edgeList.end()){
                                
                                Box box(boxID[0], boxID[1], boxID[2], boxID[3], minX, maxX, minY, maxY, isHorizontalOverlap, isTwoWayOverlap, clusterIDList, clusterIndexNew);
                                
                                //cout<<"Box:("<<minX<<","<<maxX<<","<<minY<<","<<maxY<<"), ID:("<<boxID[0]<<","<<boxID[1]<<","<<boxID[2]<<","<<boxID[3]<<")\n";
                                
                                box.dpList.push_back(dp);
                                faceWeight = dp->GetWeight(clusterIndex[0], clusterIndex[1]);
                                
                                this->graph.edge.edgeList.insert(pair<string, pair<float, Box> >(key, pair<float, Box>(faceWeight, box)));
                                
                            }else{
                                unordered_map<string, pair<float, Box> >::iterator eItr;
                                
                                if((eItr = this->graph.edge.edgeList.find(key)) != this->graph.edge.edgeList.end()){
                                    
                                    faceWeight = dp->GetWeight(clusterIndex[0], clusterIndex[1]) + eItr->second.first;
                                    
                                    eItr->second.first = faceWeight;
                                    eItr->second.second.dpList.push_back(dp);
                                }
                            }
                        }
                        
                        
                        
                        //cout<<"key:"<<key<<"\n";
                        
                    }else{
                        //cout<<"4 way " + s + " overlap.\n";
                        
                        unordered_map<string, list<DataPoint*> >::iterator nodeItr;
                        
                        if(idList[clusterIndex[0]] > 0){
                            key = "" + to_string(idList[clusterIndex[0]]);
                            if((nodeItr = this->graph.node.nodeListDP.find(key)) == this->graph.node.nodeListDP.end()){
                                
                                list<DataPoint*> phenoList;
                                phenoList.push_back(dp);
                                
                                this->graph.node.nodeListDP.insert(pair<string, list<DataPoint*> >(key, phenoList));
                            }else{
                                list<DataPoint*>::iterator tmpNdItr;
                                bool match = false;
                                
                                for(tmpNdItr = nodeItr->second.begin(); tmpNdItr != nodeItr->second.end(); tmpNdItr++){
                                    DataPoint* tph = *tmpNdItr;
                                    
                                    if(tph->ComparePoint(dp)){
                                        match = true;
                                        break;
                                    }
                                }
                                
                                if(!match)nodeItr->second.push_back(dp);
                            }
                        }
                        
                        if(idList[clusterIndex[1]] > 0){
                            key = "" + to_string(idList[clusterIndex[1]]);
                            if((nodeItr = this->graph.node.nodeListDP.find(key)) == this->graph.node.nodeListDP.end()){
                                
                                list<DataPoint*> phenoList;
                                phenoList.push_back(dp);
                                
                                this->graph.node.nodeListDP.insert(pair<string, list<DataPoint*> >(key, phenoList));
                            }else{
                                list<DataPoint*>::iterator tmpNdItr;
                                bool match = false;
                                
                                for(tmpNdItr = nodeItr->second.begin(); tmpNdItr != nodeItr->second.end(); tmpNdItr++){
                                    DataPoint* tph = *tmpNdItr;
                                    
                                    if(tph->ComparePoint(dp)){
                                        match = true;
                                        break;
                                    }
                                }
                                
                                if(!match)nodeItr->second.push_back(dp);
                            }
                        }
                        
                        if(idList[clusterIndex[2]] > 0){
                            key = "" + to_string(idList[clusterIndex[2]]);
                            if((nodeItr = this->graph.node.nodeListDP.find(key)) == this->graph.node.nodeListDP.end()){
                                
                                list<DataPoint*> phenoList;
                                phenoList.push_back(dp);
                                
                                this->graph.node.nodeListDP.insert(pair<string, list<DataPoint*> >(key, phenoList));
                            }else{
                                list<DataPoint*>::iterator tmpNdItr;
                                bool match = false;
                                
                                for(tmpNdItr = nodeItr->second.begin(); tmpNdItr != nodeItr->second.end(); tmpNdItr++){
                                    DataPoint* tph = *tmpNdItr;
                                    
                                    if(tph->ComparePoint(dp)){
                                        match = true;
                                        break;
                                    }
                                }
                                
                                if(!match)nodeItr->second.push_back(dp);
                            }
                        }
                        
                        if(idList[clusterIndex[3]] > 0){
                            key = "" + to_string(idList[clusterIndex[3]]);
                            if((nodeItr = this->graph.node.nodeListDP.find(key)) == this->graph.node.nodeListDP.end()){
                                
                                list<DataPoint*> phenoList;
                                phenoList.push_back(dp);
                                
                                this->graph.node.nodeListDP.insert(pair<string, list<DataPoint*> >(key, phenoList));
                            }else{
                                list<DataPoint*>::iterator tmpNdItr;
                                bool match = false;
                                
                                for(tmpNdItr = nodeItr->second.begin(); tmpNdItr != nodeItr->second.end(); tmpNdItr++){
                                    DataPoint* tph = *tmpNdItr;
                                    
                                    if(tph->ComparePoint(dp)){
                                        match = true;
                                        break;
                                    }
                                }
                                
                                if(!match)nodeItr->second.push_back(dp);
                            }
                        }
                        
                        
                        if(idList[0] > 0 && idList[1] > 0 && idList[2] > 0){
                            long keyIDList[]={idList[0], idList[1], idList[2], idList[3]};
                            
                            this->SortClusterIDs(keyIDList, 4);
                            
                            short clusterIndexNew[]={0,1,2,3};
                            
                            key = "" + to_string(keyIDList[0]) + "#" + to_string(keyIDList[1]) + "#" + to_string(keyIDList[2]) + "#" + to_string(keyIDList[3]);
                            if(this->graph.edge.edgeList.find(key) == this->graph.edge.edgeList.end()){
                                
                                Box box(boxID[0], boxID[1], boxID[2], boxID[3], minX, maxX, minY, maxY, isHorizontalOverlap, isTwoWayOverlap, idList, clusterIndexNew);
                                
                                //cout<<"Box:("<<minX<<","<<maxX<<","<<minY<<","<<maxY<<"), ID:("<<boxID[0]<<","<<boxID[1]<<","<<boxID[2]<<","<<boxID[3]<<")\n";
                                
                                box.dpList.push_back(dp);
                                faceWeight = dp->GetWeight();
                                
                                this->graph.edge.edgeList.insert(pair<string, pair<float, Box> >(key, pair<float, Box>(faceWeight, box)));
                                
                            }else{
                                unordered_map<string, pair<float, Box> >::iterator eItr;
                                
                                if((eItr = this->graph.edge.edgeList.find(key)) != this->graph.edge.edgeList.end()){
                                    
                                    faceWeight = dp->GetWeight() + eItr->second.first;
                                    
                                    eItr->second.first = faceWeight;
                                    eItr->second.second.dpList.push_back(dp);
                                }
                            }
                        }
                        
                        //cout<<"key:"<<key<<"\n";
                    }
                }
            }
            
        }
        
    }
}

void ClusteringRegion::SortClusterIDs(long *clusterIDList, long size){
    if(clusterIDList != NULL){
        for(long i=0; i<size; i++){
            for(long j=i+1; j<size;j++){
                long c1 = clusterIDList[i];
                long c2 = clusterIDList[j];
                
                if(c1 > c2){
                    clusterIDList[i] = clusterIDList[j];
                    clusterIDList[j] = c1;
                }
            }
        }
    }
    
}

bool ClusteringRegion::IsEligibleForClustering(float x1, float y1, float x2, float y2, long boxID){
    
        
    return false;
}

void ClusteringRegion::ManageCluster(list<Phenotype *> *pList, long minClsId, long maxClsId, set<long>* mainClsID){
    Cluster* cluster = new Cluster[maxClsId-minClsId+1];
    set<long> clsRange;
    //bool reduce = false;
    //long newID = minClsId;
    
    for(long i=minClsId; i<=maxClsId; i++){
        clsRange.insert(i);
    }
    
    list<Phenotype *>::iterator itr;
    
    for(itr=pList->begin(); itr!=pList->end(); itr++){
        Phenotype* ph = *itr;
        
        long clsIDList[TOTAL_CLUSTER_IDS];
        
        ph->GetIDList(clsIDList);
        
        for(short i=0; i<TOTAL_CLUSTER_IDS; i++){
            if(clsIDList[i] > 0 && clsRange.find(clsIDList[i]) != clsRange.end()){
                cluster[clsIDList[i]-minClsId].AddToCluster(ph);
            }
        }
    }
    
    clsRange.clear();
    list<Phenotype *> cphList;
    
    for(itr=pList->begin(); itr!=pList->end(); itr++){
        Phenotype* ph = *itr;
        
        cphList.push_back(ph);
    }
    
    pList->clear();
    
    list<Phenotype *> phList;
    set<long>::iterator sitr;
    
    for(long i=minClsId; i<=maxClsId; i++){
        cluster[i-minClsId].GetPhenotypeListOfThisCluster(&phList);
        
        //cout<<"Cluster ID:"<<i<<endl;
        for(itr=phList.begin(); itr!=phList.end(); itr++){
            Phenotype* ph = *itr;
            clsRange.insert(ph->GetUniqueID());
        }
        
        for(sitr=mainClsID->begin(); sitr!=mainClsID->end(); sitr++){
            long id = *sitr;
            
            for(itr=phList.begin(); itr!=phList.end(); itr++){
                Phenotype* ph = *itr;
                
                long idList[TOTAL_CLUSTER_IDS];
                ph->GetIDList(idList);
                
                for(short j=0; j<TOTAL_CLUSTER_IDS; j++){
                    if(idList[j] == i){
                        ph->UpdateClusterID(j, 0);
                        ph->UpdateType(j, "");
                        ph->DecreaseClusterIndex();
                        
                        break;
                    }
                }
                
                if(clsRange.find(id) != clsRange.end()){
                    pList->push_back(ph);
                }
            }
        
        }
        
        phList.clear();
        clsRange.clear();
    }
    
    /*for(itr=pList->begin(); itr!=pList->end(); itr++){
        Phenotype* ph = *itr;
        
        cout<<ph->toString();
    }*/
    
    Cluster::SetClusterID(minClsId-1);
    
    delete []cluster;
}

void ClusteringRegion::ManageCluster(list<DataPoint *> *dpList, long minClsId, long maxClsId, set<long>* mainClsID){
    Cluster* cluster = new Cluster[maxClsId-minClsId+1];
    set<long> clsRange;
    //bool reduce = false;
    //long newID = minClsId;
    
    for(long i=minClsId; i<=maxClsId; i++){
        clsRange.insert(i);
    }
    
    list<DataPoint *>::iterator itr;
    
    for(itr=dpList->begin(); itr!=dpList->end(); itr++){
        DataPoint* dp = *itr;
        
        long clsIDList[TOTAL_CLUSTER_IDS];
        
        dp->GetIDList(clsIDList);
        
        for(short i=0; i<TOTAL_CLUSTER_IDS; i++){
            if(clsIDList[i] > 0 && clsRange.find(clsIDList[i]) != clsRange.end()){
                cluster[clsIDList[i]-minClsId].AddToCluster(dp);
            }
        }
    }
    
    clsRange.clear();
    list<DataPoint *> cphList;
    
    for(itr=dpList->begin(); itr!=dpList->end(); itr++){
        DataPoint* ph = *itr;
        
        cphList.push_back(ph);
    }
    
    dpList->clear();
    
    list<DataPoint *> phList;
    set<long>::iterator sitr;
    
    for(long i=minClsId; i<=maxClsId; i++){
        cluster[i-minClsId].GetPhenotypeListOfThisCluster(&phList);
        
        //cout<<"Cluster ID:"<<i<<endl;
        for(itr=phList.begin(); itr!=phList.end(); itr++){
            DataPoint* ph = *itr;
            clsRange.insert(ph->GetUniqueID());
        }
        
        for(sitr=mainClsID->begin(); sitr!=mainClsID->end(); sitr++){
            long id = *sitr;
            
            for(itr=phList.begin(); itr!=phList.end(); itr++){
                DataPoint* ph = *itr;
                
                long idList[TOTAL_CLUSTER_IDS];
                ph->GetIDList(idList);
                
                for(short j=0; j<TOTAL_CLUSTER_IDS; j++){
                    if(idList[j] == i){
                        ph->UpdateClusterID(j, 0);
                        ph->UpdateType(j, "");
                        ph->DecreaseClusterIndex();
                        
                        break;
                    }
                }
                
                if(clsRange.find(id) != clsRange.end()){
                    dpList->push_back(ph);
                }
            }
            
        }
        
        phList.clear();
        clsRange.clear();
    }
    
    /*for(itr=pList->begin(); itr!=pList->end(); itr++){
     Phenotype* ph = *itr;
     
     cout<<ph->toString();
     }*/
    
    Cluster::SetClusterID(minClsId-1);
    
    delete []cluster;
}


