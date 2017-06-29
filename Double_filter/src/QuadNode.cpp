
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: QuadNode.cpp
 Objective: Implementation of member functions
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              02/21/2016      Initial version
 
 **************************************************************************************************/

#include <iostream>
#include "QuadNode.h"

using namespace std;

int QuadNode::totalChildren = 0;

QuadNode::QuadNode(float x, float y, float lx, float ly){
    
    for(int i=0; i<4; i++){
        this->children[i] = NULL;
    }
    
    this->data = NULL;
    this->hasChildren = false;
    this->childCount = 0;
    this->parent = NULL;
    
    this->SetArea(x, y, lx, ly);
}

QuadNode::~QuadNode(){
    for(int i=0; i<4; i++){
        if(this->children[i] !=NULL)
            delete this->children[i];
    }
    
}

//#pragma Getter
float QuadNode::GetStartXPosition(){return this->startX;}
float QuadNode::GetStartYPosition(){return this->startY;}
float QuadNode::GetLengthAlongXAxis(){return this->lengthX;}
float QuadNode::GetLengthAlongYAxis(){return this->lengthY;}
DataPoint* QuadNode::GetNodeData(){return this->data;}
int QuadNode::GetTotalChildren(){return this->childCount;}
QuadNode* QuadNode::GetParent(){return this->parent;}
void QuadNode::SetParent(QuadNode *parent){this->parent = parent;}
float QuadNode::GetNodePositionX(){return this->id_x;}
float QuadNode::GetNodePositionY(){return this->id_y;}

void QuadNode::ResetChildren(){
    for(int i=0; i<4; i++){
        this->children[i] = NULL;
    }
}

void QuadNode::SetChild(int index, QuadNode *child){
    this->children[index] = child;
}

QuadNode* QuadNode::GetChild(int index){
    if(index >=0 && index<4)
        return this->children[index];
    
    return NULL;
}

bool QuadNode::IsThisLeafNode(){
    return (this->data != NULL);
}

//#pragma Setter
void QuadNode::SetArea(float x, float y, float lx, float ly){
    
    this->id_x = x+(lx/2);
    this->id_y = y+(ly/2);
    
    this->startX = x;
    this->startY = y;
    this->lengthX = lx;
    this->lengthY = ly;
    
    this->childLocations[0][0] = x;
    this->childLocations[0][1] = y;
    
    this->childLocations[1][0] = x+(lengthX/2.0);
    this->childLocations[1][1] = y;
    
    this->childLocations[2][0] = x;
    this->childLocations[2][1] = y+(lengthY/2.0);
    
    this->childLocations[3][0] = x+(lengthX/2.0);
    this->childLocations[3][1] = y+(lengthY/2.0);
}

void QuadNode::CopyData(DataPoint *data){
    if(data != NULL){
        float x = data->getPosition(0);
        float y = data->getPosition(1);
        
        if(this->data == NULL){
            float p[]={x,y};
            this->data = new DataPoint(p);
        }else{
            this->data->setPosition(x, 0);
            this->data->setPosition(y, 1);
        }
        
        this->data->clearList();
        list<Phenotype*> phenotypeList = data->getPhList();
        
        list<Phenotype*>::iterator itr;
        
        for(itr=phenotypeList.begin(); itr != phenotypeList.end(); itr++){
            Phenotype* ph = *itr;
            
            this->data->addPhenotypeList(ph);
        }
        
    }
}

//#pragma Other_Methods
void QuadNode::AddDataToNode(DataPoint *data){
    
    // Initially check all children of root to insert data
    // It no blank node found then propagate with existing node where it fits
    for(int i=0; i<4; i++){
        float x1 = this->childLocations[i][0];
        float y1 = this->childLocations[i][1];
        float x2 = x1 + (this->lengthX/2);
        float y2 = y1 + (this->lengthY/2);
        
        if(this->SearchInArea(x1, y1, x2, y2, data)){
            if(this->children[i] == NULL){
                this->children[i] = new QuadNode(x1, y1, this->lengthX/2, this->lengthY/2);
                this->children[i]->CopyData(data);
                
                this->childCount++;
                
                totalChildren++;
                
            }else{
                this->AddDataToNode(i, data);
            }
            
            break;
            
        }
    }
}

