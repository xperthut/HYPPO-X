
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: PersistentHomology.h
 Objective: Class to controll input file reading and pass the input for graph generation
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              06/15/2016      Initial version
 
 **************************************************************************************************/


#ifndef PersistentHomology_h
#define PersistentHomology_h

#include "ClusteringRegion.h"
#include <string>

class PersistentHomology{
private:
    Grid **mesh;
    QuadTree *treeRoot;
    ClusteringRegion* clusterRegion;
    list<DataPoint*> dataPointList;
    string fileName;
    int slice_lx;
    int slice_ly;
    float gridLength_x;
    float gridLength_y;
    float minPos[FILTER];
    float maxPos[FILTER];
    
    void CreateUniqueClusters();
    
public:
    PersistentHomology();
    ~PersistentHomology();
    void CreateTreeAndMeshGrid();
    void CreateClusterAndGraph();
    
    float GetMinPos(int index);
    float GetMaxPos(int index);
};

#endif /* PersistentHomology_h */
