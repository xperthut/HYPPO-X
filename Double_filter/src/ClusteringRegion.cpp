
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
 Md. Kamruzzaman              06/20/2017      Added logic to store global range of node values
 **************************************************************************************************/

#include "ClusteringRegion.h"
#include <iostream>
#include <time.h>
#include <set>

ClusteringRegion::ClusteringRegion(QuadTree* tree, float xMin, float yMin, float xMax, float yMax, float wSizeX, float wSizeY, float overlapX, float overlapY){
    this->tree = tree;
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
    float x1, x2, y1, y2;
    list<Phenotype*> clPhList;
    list<DataPoint*> dpList;
    string cl_data = "\"Phenotype\",\"value\"\n";
    //int v = 10;
    string s = "";
    long boxID = 1, lastID, thisID;
    
    for(int i=0; i<this->maxBlockY-1; ){
        y1 = this->yPositions[i];
        
        if(i == 0){
            y2 = this->yPositions[i+2];
        }else{
            if((i+1+3)<this->maxBlockY){
                y2 = this->yPositions[i+3];
            }else{
                y2 = this->yPositions[this->maxBlockY-1];
            }
        }
        
        for(int j=0; j<this->maxBlockX-1; ){
            x1 = this->xPositions[j];
            
            if(j == 0){
                x2 = this->xPositions[j+2];
            }else{
                if((j+1+3)<this->maxBlockX){
                    x2 = this->xPositions[j+3];
                }else{
                    x2 = this->xPositions[this->maxBlockX-1];
                }
            }
            
            clPhList.clear();
            dpList.clear();
            
            this->tree->SearchSurface(x1, y1, x2, y2, &clPhList, &dpList, boxID);
            
            if(boxID == 80){
                cout<<""; 
            }
            
            if(dpList.size() > 0 && isDp){
                
                lastID = Cluster::GetClusterID();
                
                this->CreateCluster(&dpList);
                
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
                
               // s += "BoxID:" + to_string(boxID) + "(" + to_string(x1) + "," + to_string(y1) + "," + to_string(x2) + "," + to_string(y2) + ")=>" + to_string(Cluster::GetClusterID()) + "\n";
            
            }else if(clPhList.size() > 0){
                
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
                
                //s += "BoxID:" + to_string(boxID) + "(" + to_string(x1) + "," + to_string(y1) + "," + to_string(x2) + "," + to_string(y2) + ")=>" + to_string(Cluster::GetClusterID()) + "\n";
                
            }
            
            boxID++;
            
            if(j==0){
                j++;
            }else{
                j+=2;
            }
        }
        
        if(i==0){
            i++;
        }else{
            i+=2;
        }
    }
    
    totalClusters = Cluster::GetClusterID();
    
    cout<<s<<"\nTotal cluster IDs:"<<totalClusters<<endl;
}

void ClusteringRegion::SearchAndInitializeRectangle(Grid **mesh){
    float x1, x2, y1, y2;
    list<Phenotype*> clPhList;
    list<DataPoint*> dpList;
    long boxID = 1, lastID, thisID;
    string s = "";
    
    if(mesh!=NULL){
        
        for(int j=0; j<this->totalSplitY;j++){
            for(int i=0; i<this->totalSplitX; i++){
                float cx = mesh[i][j].center_x;
                float cy = mesh[i][j].center_y;
                
                float lx = (this->windowSizeX*(1+0))/2;
                x1 = cx - lx;
                x2 = cx + lx;
                
                if(x1 < this->xMin) x1 = this->xMin;
                if(x2 > this->xMax) x2 = this->xMax;
                
                float ly = (this->windowSizeY*(1+0))/2;
                y1 = cy - ly;
                y2 = cy + ly;
                
                if(y1 < this->yMin) y1 = this->yMin;
                if(y2 > this->yMax) y2 = this->yMax;
                
                clPhList.clear();
                dpList.clear();
                
                // Testing code
                if(boxID == 52){
                    cout<<"";
                }
                
                this->tree->SearchSurface(x1, y1, x2, y2, &clPhList, &dpList, boxID);
                
                
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
                    
                    s += "BoxID:" + to_string(boxID) + "(" + to_string(x1) + "," + to_string(y1) + "," + to_string(x2) + "," + to_string(y2) + ")=>" + to_string(Cluster::GetClusterID()) + "\n";
                    
                }
                
                boxID++;
                
            }
        }
    }
    
    totalClusters = Cluster::GetClusterID();
    
    //cout<<s<<"\nTotal cluster IDs:"<<totalClusters<<endl;

}

void ClusteringRegion::SearchRectangle(){
    
    float x1, x2, y1, y2;
    list<Phenotype*> clPhList;
    list<DataPoint*> dpList;
    string cl_data = "\"Phenotype\",\"value\"\n";
    //int v = 10;
    string s = "";
    long boxID = 1, prevClsId, thisClsId;
    set<long> mainClsIDSet;
    unordered_set<string> phListTrack;
    
    for(int i=0; i<this->maxBlockY-1; ){
        y1 = this->yPositions[i];
        
        if(i == 0){
            y2 = this->yPositions[i+2];
        }else{
            if((i+1+3)<this->maxBlockY){
                y2 = this->yPositions[i+3];
            }else{
                y2 = this->yPositions[this->maxBlockY-1];
            }
        }
        
        for(int j=0; j<this->maxBlockX-1; ){
            x1 = this->xPositions[j];
            
            if(j == 0){
                x2 = this->xPositions[j+2];
            }else{
                if((j+1+3)<this->maxBlockX){
                    x2 = this->xPositions[j+3];
                }else{
                    x2 = this->xPositions[this->maxBlockX-1];
                }
            }
            
            clPhList.clear();
            dpList.clear();
            
            // Testing code
            if(boxID == 52){
                cout<<"";
            }
            
            if(IsEligibleForClustering(x1, y1, x2, y2, boxID)){
            
                this->tree->SearchSurface(x1, y1, x2, y2, &clPhList, &dpList, boxID);
            }
            
            
            if(dpList.size() > 0 && isDp){
                
                // Testing code
                /*list<DataPoint*>::iterator ditr;
                for(ditr = dpList.begin(); ditr!=dpList.end(); ditr++){
                    DataPoint* ph = *ditr;
                    if(ph->positionX >= 14.2042055 && ph->positionX <= 14.2042056 && ph->positionY >= 3.67476654 && ph->positionY <= 3.67476655){
                        cout<<"";
                        break;
                    }
                }*/
                
                //cout<<"Rect"<<"("<<x1<<","<<y1<<","<<x2<<","<<y2<<")\n";
                
                
                prevClsId = Cluster::GetClusterID();
                
                this->CreateCluster(&dpList);
                
                thisClsId = Cluster::GetClusterID();
                
                map<long, BoxRectangle>::iterator itrBRM = this->boxRectangleMap.find(boxID);
                itrBRM->second.GetClusterID(&mainClsIDSet);
                
                if(thisClsId-prevClsId > 1){
                    this->ManageCluster(&dpList, prevClsId+1, thisClsId, &mainClsIDSet);
                    
                    this->CreateCluster(&dpList);
                }
                
                //s += "BoxID:" + to_string(boxID) + "(" + to_string(x1) + "," + to_string(y1) + "," + to_string(x2) + "," + to_string(y2) + ")=>" + to_string(Cluster::GetClusterID()) + "\n";
                
                cout<<"BoxID:"<<boxID<<"("<<x1<<","<<y1<<","<<x2<<","<<y2<<")=>"<<to_string(Cluster::GetClusterID())<<"\n";
                
                //s += "\nSearching between (" + to_string(x1) + "," + to_string(y1) + ") and (" + to_string(x2) + "," + to_string(y2) + ")=>" + to_string(clPhList.size()) + ", Clusters=>" + to_string(Cluster::GetClusterID());
                
            }else if(clPhList.size() > 0){
                
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
                
                map<long, BoxRectangle>::iterator itrBRM = this->boxRectangleMap.find(boxID);
                itrBRM->second.GetClusterID(&mainClsIDSet);
                
                if(thisClsId-prevClsId > 1){
                    this->ManageCluster(&clPhList, prevClsId+1, thisClsId, &mainClsIDSet);
                    
                    this->CreateCluster(&clPhList, false);
                }
                
                //s += "BoxID:" + to_string(boxID) + "(" + to_string(x1) + "," + to_string(y1) + "," + to_string(x2) + "," + to_string(y2) + ")=>" + to_string(Cluster::GetClusterID()) + "\n";
                
                cout<<"BoxID:"<<boxID<<"("<<x1<<","<<y1<<","<<x2<<","<<y2<<")=>"<<to_string(Cluster::GetClusterID())<<"\n";
                
                //s += "\nSearching between (" + to_string(x1) + "," + to_string(y1) + ") and (" + to_string(x2) + "," + to_string(y2) + ")=>" + to_string(clPhList.size()) + ", Clusters=>" + to_string(Cluster::GetClusterID());
            }
            
            //}
            
            /*this->CopyCluster(clPhList);
            
            list<Phenotype*>::iterator itr;
            
            for(itr=clPhList.begin(); itr != clPhList.end(); itr++){
                Phenotype* ph = *itr;
                
                cl_data += "\""+to_string(ph->GetValue())+"\",\""+to_string(v)+"\"\n";
            }*/
            
            boxID++;
            mainClsIDSet.clear();
            
            //v += 10;
            
            
            if(j==0){
                j++;
            }else{
                j+=2;
            }
        }
        
        if(i==0){
            i++;
        }else{
            i+=2;
        }
    }
    
    totalClusters = Cluster::GetClusterID();
    
    cout<<s<<"\nTotal cluster IDs:"<<totalClusters<<endl;
    
    
}

void ClusteringRegion::SearchRectangle(Grid **mesh){
    float x1, x2, y1, y2;
    list<Phenotype*> clPhList;
    list<DataPoint*> dpList;
    long boxID = 1, prevClsId, thisClsId;
    set<long> mainClsIDSet;
    unordered_set<string> phListTrack;
    
    if(mesh!=NULL){
    
        for(int j=0; j<this->totalSplitY;j++){
            for(int i=0; i<this->totalSplitX; i++){
                float cx = mesh[i][j].center_x;
                float cy = mesh[i][j].center_y;
                
                float lx = (this->windowSizeX*(1+this->overlapX))/2;
                x1 = cx - lx;
                x2 = cx + lx;
                
                if(x1 < this->xMin) x1 = this->xMin;
                if(x2 > this->xMax) x2 = this->xMax;
                
                float ly = (this->windowSizeY*(1+this->overlapY))/2;
                y1 = cy - ly;
                y2 = cy + ly;
                
                if(y1 < this->yMin) y1 = this->yMin;
                if(y2 > this->yMax) y2 = this->yMax;
                
                clPhList.clear();
                dpList.clear();
                
                // Testing code
                if(boxID == 52){
                    cout<<"";
                }
                
                if(IsEligibleForClustering(x1, y1, x2, y2, boxID)){
                    
                    this->tree->SearchSurface(x1, y1, x2, y2, &clPhList, &dpList, boxID);
                }
                
                if(clPhList.size() > 0){
                    
                    prevClsId = Cluster::GetClusterID();
                    
                    this->CreateCluster(&clPhList, false);
                    
                    thisClsId = Cluster::GetClusterID();
                    
                    if(thisClsId-prevClsId > 1){
                        map<long, BoxRectangle>::iterator itrBRM = this->boxRectangleMap.find(boxID);
                        itrBRM->second.GetClusterID(&mainClsIDSet);
                        
                        this->ManageCluster(&clPhList, prevClsId+1, thisClsId, &mainClsIDSet);
                        
                        this->CreateCluster(&clPhList, false);
                    }
                    
                    //cout<<"BoxID:"<<boxID<<"("<<x1<<","<<y1<<","<<x2<<","<<y2<<")=>"<<to_string(Cluster::GetClusterID())<<"\n";
                }
                
                boxID++;
                mainClsIDSet.clear();

            }
        }
    }
    
    totalClusters = Cluster::GetClusterID();
    
    //cout<<"\nTotal cluster IDs:"<<totalClusters<<endl;
}

