
/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: quadNode.h
 Objective: Class holds data of a node of a quad tree
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              02/21/2016      Initial version
 
 **************************************************************************************************/

#ifndef quadNode_h
#define quadNode_h

#include "dataPoint.h"

class QuadNode{
private:
    float id_x;
    float id_y;
    float startX;
    float startY;
    float lengthX;
    float lengthY;
   
    float childLocations[4][2];
    QuadNode *children[4];
    
    DataPoint *data;
    bool hasChildren;
    int childCount;
    QuadNode* parent;
    
    void AddDataToNode(int childIndex, DataPoint *data);
    bool SearchInArea(float x1, float y1, float x2, float y2, DataPoint* query);
    bool SearchInArea(DataPoint* query);
    void FindResolution(QuadNode* child, DataPoint* query, float* d);
    void FindResolution(DataPoint* query1, DataPoint* query2, float* d);
    int InsertNewNodes(QuadNode* newNode, QuadNode* child);
    
public:
    QuadNode(float x, float y, float lx, float ly);
    ~QuadNode();
    
    static int totalChildren;
    
    // Getter methods
    float GetStartXPosition();
    float GetStartYPosition();
    float GetLengthAlongXAxis();
    float GetLengthAlongYAxis();
    QuadNode* GetChild(int index);
    void SetChild(int index, QuadNode* child);
    bool IsThisLeafNode();
    DataPoint* GetNodeData();
    int GetTotalChildren();
    QuadNode* GetParent();
    void SetParent(QuadNode* parent);
    void ResetChildren();
    float GetNodePositionX();
    float GetNodePositionY();
    
    // Setter
    void SetArea(float x, float y, float lx, float ly);
    
    //Other methods
    void AddDataToNode(DataPoint *data);
    void CopyData(DataPoint* data);
};

#endif /* QuadNode_h */
