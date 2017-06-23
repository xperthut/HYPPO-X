
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
        
        // this->mesh_Y = (Grid*)malloc(sizeof(Grid)*(this->slice_ly));
    }
    
    this->minX=0, this->minY=0, this->maxX=0, this->maxY=0;
}

PersistentHomology::~PersistentHomology(){
    if(this->slice_lx>0 && this->slice_ly>0){
        
        for(int i=0; i<this->slice_lx; i++){
            free(this->mesh[i]);
        }
        
        free(this->mesh);
    }
}

float PersistentHomology::GetMinX(){return this->minX;}
float PersistentHomology::GetMaxX(){return this->maxX;}
float PersistentHomology::GetMinY(){return this->minY;}
float PersistentHomology::GetMaxY(){return this->maxY;}

void PersistentHomology::CreateTreeAndMeshGrid(){
    list<Phenotype*> phynList;
    
    FileHandler fileHandler(this->fileName);
    fileHandler.ReadFileData(&this->dataPointList);
    
    if(this->dataPointList.size()==0){
        cout<<"Can't read file."<<"\n";
        exit(0);
    }
    
    DataPoint* tmpDP;
    
    list<Phenotype*>::iterator ptr;
    
    bool first = true;
    
    for (list<DataPoint*>::iterator itr = dataPointList.begin(); itr != dataPointList.end(); itr++)
    {
        tmpDP = *itr;
        
        //cout<<"("<<tmpDP->positionX<<","<<tmpDP->positionY<<")"<<endl;
        
        if(first){
            this->minX = tmpDP->getPosition(0);
            this->maxX = tmpDP->getPosition(0);
            
            first = false;
        }
        
        if(this->minX > tmpDP->getPosition(0)){this->minX = tmpDP->getPosition(0);}

        if(this->maxX < tmpDP->getPosition(0)){this->maxX = tmpDP->getPosition(0);}
        
    }
    
    //cout<<this->minX<<endl<<(this->minY)<<endl<<(this->maxX)<<endl<<(this->maxY)<<endl<<endl;
    
    // For safety, increase the search region
    minX = floor(minX)-1;
    maxX = ceil(maxX)+1;
    minY = floor(minY);
    maxY = ceil(maxY);
    
    //cout<<"("<<minX<<","<<maxX<<"),("<<minY<<","<<maxY<<")"<<endl;
    
    gridLength_x = (slice_lx>0)?(maxX - minX)/slice_lx:(maxX - minX);
    gridLength_y = (slice_ly>0)?(maxY - minY)/slice_ly:(maxY - minY);
    
    float start_x = minX, start_y = minY;
    
    for(int i=0; i<this->slice_lx; i++){
        
        start_y = minY;
        
        for(int j=0; j<this->slice_ly; j++){
            this->mesh[i][j].center_x = start_x + (this->gridLength_x/2);
            this->mesh[i][j].center_y = start_y + (this->gridLength_y/2);
            
            start_y += this->gridLength_y;
        }
        
        start_x += this->gridLength_x;
    }
    
    this->clusterRegion = new ClusteringRegion(&dataPointList, minX, minY, maxX, maxY, gridLength_x, gridLength_y, OVERLAP_X, OVERLAP_Y);
    this->clusterRegion->setSplit(this->slice_lx, this->slice_ly);
    
    this->clusterRegion->AssignPoints(this->slice_lx, this->slice_ly);
}

