/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: Graph.h
 Objective: Class to generate graph
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              02/16/2017      Initial version
 Md. Kamruzzaman              06/20/2017      Added logic to color connected components, either all components under same coloring scheme or separate coloring scheme for each connect component, controlled by a flag
 Md. Kamruzzaman              01/06/2018      Code revision, fixing obscure logic issue
 **************************************************************************************************/

#ifndef graph_h
#define graph_h

#include "fileHandler.h"
#include "individual.h"
#include "RGBColor.h"
#include <bitset>
#include <vector>

class InterestingPath{
private:
    CustomHashSet<string> path;
    float score;
    unsigned long rank;
    string color;
    
public:
    InterestingPath();
    ~InterestingPath();
    
    void setScore(float s);
    float getScore();
    void setRank(unsigned long r);
    unsigned long getRank();
    void addToPath(string edge);
    unordered_set<string>& getPath();
    void setColor(string c);
    string getColor();
    bool PathContainsEdge(string eID);
};

class CostTableCell{
private:
    float score;
    CostTableCell* maxPred;
    string edgeID;
    
public:
    CostTableCell();
    ~CostTableCell();
    void setScore(float s);
    float getScore();
    void setCell(CostTableCell* mp);
    CostTableCell* getCell();
    string getEdgeID();
    void setEdgeID(string eID);
};

class Property{
private:
    float *weight;
    unsigned long depth;
public:
    Property();
    ~Property();
    float getWeight(short index);
    void setWeight(float value, short index);
    unsigned long getDepth();
    void setDepth(unsigned long d);
};

class rNode:public Property{
private:
    unsigned long id;
    unsigned long in_degree;
    unsigned long out_degree;
    float size;
    bool isSource;
    bool isMarked;
    vector<rNode*> nextNodes;
    vector<rNode*> prevNodes;
    CustomHashMap<string, Phenotype*> points;
    
    
public:
    rNode(unsigned long id);
    ~rNode();
    
    unsigned long getID();
    float getSize();
    unsigned long getTotalInDegree();
    unsigned long getTotalOutDegree();
    bool IsThisNodeASource();
    rNode* getNextNode(unsigned long index);
    rNode* getPrevNode(unsigned long index);
    void removeNextNode(rNode* n);
    void removePrevNode(rNode* n);
    
    void setSize(float size);
    void addInDegree();
    void addOutDegree();
    void setSource(bool source);
    
    void createLinkNodes();
    void addNextNode(rNode* nextNode);
    void addPrevNode(rNode* prevNode);
    void resetLinkNodes();
    void addPoint(Phenotype* ph);
    string getPhListForD3();
};

class rEdge:public Property{
private:
    rNode* fromNode;
    rNode* toNode;
    rEdge* refEdge;
    bitset<BIT_SET_SIZE> signature;
    CostTableCell* cells;
    
    bool isMarked;
    string id;
    unsigned long ipRank;
    string color;
    
public:
    rEdge(rNode* node1, rNode* node2);
    ~rEdge();
    
    string getID();
    rNode* getFromNode();
    rNode* getToNode();
    bool getMark();
    void getSignature(unsigned long &sig);
    void getSignature(string &sig);
    bool getSignatureAtIndex(short index);
    rEdge* getRefEdge();
    unsigned long getIPRank();
    void setIPRank(unsigned long r);
    void setColor(string c);
    string getColor();
    
    void setMark(bool mark);
    void setSignatue(bool value, short index);
    void setRefEdge(rEdge* e);
    
    void swapNodes();
    bool matchSignature(rEdge* e);
    bool strictMatchSignature(rEdge* e);
    void assignScore(rEdge *eMax);
    
    void createCells();
    void destroyCells();
    float getScoreOfCell(unsigned long index);
    void setScoreOfCell(float score, unsigned long index);
    CostTableCell* getCostTableCellOfIndex(unsigned long index);
    void setCostTableCellOfIndex(CostTableCell* cell, unsigned long index);
};

class IsolatedSubGraph{
private:
    CustomHashMap<unsigned long, rNode*> nodes;
    CustomHashMap<string, rEdge*> edges;
    CustomHashSet<unsigned long> sourceNodeIdSet;
    CustomHashSet<unsigned long> exploreSet;
    CustomHashSet<string> terminalEdgeSet;
    
    unsigned int ccNumber;
    float nodeSizeRange[2];
    float **nodeWeightRange;
    string *envName;
    unsigned long maxDepth;
    CostTableCell* maxScoredCell;
    
    float globalNodeSizeRange[2];
    float **globalNodeWeightRange;
    
    void addNode(unsigned long id);
    void setRange();
    rEdge* getMaxPredecessor(rEdge* e);
    bool needMoreExplore(rEdge* e);
    bool checkTerminalEdge(rEdge* e);
    void exploreNode();
    void resetDepth();
    void identifySourceNodes();
    void runBFS();
    unsigned long computeMaxLengthPath();
    void computeCostTableForColumn(unsigned long index);
    void computeCostTable();
    void createEdgeCostList();
    void printTable();
    void getMaximumInterestingPath(CustomList<InterestingPath>* IPs);
    bool EdgeBelongsInPathList(string eID, CustomList<InterestingPath>* IPs);
    string getEdgeColor(string eID, CustomList<InterestingPath>* IPs);
    float adjustSize(float size);
public:
    IsolatedSubGraph();
    ~IsolatedSubGraph();
    
    bool hasNode(unsigned long id);
    rNode* getNodeById(unsigned long id);
    void addPointToNode(unsigned long id, Phenotype* ph);
    void addEdge(unsigned long id1, unsigned long id2);
    void setupLinks();
    string getGMLNodes();
    string getGMLEdges(CustomList<InterestingPath>* IPs);
    void getNodeSizeAndWeightRange(float *size, float **weight);
    void setGlobalSizeRange(float size, short index);
    void setGlobalWeightRange(float *size, short index);
    
    void computeInterestingPaths(CustomList<InterestingPath>* IPs);
    void recoverNodeLinks();
    bool addRankColorToEdge(string edgeID, string color, unsigned long rank);
    
    void getD3Nodes(CustomList<string>* nodeListJS);
    void getD3Edges(CustomList<string>* edgeListJS, CustomList<InterestingPath>* IPs);
};

class Graph{
private:
    IsolatedSubGraph* connectedComponent;
    short totalCC;
    float globalNodeSizeRange[2];
    float **globalNodeWeightRange;
    CustomList<InterestingPath> IPs;
    
    void generatePathColor();
    
public:
    Graph(short totalCC);
    ~Graph();
    
    IsolatedSubGraph* getConnectedComponent(short index);
    void printGMLGraph();
    void setGlobalNodeSizeAndWeightRange(float *size, float **weight);
    void generateInterestingPaths();
    void printD3Graph();
};

//// Definition of global methods ///////
string fixPrecision(float value, int precision);
bool comparePaths(InterestingPath& p1, InterestingPath& p2);

#endif /* graph_h */
