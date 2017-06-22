
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: PersistentHomology.cpp
 Objective: Implementation of member functions
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              06/15/2016      Initial version
 
 **************************************************************************************************/

#include "PersistentHomology.h"
#include <stdlib.h>
#include <list>

PersistentHomology::PersistentHomology(){
    this->fileName = DATA_FILE_NAME;
    this->slice_lx = WINDOW_X;
    this->slice_ly = WINDOW_Y;
    
    if(this->slice_lx>0 && this->slice_ly>0){
        this->mesh = (Grid**)malloc(sizeof(Grid*)*(this->slice_lx));
        
        for(int i=0; i<this->slice_lx; i++){
            this->mesh[i] = (Grid*)malloc(sizeof(Grid)*(this->slice_ly));
        }
    }
    
    for(int i=0; i<FILTER; i++){
        this->minPos[i] = 0;
        this->maxPos[i] = 0;
    }
}

PersistentHomology::~PersistentHomology(){
    if(this->slice_lx>0 && this->slice_ly>0){
        
        for(int i=0; i<this->slice_lx; i++){
            free(this->mesh[i]);
        }
        
        free(this->mesh);
    }
}

float PersistentHomology::GetMinPos(int index){return this->minPos[index];}
float PersistentHomology::GetMaxPos(int index){return this->maxPos[index];}

void PersistentHomology::CreateTreeAndMeshGrid(){
    list<Phenotype*> phynList;
    
    FileHandler fileHandler(this->fileName);
    fileHandler.ReadFileData(&this->dataPointList);
    
    DataPoint* tmpDP;
    
    list<DataPoint*>::iterator itr;
    list<Phenotype*>::iterator ptr;
    
    bool first = true;
    
    for (itr = dataPointList.begin(); itr != dataPointList.end(); itr++)
    {
        tmpDP = *itr;
        
        //cout<<"("<<tmpDP->positionX<<","<<tmpDP->positionY<<")"<<endl;
        
        if(first){
            for(int i=0; i<FILTER; i++){
                this->minPos[i] = tmpDP->getPosition(i);
                this->maxPos[i] = tmpDP->getPosition(i);
            }
            
            first = false;
        }
        
        for(int i=0; i<FILTER; i++){
            if(this->minPos[i] > tmpDP->getPosition(i)) this->minPos[i] = tmpDP->getPosition(i);
            if(this->maxPos[i] < tmpDP->getPosition(i)) this->maxPos[i] = tmpDP->getPosition(i);
        }
    }
    
    //cout<<this->minX<<endl<<(this->minY)<<endl<<(this->maxX)<<endl<<(this->maxY)<<endl<<endl;
    
    // For safety, increase the search region
    for(int i=0; i<FILTER; i++){
        this->minPos[i] -= 1;
        this->maxPos[i] += 1;
    }
    
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
    cout<<"\nQuad tree construction completed in: "<<(t2-t1)/CLK_TCK<<"(ms)"<<endl;
    
    gridLength_x = (slice_lx>0)?(this->maxPos[0] - this->minPos[0])/slice_lx:(this->maxPos[0] - this->minPos[0]);
    gridLength_y = (slice_ly>0)?(this->maxPos[1] - this->minPos[1])/slice_ly:(this->maxPos[1] - this->minPos[1]);
    
    float start_x = this->minPos[0], start_y = this->minPos[1];
    
    for(int i=0; i<this->slice_lx; i++){
        
        start_y = this->minPos[1];
        
        for(int j=0; j<this->slice_ly; j++){
            this->mesh[i][j].center_x = start_x + (this->gridLength_x/2);
            this->mesh[i][j].center_y = start_y + (this->gridLength_y/2);
        
            start_y += this->gridLength_y;
        }
        
        start_x += this->gridLength_x;
    }
    
    this->clusterRegion = new ClusteringRegion(treeRoot, this->minPos[0], this->minPos[1], this->maxPos[0], this->maxPos[1], gridLength_x, gridLength_y, OVERLAP_X, OVERLAP_Y);
    this->clusterRegion->setSplit(this->slice_lx, this->slice_ly);
    
    //this->clusterRegion->AssignPoints(this->slice_lx, this->slice_ly);
}

