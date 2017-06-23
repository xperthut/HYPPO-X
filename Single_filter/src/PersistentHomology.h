
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
    ClusteringRegion* clusterRegion;
    list<DataPoint*> dataPointList;
    string fileName;
    int slice_lx;
    int slice_ly;
    float gridLength_x;
    float gridLength_y;
    float minX;
    float maxX;
    float minY;
    float maxY;
    
    void GetCoordinates(list<float> *coordList, float percentageIncrease, float center_shift, bool xAxis);
    void CreateUniqueClusters();
    
public:
    PersistentHomology();
    ~PersistentHomology();
    void CreateTreeAndMeshGrid();
    void CreateClusterAndGraph();
    
    float GetMinX();
    float GetMaxX();
    float GetMinY();
    float GetMaxY();
};

#endif /* PersistentHomology_h */
