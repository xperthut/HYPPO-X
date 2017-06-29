
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: QuadTree.h
 Objective: Class holds data of a quad tree
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              02/21/2016      Initial version
 
 **************************************************************************************************/

#ifndef QuadTree_h
#define QuadTree_h

#include <iostream>
#include "QuadNode.h"
#include "QuickSort.h"
#include <set>

class QuadTree{
private:
    QuadNode* root;
    
    int PrintNode(QuadNode* node);
    int SearchSurfaceInNode(QuadNode *node, float x1, float y1, float x2, float y2, list<Phenotype*> *phList, list<DataPoint*> *dpList, long boxID);
    int SearchDataPoint(QuadNode *node, float x1, float y1, float x2, float y2, set<long> *boxIdList);
    
public:
    QuadTree(float x, float y, float lx, float ly);
    ~QuadTree();
    QuadNode* GetRoot();
    void AddDataToNode(DataPoint* data);
    void PrintQuadTree();
    void SearchSurface(float x1, float y1, float x2, float y2, list<Phenotype*> *phList, list<DataPoint*> *dpList, long boxID);
    void SearchDataPoints(float x1, float y1, float x2, float y2, set<long> *boxIdList);
    void SearchAndGetDataPointOfBoxId(float x1, float y1, float x2, float y2, long boxID);
};

#endif /* QuadTree_h */
