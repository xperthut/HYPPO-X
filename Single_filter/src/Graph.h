
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
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
 
 **************************************************************************************************/

#include "RGBColor.h"
#include "config.h"
#include <stdlib.h>
#include <math.h>
#include <map>
#include <iostream>
#include "FileHandler.h"
#include "Box.h"
#include "Individual.h"


class AnEdge{
    long node1;
    long node2;
    string id;
public:
    AnEdge(long n1, long n2){
        this->node1 = n1;
        this->node2 = n2;
        
        // An undirected edge from low cluster id to high cluster id
        id = (n1<n2)?to_string(n1)+"#"+to_string(n2):to_string(n2)+"#"+to_string(n1);
    }
    
    ~AnEdge(){}
    long getNode1(){return node1;}
    long getNode2(){return node2;}
};



class Edge{
public:
    unordered_map<string, pair<float, Box>> edgeList;
};

class Edges{
public:
    long node1;
    long node2;
    float face_weight;
    
    Edges(long node1, long node2, float face_weight){
        this->node1 = node1;
        this->node2 = node2;
        this->face_weight = face_weight;
    }
};

class Node{
public:
    unordered_map<string,list<Phenotype*>> nodeList;
    unordered_map<string,list<DataPoint*>> nodeListDP;
    unordered_map<long, unordered_map<string, list<float>>> phenotypeVariation; // KS_A,KS_B,NE_A,NE_B
    unordered_map<long, unordered_map<string, list<float>>> envVariation[FILTER];
};

class Graph{
public:
    Node node;
    Edge edge;
    unordered_map<string, Edges> edgeList;
    unordered_map<string, Edges>::iterator edgeItr;
    unordered_map<long, unordered_map<long, list<Phenotype*>>> nodeList;
};

class GmlNode{
public:
    long id;
    float size;
    int relSize=20;
    string color="#ffffff";
    string colorX="#ffffff";
    string colorY="#ffffff";
    float avgValue;
    float avgValueX[FILTER];
    
    void adjustColor(float min, float max){
        RGBColor rgbColor(min, max, .1);
        color = rgbColor.GetColor(avgValue);
    }
    
    void adjustColorByX(float min, float max, int index){
        RGBColor rgbColor(min, max, .1);
        colorX = rgbColor.GetColor(avgValueX[index]);
    }
    
    void adjustSize(float minSize, float maxSize){
        
        int max = 50, min = 20;
        
        float alpha = (maxSize==minSize)?0:(size-minSize)/(maxSize-minSize);
        relSize = alpha*(max-min)+min;
        
    }
};

class GmlEdge{
public:
    long node1;
    long node2;
    float weight;
    int relWeight;
    bool arrow = false;
    //string color = "#a6a6a6";
    string color = "#000000";
    
    void adjustWeight(float minW, float maxW){
        int max = 8, min = 2;
        
        float alpha = (minW==maxW)?0:(weight-minW)/(maxW-minW);
        relWeight = alpha*(max-min)+min;
    }
};

class GmlGraph{
public:
    list<GmlNode> gmlNodeList;
    list<GmlEdge> gmlEdgeList;
    list<GmlNode>::iterator gnItr;
    list<GmlEdge>::iterator geItr;
};

class Environment{
private:
    float weight[FILTER];
    
public:
    Environment();
    ~Environment();
    
    // Getter
    float getWeight(int index);
    
    // Setter
    void setWeight(float value, int index);
};

class rNode{
private:
    long id;
    float weight;
    Environment env;
    int in_degree;
    int out_degree;
    float size;
    bool isSource;
    bool isMarked;
    short variation;
    rNode** nextNodes;
    rNode** prevNodes;
    CustomHashMap<string, list<float>> phenotypeSummary;
    
public:
    rNode(rNode* node);
    rNode(long id, float weight, int size);
    ~rNode();
    
    // Getter
    long getID();
    float getWeight();
    float getSize();
    int getTotalInDegree();
    int getTotalOutDegree();
    float getEnvWeight(int index);
    string getEnvName(int index);
    bool IsThisNodeASource();
    rNode* getNextNode(int index);
    rNode* getPrevNode(int index);
    short getVariations();
    unordered_map<string, list<float>>& getPhenotyeSummary();
    int getGenLocWeight(string gen, string loc);
    string getGenLoc();
    string getNodeJS();
    
    // Setter
    void setID(long id);
    void setWeight(float weight);
    void setSize(float size);
    void addInDegree();
    void addOutDegree();
    void setEnvWeight(float weight, int index);
    void setEnvName(string name, int index);
    void setSource(bool source);
    void setVariations(short v);
    void setPhenotypeSummary(string genotype, string location, long plantID, int dap, float filter, float value);
    int getGenLocColorIndex();
    
    void createLinkNodes();
    void addNextNode(rNode* nextNode);
    void addPrevNode(rNode* prevNode);
    void resetLinkNodes();
};

class rEdge{
private:
    rNode* fromNode;
    rNode* toNode;
    rEdge* refEdge;
    bitset<FILTER> signature;
    
    float normalizeWeight[FILTER+1];
    float edgeWeight[FILTER+1];
    float weight;
    float score;
    int rank;
    bool isMarked;
    
public:
    rEdge(rNode* node1, rNode* node2);
    ~rEdge();
    
    // Getter
    rNode* getFromNode();
    rNode* getToNode();
    float getWeight();
    float getScore();
    int getRank();
    bool getMark();
    void getSignature(unsigned long &sig);
    void getSignature(string &sig);
    float getNormalizedWeight(int index); // index=0 for phenotype
    float getEdgeWeight(int index);
    bool getSignatureAtIndex(int index);
    rEdge* getRefEdge();
    
