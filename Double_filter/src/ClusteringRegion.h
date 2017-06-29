
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: ClusteringRegion.h
 Objective: This class does following tasks:
            1. Split windows along each filter function
            2. Create clusters 
            3. Create simplicial complex
            4. Generate and output graph as a gml formated file
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              03/02/2016      Initial version
 Md. Kamruzzaman              07/17/2016      Each rectangle has a center. Fix the center point and
                                                increase the length along each filter function to create 
                                                an overlapping.
 Md. Kamruzzaman              05/02/2017      Change graph building logic
 Md. Kamruzzaman              05/20/2016      Change simplex building logic
 
 **************************************************************************************************/

#ifndef ClusteringRegion_h
#define ClusteringRegion_h

#include "QuadTree.h"
#include "DBScan.h"
#include "UnionFindWithPathCompression.h"
#include "SimplicialComplex.h"
#include "Graph.h"
#include <vector>

class Grid{
public:
    float center_x;
    float center_y;
};

class ClusteringRegion{
private:
    QuadTree* tree;
    float xMin, xMax, yMin, yMax;
    float windowSizeX, windowSizeY, overlapX, overlapY, shiftX, shiftY;
    int totalSplitX,totalSplitY;
    float* xPositions, *yPositions;
    int maxBlockX, maxBlockY;
    list<Phenotype*> clusterPhList;
    float Eps;
    float minPoints;
    set<long> nodeList;
    list<set<long>> edgeList;
    Graph graph;
    GmlGraph gmlGraph;
    long totalClusters;
    map<long, string> tries, tetras;
    map<string, long> ctries, ctetras;
    map<long,BoxRectangle> boxRectangleMap;
    map<int, int> varietyMap;
    bool isVarietyMapHasCreated;
    bool isDp;
    map<string, int> edgeThresholdMap;
    
    void SplitRegion();
    void AssignPoints();
    void CreateCluster(list<Phenotype*> *pList, bool setUniqueId);
    void CreateCluster(list<DataPoint*> *dpList);
    void CopyCluster(list<Phenotype*> cpList);
    void CreateGraph(list<Phenotype*> cpList, short* clusterIndex, bool isHorizontalOverlap, float *BoxCoOrdinate, int *positionCoOrdinate, bool isTwoWayOverlap);
    
    void CreateGraph_new(list<Phenotype*> cpList, short* clusterIndex, bool isHorizontalOverlap, float *BoxCoOrdinate, int *positionCoOrdinate, bool isTwoWayOverlap);
    
    void CreateGraph(list<DataPoint*> dpList, short* clusterIndex, bool isHorizontalOverlap, float *BoxCoOrdinate, int *positionCoOrdinate, bool isTwoWayOverlap);
    void SortClusterIDs(long* clusterIDList, long size);
    bool IsEligibleForClustering(float x1, float y1, float x2, float y2, long boxID);
    void ManageCluster(list<Phenotype*> *pList, long minClsId, long maxClsId, set<long>* mainClsID);
    void ManageCluster(list<DataPoint*> *dpList, long minClsId, long maxClsId, set<long>* mainClsID);
    
    // Menu related methods
    void menu(MainGraph* mGraph, Graph* graph, string suffix, int totalCC);
    void setOverlappedIndv(unordered_set<string>* indvSet, list<Phenotype*>* clPhList, list<Phenotype*>* overlappedPhList, list<AnEdge*>* edgeList, unordered_set<string>* edgeSet);
    
public:
    ClusteringRegion(QuadTree* tree, float xMin, float yMin, float xMax, float yMax, float wSizeX, float wSizeY, float overlapX, float overlapY);
    ~ClusteringRegion();
    void SplitRegion(list<float> *x, list<float> *y);
    void AssignPoints(int totalBloc_x, int totalBlock_y);
    void SearchRectangle();
    void SearchRectangle(Grid** mesh);
    void SearchAndInitializeRectangle();
    void SearchAndInitializeRectangle(Grid** mesh);
    void SearchRegion();
    string GetAllClusteredPhenotype();
    string GetAllPhenotypes();
    void setOverlapX(float ox);
    void setOverlapY(float oy);
    void ClearCluster();
    void SetShift(float sx, float sy);
    void setSplit(int &x, int &y);
    
    void GenerateGraphUsingOverlappingClusters();
    void GenerateGraphUsingOverlappingClusters(Grid** mesh, list<Phenotype*>* overlappedPhList, list<AnEdge*>* edgeList);
    string PrintGraph(list<Phenotype*>* overlappedPhList, list<AnEdge*>* edgeList);
    void AddSimplicialComplex(SimplicialComplex* sc, Grid **mesh, float overlap);
    string PrintGraph(SimplicialComplex* sc);
    string PrintGraphForDataPoint(SimplicialComplex* sc);
    string GetSimplexInformation();
    void ShuffleClusters();
};

#endif /* ClusteringRegion_h */