//#pragma Private_Methods
void QuadNode::AddDataToNode(int childIndex, DataPoint *data){
    
    QuadNode* child = this->children[childIndex];
    QuadNode* newNode = NULL;
    
    float x1 = child->startX;
    float y1 = child->startY;
    float x2 = x1 + (child->lengthX/2);
    float y2 = y1 + (child->lengthY/2);
    
    // If Child data has more children
    if(child->data == NULL){
        
        // Search whether data will fit in this child area or not
        if(child->SearchInArea(data)){
            // For each of the child check whether data will fit or not
            for(int i=0; i<4; i++){
                x1 = child->childLocations[i][0];
                y1 = child->childLocations[i][1];
                x2 = x1 + (child->lengthX/2);
                y2 = y1 + (child->lengthY/2);
                
                if(child->SearchInArea(x1, y1, x2, y2, data)){
                    if(child->children[i] == NULL){
                        child->children[i] = new QuadNode(x1, y1, child->lengthX/2, child->lengthY/2);
                        child->children[i]->CopyData(data);
                        
                        child->childCount++;
                        
                        totalChildren++;
                        
                    }else{

                        child->AddDataToNode(i, data);
                    }
                    
                    break;
                }
            }
        }else{
            // If data is under parent region but not in child region
            // There must have a node in between parent and child where data will fit
            // Find that resolution point
            // Create new node, make this node as a child of parent and previous child of parent will be a child
            // of this new node.
            float d[4];
            this->FindResolution(child, data, d);
            newNode = new QuadNode(d[0], d[1], d[2], d[3]);
            
            bool first = false, second = false;
            
            for(int i=0; i<4; i++){
                x1 = newNode->childLocations[i][0];
                y1 = newNode->childLocations[i][1];
                x2 = x1 + (newNode->lengthX/2);
                y2 = y1 + (newNode->lengthY/2);
                
                if(newNode->SearchInArea(x1, y1, x2, y2, data)){
                    newNode->children[i] = new QuadNode(x1, y1, newNode->lengthX/2, newNode->lengthY/2);
                    
                    newNode->children[i]->CopyData(data);
                    
                    newNode->childCount++;
                    
                    totalChildren++;
                    
                    first = true;
                    
                }else if(child->startX >= x1 && child->startY >= y1 && (child->startX+child->lengthX) <= x2 &&
                         (child->startY+child->lengthY) <= y2){
                    
                    newNode->children[i] = child;
                    
                    newNode->childCount++;
                    
                    //totalChildren++;
                    
                    second = true;
                }
                
                if(first&& second)break;
            }
            
            this->children[childIndex] = newNode;
            
            if(second == false){
                this->InsertNewNodes(newNode, child);
                
                delete child;
            }
        }
    }else{
        // Child node has no children
        // Search all children where data will be fit.
        if(child->SearchInArea(data)){
            
            // Check this node is the root of these two datapoint or not
            float qx = roundf(child->data->getPosition(0)*100000);
            float qy = roundf(child->data->getPosition(1)*100000);
            
            float dx = roundf(data->getPosition(0)*100000);
            float dy = roundf(data->getPosition(1)*100000);
            
            float cx = roundf(child->id_x*100000);
            float cy = roundf(child->id_y*100000);
            
            float p = (qx - cx)>=0?(dx-cx)>=0?1:-1:(dx-cx)<0?1:-1;
            float q = (qy - cy)>=0?(dy-cy)>=0?1:-1:(dy-cy)<0?1:-1;
            
            // In two different children
            if(p*q == -1){
                bool first = false, second = false;
                
                for(int i=0; i<4; i++){
                    x1 = child->childLocations[i][0];
                    y1 = child->childLocations[i][1];
                    x2 = x1 + (child->lengthX/2);
                    y2 = y1 + (child->lengthY/2);
                    
                    if(child->SearchInArea(x1, y1, x2, y2, data)){
                        
                        child->children[i] = new QuadNode(x1, y1, child->lengthX/2, child->lengthY/2);
                        
                        child->children[i]->CopyData(data);
                        
                        child->childCount++;
                        
                        totalChildren++;
                        
                        first = true;
                        
                    }else if(child->SearchInArea(x1, y1, x2, y2, child->data)){
                        
                        child->children[i] = new QuadNode(x1, y1, child->lengthX/2, child->lengthY/2);
                        
                        child->children[i]->CopyData(child->data);
                        
                        child->childCount++;
                        
                        second = true;
                    }
                    
                    if(first&&second)break;
                }
                
                child->data = NULL;
            
            }else{
                float d[4];
                child->FindResolution(child->data, data, d);
                newNode = new QuadNode(d[0], d[1], d[2], d[3]);
                
                bool first = false, second = false;
                
                for(int i=0; i<4; i++){
                    x1 = newNode->childLocations[i][0];
                    y1 = newNode->childLocations[i][1];
                    x2 = x1 + (newNode->lengthX/2);
                    y2 = y1 + (newNode->lengthY/2);
                    
                    if(newNode->SearchInArea(x1, y1, x2, y2, data)){
                        newNode->children[i] = new QuadNode(x1, y1, newNode->lengthX/2, newNode->lengthY/2);
                        
                        newNode->children[i]->CopyData(data);
                        
                        newNode->childCount++;
                        
                        totalChildren++;
                        
                        first = true;
                        
                    }else if(newNode->SearchInArea(x1, y1, x2, y2, child->data)){
                        newNode->children[i] = new QuadNode(x1, y1, newNode->lengthX/2, newNode->lengthY/2);
                        
                        newNode->children[i]->CopyData(child->data);
                        
                        newNode->childCount++;
                        
                        second = true;
                    }
                    
                    if(first && second) break;
                }
                
                this->children[childIndex] = newNode;
                
                if(second == false){
                    this->InsertNewNodes(newNode, child);
                }
                
                delete child;
            }
            
            
        }else{
            float d[4];
            this->FindResolution(child, data, d);
            newNode = new QuadNode(d[0], d[1], d[2], d[3]);
            
            bool first = false, second = false;
            
            for(int i=0; i<4; i++){
                x1 = newNode->childLocations[i][0];
                y1 = newNode->childLocations[i][1];
                x2 = x1 + (newNode->lengthX/2);
                y2 = y1 + (newNode->lengthY/2);
                
                if(newNode->SearchInArea(x1, y1, x2, y2, data)){
                    newNode->children[i] = new QuadNode(x1, y1, newNode->lengthX/2, newNode->lengthY/2);
                    
                    newNode->children[i]->CopyData(data);
                    
                    newNode->childCount++;
                    
                    totalChildren++;
                    
                    first = true;
                    
                }else if(child->startX >= x1 && child->startY >= y1 && (child->startX+child->lengthX) <= x2 &&
                         (child->startY+child->lengthY) <= y2){
                    
                    newNode->children[i] = child;
                    
                    newNode->childCount++;
                    
                    totalChildren++;
                    
                    second = true;
                }
                
                if(first && second) break;
            }
            
            this->children[childIndex] = newNode;
            
            if(second == false){
                this->InsertNewNodes(newNode, child);
                
                delete child;
            }
        }
    }
}