    // Setter
    void setScore(float score);
    void setRank(int rank);
    void setWeight(float weight);
    void setMark(bool mark);
    void setSignatue(bool value, int index);
    void setNormalizedWeight(float value, int index);
    void setEdgeWeight(float value, int index);
    void setRefEdge(rEdge* e);
    
    void swapNodes();
    bool matchSignature(rEdge* e);
    bool strictMatchSignature(rEdge* e);
};

class Path{
private:
    CustomHashSet<string> path;
    float score;
    string color;
    float width;
    
public:
    Path();
    ~Path();
    void addToPath(string id);
    unordered_set<string>& getPath();
    string getNodeIds();
    void setScore(float score);
    float getScore();
    void setColor(string color);
    string getColor();
    bool hasNodes(string id);
    list<long> getAllNodesInAPath();
};

class RGB{
private:
    short R,G,B;
public:
    RGB();
    RGB(short R, short G, short B);
    ~RGB();
    
    // Getter
    short getR();
    short getG();
    short getB();
    
    // Setter
    void setR(short R);
    void setG(short G);
    void setB(short B);
};

bool comparePaths(Path* p1, Path* p2);
bool compareEdgeScore(rEdge* e1, rEdge* e2);
string getGenLocColor(string gen, string loc);
string getPlantIdColor(int plantID);
string fixPrecision(float value, int precision);

class CompositeGraph{
private:
    CustomHashMap<long, rNode*> nodeList;
    CustomHashMap<string, rEdge*> edgeMap;
    CustomHashSet<long> sourceNodeIds;
    CustomHashSet<long> additionalSrcNodeIds;
    CustomHashSet<rEdge*> terminalEdges;
    CustomHashMap<int, Path*> interestingPaths;
    CustomHashMap<string, rEdge*> maxTermEdges;
    CustomHashSet<long> exploreList;
    
    //set<string> newIdSet;
    
    int pathCounter;
    float nodeSizeRange[2];
    float nodeWeightRange[2];
    float envWeightRange[FILTER][2];
    string envName[FILTER];
    
    bool hasNode(long id);
    rNode* getNodeById(long id);
    float adjustSize(float size);
    string calculateNodeDegree(bool isPhenotype, bool isEnv, int index);
    void constructGraph(bool isPhenotype, bool isEnv, int index);
    string printSourceNodes();
    void runBFS();
    bool exploreNode();
    bool needMoreExplore(rEdge* e);
    float calculateInterestingnessScore(float weight, int rank);
    bool willFollowEdge(long fromID, long toID, rEdge* eMax);
    void assignScore(long fromID, long toID, rEdge* eMax);
    rEdge* getMaxRank(long id);
    rEdge* getMaxRank(long id, rEdge* e);
    rEdge* getMaxRank(rEdge* edge);
    void reconstructPaths();
    bool exploreEdge(rEdge* edge);
    rEdge* getEdgeOfRank(rEdge* edge);
    string printPaths();
    void getWidthAndColor(long fromID, long toID, string* property);
    void assignPathColor();
    void normalizeByUniformDistribution();
    void normalizeByStandardDeviation();
    void normalizeWeight();
    void absoluteDistanceWeight();
    void setEdgeWeightAndSignature();
    bool getEdgeDirection(rEdge* edge, int index);
    string printGraph(bool isPhenotype, bool isEnv, int index, string fileNameSuffix);
    void constructCompositeGraph();
    string printCompositeGraph(bool withSignature, string fileNameSuffix);
    bool checkTerminalEdge(long toID, rEdge* e);
    string printTerminalEdges();
    void computeMaxTerminalEdges();
    
    // Remove edges where all the nodes are not in same variations
    void filterEdges();
    void updateStorageRange(float nsRange[2], float nwRange[2], float ewRange[FILTER][2]);
    
    // Special cases, find longest path on phenotype graph only
    void runBFSonPhenotype();
    bool exploreNodeInPhenotypeGraph();
    rEdge* getMaxRankInPhenotypeGraph(long id);
    bool checkTerminalEdgeForPhenotype(long id);
    
public:
    CompositeGraph();
    ~CompositeGraph();
    
    bool hasNode(long id1, long id2);
    void addNode(rNode *node);
    void addEdge(long id1, long id2);
    void storeRange(float nsRange[2], float nwRange[2], float ewRange[FILTER][2], string ewName[FILTER]);
    float getMeanValue(list<float> val);
    string printCompositeGraphsWithPath(string fileNameSuffix);
    string printOriginarGraphs(string fileNameSuffix, bool isPhenotype, bool isEnv, int index);
    
    void findInterestingPaths(string fileNameSuffix);
    void analyzePaths(int begin, size_t last, bool allPaths, Graph* graph, string fileNameSuffix);
    string analyzeNodes();
    void analyzeAndSummarizePaths(string fileNameSuffix);
};

class MainGraph{
private:
    CompositeGraph* connectedComponents;
    int ccCounter;
    string envName[FILTER];
    
    void printCompositeGraph(string fileNameSuffix);
    void printOriginalGraph(string fileNameSuffix);
public:
    MainGraph(int ccCounter);
    ~MainGraph();
    void setEnvName(string env[]);
    CompositeGraph* getCC(int index);
    void printAllGraphs(string fileNameSuffix);
    void PrintAllNodeStatistics(string fileNameSuffix);
};