void ClusteringRegion::ClearCluster(){
    list<Phenotype*> clPhList;
    list<DataPoint*> dpList;
    
    clPhList.clear();
    
    this->tree->SearchSurface(this->xMin-0.5, this->yMin-0.5, this->xMax+0.5, this->yMax+0.5, &clPhList, &dpList, 0);
    
    list<Phenotype*>::iterator itr;
    set<int> varIdSet;
    
    for(itr = clPhList.begin(); itr != clPhList.end(); itr++){
        Phenotype* ph = *itr;
        
        ph->ResetClusterInformation();
        varIdSet.insert(ph->GetVarietyID());
    }
    
    list<DataPoint*>::iterator ditr;
    
    for(ditr = dpList.begin(); ditr != dpList.end(); ditr++){
        DataPoint* dp = *ditr;
        
        dp->ResetClusterInformation();
    }
    
    if(!this->isVarietyMapHasCreated && isDp){
    
        list<int> varIdList;
        set<int>::iterator sitr;
        for(sitr=varIdSet.begin(); sitr!=varIdSet.end(); sitr++){
            varIdList.push_back(*sitr);
        }
        
        varIdList.sort();
        list<int>::iterator vitr;
        int vid = 0;
        
        for(vitr=varIdList.begin(); vitr != varIdList.end(); vitr++){
            int varID = *vitr;
            
            vid++;
            this->varietyMap.insert(pair<int,int>(varID, vid));
        }
        
        this->isVarietyMapHasCreated = true;
        
        for(ditr = dpList.begin(); ditr != dpList.end(); ditr++){
            DataPoint* dp = *ditr;
            
            dp->CreateVarietyList(vid, &this->varietyMap);
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
    
    cout<<"\nMax x positions: "<<this->maxBlockX<<"\nMax y positions: "<<this->maxBlockY<<endl;
    
    float x1, x2, y1, y2;
    list<Phenotype*> phList;
    list<DataPoint*> dpList;
    
    for(int i=0; i<this->maxBlockY-1; i++){
        for(int j=0; j<this->maxBlockX-1; j++){
            x1 = this->xPositions[j];
            x2 = this->xPositions[j+1];
            y1 = this->yPositions[i];
            y2 = this->yPositions[i+1];
            
            cout<<"\nSearching between ("<<x1<<","<<y1<<") and ("<<x2<<","<<y2<<"), Result:\n";
            this->tree->SearchSurface(x1, y1, x2, y2, &phList, &dpList, 0);
            cout<<endl;
        }
    }
    
    cout<<endl;
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
    list<Phenotype*>::iterator itr;
    string str = "";
    list<Phenotype*> clPhList;
    list<DataPoint*> dpList;
    
    this->tree->SearchSurface(xMin, yMin, xMax, yMax, &clPhList, &dpList, 0);
    
    for(itr = clPhList.begin(); itr != clPhList.end(); itr++){
        Phenotype* ph = *itr;
        
        str += ph->toString();
    }
    
    return str;
}

void ClusteringRegion::GenerateGraphUsingOverlappingClusters(){
    string str;
    float x1, x2, y1, y2;
    short cIndex[2];
    list<Phenotype*> phList;
    list<DataPoint*> dpList;
    float minX=0, maxX=0, minY=0, maxY=0;
    
    for(int i=0; ((2*i)+1)<this->maxBlockY; i++){
        for(int j=0; ((2*j)+1)<this->maxBlockX; j++){
            
            phList.clear();
            dpList.clear();
            
            // Horizontal overlap
            if(2*(j+1) < this->maxBlockX){
                minX = x1 = this->xPositions[(2*j)+1];
                maxX = x2 = this->xPositions[2*(j+1)];
                
                if((2*i)-1 >= 0){
                    minY = this->yPositions[(2*i)-1];
                }else{
                    minY = this->yPositions[2*i];
                }
                
                if(2*(i+1) < this->maxBlockY){
                    maxY = this->yPositions[2*(i+1)];
                }else{
                    maxY = this->yPositions[(2*i)+1];
                }
                
                float boxCoordinate[]={minX, minY, maxX, maxY};
                int positionCoordinate[]={i,j};
                
                // Top 4 way overlap
                if(2*(i+1) < this->maxBlockY){
                    y1 = this->yPositions[(2*i)+1];
                    y2 = this->yPositions[2*(i+1)];
                    
                    this->tree->SearchSurface(x1, y1, x2, y2, &phList, &dpList, 0);
                    
                    cIndex[0] = 0;
                    cIndex[1] = 1;
                    
                    if(dpList.size() > 0 && isDp){
                        this->CreateGraph(dpList, cIndex, true, boxCoordinate, positionCoordinate, true);
                        
                    }else if(phList.size() > 0){
                        
                        this->CreateGraph(phList, cIndex, true, boxCoordinate, positionCoordinate, true);
                    }
                }
                
                phList.clear();
                dpList.clear();
                
                // Middle 2 way overlap
                y1 = this->yPositions[2*i];
                y2 = this->yPositions[(2*i)+1];
                
                this->tree->SearchSurface(x1, y1, x2, y2, &phList, &dpList, 0);
                
                cIndex[0] = 0;
                cIndex[1] = 1;
                
                if(dpList.size() > 0 && isDp){
                    this->CreateGraph(dpList, cIndex, true, boxCoordinate, positionCoordinate, true);
                    
                }else if(phList.size() > 0){
                    
                    this->CreateGraph(phList, cIndex, true, boxCoordinate, positionCoordinate, true);
                }
                
                phList.clear();
                dpList.clear();
                
                // Bottom 4 way overlap
                if((2*i)-1 >= 0){
                    y1 = this->yPositions[(2*i)-1];
                    y2 = this->yPositions[2*i];
                    
                    this->tree->SearchSurface(x1, y1, x2, y2, &phList, &dpList, 0);
                    
                    cIndex[0] = 0;
                    cIndex[1] = 1;
                    
                    if(dpList.size() > 0 && isDp){
                        this->CreateGraph(dpList, cIndex, true, boxCoordinate, positionCoordinate, true);
                        
                    }else if(phList.size() > 0){
                        
                        this->CreateGraph(phList, cIndex, true, boxCoordinate, positionCoordinate, true);
                    }
                    
                    cIndex[0] = 2;
                    cIndex[1] = 3;
                    
                    if(dpList.size() > 0 && isDp){
                        this->CreateGraph(dpList, cIndex, true, boxCoordinate, positionCoordinate, true);
                        
                    }else if(phList.size() > 0){
                        
                        this->CreateGraph(phList, cIndex, true, boxCoordinate, positionCoordinate, true);
                    }
                }
                
            }
            
            phList.clear();
            dpList.clear();
            
            // vertical overlap
            if(2*(i+1) < this->maxBlockY){
                minY = y1 = this->yPositions[(2*i)+1];
                maxY = y2 = this->yPositions[2*(i+1)];
                
                if((2*j)-1 >= 0){
                    minX = this->xPositions[(2*j)-1];
                }else{
                    minX = this->xPositions[2*j];
                }
                
                if(2*(j+1) < this->maxBlockX){
                    maxX = this->xPositions[2*(j+1)];
                }else{
                    maxX = this->xPositions[(2*j)+1];
                }
                
                float boxCoordinate[]={minX, minY, maxX, maxY};
                int positionCoordinate[]={i,j};
                
                // Right 4 way overlap
                if(2*(j+1) < this->maxBlockX){
                    x1 = this->xPositions[(2*j)+1];
                    x2 = this->xPositions[2*(j+1)];
                    
                    this->tree->SearchSurface(x1, y1, x2, y2, &phList, &dpList, 0);
                    
                    cIndex[0] = 0;
                    cIndex[1] = 2;
                    
                    if(dpList.size() > 0 && isDp){
                        this->CreateGraph(dpList, cIndex, false, boxCoordinate, positionCoordinate, true);
                        
                    }else if(phList.size() > 0){
                        
                        this->CreateGraph(phList, cIndex, false, boxCoordinate, positionCoordinate, true);
                    }
                    
                    // Cross area overlap
                    cIndex[0] = 1;
                    cIndex[1] = 2;
                    
                    if(dpList.size() > 0 && isDp){
                        this->CreateGraph(dpList, cIndex, false, boxCoordinate, positionCoordinate, true);
                        
                    }else if(phList.size() > 0){
                        
                        this->CreateGraph(phList, cIndex, false, boxCoordinate, positionCoordinate, true);
                    }
                }
                
                phList.clear();
                dpList.clear();
                
                // Middle 2 way overlap
                x1 = this->xPositions[2*j];
                x2 = this->xPositions[(2*j)+1];
                
                this->tree->SearchSurface(x1, y1, x2, y2, &phList, &dpList, 0);
                
                cIndex[0] = 0;
                cIndex[1] = 1;
                
                if(dpList.size() > 0 && isDp){
                    this->CreateGraph(dpList, cIndex, false, boxCoordinate, positionCoordinate, true);
                    
                }else if(phList.size() > 0){
                    
                    this->CreateGraph(phList, cIndex, false, boxCoordinate, positionCoordinate, true);
                }
                
                phList.clear();
                dpList.clear();
                
                // Left 4 way overlap
                if((2*j)-1 >= 0){
                    x1 = this->xPositions[(2*j)-1];
                    x2 = this->xPositions[2*j];
                    
                    this->tree->SearchSurface(x1, y1, x2, y2, &phList, &dpList, 0);
                    
                    cIndex[0] = 1;
                    cIndex[1] = 3;
                    
                    if(dpList.size() > 0 && isDp){
                        this->CreateGraph(dpList, cIndex, false, boxCoordinate, positionCoordinate, true);
                        
                    }else if(phList.size() > 0){
                        
                        this->CreateGraph(phList, cIndex, false, boxCoordinate, positionCoordinate, true);
                    }
                    
                    // Cross area overlap
                    cIndex[0] = 0;
                    cIndex[1] = 3;
                    
                    if(dpList.size() > 0 && isDp){
                        this->CreateGraph(dpList, cIndex, false, boxCoordinate, positionCoordinate, true);
                        
                    }else if(phList.size() > 0){
                        
                        this->CreateGraph(phList, cIndex, false, boxCoordinate, positionCoordinate, true);
                    }
                }
            }
            
            phList.clear();
            dpList.clear();
            
            // 4 way overlap
            if((2*(j+1)) < this->maxBlockX){
                minX = x1 = this->xPositions[(2*j)+1];
                maxX = x2 = this->xPositions[2*(j+1)];
                short clsIndex[]={0,1,2,3};
                
                // Top
                if(2*(i+1) < this->maxBlockY){
                    minY = y1 = this->yPositions[(2*i)+1];
                    maxY = y2 = this->yPositions[2*(i+1)];
                    
                    this->tree->SearchSurface(x1, y1, x2, y2, &phList, &dpList, 0);
                    
                    if(dpList.size() > 0 && isDp){
                        float boxCoordinate[]={minX, minY, maxX, maxY};
                        int positionCoordinate[]={i,j};
                        this->CreateGraph(dpList, clsIndex, true, boxCoordinate, positionCoordinate, false);
                        
                    }else if(phList.size() > 0){
                        
                        float boxCoordinate[]={minX, minY, maxX, maxY};
                        int positionCoordinate[]={i,j};
                        this->CreateGraph(phList, clsIndex, true, boxCoordinate, positionCoordinate, false);
                    }
                }
                
                phList.clear();
                dpList.clear();
                
                // Bottom
                if((2*i)-1 >= 0){
                    minY = y1 = this->yPositions[(2*i)-1];
                    maxY = y2 = this->yPositions[2*i];
                    
                    this->tree->SearchSurface(x1, y1, x2, y2, &phList, &dpList, 0);
                    
                    if(dpList.size() > 0 && isDp){
                        float boxCoordinate[]={minX, minY, maxX, maxY};
                        int positionCoordinate[]={i,j};
                        this->CreateGraph(dpList, clsIndex, true, boxCoordinate, positionCoordinate, false);
                        
                    }else if(phList.size() > 0){
                        
                        float boxCoordinate[]={minX, minY, maxX, maxY};
                        int positionCoordinate[]={i,j};
                        this->CreateGraph(phList, clsIndex, true, boxCoordinate, positionCoordinate, false);
                    }
                }
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
            
            long idList[4];
            ph->GetIDList(idList);
            vector<long> idVector;
            
            for(int i=0;i<4;i++){
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
    float x1, x2, y1, y2;
    list<Phenotype*> clPhList;
    list<DataPoint*> dpList;
    unordered_set<string> indvSet;
    unordered_set<string> edgeSet;
    long boxID = 1;
    
    for(int j=0; j<this->totalSplitY;j++){
        for(int i=0; i<this->totalSplitX; i++){
            
            clPhList.clear();
            dpList.clear();
            
            if(i>0){
                float cx2 = mesh[i][j].center_x;
                float cx1 = mesh[i-1][j].center_x;
                float cy2 = mesh[i][j].center_y;
                float cy1 = mesh[i-1][j].center_y;
                
                
                float lx = (this->windowSizeX*(1+this->overlapX))/2;
                x1 = cx2 - lx;
                x2 = cx1 + lx;
                
                float ly = (this->windowSizeY*(1+this->overlapY))/2;
                y1 = cy1 - ly;
                y2 = cy2 + ly;
                
                if(y1 < this->yMin) y1 = this->yMin;
                if(y2 > this->yMax) y2 = this->yMax;
                
                this->tree->SearchSurface(x1, y1, x2, y2, &clPhList, &dpList, boxID);
            }
            
            if(j>0){
                float cx2 = mesh[i][j].center_x;
                float cx1 = mesh[i][j-1].center_x;
                float cy2 = mesh[i][j].center_y;
                float cy1 = mesh[i][j-1].center_y;
                
                
                float lx = (this->windowSizeX*(1+this->overlapX))/2;
                x1 = cx1 - lx;
                x2 = cx2 + lx;
                
                if(x1 < this->xMin) x1 = this->xMin;
                if(x2 > this->xMax) x2 = this->xMax;
                
                float ly = (this->windowSizeY*(1+this->overlapY))/2;
                y1 = cy2 - ly;
                y2 = cy1 + ly;
                
                this->tree->SearchSurface(x1, y1, x2, y2, &clPhList, &dpList, boxID);
            }
            
            boxID++;
            
            setOverlappedIndv(&indvSet, &clPhList, overlappedPhList, edgeList, &edgeSet);
        }
    }
}

string ClusteringRegion::PrintGraph(list<Phenotype *> *overlappedPhList, list<AnEdge *> *edgeList){
    
    
    string envName[]=ENV_NAMES;
    string fileName_suffix = to_string(this->overlapX).substr(0,4) + "_" + "R" + to_string(this->Eps).substr(0,4) + "_{" + envName[0] + "," + envName[1] + "}";
    
    UnionFindWithPathCompression ufpc(Cluster::GetClusterID());
    
    for(list<AnEdge *>::iterator itr = edgeList->begin(); itr!=edgeList->end(); itr++){
        
        AnEdge* e = *itr;
        
        long node_1 = e->getNode1();
        long node_2 = e->getNode2();
        
        ufpc.Union(node_1, node_2);
    }
    
    unordered_map<long, list<long>> scc = ufpc.GetAllConnectedComponents();
    //long maxSize = 0;
    list<long> maxCC;
    
    /*cout<<"Connected components are:\n";
    for(unordered_map<long, list<long>>::iterator itr = scc.begin(); itr!=scc.end(); itr++){
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
    
    unordered_map<long, unordered_map<string, Phenotype*>> nodeList;
    unordered_map<long, unordered_map<string, Phenotype*>>::iterator nodeItr;
    
    for(list<Phenotype *>::iterator itr=overlappedPhList->begin(); itr!=overlappedPhList->end(); itr++){
        Phenotype* ph = *itr;
        
        long idList[4]={0};
        ph->GetIDList(idList);
        
        string key = ph->getIndividualId();
        
        for(short i=0; i<4; i++){
            if(idList[i]>0){
                long id = idList[i];
                
                //if(id==6) cout<<"\n"<<key<<":"<<ph->getDAP()<<endl;
                
                if((nodeItr=nodeList.find(id)) == nodeList.end()){
                    
                    unordered_map<string, Phenotype*> phMap;
                    phMap.insert(pair<string, Phenotype*>(key,ph));
                    
                    nodeList.insert(pair<long, unordered_map<string, Phenotype*>>(id, phMap));
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
    list<float> rainFallList;
    
    for(unordered_map<long, list<long>>::iterator itr = scc.begin(); itr!=scc.end(); itr++){
        
        CompositeGraph* rGraph =  mainGraph->getCC(ccIndex);
        ccIndex++;
        
        //if(maxCC.size()>0){
        list<long> lstNode = itr->second;//maxCC;
        
        float range[FILTER+2][2]={0.0};
        bool first = true;
        bool mean_first = true;
        
        for(list<long>::iterator nItr = lstNode.begin(); nItr!=lstNode.end(); nItr++){
            long nodeID = *nItr;
            
            // Create node in a CC graph
            rNode *rn = new rNode(nodeID, 0.0, 0.0);
            
            float size = 0.0;
            float sum[FILTER+1]={0.0};
            float meanValues[FILTER+1]={0.0};
            rainFallList.clear();
            
            unordered_map<long, unordered_map<string, Phenotype*>>::iterator nodeItr = nodeList.find(nodeID);
            
            if(nodeItr!=nodeList.end()){
                for(unordered_map<string, Phenotype*>::iterator phItr=nodeItr->second.begin(); phItr!=nodeItr->second.end(); phItr++){
                    
                    Phenotype* ph = phItr->second;
                    
                    // Add rainfall record
                    if(ph->getEnvValue(3)>0) rainFallList.push_back(ph->getEnvValue(3));
                    
                    if(PLANT_ID_LIST){
                        list<long> pidList = ph->getPlantIdList();
                        
                        for(list<long>::iterator piditr = pidList.begin(); piditr!=pidList.end(); piditr++){
                            rn->setPhenotypeSummary(ph->getGenotype(), ph->GetSite(), *piditr, ph->getDAP(), ph->GetValue());
                        }
                    }else{
                        rn->setPhenotypeSummary(ph->getGenotype(), ph->GetSite(), ph->GetVariety(), ph->getDAP(), ph->GetValue());
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
                
                // size: min, max
                range[0][0] = size;
                range[0][1] = size;
                
            }else{
                // size: min, max
                if(range[0][0]>size) range[0][0]=size;
                if(range[0][1]<size) range[0][1]=size;
                
            }
            
            for(short e=0; e<=FILTER;e++){
                meanValues[e] = sum[e]/nodeItr->second.size();
                
                //if(e==0){
                    if(mean_first){
                        
                        range[e+1][0]=meanValues[e];
                        range[e+1][1]=meanValues[e];
                    }else{
                        if(range[e+1][0]>meanValues[e]) range[e+1][0]=meanValues[e];
                        if(range[e+1][1]<meanValues[e]) range[e+1][1]=meanValues[e];
                    }
                //}
                
            }
            
            mean_first = false;
            
            rn->setSize(size);
            rn->setWeight(meanValues[0]);
            
            for(int e=0;e<FILTER;e++){
                rn->setEnvWeight(meanValues[e+1], e);
            }
            rn->setVariations(nodeItr->second.size());
            
            rGraph->addNode(rn);
            
            
            if(rainFallList.size()>0&&false){
                string s="";
                float t=0;
                for(list<float>::iterator rItr=rainFallList.begin(); rItr!=rainFallList.end(); rItr++){
                    if(s.length()>0) s+=",";
                    float rainFall = (*rItr)*25.4;
                    
                    s += to_string(rainFall);
                    t += rainFall;
                }
                cout<<"\n"<<nodeID<<":{"<<s<<"}["<<t/rainFallList.size()<<"]";
            }
        }
        
        if(!PRINT_BARCODE&&false){
            cout<<"\n\nRanges:";
            for(short e=0;e<FILTER;e++){
                cout<<"\n\t"<<envName[e]<<":(" + to_string(range[e+2][0]) + "," + to_string(range[e+2][1]) + ")";
            }
            cout<<"\n\tGR:(" + to_string(range[1][0]) + "," + to_string(range[1][1]) + ")";
        }
        
        float nw[] = {range[1][0],range[1][1]};
        float ns[] = {range[0][0],range[0][1]};
        float ew[FILTER][2]={0};
        for(int e=0;e<FILTER;e++){
            ew[e][0] = range[e+2][0];
            ew[e][1]=range[e+2][1];
        }
        //rGraph->storeRange(ns, nw, ew, envName);
        mainGraph->setRangeValues(rGraph, ns, nw, ew, envName);
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
    float x1, x2, y1,y2;
    overlap = overlap*50;
    list<Phenotype*> clPhList;
    list<DataPoint*> dpList;
    long boxID = 1;
    
    if(mesh!=NULL){
        sc->clearTracker();
        
        for(int j=0; j<this->totalSplitY;j++){
            for(int i=0; i<this->totalSplitX; i++){
                
                clPhList.clear();
                dpList.clear();
                
                float cx = mesh[i][j].center_x;
                float cy = mesh[i][j].center_y;
                
                float lx = (this->windowSizeX*(1+0))/2;
                x1 = cx - lx;
                x2 = cx + lx;
                
                if(x1 < this->xMin) x1 = this->xMin;
                if(x2 > this->xMax) x2 = this->xMax;
                
                float ly = (this->windowSizeY*(1+0))/2;
                y1 = cy - ly;
                y2 = cy + ly;
                
                if(y1 < this->yMin) y1 = this->yMin;
                if(y2 > this->yMax) y2 = this->yMax;
                
                this->tree->SearchSurface(x1, y1, x2, y2, &clPhList, &dpList, boxID);
                
                for(list<Phenotype*>::iterator pItr = clPhList.begin(); pItr!=clPhList.end(); pItr++){
                    Phenotype* ph = *pItr;
                    
                    sc->AddSimplicialComplex(ph, overlap);
                }
                
                boxID++;
            }
        }
    }
}

string ClusteringRegion::PrintGraph(SimplicialComplex* sc){
    string str = "";
    float minVal = 16777215.0, maxVal = 0.0, minMeanVal = 16777215.0, maxMeanVal = 0.0;
    bool isPh = true;
    bool isTemp = false;
    long totalNodes = 0;
    string type = "";
    
    string envName[]={"temp","sr"};
    
    if(isPh){
        type = "ph";
    }else if(isTemp){
        type = envName[0];
    }else{
        type = envName[1];
    }
    
    // o=.5, lx=2, ly=.5
    set<long>nodeList;
    string fileName_suffix = to_string(this->overlapX).substr(0,4) + "_" + to_string(this->overlapY).substr(0,4) + "R" + to_string(this->Eps).substr(0,4);
    
    
    // Print isolated clusters
    long totalClusters = Cluster::GetClusterID();
    for(long nnid=1; nnid <= totalClusters; nnid++){
        sc->AddZeroSimplex(nnid, 0.00);
    }
    
    UnionFindWithPathCompression ufpc(Cluster::GetClusterID());
    
    for(this->graph.edgeItr = this->graph.edgeList.begin(); this->graph.edgeItr != this->graph.edgeList.end(); this->graph.edgeItr++){
        
        long node_1 = this->graph.edgeItr->second.node1;
        long node_2 = this->graph.edgeItr->second.node2;
        
        ufpc.Union(node_1, node_2);
    }
    
    unordered_map<long, list<long>> scc = ufpc.GetAllConnectedComponents();
    unordered_map<long, unordered_map<string, list<float>>>::iterator vItr;
    unordered_map<string, list<float>>::iterator kItr;
    unordered_map<long, list<long>>::iterator mitr;
    long maxSize = 0;
    list<long> maxCC;
    list<long>::iterator litr;
    float minPh, maxPh;
    
    cout<<"Connected components are:\n";
    
    for(mitr = scc.begin(); mitr!=scc.end(); mitr++){
        long index = mitr->first;
        list<long> lst = mitr->second;
        
        if(lst.size() > maxSize){
            maxSize = lst.size();
            maxCC = lst;
        }
        
        cout<<index<<":"<<lst.size()<<" ";
        
        for(litr = lst.begin(); litr!=lst.end(); litr++){
            long ide = *litr;
            
            cout<<ide<<",";
        }
        
        cout<<"\n\n";
    }
    
    unordered_map<string,list<Phenotype*>>::iterator nodeItr;
    list<Phenotype*>::iterator nphItr;
    
    clock_t start_Time = clock();
    long lastTime = 0;
    
    int total_CC = (int)scc.size();
    MainGraph* mainGraph = new MainGraph(total_CC);
    mainGraph->setEnvName(envName);
    int ccIndex = 0;
    
    if(MAX_CONNECTED_COMPONENT_ONLY){
        total_CC=1;
        CompositeGraph* rGraph =  mainGraph->getCC(ccIndex);
        list<long> lstNode = maxCC;
        list<long>::iterator nlItr;
        
        this->gmlGraph.gmlNodeList.clear();
        
        minVal = 16777215.0;
        maxVal = 0.0;
        minMeanVal = 16777215.0;
        maxMeanVal = 0.0;
        float minX[FILTER] = {16777215.0};
        float maxX[FILTER] = {0.0};
        minPh = 16777215.0;
        maxPh = 0.0;
        float tmpMax = 0;
        float tmpMin= 16777215.0;
        string gr_dist="";
        
        for(int e=0;e<FILTER;e++){
            minX[e] = 16777215.0;
            maxX[e] = 0.0;
        }
        
        for(nlItr = lstNode.begin(); nlItr!=lstNode.end(); nlItr++){
            long ide = *nlItr;
            
            nodeItr = this->graph.node.nodeList.find(to_string(ide));
            
            string id = nodeItr->first;
            
            float size = 0;
            float sum = 0.0;
            float sumX[FILTER] = {0.0};
            GmlNode node;
            node.id = ide;
            
            tmpMax=0;
            tmpMin= 16777215.0;
            gr_dist="";
            
            list<Phenotype*> nodePhList = nodeItr->second;
            for(nphItr = nodePhList.begin(); nphItr != nodePhList.end(); nphItr++){
                Phenotype* ph = *nphItr;
                sum += ph->GetValue();
                
                if(minPh>ph->GetValue())
                    minPh=ph->GetValue();
                
                if(tmpMin>ph->GetValue())
                    tmpMin=ph->GetValue();
                
                if(maxPh<ph->GetValue())
                    maxPh=ph->GetValue();
                
                if(tmpMax<ph->GetValue())
                    tmpMax=ph->GetValue();
                
                //if(gr_dist.length()>0) gr_dist+=",";
                //gr_dist += to_string(ph->GetValue());
                
                size += ph->GetWeight(stol(id));
                
                for(int e=0;e<FILTER;e++){
                    sumX[e] += ph->getCoOrdinate(e);
                }
                
                string key = ph->GetSite()+"_"+ph->getGenotype();
                
                if((vItr = this->graph.node.phenotypeVariation.find(ide)) == this->graph.node.phenotypeVariation.end()){
                    
                    list<float> alist;
                    alist.push_back(ph->GetValue());
                    
                    unordered_map<string, list<float>> aMap;
                    aMap.insert(pair<string, list<float>>(key,alist));
                    
                    this->graph.node.phenotypeVariation.insert(pair<long, unordered_map<string, list<float>>>(ide, aMap));
                }else{
                    
                    if((kItr = vItr->second.find(key))==vItr->second.end()){
                        list<float> alist;
                        alist.push_back(ph->GetValue());
                        
                        vItr->second.insert(pair<string, list<float>>(key, alist));
                    }else{
                        kItr->second.push_back(ph->GetValue());
                    }
                }
                
                for(int e=0;e<FILTER;e++){
                    if((vItr = this->graph.node.envVariation[e].find(ide)) == this->graph.node.envVariation[e].end()){
                        
                        list<float> alist;
                        
                        alist.push_back(ph->getCoOrdinate(e));
                        
                        
                        unordered_map<string, list<float>> aMap;
                        aMap.insert(pair<string, list<float>>(key,alist));
                        
                        this->graph.node.envVariation[e].insert(pair<long, unordered_map<string, list<float>>>(ide, aMap));
                    }else{
                        
                        if((kItr = vItr->second.find(key))==vItr->second.end()){
                            list<float> alist;
                            
                            alist.push_back(ph->getCoOrdinate(e));
                            
                            vItr->second.insert(pair<string, list<float>>(key, alist));
                        }else{
                            kItr->second.push_back(ph->getCoOrdinate(e));
                        }
                    }
                }
            }
            
            if(tmpMax==10.0||true){
                //cout<<"\n\nGot max at nodeid:"<<ide<<": ["<<tmpMin<<","<<tmpMax<<","<<nodePhList.size()<<","<<(sum/nodePhList.size())<<"]";
                //cout<<"]==[c("<<gr_dist<<")]";
            }
            
            vItr = this->graph.node.phenotypeVariation.find(ide);
            float score[4]= {0,0,0,0};
            float finalScore=0;
            
            int c=0;
            for(kItr=vItr->second.begin(); kItr!=vItr->second.end();kItr++){
                score[c]=rGraph->getMeanValue(kItr->second);
                finalScore += (score[c]*score[c]);
                c++;
            }
            
            finalScore=sqrt(finalScore/c);
            
            //float avg = finalScore;
            float avg = sum/nodePhList.size();
            
            score[0]=score[1]=score[2]=score[3]=0;
            finalScore=0;
            c=0;
            vItr = this->graph.node.envVariation[0].find(ide);
            for(kItr=vItr->second.begin(); kItr!=vItr->second.end();kItr++){
                score[c]=rGraph->getMeanValue(kItr->second);
                finalScore += (score[c]*score[c]);
                c++;
            }
            
            finalScore=sqrt(finalScore/c);
            
            //float avgX = finalScore;
            float avgX[FILTER]={0};
            for(int e=0;e<FILTER;e++){
                avgX[e]=sumX[e]/nodePhList.size();
            }
            
            score[0]=score[1]=score[2]=score[3]=0;
            finalScore=0;
            c=0;
            vItr = this->graph.node.envVariation[1].find(ide);
            for(kItr=vItr->second.begin(); kItr!=vItr->second.end();kItr++){
                score[c]=rGraph->getMeanValue(kItr->second);
                finalScore += (score[c]*score[c]);
                c++;
            }
            
            finalScore=sqrt(finalScore/c);
            
            vItr = this->graph.node.phenotypeVariation.find(ide);
            
            node.size = size;
            node.avgValue = avg;
            
            for(int e=0;e<FILTER;e++){
                node.avgValueX[e] = avgX[e];
            }
            
            this->gmlGraph.gmlNodeList.push_back(node);
            
            rNode *rn = new rNode(ide, avg, size);
            for(int e=0;e<FILTER;e++){
                rn->setEnvWeight(avgX[e], e);
            }
            rn->setVariations(vItr->second.size());
            
            nodePhList = nodeItr->second;
            for(nphItr = nodePhList.begin(); nphItr != nodePhList.end(); nphItr++){
                Phenotype* ph = *nphItr;
                
                if(PLANT_ID_LIST){
                    list<long> pidList = ph->getPlantIdList();
                    
                    for(list<long>::iterator litr = pidList.begin(); litr!=pidList.end(); litr++){
                        rn->setPhenotypeSummary(ph->getGenotype(), ph->GetSite(), *litr, ph->getDAP(), ph->GetValue());
                    }
                }else{
                    rn->setPhenotypeSummary(ph->getGenotype(), ph->GetSite(), ph->GetVariety(), ph->getDAP(), ph->GetValue());
                }
            }
            
            rGraph->addNode(rn);
            
            if(minVal > size && size > 0) minVal = size;
            if(maxVal < size) maxVal = size;
            
            if(maxMeanVal < avg) maxMeanVal = avg;
            if(minMeanVal > avg && avg > 0) minMeanVal = avg;
            
            for(int e=0;e<FILTER;e++){
                if(maxX[e] < avgX[e]) maxX[e] = avgX[e];
                if(minX[e] > avgX[e] && avgX[e] > 0) minX[e] = avgX[e];
            }
        }
        
        cout<<"\n\nRanges:\n\tTemp:(" + to_string(minX[0]) + "," + to_string(maxX[0]) + ")";
        cout<<"\n\tSol.Rad:(" + to_string(minX[1]) + "," + to_string(maxX[1]) + ")";
        cout<<"\n\tHumidity:(" + to_string(minX[2]) + "," + to_string(maxX[2]) + ")";
        cout<<"\n\tGR:(" + to_string(minPh) + "," + to_string(maxPh) + ")";
        cout<<"\n\tAvg.GR:(" + to_string(minMeanVal) + "," + to_string(maxMeanVal) + ")\n";
        
        float nw[] = {minMeanVal,maxMeanVal};
        float ns[] = {minVal,maxVal};
        float ew[FILTER][2]={0};
        for(int e=0;e<FILTER;e++){
            ew[e][0] = minX[e];
            ew[e][1]=maxX[e];
        }
        
        
        rGraph->storeRange(ns, nw, ew, envName);
        
        //start_Time = clock();
        
        for(this->gmlGraph.gnItr = this->gmlGraph.gmlNodeList.begin(); this->gmlGraph.gnItr != this->gmlGraph.gmlNodeList.end(); this->gmlGraph.gnItr++){
            
            GmlNode node = *this->gmlGraph.gnItr;
            
            node.adjustColor(minMeanVal, maxMeanVal);
            
            for(int e=0;e<FILTER;e++){
                node.adjustColorByX(minX[e], maxX[e],e);
            }
            
            node.adjustSize(minVal, maxVal);
            
            lastTime = (clock()-start_Time)/CLK_TCK;
            //str_non_isolatedVertices += "stream.addVertex(" + to_string(node.id) + ", " + to_string(this->overlapX*100).substr(0,5) + ");\n";
            //sc->AddZeroSimplex(node.id, this->overlapX*50);
            
            if(nodeList.find(node.id) == nodeList.end()){
                nodeList.insert(node.id);
            }
            
            totalNodes++;
            
        }
    
    }else{
        for(mitr = scc.begin(); mitr!=scc.end(); mitr++){
            
            CompositeGraph* rGraph =  mainGraph->getCC(ccIndex);
            ccIndex++;
            
            //if(maxCC.size()>0){
            list<long> lstNode = mitr->second;//maxCC;
            list<long>::iterator nlItr;
            
            this->gmlGraph.gmlNodeList.clear();
            
            minVal = 16777215.0;
            maxVal = 0.0;
            minMeanVal = 16777215.0;
            maxMeanVal = 0.0;
            float minX[FILTER] = {16777215.0};
            float maxX[FILTER] = {0.0};
            minPh = 16777215.0;
            maxPh = 0.0;
            float tmpMax = 0;
            float tmpMin= 16777215.0;
            string gr_dist="";
            
            for(int e=0;e<FILTER;e++){
                minX[e] = 16777215.0;
                maxX[e] = 0.0;
            }
            
            for(nlItr = lstNode.begin(); nlItr!=lstNode.end(); nlItr++){
                long ide = *nlItr;
                
                if(ide==15)
                    cout<<"";
                
                nodeItr = this->graph.node.nodeList.find(to_string(ide));
                
                string id = nodeItr->first;
                
                float size = 0;
                float sum = 0.0;
                float sumX[FILTER] = {0.0};
                GmlNode node;
                node.id = ide;
                
                tmpMax=0;
                tmpMin= 16777215.0;
                gr_dist="";
                
                list<Phenotype*> nodePhList = nodeItr->second;
                for(nphItr = nodePhList.begin(); nphItr != nodePhList.end(); nphItr++){
                    Phenotype* ph = *nphItr;
                    sum += ph->GetValue();
                    
                    if(ph->GetValue()>9){
                        cout<<"";
                    }
                    
                    if(minPh>ph->GetValue())
                        minPh=ph->GetValue();
                    
                    if(tmpMin>ph->GetValue())
                        tmpMin=ph->GetValue();
                    
                    if(maxPh<ph->GetValue())
                        maxPh=ph->GetValue();
                    
                    if(tmpMax<ph->GetValue())
                        tmpMax=ph->GetValue();
                    
                    if(gr_dist.length()>0) gr_dist+=",";
                    gr_dist += to_string(ph->GetValue());
                    
                    size += ph->GetWeight(stol(id));
                    
                    for(int e=0;e<FILTER;e++){
                        sumX[e] += ph->getCoOrdinate(e);
                    }
                    
                    string key = ph->GetSite()+"_"+ph->getGenotype();
                    
                    if((vItr = this->graph.node.phenotypeVariation.find(ide)) == this->graph.node.phenotypeVariation.end()){
                        
                        list<float> alist;
                        alist.push_back(ph->GetValue());
                        
                        unordered_map<string, list<float>> aMap;
                        aMap.insert(pair<string, list<float>>(key,alist));
                        
                        this->graph.node.phenotypeVariation.insert(pair<long, unordered_map<string, list<float>>>(ide, aMap));
                    }else{
                        
                        if((kItr = vItr->second.find(key))==vItr->second.end()){
                            list<float> alist;
                            alist.push_back(ph->GetValue());
                            
                            vItr->second.insert(pair<string, list<float>>(key, alist));
                        }else{
                            kItr->second.push_back(ph->GetValue());
                        }
                    }
                    
                    for(int e=0;e<FILTER;e++){
                        if((vItr = this->graph.node.envVariation[e].find(ide)) == this->graph.node.envVariation[e].end()){
                            
                            list<float> alist;
                            
                            alist.push_back(ph->getCoOrdinate(e));
                            
                            
                            unordered_map<string, list<float>> aMap;
                            aMap.insert(pair<string, list<float>>(key,alist));
                            
                            this->graph.node.envVariation[e].insert(pair<long, unordered_map<string, list<float>>>(ide, aMap));
                        }else{
                            
                            if((kItr = vItr->second.find(key))==vItr->second.end()){
                                list<float> alist;
                                
                                alist.push_back(ph->getCoOrdinate(e));
                                
                                vItr->second.insert(pair<string, list<float>>(key, alist));
                            }else{
                                kItr->second.push_back(ph->getCoOrdinate(e));
                            }
                        }
                    }
                }
                
                if(tmpMax==199.8487||true){
                    cout<<"\nRange at nodeid:"<<ide<<": ["<<tmpMin<<","<<tmpMax<<","<<nodePhList.size()<<","<<(sum/nodePhList.size())<<"]";
                    cout<<"]==[c("<<gr_dist<<")]";
                }
                
                vItr = this->graph.node.phenotypeVariation.find(ide);
                float score[4]= {0,0,0,0};
                float finalScore=0;
                
                int c=0;
                for(kItr=vItr->second.begin(); kItr!=vItr->second.end();kItr++){
                    score[c]=rGraph->getMeanValue(kItr->second);
                    finalScore += (score[c]*score[c]);
                    c++;
                }
                
                finalScore=sqrt(finalScore/c);
                
                //float avg = finalScore;
                float avg = sum/nodePhList.size();
                
                score[0]=score[1]=score[2]=score[3]=0;
                finalScore=0;
                c=0;
                vItr = this->graph.node.envVariation[0].find(ide);
                for(kItr=vItr->second.begin(); kItr!=vItr->second.end();kItr++){
                    score[c]=rGraph->getMeanValue(kItr->second);
                    finalScore += (score[c]*score[c]);
                    c++;
                }
                
                finalScore=sqrt(finalScore/c);
                
                //float avgX = finalScore;
                float avgX[FILTER]={0};
                for(int e=0;e<FILTER;e++){
                    avgX[e]=sumX[e]/nodePhList.size();
                }
                
                score[0]=score[1]=score[2]=score[3]=0;
                finalScore=0;
                c=0;
                vItr = this->graph.node.envVariation[1].find(ide);
                for(kItr=vItr->second.begin(); kItr!=vItr->second.end();kItr++){
                    score[c]=rGraph->getMeanValue(kItr->second);
                    finalScore += (score[c]*score[c]);
                    c++;
                }
                
                finalScore=sqrt(finalScore/c);
                
                vItr = this->graph.node.phenotypeVariation.find(ide);
                
                node.size = size;
                node.avgValue = avg;
                
                for(int e=0;e<FILTER;e++){
                    node.avgValueX[e] = avgX[e];
                }
                
                this->gmlGraph.gmlNodeList.push_back(node);
                
                rNode *rn = new rNode(ide, avg, size);
                for(int e=0;e<FILTER;e++){
                    rn->setEnvWeight(avgX[e], e);
                }
                rn->setVariations(vItr->second.size());
                
                nodePhList = nodeItr->second;
                for(nphItr = nodePhList.begin(); nphItr != nodePhList.end(); nphItr++){
                    Phenotype* ph = *nphItr;
                    
                    if(PLANT_ID_LIST){
                        list<long> pidList = ph->getPlantIdList();
                        
                        for(list<long>::iterator litr = pidList.begin(); litr!=pidList.end(); litr++){
                            rn->setPhenotypeSummary(ph->getGenotype(), ph->GetSite(), *litr, ph->getDAP(), ph->GetValue());
                        }
                    }else{
                        rn->setPhenotypeSummary(ph->getGenotype(), ph->GetSite(), ph->GetVariety(), ph->getDAP(), ph->GetValue());
                    }
                }
                
                rGraph->addNode(rn);
                
                if(minVal > size && size > 0) minVal = size;
                if(maxVal < size) maxVal = size;
                
                if(maxMeanVal < avg) maxMeanVal = avg;
                if(minMeanVal > avg && avg > 0) minMeanVal = avg;
                
                for(int e=0;e<FILTER;e++){
                    if(maxX[e] < avgX[e]) maxX[e] = avgX[e];
                    if(minX[e] > avgX[e] && avgX[e] > 0) minX[e] = avgX[e];
                }
            }
            
            cout<<"\n\nRanges:\n\tTemp:(" + to_string(minX[0]) + "," + to_string(maxX[0]) + ")";
            cout<<"\n\tHumidity:(" + to_string(minX[1]) + "," + to_string(maxX[1]) + ")";
            cout<<"\n\tGR:(" + to_string(minPh) + "," + to_string(maxPh) + ")";
            cout<<"\n\tAvg.GR:(" + to_string(minMeanVal) + "," + to_string(maxMeanVal) + ")\n";
            
            float nw[] = {minMeanVal,maxMeanVal};
            float ns[] = {minVal,maxVal};
            float ew[FILTER][2]={0};
            for(int e=0;e<FILTER;e++){
                ew[e][0] = minX[e];
                ew[e][1]=maxX[e];
            }
            
            
            rGraph->storeRange(ns, nw, ew, envName);
            
            //start_Time = clock();
            
            for(this->gmlGraph.gnItr = this->gmlGraph.gmlNodeList.begin(); this->gmlGraph.gnItr != this->gmlGraph.gmlNodeList.end(); this->gmlGraph.gnItr++){
                
                GmlNode node = *this->gmlGraph.gnItr;
                
                node.adjustColor(minMeanVal, maxMeanVal);
                
                for(int e=0;e<FILTER;e++){
                    node.adjustColorByX(minX[e], maxX[e],e);
                }
                
                node.adjustSize(minVal, maxVal);
                
                lastTime = (clock()-start_Time)/CLK_TCK;
                //str_non_isolatedVertices += "stream.addVertex(" + to_string(node.id) + ", " + to_string(this->overlapX*100).substr(0,5) + ");\n";
                //sc->AddZeroSimplex(node.id, this->overlapX*50);
                
                if(nodeList.find(node.id) == nodeList.end()){
                    nodeList.insert(node.id);
                }
                
                totalNodes++;
                
            }
            //}
        }
    
    }
    
    unordered_map<string, pair<float, Box>>::iterator edgeItr;
    unordered_map<string, Edges> edgeListFinal;
    
    for(edgeItr=this->graph.edge.edgeList.begin(); edgeItr != this->graph.edge.edgeList.end(); edgeItr++){
        Box b = edgeItr->second.second;
        
        str += b.toString() + "Face weight:" + to_string(edgeItr->second.first) + "\n";
        
        list<Phenotype*> phList = b.phenoList;
        if(phList.size() > 0){
            list<Phenotype*>::iterator itr;
            
            for(itr = phList.begin(); itr != phList.end(); itr++){
                Phenotype* ph = *itr;
                
                if(b.twoWayOverlap){
                    short id[2], k=0;
                    
                    for(short i=0; i<4; i++){
                        if(b.clusterIndex[i] != -1){
                            id[k] = i;
                            k++;
                        }
                    }
                    
                    str += ph->toString(id[0], id[1]);
                }else{
                    str += ph->toString();
                }
            }
        }
    }
    
    start_Time = clock();
    
    minVal = 16777215.0, maxVal=0.0;
    
    for(this->graph.edgeItr = this->graph.edgeList.begin(); this->graph.edgeItr != this->graph.edgeList.end(); this->graph.edgeItr++){
        
        for(int i=0; i<total_CC; i++){
            CompositeGraph* rGraph = mainGraph->getCC(i);
            
            if(rGraph->hasNode(this->graph.edgeItr->second.node1, this->graph.edgeItr->second.node2)){
                rGraph->addEdge(this->graph.edgeItr->second.node1, this->graph.edgeItr->second.node2);
                
                //if(this->graph.edgeItr->second.face_weight > 0){
                
                float w = this->graph.edgeItr->second.face_weight;
                
                if(minVal > w && w>0) minVal = w;
                if(maxVal < w) maxVal = w;
                
                GmlEdge edge;
                
                edge.node1 = this->graph.edgeItr->second.node1;
                edge.node2 = this->graph.edgeItr->second.node2;
                edge.weight = this->graph.edgeItr->second.face_weight;
                
                this->gmlGraph.gmlEdgeList.push_back(edge);
                
                //}
            }
        }
        
    }
    
    for(this->gmlGraph.geItr = this->gmlGraph.gmlEdgeList.begin(); this->gmlGraph.geItr != this->gmlGraph.gmlEdgeList.end(); this->gmlGraph.geItr++){
        GmlEdge edge = *this->gmlGraph.geItr;
        edge.adjustWeight(minVal, maxVal);
        
        string face_weight = to_string(edge.weight);
        long pos = 0;
        if((pos =face_weight.find(".")) != string::npos){
            pos += 3;
            face_weight = face_weight.substr(0, pos);
        }
        
        //str_edges += "stream.addElement(new int[]{" + to_string(edge.node1) + ", " + to_string(edge.node2) + "},  " + to_string(this->overlapX*100).substr(0,5) + ");\n";
        sc->AddOneSimplex(edge.node1, edge.node2, this->overlapX*50);
        
        //edges += "edge\n[\n source " + to_string(edge.node1) + "\n target " + to_string(edge.node2) + "\n width " + to_string(edge.relWeight) + "\n color \"" + edge.color + "\"\n]\n";
    }
    
    map<long, string>::iterator simItr;
    
    if(this->tries.size() > 0){
        for(simItr = this->tries.begin(); simItr != this->tries.end(); simItr++){
            string val = simItr->second + ",";
            
            short pos = -1, index=0;
            long tempID[] = {-1,-1,-1};
            
            while((pos = val.find(",")) != string::npos){
                tempID[index] = stol(val.substr(0, pos));
                val = val.substr(pos+1, val.length());
                index++;
            }
            
            //str_triangles += "stream.addElement(new int[]{" + to_string(tempID[0]) + ", " + to_string(tempID[1]) + ", " + to_string(tempID[2]) + "},  " + to_string(this->overlapX*100).substr(0,5) + ");\n";
            sc->AddTwoSimplex(tempID[0], tempID[1], tempID[2], this->overlapX*50);
        }
        
    }
    
    if(this->tetras.size() > 0){
        for(simItr = this->tetras.begin(); simItr != this->tetras.end(); simItr++){
            string val = simItr->second + ",";;
            short pos = -1, index=0;
            long tempID[] = {-1,-1,-1,-1};
            
            while((pos = val.find(",")) != string::npos){
                tempID[index] = stol(val.substr(0, pos));
                val = val.substr(pos+1, val.length());
                index++;
            }
            
            sc->AddTwoSimplex(tempID[0], tempID[1], tempID[2], this->overlapX*50);
            
            sc->AddTwoSimplex(tempID[0], tempID[2], tempID[3], this->overlapX*50);
            
            sc->AddTwoSimplex(tempID[0], tempID[1], tempID[3], this->overlapX*50);
            
            sc->AddTwoSimplex(tempID[1], tempID[2], tempID[3], this->overlapX*50);
            
            sc->AddThreeSimplex(tempID[0], tempID[1], tempID[2], tempID[3], this->overlapX*50);
        }
        
    }
    
    mainGraph->printAllGraphs(fileName_suffix);
    
    if(!PRINT_BARCODE)
        menu(mainGraph, &this->graph, fileName_suffix, total_CC);
    
    delete mainGraph;
    
    cout<<"\n("<<totalNodes<<"/"<<Cluster::GetClusterID()<<")"<<endl;
    
    //cout<<str_isolatedVertices<<str_non_isolatedVertices<<str_edges<<str_triangles<<str_tetrahiddrens<<endl;
    
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
    /*string str = "";
    //string nodes = "graph\n[\ndirected 0\nweighted 1\n";
    //string edges = "";
    //string gmlData = "";
    float minVal = 16777215.0, maxVal = 0.0, minMeanVal = 16777215.0, maxMeanVal = 0.0;
    float minX = 16777215.0, minY = 16777215.0, maxX = 0.0, maxY = 0.0;
    bool isPh = false;
    bool isTemp = true;
    long totalNodes = 0;
    string type = "";
    CompositeGraph* rGraph = new CompositeGraph();
    
    if(isPh){
        type = "ph";
    }else if(isTemp){
        type = "silt";
    }else{
        type = "prec";
    }
    
    string envName[]={"silt","prec"};
    
    // o=.5, lx=2, ly=.5
    set<long>nodeList;
    //string fileName = "graph_"+ type +"_" + to_string(this->windowSizeX).substr(0,3) + "_" + to_string(this->windowSizeY).substr(0,3) +"_" + to_string(this->shiftX).substr(0,4) + "_" + to_string(this->shiftY).substr(0,4) + "_" + to_string(this->overlapX).substr(0,4) + "_" + to_string(this->overlapY).substr(0,4) + "R" + to_string(this->Eps).substr(0,2);
    
    string fileName_suffix = to_string(this->windowSizeX).substr(0,3) + "_" + to_string(this->windowSizeY).substr(0,3) +
        "_" + to_string(this->shiftX).substr(0,4) + "_" + to_string(this->shiftY).substr(0,4) + "_" +
        to_string(this->overlapX).substr(0,4) + "_" + to_string(this->overlapY).substr(0,4) + "R" + to_string(this->Eps).substr(0,2);
    
    // Print isolated clusters
    long totalClusters = Cluster::GetClusterID();
    for(long nnid=1; nnid <= totalClusters; nnid++){
        sc->AddZeroSimplex(nnid, 0.00);
    }
    
    UnionFindWithPathCompression ufpc(Cluster::GetClusterID());
    
    for(this->graph.edgeItr = this->graph.edgeList.begin(); this->graph.edgeItr != this->graph.edgeList.end(); this->graph.edgeItr++){
        
        long node_1 = this->graph.edgeItr->second.node1;
        long node_2 = this->graph.edgeItr->second.node2;
        
        ufpc.Union(node_1, node_2);
    }
    
    unordered_map<long, list<long>> scc = ufpc.GetAllConnectedComponents();
    unordered_map<long, list<long>>::iterator mitr;
    long maxSize = 0;
    list<long> maxCC;
    
    cout<<"Connected components are:\n";
    
    for(mitr = scc.begin(); mitr!=scc.end(); mitr++){
        long index = mitr->first;
        list<long> lst = mitr->second;
        list<long>::iterator litr;
        
        if(lst.size() > maxSize){
            maxSize = lst.size();
            maxCC = lst;
        }
        
        cout<<index<<": ";
        
        for(litr = lst.begin(); litr!=lst.end(); litr++){
            long ide = *litr;
            
            cout<<ide<<",";
        }
        
        cout<<"\n\n";
    }
    
    unordered_map<string,list<DataPoint*>>::iterator nodeItr;
    list<DataPoint*>::iterator nphItr;
    
    clock_t start_Time = clock();
    long lastTime = 0;
    
    //for(mitr = scc.begin(); mitr!=scc.end(); mitr++){
    if(maxCC.size()>0){
        list<long> lstNode = maxCC;
        list<long>::iterator nlItr;
        
        this->gmlGraph.gmlNodeList.clear();
        
        minVal = 16777215.0;
        maxVal = 0.0;
        minMeanVal = 16777215.0;
        maxMeanVal = 0.0;
        minX = 16777215.0;
        minY = 16777215.0;
        maxX = 0.0;
        maxY = 0.0;
        
        for(nlItr = lstNode.begin(); nlItr!=lstNode.end(); nlItr++){
            long ide = *nlItr;
            
            nodeItr = this->graph.node.nodeListDP.find(to_string(ide));
            
            string id = nodeItr->first;
            
            float size = 0;
            float sum = 0.0;
            float sumX = 0.0, sumY = 0.0;
            GmlNode node;
            node.id = ide;
            long count = 0;
            
            list<DataPoint*> nodePhList = nodeItr->second;
            
            for(nphItr = nodePhList.begin(); nphItr != nodePhList.end(); nphItr++){
                DataPoint* ph = *nphItr;
                count += ph->phenotypeList.size();
                
                sum += ph->GetValue();
                
                size += ph->GetWeight(stol(id));
                
                sumX += ph->positionX;
                sumY += ph->positionY;
            }
            
            float avg = sum/count;
            float avgX = sumX/nodePhList.size();
            float avgY = sumY/nodePhList.size();
            
            node.size = size;
            node.avgValue = avg;
            node.avgValueX = avgX;
            node.avgValueY = avgY;
            
            this->gmlGraph.gmlNodeList.push_back(node);
            
            rNode *rn = new rNode(ide, avg, size);
            rn->setEnvWeight(avgX, 0);
            rn->setEnvWeight(avgY, 1);
            rGraph->addNode(rn);
            
            if(minVal > size && size > 0) minVal = size;
            if(maxVal < size) maxVal = size;
            
            if(maxMeanVal < avg) maxMeanVal = avg;
            if(minMeanVal > avg && avg > 0) minMeanVal = avg;
            
            if(maxX < avgX) maxX = avgX;
            if(minX > avgX && avgX > 0) minX = avgX;
            
            if(maxY < avgY) maxY = avgY;
            if(minY > avgY && avgY > 0) minY = avgY;
        }
        
        //start_Time = clock();
        float nw[] = {minMeanVal,maxMeanVal};
        float ns[] = {minVal,maxVal};
        float ew[FILTER][2]={{minX,maxX},{minY,maxY}};
        
        rGraph->storeRange(ns, nw, ew, envName);
        
        for(this->gmlGraph.gnItr = this->gmlGraph.gmlNodeList.begin(); this->gmlGraph.gnItr != this->gmlGraph.gmlNodeList.end(); this->gmlGraph.gnItr++){
            
            GmlNode node = *this->gmlGraph.gnItr;
            
            node.adjustColor(minMeanVal, maxMeanVal);
            node.adjustColorByX(minX, maxX);
            node.adjustColorByY(minY, maxY);
            node.adjustSize(minVal, maxVal);
            
            string size = to_string(node.size);
            long pos = 0;
            if((pos =size.find(".")) != string::npos){
                pos += 3;
                size = size.substr(0, pos);
            }
            
            string avgVal = to_string(node.avgValue);
            pos = 0;
            if((pos =avgVal.find(".")) != string::npos){
                pos += 3;
                avgVal = avgVal.substr(0, pos);
            }
            
            string avgValX = to_string(node.avgValueX);
            pos = 0;
            if((pos =avgValX.find(".")) != string::npos){
                pos += 3;
                avgValX = avgValX.substr(0, pos);
            }
            
            string avgValY = to_string(node.avgValueY);
            pos = 0;
            if((pos =avgValY.find(".")) != string::npos){
                pos += 3;
                avgValY = avgValY.substr(0, pos);
            }
            
            lastTime = (clock()-start_Time)/CLK_TCK;
            //str_non_isolatedVertices += "stream.addVertex(" + to_string(node.id) + ", " + to_string(this->overlapX*100).substr(0,5) + ");\n";
            //sc->AddZeroSimplex(node.id, this->overlapX*50);
            
            if(nodeList.find(node.id) == nodeList.end()){
                nodeList.insert(node.id);
            }
            
            /*if(isPh){
                // Node color based on avg ph value
                nodes += "node\n[\n id " + to_string(node.id) + "\n label \"" + avgVal + "\" \n size " + to_string(node.relSize) + "\n color \"" + node.color + "\"\n textcolor \"#ff0000\" \n shape \"circle\" \n]\n";
                // No color
                //nodes += "node\n[\n id " + to_string(node.id) + "\n size " + to_string(15) + "\n label \""+ to_string(node.id) +"\" \n color \"#ffffff\" \n shape \"circle\" \n]\n";
                // No node label
                //nodes += "node\n[\n id " + to_string(node.id) + "\n label \"\" \n size " + to_string(node.relSize) + "\n color \"" + node.color + "\" \n shape \"circle\" \n]\n";
            }else if(isTemp){
                // Node color based on x value
                nodes += "node\n[\n id " + to_string(node.id) + "\n label \"" + avgValX + "\" \n size " + to_string(node.relSize) + "\n color \"" + node.colorX + "\" \n shape \"circle\" \n]\n";
                //No node label
                //nodes += "node\n[\n id " + to_string(node.id) + "\n label \"\" \n size " + to_string(node.relSize) + "\n color \"" + node.colorX + "\" \n shape \"circle\" \n]\n";
            }else{
                // Node color based on y value
                nodes += "node\n[\n id " + to_string(node.id) + "\n label \"" +  avgValY + "\" \n size " + to_string(node.relSize) + "\n color \"" + node.colorY + "\" \n shape \"circle\" \n]\n";
                // No node label
                //nodes += "node\n[\n id " + to_string(node.id) + "\n label \"\" \n size " + to_string(node.relSize) + "\n color \"" + node.colorY + "\" \n shape \"circle\" \n]\n";
            }*/
            
            /*totalNodes++;
            
        }
    }
    
    unordered_map<string, pair<float, Box>>::iterator edgeItr;
    unordered_map<string, Edges> edgeListFinal;
    
    for(edgeItr=this->graph.edge.edgeList.begin(); edgeItr != this->graph.edge.edgeList.end(); edgeItr++){
        Box b = edgeItr->second.second;
        
        str += b.toString() + "Face weight:" + to_string(edgeItr->second.first) + "\n";
        
        list<DataPoint*> phList = b.dpList;
        if(phList.size() > 0){
            list<DataPoint*>::iterator itr;
            
            for(itr = phList.begin(); itr != phList.end(); itr++){
                DataPoint* ph = *itr;
                
                if(b.twoWayOverlap){
                    short id[2], k=0;
                    
                    for(short i=0; i<4; i++){
                        if(b.clusterIndex[i] != -1){
                            id[k] = i;
                            k++;
                        }
                    }
                    
                    str += ph->toString(id[0], id[1]);
                }else{
                    str += ph->toString();
                }
            }
        }
    }
    
    start_Time = clock();
    
    minVal = 16777215.0, maxVal=0.0;
    
    for(this->graph.edgeItr = this->graph.edgeList.begin(); this->graph.edgeItr != this->graph.edgeList.end(); this->graph.edgeItr++){
        
        if(rGraph->hasNode(this->graph.edgeItr->second.node1, this->graph.edgeItr->second.node2)){
            rGraph->addEdge(this->graph.edgeItr->second.node1, this->graph.edgeItr->second.node2);
            
            //if(this->graph.edgeItr->second.face_weight > 0){
                
                float w = this->graph.edgeItr->second.face_weight;
                
                if(minVal > w && w>0) minVal = w;
                if(maxVal < w) maxVal = w;
                
                GmlEdge edge;
                
                edge.node1 = this->graph.edgeItr->second.node1;
                edge.node2 = this->graph.edgeItr->second.node2;
                edge.weight = this->graph.edgeItr->second.face_weight;
                
                this->gmlGraph.gmlEdgeList.push_back(edge);
                
            //}else{
              //  cout<<"";
            //}
        }
    }
    
    for(this->gmlGraph.geItr = this->gmlGraph.gmlEdgeList.begin(); this->gmlGraph.geItr != this->gmlGraph.gmlEdgeList.end(); this->gmlGraph.geItr++){
        GmlEdge edge = *this->gmlGraph.geItr;
        edge.adjustWeight(minVal, maxVal);
        
        string face_weight = to_string(edge.weight);
        long pos = 0;
        if((pos =face_weight.find(".")) != string::npos){
            pos += 3;
            face_weight = face_weight.substr(0, pos);
        }
        
        //str_edges += "stream.addElement(new int[]{" + to_string(edge.node1) + ", " + to_string(edge.node2) + "},  " + to_string(this->overlapX*100).substr(0,5) + ");\n";
        sc->AddOneSimplex(edge.node1, edge.node2, this->overlapX*50);
        
        //edges += "edge\n[\n source " + to_string(edge.node1) + "\n target " + to_string(edge.node2) + "\n width " + to_string(edge.relWeight) + "\n color \"" + edge.color + "\"\n]\n";
    }
    
    map<long, string>::iterator simItr;
    
    if(this->tries.size() > 0){
        for(simItr = this->tries.begin(); simItr != this->tries.end(); simItr++){
            string val = simItr->second + ",";
            
            short pos = -1, index=0;
            long tempID[] = {-1,-1,-1};
            
            while((pos = val.find(",")) != string::npos){
                tempID[index] = stol(val.substr(0, pos));
                val = val.substr(pos+1, val.length());
                index++;
            }
            
            //str_triangles += "stream.addElement(new int[]{" + to_string(tempID[0]) + ", " + to_string(tempID[1]) + ", " + to_string(tempID[2]) + "},  " + to_string(this->overlapX*100).substr(0,5) + ");\n";
            sc->AddTwoSimplex(tempID[0], tempID[1], tempID[2], this->overlapX*50);
        }
        
    }
    
    if(this->tetras.size() > 0){
        for(simItr = this->tetras.begin(); simItr != this->tetras.end(); simItr++){
            string val = simItr->second + ",";;
            short pos = -1, index=0;
            long tempID[] = {-1,-1,-1,-1};
            
            while((pos = val.find(",")) != string::npos){
                tempID[index] = stol(val.substr(0, pos));
                val = val.substr(pos+1, val.length());
                index++;
            }
            
            //str_triangles += "stream.addElement(new int[]{" + to_string(tempID[0]) + ", " + to_string(tempID[1]) + ", " + to_string(tempID[2]) + "}, " + to_string(this->overlapX*100).substr(0,5) + ");\n";
            sc->AddTwoSimplex(tempID[0], tempID[1], tempID[2], this->overlapX*50);
            
            //str_triangles += "stream.addElement(new int[]{" + to_string(tempID[0]) + ", " + to_string(tempID[2]) + ", " + to_string(tempID[3]) + "}, " + to_string(this->overlapX*100).substr(0,5) + ");\n";
            sc->AddTwoSimplex(tempID[0], tempID[2], tempID[3], this->overlapX*50);
            
            //str_triangles += "stream.addElement(new int[]{" + to_string(tempID[0]) + ", " + to_string(tempID[1]) + ", " + to_string(tempID[3]) + "}, " + to_string(this->overlapX*100).substr(0,5) + ");\n";
            sc->AddTwoSimplex(tempID[0], tempID[1], tempID[3], this->overlapX*50);
            
            //str_triangles += "stream.addElement(new int[]{" + to_string(tempID[1]) + ", " + to_string(tempID[2]) + ", " + to_string(tempID[3]) + "}, " + to_string(this->overlapX*100).substr(0,5) + ");\n";
            sc->AddTwoSimplex(tempID[1], tempID[2], tempID[3], this->overlapX*50);
            
            //str_tetrahiddrens += "stream.addElement(new int[]{" + to_string(tempID[0]) + ", " + to_string(tempID[1]) + ", " + to_string(tempID[2]) + ", " + to_string(this->overlapX*100).substr(0,5) + ");\n";
            sc->AddThreeSimplex(tempID[0], tempID[1], tempID[2], tempID[3], this->overlapX*50);
        }
        
    }
    
    /*gmlData = nodes+edges+"]";
    
    FileHandler* fileHandler = new FileHandler("");
    fileHandler->WriteDataToFile(fileName, "gml", gmlData, false);
    delete fileHandler;*/
    
    /*rGraph->findInterestingPaths(fileName_suffix);
    
    delete rGraph;
    
    cout<<"\n("<<totalNodes<<"/"<<Cluster::GetClusterID()<<")"<<endl;
    
    //cout<<str_isolatedVertices<<str_non_isolatedVertices<<str_edges<<str_triangles<<str_tetrahiddrens<<endl;
    */
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
    list<Phenotype*> clPhList;
    list<DataPoint*> dpList;
    
    this->tree->SearchSurface(this->xMin-0.5, this->yMin-0.5, this->xMax+0.5, this->yMax+0.5, &clPhList, &dpList, 0);
    
    long tc = Cluster::GetClusterID();
    
    Cluster* allClusters = new Cluster[tc+1];
    
    list<Phenotype*>::iterator itr;
    
    for(itr = clPhList.begin(); itr != clPhList.end(); itr++){
        Phenotype* ph = *itr;
        
        long idList[4];
        ph->GetIDList(idList);
        
        for(short i=0; i<4; i++){
            if(idList[i] > 0){
                allClusters[idList[i]].AddToCluster(ph);
            }
        }
    }
    
    for(long i=1; i<=tc; i++){
        list<Phenotype*> tphList;
        
        allClusters[i].GetPhenotypeListOfThisCluster(&tphList);
        
        cout<<"Cluster ID:"<<to_string(i)<<endl;
        
        for(itr = tphList.begin(); itr != tphList.end(); itr++){
            Phenotype* ph = *itr;
            
            cout<<ph->toString();
        }
    }
    
    
    delete []allClusters;
}

#pragma Private_Methods

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
    
    std::cout<<"X points:(";
    for(int i=0; i<this->maxBlockX; i++){
        std::cout<<this->xPositions[i]<<",";
    }
    std::cout<<")"<<endl;
    
    std::cout<<"Y points:(";
    for(int i=0; i<this->maxBlockY; i++){
        std::cout<<this->yPositions[i]<<",";
    }
    std::cout<<")"<<endl;
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
    
    cout<<"\nPrint x coord:\n";
    for(itr = x->begin(); itr != x->end(); itr++){
        float val = *itr;
        cout<<val<<",";
        this->xPositions[index] = val;
        index++;
    }
    
    index = 0;
    
    cout<<"\n\nPrint y coord:\n";
    for(itr = y->begin(); itr != y->end(); itr++){
        float val = *itr;
        cout<<val<<",";
        this->yPositions[index] = val;
        index++;
    }
    cout<<"\n\n";
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
    
    long idList[]={-1,-1,-1,-1};
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
            for(short i=0; i<4; i++){
                if(idList[i] > 0) totalOL++;
            }
            
            
            string ids = "";
            long fid=0;
            bool first = true;
            
            for(short i=0; i<4; i++){
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
                
                unordered_map<string, list<Phenotype*>>::iterator nodeItr;
                
                //if(idList[clusterIndex[0]] == 23){
                    //cout<<"\n";
                //}
                
                if(idList[clusterIndex[0]] > 0){
                    key = "" + to_string(idList[clusterIndex[0]]);
                    if((nodeItr = this->graph.node.nodeList.find(key)) == this->graph.node.nodeList.end()){
                        
                        list<Phenotype*> phenoList;
                        phenoList.push_back(ph);
                        
                        this->graph.node.nodeList.insert(pair<string, list<Phenotype*>>(key, phenoList));
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
                        
                        this->graph.node.nodeList.insert(pair<string, list<Phenotype*>>(key, phenoList));
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
                        
                        this->graph.edge.edgeList.insert(pair<string, pair<float, Box>>(key, pair<float, Box>(faceWeight, box)));
                        
                    }else{
                        unordered_map<string, pair<float, Box>>::iterator eItr;
                        
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
                
                unordered_map<string, list<Phenotype*>>::iterator nodeItr;
                
                if(idList[clusterIndex[0]] > 0){
                    key = "" + to_string(idList[clusterIndex[0]]);
                    if((nodeItr = this->graph.node.nodeList.find(key)) == this->graph.node.nodeList.end()){
                        
                        list<Phenotype*> phenoList;
                        phenoList.push_back(ph);
                        
                        this->graph.node.nodeList.insert(pair<string, list<Phenotype*>>(key, phenoList));
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
                        
                        this->graph.node.nodeList.insert(pair<string, list<Phenotype*>>(key, phenoList));
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
                        
                        this->graph.node.nodeList.insert(pair<string, list<Phenotype*>>(key, phenoList));
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
                        
                        this->graph.node.nodeList.insert(pair<string, list<Phenotype*>>(key, phenoList));
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
                
                if(idList[0] > 0 && idList[1] > 0 && idList[2] > 0 && idList[3] > 0){
                    long keyIDList[]={idList[0], idList[1], idList[2], idList[3]};
                    
                    this->SortClusterIDs(keyIDList, 4);
                    
                    short clusterIndexNew[]={0,1,2,3};
                    
                    key = "" + to_string(keyIDList[0]) + "#" + to_string(keyIDList[1]) + "#" + to_string(keyIDList[2]) + "#" + to_string(keyIDList[3]);
                    if(this->graph.edge.edgeList.find(key) == this->graph.edge.edgeList.end()){
                        
                        Box box(boxID[0], boxID[1], boxID[2], boxID[3], minX, maxX, minY, maxY, isHorizontalOverlap, isTwoWayOverlap, idList, clusterIndexNew);
                        
                        //cout<<"Box:("<<minX<<","<<maxX<<","<<minY<<","<<maxY<<"), ID:("<<boxID[0]<<","<<boxID[1]<<","<<boxID[2]<<","<<boxID[3]<<")\n";
                        
                        box.phenoList.push_back(ph);
                        faceWeight = ph->GetWeight();
                        
                        this->graph.edge.edgeList.insert(pair<string, pair<float, Box>>(key, pair<float, Box>(faceWeight, box)));
                        
                    }else{
                        unordered_map<string, pair<float, Box>>::iterator eItr;
                        
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
    
    long idList[]={-1,-1,-1,-1};
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
            
            string s = ((isHorizontalOverlap)?"horizontal":"vertical");
            
            if(isTwoWayOverlap){
                //cout<<"2 way " + s + " overlap.\n";
                
                unordered_map<string, list<Phenotype*>>::iterator nodeItr;
                
                //if(idList[clusterIndex[0]] == 23){
                //cout<<"\n";
                //}
                
                if(idList[clusterIndex[0]] > 0){
                    key = "" + to_string(idList[clusterIndex[0]]);
                    
                    long thisID = idList[clusterIndex[0]];
                    
                    if(this->graph.nodeList.find(thisID)==this->graph.nodeList.end()){
                        for(int k=0; k<4; k++){
                            if(idList[k]>0 && thisID!=idList[k]){
                                unordered_map<long, list<Phenotype*>> aMap;
                                list<Phenotype*> aPh;
                                
                                aPh.push_back(ph);
                                aMap.insert(pair<long, list<Phenotype*>>(idList[k], aPh));
                            }
                        }
                    }else{
                        unordered_map<long, unordered_map<long, list<Phenotype*>>>::iterator nlItr = this->graph.nodeList.find(thisID);
                        
                        for(int k=0; k<4; k++){
                            if(idList[k]>0 && thisID!=idList[k]){
                                unordered_map<long, list<Phenotype*>>::iterator nl2Itr = nlItr->second.find(idList[k]);
                                
                                if(nl2Itr == nlItr->second.end()){
                                    list<Phenotype*> aPh;
                                    
                                    aPh.push_back(ph);
                                    nlItr->second.insert(pair<long, list<Phenotype*>>(idList[k], aPh));
                                }else{
                                    nl2Itr->second.push_back(ph);
                                }
                            }
                        }
                    }
                    
                    
                    if((nodeItr = this->graph.node.nodeList.find(key)) == this->graph.node.nodeList.end()){
                        
                        list<Phenotype*> phenoList;
                        phenoList.push_back(ph);
                        
                        this->graph.node.nodeList.insert(pair<string, list<Phenotype*>>(key, phenoList));
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
                        for(int k=0; k<4; k++){
                            if(idList[k]>0 && thisID!=idList[k]){
                                unordered_map<long, list<Phenotype*>> aMap;
                                list<Phenotype*> aPh;
                                
                                aPh.push_back(ph);
                                aMap.insert(pair<long, list<Phenotype*>>(idList[k], aPh));
                            }
                        }
                    }else{
                        unordered_map<long, unordered_map<long, list<Phenotype*>>>::iterator nlItr = this->graph.nodeList.find(thisID);
                        
                        for(int k=0; k<4; k++){
                            if(idList[k]>0 && thisID!=idList[k]){
                                unordered_map<long, list<Phenotype*>>::iterator nl2Itr = nlItr->second.find(idList[k]);
                                
                                if(nl2Itr == nlItr->second.end()){
                                    list<Phenotype*> aPh;
                                    
                                    aPh.push_back(ph);
                                    nlItr->second.insert(pair<long, list<Phenotype*>>(idList[k], aPh));
                                }else{
                                    nl2Itr->second.push_back(ph);
                                }
                            }
                        }
                    }
                    
                    
                    if((nodeItr = this->graph.node.nodeList.find(key)) == this->graph.node.nodeList.end()){
                        
                        list<Phenotype*> phenoList;
                        phenoList.push_back(ph);
                        
                        this->graph.node.nodeList.insert(pair<string, list<Phenotype*>>(key, phenoList));
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
                
                unordered_map<string, list<Phenotype*>>::iterator nodeItr;
                
                if(idList[clusterIndex[0]] > 0){
                    key = "" + to_string(idList[clusterIndex[0]]);
                    
                    long thisID = idList[clusterIndex[0]];
                    
                    if(this->graph.nodeList.find(thisID)==this->graph.nodeList.end()){
                        for(int k=0; k<4; k++){
                            if(idList[k]>0 && thisID!=idList[k]){
                                unordered_map<long, list<Phenotype*>> aMap;
                                list<Phenotype*> aPh;
                                
                                aPh.push_back(ph);
                                aMap.insert(pair<long, list<Phenotype*>>(idList[k], aPh));
                            }
                        }
                    }else{
                        unordered_map<long, unordered_map<long, list<Phenotype*>>>::iterator nlItr = this->graph.nodeList.find(thisID);
                        
                        for(int k=0; k<4; k++){
                            if(idList[k]>0 && thisID!=idList[k]){
                                unordered_map<long, list<Phenotype*>>::iterator nl2Itr = nlItr->second.find(idList[k]);
                                
                                if(nl2Itr == nlItr->second.end()){
                                    list<Phenotype*> aPh;
                                    
                                    aPh.push_back(ph);
                                    nlItr->second.insert(pair<long, list<Phenotype*>>(idList[k], aPh));
                                }else{
                                    nl2Itr->second.push_back(ph);
                                }
                            }
                        }
                    }
                    
                    if((nodeItr = this->graph.node.nodeList.find(key)) == this->graph.node.nodeList.end()){
                        
                        list<Phenotype*> phenoList;
                        phenoList.push_back(ph);
                        
                        this->graph.node.nodeList.insert(pair<string, list<Phenotype*>>(key, phenoList));
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
                        for(int k=0; k<4; k++){
                            if(idList[k]>0 && thisID!=idList[k]){
                                unordered_map<long, list<Phenotype*>> aMap;
                                list<Phenotype*> aPh;
                                
                                aPh.push_back(ph);
                                aMap.insert(pair<long, list<Phenotype*>>(idList[k], aPh));
                            }
                        }
                    }else{
                        unordered_map<long, unordered_map<long, list<Phenotype*>>>::iterator nlItr = this->graph.nodeList.find(thisID);
                        
                        for(int k=0; k<4; k++){
                            if(idList[k]>0 && thisID!=idList[k]){
                                unordered_map<long, list<Phenotype*>>::iterator nl2Itr = nlItr->second.find(idList[k]);
                                
                                if(nl2Itr == nlItr->second.end()){
                                    list<Phenotype*> aPh;
                                    
                                    aPh.push_back(ph);
                                    nlItr->second.insert(pair<long, list<Phenotype*>>(idList[k], aPh));
                                }else{
                                    nl2Itr->second.push_back(ph);
                                }
                            }
                        }
                    }
                    
                    
                    if((nodeItr = this->graph.node.nodeList.find(key)) == this->graph.node.nodeList.end()){
                        
                        list<Phenotype*> phenoList;
                        phenoList.push_back(ph);
                        
                        this->graph.node.nodeList.insert(pair<string, list<Phenotype*>>(key, phenoList));
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
                        for(int k=0; k<4; k++){
                            if(idList[k]>0 && thisID!=idList[k]){
                                unordered_map<long, list<Phenotype*>> aMap;
                                list<Phenotype*> aPh;
                                
                                aPh.push_back(ph);
                                aMap.insert(pair<long, list<Phenotype*>>(idList[k], aPh));
                            }
                        }
                    }else{
                        unordered_map<long, unordered_map<long, list<Phenotype*>>>::iterator nlItr = this->graph.nodeList.find(thisID);
                        
                        for(int k=0; k<4; k++){
                            if(idList[k]>0 && thisID!=idList[k]){
                                unordered_map<long, list<Phenotype*>>::iterator nl2Itr = nlItr->second.find(idList[k]);
                                
                                if(nl2Itr == nlItr->second.end()){
                                    list<Phenotype*> aPh;
                                    
                                    aPh.push_back(ph);
                                    nlItr->second.insert(pair<long, list<Phenotype*>>(idList[k], aPh));
                                }else{
                                    nl2Itr->second.push_back(ph);
                                }
                            }
                        }
                    }
                    
                    if((nodeItr = this->graph.node.nodeList.find(key)) == this->graph.node.nodeList.end()){
                        
                        list<Phenotype*> phenoList;
                        phenoList.push_back(ph);
                        
                        this->graph.node.nodeList.insert(pair<string, list<Phenotype*>>(key, phenoList));
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
                        for(int k=0; k<4; k++){
                            if(idList[k]>0 && thisID!=idList[k]){
                                unordered_map<long, list<Phenotype*>> aMap;
                                list<Phenotype*> aPh;
                                
                                aPh.push_back(ph);
                                aMap.insert(pair<long, list<Phenotype*>>(idList[k], aPh));
                            }
                        }
                    }else{
                        unordered_map<long, unordered_map<long, list<Phenotype*>>>::iterator nlItr = this->graph.nodeList.find(thisID);
                        
                        for(int k=0; k<4; k++){
                            if(idList[k]>0 && thisID!=idList[k]){
                                unordered_map<long, list<Phenotype*>>::iterator nl2Itr = nlItr->second.find(idList[k]);
                                
                                if(nl2Itr == nlItr->second.end()){
                                    list<Phenotype*> aPh;
                                    
                                    aPh.push_back(ph);
                                    nlItr->second.insert(pair<long, list<Phenotype*>>(idList[k], aPh));
                                }else{
                                    nl2Itr->second.push_back(ph);
                                }
                            }
                        }
                    }
                    
                    if((nodeItr = this->graph.node.nodeList.find(key)) == this->graph.node.nodeList.end()){
                        
                        list<Phenotype*> phenoList;
                        phenoList.push_back(ph);
                        
                        this->graph.node.nodeList.insert(pair<string, list<Phenotype*>>(key, phenoList));
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
    
    long idList[]={-1,-1,-1,-1};
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
                    for(short i=0; i<4; i++){
                        if(idList[i] > 0) totalOL++;
                    }
                    
                    string ids = "";
                    long fid=0;
                    bool first = true;
                    
                    for(short i=0; i<4; i++){
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
                        
                        unordered_map<string, list<DataPoint*>>::iterator nodeItr;
                        
                        //if(idList[clusterIndex[0]] == 23){
                        //cout<<"\n";
                        //}
                        
                        if(idList[clusterIndex[0]] > 0){
                            key = "" + to_string(idList[clusterIndex[0]]);
                            if((nodeItr = this->graph.node.nodeListDP.find(key)) == this->graph.node.nodeListDP.end()){
                                
                                list<DataPoint*> dataPointList;
                                dataPointList.push_back(dp);
                                
                                this->graph.node.nodeListDP.insert(pair<string, list<DataPoint*>>(key, dataPointList));
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
                                
                                this->graph.node.nodeListDP.insert(pair<string, list<DataPoint*>>(key, phenoList));
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
                                
                                this->graph.edge.edgeList.insert(pair<string, pair<float, Box>>(key, pair<float, Box>(faceWeight, box)));
                                
                            }else{
                                unordered_map<string, pair<float, Box>>::iterator eItr;
                                
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
                        
                        unordered_map<string, list<DataPoint*>>::iterator nodeItr;
                        
                        if(idList[clusterIndex[0]] > 0){
                            key = "" + to_string(idList[clusterIndex[0]]);
                            if((nodeItr = this->graph.node.nodeListDP.find(key)) == this->graph.node.nodeListDP.end()){
                                
                                list<DataPoint*> phenoList;
                                phenoList.push_back(dp);
                                
                                this->graph.node.nodeListDP.insert(pair<string, list<DataPoint*>>(key, phenoList));
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
                                
                                this->graph.node.nodeListDP.insert(pair<string, list<DataPoint*>>(key, phenoList));
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
                                
                                this->graph.node.nodeListDP.insert(pair<string, list<DataPoint*>>(key, phenoList));
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
                                
                                this->graph.node.nodeListDP.insert(pair<string, list<DataPoint*>>(key, phenoList));
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
                        
                        
                        if(idList[0] > 0 && idList[1] > 0 && idList[2] > 0 && idList[3] > 0){
                            long keyIDList[]={idList[0], idList[1], idList[2], idList[3]};
                            
                            this->SortClusterIDs(keyIDList, 4);
                            
                            short clusterIndexNew[]={0,1,2,3};
                            
                            key = "" + to_string(keyIDList[0]) + "#" + to_string(keyIDList[1]) + "#" + to_string(keyIDList[2]) + "#" + to_string(keyIDList[3]);
                            if(this->graph.edge.edgeList.find(key) == this->graph.edge.edgeList.end()){
                                
                                Box box(boxID[0], boxID[1], boxID[2], boxID[3], minX, maxX, minY, maxY, isHorizontalOverlap, isTwoWayOverlap, idList, clusterIndexNew);
                                
                                //cout<<"Box:("<<minX<<","<<maxX<<","<<minY<<","<<maxY<<"), ID:("<<boxID[0]<<","<<boxID[1]<<","<<boxID[2]<<","<<boxID[3]<<")\n";
                                
                                box.dpList.push_back(dp);
                                faceWeight = dp->GetWeight();
                                
                                this->graph.edge.edgeList.insert(pair<string, pair<float, Box>>(key, pair<float, Box>(faceWeight, box)));
                                
                            }else{
                                unordered_map<string, pair<float, Box>>::iterator eItr;
                                
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
    
    set<long> bxIdList;
    
    this->tree->SearchDataPoints(x1, y1, x2, y2, &bxIdList);
    
    if(bxIdList.size()>0){
        if(bxIdList.find(boxID) != bxIdList.end()){
            return true;
        }
    }
    
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
        
        long clsIDList[4];
        
        ph->GetIDList(clsIDList);
        
        for(short i=0; i<4; i++){
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
                
                long idList[4];
                ph->GetIDList(idList);
                
                for(short j=0; j<4; j++){
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
        
        long clsIDList[4];
        
        dp->GetIDList(clsIDList);
        
        for(short i=0; i<4; i++){
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
                
                long idList[4];
                ph->GetIDList(idList);
                
                for(short j=0; j<4; j++){
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