int QuadNode::InsertNewNodes(QuadNode *newNode, QuadNode *child){

    if(child == NULL) return 0;
    
    for(short i=0; i<4; i++){
        QuadNode* gChild = child->children[i];
        
        if (gChild != NULL) {
            if(gChild->data != NULL){
                totalChildren--;
                newNode->AddDataToNode(gChild->data);
            
            }else{
                this->InsertNewNodes(newNode, gChild);
            }
        }
    }
    
    
    return 0;
}


bool QuadNode::SearchInArea(float x1, float y1, float x2, float y2, DataPoint *query){
    
    float x = roundf(query->getPosition(0)*100000);
    float y = roundf(query->getPosition(1)*100000);
    
    x1 = roundf(x1*100000);
    y1 = roundf(y1*100000);
    x2 = roundf(x2*100000);
    y2 = roundf(y2*100000);
    
    if(x >= x1 && x<=x2 && y >= y1 && y<=y2){
        return true;
    }
    
    return false;
}

bool QuadNode::SearchInArea(DataPoint *query){
    
    float x = roundf(query->getPosition(0)*100000);
    float y = roundf(query->getPosition(1)*100000);
    
    float x1 = roundf(this->startX*100000);
    float y1 = roundf(this->startY*100000);
    float x2 = roundf((this->startX+this->lengthX)*100000);
    float y2 = roundf((this->startY+this->lengthY)*100000);
    
    if(x >= x1 && y >= y1 && x<=x2 && y<=y2){
        return true;
    }
    
    return false;
}

