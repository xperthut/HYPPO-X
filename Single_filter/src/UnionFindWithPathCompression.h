
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: UnionFindWithPathCompression.h
 Objective: Class to find connected components in a graph
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              07/16/2016      Initial version
 
 **************************************************************************************************/

#ifndef UnionFindWithPathCompression_h
#define UnionFindWithPathCompression_h

#include <iostream>
#include <list>
#include <unordered_map>

using namespace std;

class UnionFindWithPathCompression{
private:
    long totalIds;
    long* s;
    
    void UnionSets(long rootA, long rootB);
    long Find(long id);
    void Adjust();
    
public:
    UnionFindWithPathCompression(long totalIds);
    ~UnionFindWithPathCompression();
    void Union(long idA, long idB);
    
    void PrintArray();
    void PrintForest();
    unordered_map<long, list<long> > GetAllConnectedComponents();
};

#endif /* UnionFindWithPathCompression_h */
