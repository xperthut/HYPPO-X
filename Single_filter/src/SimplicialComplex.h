
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: SimplicialComplex.h
 Objective: Class to create simplicial complex
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              08/17/2016      Initial version
 Md. Kamruzzaman              04/11/2017      Implement bit logic to create simplex
 
 **************************************************************************************************/

#ifndef SimplicialComplex_h
#define SimplicialComplex_h

#include <unordered_map>
#include <map>
#include <unordered_set>
#include <list>
#include <string>
#include <bitset>
#include "Phenotype.h"

#define BIT_SET_SIZE 10

using namespace std;

class SimplexList{
public:
    list<string> slist;
    SimplexList(){}
    string GetAllData(){
        string str = "";
        
        list<string>::iterator sitr;
        
        for(sitr = slist.begin(); sitr != slist.end(); sitr++){
            if(str.length() > 0) str += "\n";
            
            str += *sitr;
        }
        
        return str;
    }
};

class SimplicialComplex{
private:
    unordered_map<long, float> zeroSimplex;
    unordered_map<string, float> oneOrMoreSimplex;
    map<float, SimplexList> simplexMap;
    
    unordered_set<string> indvTracker;
    unordered_set<string> keyTracker;
    unordered_map<string, float> simplexFiltrationMap;
    
    void AddToOneOrMoreSimplex(string nodeList, float filtrationValue);
    void AddToSimplexMap(string key, float &filteration);
public:
    SimplicialComplex();
    ~SimplicialComplex();
    
    void AddSimplicialComplex(Phenotype* ph, float &filtration);
    void CreateSimplex(list<long>* idList, float &filtration);
    void clearTracker();
    string PrintSimplex();
    
    // Depricated methods
    void AddZeroSimplex(long node, float filtrationValue);
    void AddOneSimplex(long node1, long node2, float filtrationValue);
    void AddTwoSimplex(long node1, long node2, long node3, float filtrationValue);
    void AddThreeSimplex(long node1, long node2, long node3, long node4, float filtrationValue);
    string PrintSimplex_depricated();
    
};

#endif /* SimplicialComplex_h */