void QuadNode::FindResolution(QuadNode *child, DataPoint *query, float* d){
    
    float lrx = roundf((this->lengthX/2)*100000);
    float lry = roundf((this->lengthY/2)*100000);
    
    float qx = roundf(child->id_x*100000);
    float qy = roundf(child->id_y*100000);
    
    float dx = roundf(query->getPosition(0)*100000);
    float dy = roundf(query->getPosition(1)*100000);
    
    float cx = roundf(this->id_x*100000);
    float cy = roundf(this->id_y*100000);
    
    float p = (qx - cx)>=0?(dx-cx)>=0?1:-1:(dx-cx)<0?1:-1;
    float q = (qy - cy)>=0?(dy-cy)>=0?1:-1:(dy-cy)<0?1:-1;
    
    while(p == 1 && q == 1){
        
        lrx = lrx/2;
        lry = lry/2;
        
        if(cx<qx){
            cx = cx + lrx;
        }else{
            cx = cx - lrx;
        }
        
        if(cy<qy){
            cy = cy + lry;
        }else{
            cy = cy - lry;
        }
        
        p = (qx - cx)>=0?(dx-cx)>=0?1:-1:(dx-cx)<0?1:-1;
        q = (qy - cy)>=0?(dy-cy)>=0?1:-1:(dy-cy)<0?1:-1;
    }
    
    //newNode = new QuadNode(cx-lrx, cy-lry, 2*lrx, 2*lry);
    
    d[0] = (cx-lrx)/100000;
    d[1] = (cy-lry)/100000;
    d[2] = (2*lrx)/100000;
    d[3] = (2*lry)/100000;
}

void QuadNode::FindResolution(DataPoint *query1, DataPoint *query2, float* d){
    float qx = roundf(query1->getPosition(0)*100000);
    float qy = roundf(query1->getPosition(1)*100000);
    
    float dx = roundf(query2->getPosition(0)*100000);
    float dy = roundf(query2->getPosition(1)*100000);
    
    float cx = roundf(this->id_x*100000);
    float cy = roundf(this->id_y*100000);
    
    float p = (qx - cx)>=0?(dx-cx)>=0?1:-1:(dx-cx)<0?1:-1;
    float q = (qy - cy)>=0?(dy-cy)>=0?1:-1:(dy-cy)<0?1:-1;
    
    float lrx = roundf((this->lengthX/2)*100000);
    float lry = roundf((this->lengthY/2)*100000);
    
    while(p == 1 && q == 1){
        
        lrx = lrx/2;
        lry = lry/2;
        
        if(cx<qx){
            cx = cx + lrx;
        }else{
            cx = cx - lrx;
        }
        
        if(cy<qy){
            cy = cy + lry;
        }else{
            cy = cy - lry;
        }
        
        p = (qx - cx)>=0?(dx-cx)>=0?1:-1:(dx-cx)<0?1:-1;
        q = (qy - cy)>=0?(dy-cy)>=0?1:-1:(dy-cy)<0?1:-1;
    }
    
    //newNode = new QuadNode(cx-lrx, cy-lry, 2*lrx, 2*lry);
    
    d[0] = (cx-lrx)/100000;
    d[1] = (cy-lry)/100000;
    d[2] = (2*lrx)/100000;
    d[3] = (2*lry)/100000;
}