void PersistentHomology::CreateClusterAndGraph(){
    this->CreateUniqueClusters();
    
    list<float> x,y;
    float percentageIncrease_x=OVERLAP_X, percentageIncrease_y=OVERLAP_Y;
    SimplicialComplex sc;
    float shift = (this->maxX-this->minX)/this->slice_lx;
    string str = "";
    
    float overLap=percentageIncrease_x;
    
    list<Phenotype*> overlappedIndiv;
    list<AnEdge*> edgeList;
    
    
    if(PRINT_BARCODE){
        for(overLap=0.00; PRINT_BARCODE && overLap<=1.00; overLap+=0.05){
            
            //cout<<"\n\nOverlap: "<<overLap*50<<"%\n";
            
            percentageIncrease_x = overLap;
            percentageIncrease_y = overLap;
            
            float cycle=0;
            
            
            //for(int cycle=1; cycle<10; cycle++){
            this->GetCoordinates(&x, percentageIncrease_x, shift*cycle, true);
            this->GetCoordinates(&y, percentageIncrease_y, 0.0, false);
            
            this->clusterRegion->setOverlapX(percentageIncrease_x);
            this->clusterRegion->setOverlapY(percentageIncrease_y);
            
            this->clusterRegion->SetShift(shift*cycle, 0.0);
            
            this->clusterRegion->ClearCluster();
            this->clusterRegion->SplitRegion(&x, &y);
            this->clusterRegion->SearchRectangle(mesh);
            
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
            
            /*cout<<str<<endl<<"Simplicial complexes:\n";
            
            str = this->clusterRegion->GetSimplexInformation();
            
            cout<<str<<endl<<"Finish...\n\n";*/
            
            this->clusterRegion->ClearCluster();
            x.clear();
            y.clear();
            //}
        }
        
        cout<<"\nDump the following codes to Java file to generate barcode.\n\n/************ Start from here *************/\n"<<sc.PrintSimplex()<<"/************ End here *************/\n\n";
        
    }else{
        
        //cout<<"\n\nOverlap: "<<overLap*50<<"%\n";
        
        percentageIncrease_x = overLap;
        percentageIncrease_y = overLap;
        
        float cycle=0;
        
        
        //for(int cycle=1; cycle<10; cycle++){
        this->GetCoordinates(&x, percentageIncrease_x, shift*cycle, true);
        this->GetCoordinates(&y, percentageIncrease_y, 0.0, false);
        
        this->clusterRegion->setOverlapX(percentageIncrease_x);
        this->clusterRegion->setOverlapY(percentageIncrease_y);
        
        this->clusterRegion->SetShift(shift*cycle, 0.0);
        
        this->clusterRegion->ClearCluster();
        this->clusterRegion->SplitRegion(&x, &y);
        this->clusterRegion->SearchRectangle(this->mesh);
        
        overlappedIndiv.clear();
        edgeList.clear();
        
        /*int min,max;
        bool f=true;
        for (list<DataPoint*>::iterator itr = dataPointList.begin(); itr != dataPointList.end(); itr++)
        {
            DataPoint* tmpDP = *itr;
            
            //cout<<"("<<tmpDP->positionX<<","<<tmpDP->positionY<<")"<<endl;
            
            list<Phenotype*> pList = tmpDP->getPhList();
            
            for(list<Phenotype*>::iterator pitr=pList.begin(); pitr!=pList.end(); pitr++){
                Phenotype* p = *pitr;
                long il[4]={0};
                
                p->GetIDList(il);
                
                int c=0;
                for(int i=0;i<4;i++){
                    if(il[i]>0)c++;
                }
                
                if(f){
                    f=false;
                    min=c;
                    max=c;
                }else{
                    if(min>c)min=c;
                    if(max<c)max=c;
                }
            }
            
        }
        
        cout<<"\nCluster id range: ["<<min<<", "<<max<<"]\n";*/
        
        this->clusterRegion->GenerateGraphUsingOverlappingClusters(this->mesh,&overlappedIndiv, &edgeList);
        //this->clusterRegion->AddSimplicialComplex(&sc, mesh, percentageIncrease_x);
        
        bool hd = HAMMING_DISTANCE;
        
        if(hd){
            str = this->clusterRegion->PrintGraphForDataPoint(&sc);
        }else{
            str = this->clusterRegion->PrintGraph(&overlappedIndiv, &edgeList);
        }
        
        /*cout<<str<<endl<<"Simplicial complexes:\n";
        
        str = this->clusterRegion->GetSimplexInformation();
        
        cout<<str<<endl<<"Finish...\n\n";*/
        
        this->clusterRegion->ClearCluster();
        x.clear();
        y.clear();
        //}
        
    }
    
}


//#pragma PrivateMethods

void PersistentHomology::GetCoordinates(list<float> *coordList, float percentageIncrease, float center_shift, bool xAxis){
    
    if(percentageIncrease > 1){
        percentageIncrease /= 100;
    }
    
    list<float>::iterator itr;
    
    coordList->clear();
    
    if(xAxis){
        list<float> coordinate_x;
        
        //cout<<this->gridLength_x<<endl;
        
        float center_x = 0.0;
        
        for(int i=0; i<this->slice_lx; i++){
            center_x = this->mesh[i][0].center_x + center_shift;
            if(center_x > this->maxX) center_x = (center_x-this->maxX)+this->minX;
            
            float lx = (this->gridLength_x*(1+percentageIncrease))/2;
            float sx = center_x - lx;
            float ex = center_x + lx;
            
            //cout<<center_x<<",";
            
            if(sx < this->minX) sx = this->minX;
            if(ex > this->maxX) ex = this->maxX;
            
            coordinate_x.push_back(sx);
            coordinate_x.push_back(ex);
        }
        
        //cout<<endl;
        
        coordinate_x.sort();
        
        //float lastVal = 0;
        
        for(itr = coordinate_x.begin(); itr != coordinate_x.end(); itr++){
            float val = *itr;
            coordList->push_back(val);
            
            //cout<<val<<",";
            
            /*if(val != lastVal && false){
                coordList->push_back(val);
                
                lastVal = val;
            }*/
        }
        
        //cout<<endl;
        
    }else{
        list<float> coordinate_y;
        
        //cout<<this->gridLength_y<<endl;
        
        for(int i=0; i<this->slice_ly; i++){
            float ly = (this->gridLength_y*(1+percentageIncrease))/2;
            float sy = this->mesh[0][i].center_y - ly;
            float ey = this->mesh[0][i].center_y + ly;
            
            //cout<<this->mesh[0][i].center_y<<",";
            
            if(sy < minY) sy = minY;
            if(ey > maxY) ey = maxY;
            
            coordinate_y.push_back(sy);
            coordinate_y.push_back(ey);
        }
        
        //cout<<endl;
        
        coordinate_y.sort();
    
        //float lastVal = 0;
        
        for(itr = coordinate_y.begin(); itr != coordinate_y.end(); itr++){
            float val = *itr;
            coordList->push_back(val);
            
            //cout<<val<<",";
            
            /*if(val != lastVal && false){
                coordList->push_back(val);
                
                lastVal = val;
            }*/
        }
        
        //cout<<endl;
        
    }

}

void PersistentHomology::CreateUniqueClusters(){
    list<float> x,y;
    
    this->GetCoordinates(&x, 0.0, 0.0, true);
    this->GetCoordinates(&y, 0.0, 0.0, false);
    
    this->clusterRegion->setOverlapX(0.0);
    this->clusterRegion->setOverlapY(0.0);
    
    this->clusterRegion->SetShift(0.0, 0.0);
    
    this->clusterRegion->ClearCluster();
    this->clusterRegion->SplitRegion(&x, &y);
    this->clusterRegion->SearchAndInitializeRectangle(this->mesh);
    
    Cluster::SetClusterID(0);
}

