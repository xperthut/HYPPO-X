
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: QuadTree.cpp
 Objective: Implementation of member functions
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              02/21/2016      Initial version
 
 **************************************************************************************************/

#include "QuadTree.h"

using namespace std;

QuadTree::QuadTree(float x, float y, float lx, float ly){
    this->root = new QuadNode(x, y, lx, ly);
}

QuadTree::~QuadTree(){
    delete this->root;
}

#pragma Public_Methods
void QuadTree::AddDataToNode(DataPoint* data){
    this->root->AddDataToNode(data);
}

void QuadTree::PrintQuadTree(){
    this->PrintNode(this->root);
}

QuadNode* QuadTree::GetRoot(){return this->root;}

void QuadTree::SearchSurface(float x1, float y1, float x2, float y2, list<Phenotype*> *phList, list<DataPoint*> *dpList, long boxID){
    
    this->SearchSurfaceInNode(this->root, x1, y1, x2, y2, phList, dpList, boxID);
}

void QuadTree::SearchDataPoints(float x1, float y1, float x2, float y2, set<long> *boxIdList){
    
    this->SearchDataPoint(this->root, x1, y1, x2, y2, boxIdList);

}

#pragma Private_Methods
int QuadTree::PrintNode(QuadNode *node){
    if(node == NULL) return 0;
    
    for(int i=0; i<4; i++){
        
        this->PrintNode(node->GetChild(i));
        
    }
    
    DataPoint* d = node->GetNodeData();
    
    if(d != NULL){
        cout<<"["<<d->getPosition(0)<<","<<d->getPosition(1)<<"]=>"<<d->getBoxId()<<"\n";
    }
    
    return 0;
}

int QuadTree::SearchSurfaceInNode(QuadNode *node, float x1, float y1, float x2, float y2, list<Phenotype *> *phList, list<DataPoint*> *dpList, long boxID){
    if(node == NULL) return 0;
    
    for(int i=0; i<4; i++){
    
        this->SearchSurfaceInNode(node->GetChild(i), x1, y1, x2, y2, phList, dpList, boxID);
    }
    
    DataPoint* d = node->GetNodeData();
    
    if(d != NULL){
        
        float x = d->getPosition(0)*100000;
        float y = d->getPosition(1)*100000;
        
        float px[]={x1*100000, x2*100000};
        float py[]={y1*100000, y2*100000};
        
        if(px[0]<=x && x<=px[1] && py[0]<=y && y<=py[1]){
            if(boxID > 0){
                if(d->getBoxId()==0) d->setBoxId(boxID);
            }
            
            dpList->push_back(d);
            list<Phenotype*>::iterator itr;
            list<Phenotype*> pL = d->getPhList();
            
            for (itr = pL.begin(); itr != pL.end(); itr++)
            {
                phList->push_front(*itr);
            }
        }
    }
    
    return 0;
}

int QuadTree::SearchDataPoint(QuadNode *node, float x1, float y1, float x2, float y2, set<long> *boxIdList){
    if(node == NULL) return 0;
    
    for(int i=0; i<4; i++){
        
        this->SearchDataPoint(node->GetChild(i), x1, y1, x2, y2, boxIdList);
    }
    
    DataPoint* d = node->GetNodeData();
    
    if(d != NULL){
        
        float x = d->getPosition(0)*100000;
        float y = d->getPosition(1)*100000;
        
        float px[]={x1*100000, x2*100000};
        float py[]={y1*100000, y2*100000};
        
        if(px[0]<=x && x<=px[1] && py[0]<=y && y<=py[1]){
            boxIdList->insert(d->getBoxId());
        }
    }
    
    return 0;
    
}