void PersistentHomology::CreateClusterAndGraph(){
    this->CreateUniqueClusters();
    
    list<float> x,y;
    float percentageIncrease_x=OVERLAP_X, percentageIncrease_y=OVERLAP_Y;
    SimplicialComplex sc;
    float shift = (this->maxPos[0]-this->minPos[0])/this->slice_lx;
    string str = "";
    
    float overLap=percentageIncrease_x;
    
    list<Phenotype*> overlappedIndiv;
    list<AnEdge*> edgeList;
    
    if(PRINT_BARCODE){
        for(overLap=0.00; overLap<=1.00; overLap+=0.05){
            
            //cout<<"\n\nOverlap: "<<overLap*50<<"%\n";
            
            percentageIncrease_x = overLap;
            percentageIncrease_y = overLap;
            
            float cycle=0;
            
            //for(int cycle=1; cycle<10; cycle++){
            this->clusterRegion->setOverlapX(percentageIncrease_x);
            this->clusterRegion->setOverlapY(percentageIncrease_y);
            
            this->clusterRegion->SetShift(shift*cycle, 0.0);
            
            this->clusterRegion->ClearCluster();
            this->clusterRegion->SearchRectangle(this->mesh);
            
            overlappedIndiv.clear();
            edgeList.clear();
            
            this->clusterRegion->GenerateGraphUsingOverlappingClusters(mesh, &overlappedIndiv, &edgeList);
            this->clusterRegion->AddSimplicialComplex(&sc, mesh, percentageIncrease_x);
            
            bool hd = HAMMING_DISTANCE;
            
            if(hd){
                str = this->clusterRegion->PrintGraphForDataPoint(&sc);
            }else{
                str = this->clusterRegion->PrintGraph(&overlappedIndiv, &edgeList);
            }
            
            this->clusterRegion->ClearCluster();
            x.clear();
            y.clear();
            //}
        }
        
        cout<<"\nDump the following codes to Java file to generate barcode.\n"<<sc.PrintSimplex()<<endl;
        
    }else{
        
        //cout<<"\n\nOverlap: "<<overLap*50<<"%\n";
        
        percentageIncrease_x = overLap;
        percentageIncrease_y = overLap;
        
        float cycle=0;
        
        
        //for(int cycle=1; cycle<10; cycle++){
        this->clusterRegion->setOverlapX(percentageIncrease_x);
        this->clusterRegion->setOverlapY(percentageIncrease_y);
        
        this->clusterRegion->SetShift(shift*cycle, 0.0);
        
        this->clusterRegion->ClearCluster();
        this->clusterRegion->SearchRectangle(this->mesh);
        
        overlappedIndiv.clear();
        edgeList.clear();
        
        this->clusterRegion->GenerateGraphUsingOverlappingClusters(this->mesh,&overlappedIndiv, &edgeList);
        
        bool hd = HAMMING_DISTANCE;
        
        if(hd){
            str = this->clusterRegion->PrintGraphForDataPoint(&sc);
        }else{
            str = this->clusterRegion->PrintGraph(&overlappedIndiv, &edgeList);
        }
        
        this->clusterRegion->ClearCluster();
        x.clear();
        y.clear();
        //}
        
    }
    
}


#pragma PrivateMethods

void PersistentHomology::CreateUniqueClusters(){
    list<float> x,y;
    
    //this->GetCoordinates(&x, 0.0, 0.0, true);
    //this->GetCoordinates(&y, 0.0, 0.0, false);
    
    this->clusterRegion->setOverlapX(0.0);
    this->clusterRegion->setOverlapY(0.0);
    
    this->clusterRegion->SetShift(0.0, 0.0);
    
    this->clusterRegion->ClearCluster();
    //this->clusterRegion->SplitRegion(&x, &y);
    this->clusterRegion->SearchAndInitializeRectangle(this->mesh);
    
    Cluster::SetClusterID(0);
}

