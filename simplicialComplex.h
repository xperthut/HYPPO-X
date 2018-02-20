
/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: simplicialComplex.h
 Objective: Class to create simplicial complex
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              08/17/2016      Initial version
 Md. Kamruzzaman              04/11/2017      Implement bit logic to create simplex
 
 **************************************************************************************************/

#ifndef simplicialComplex_h
#define simplicialComplex_h

#include "Gudhi/graph_simplicial_complex.h"
#include "Gudhi/Simplex_tree.h"
#include "Gudhi/Persistent_cohomology.h"

#include <iostream>
#include <ctime>
#include <utility>
#include <fstream>

#include <unordered_map>
#include <map>
#include <unordered_set>
#include <list>
#include <string>
#include <bitset>
#include <vector>
#include "phenotype.h"

using Simplex_tree = Gudhi::Simplex_tree<>;
using Filtration_value = Simplex_tree::Filtration_value;
using Field_Zp = Gudhi::persistent_cohomology::Field_Zp;
using Persistent_cohomology = Gudhi::persistent_cohomology::Persistent_cohomology<Simplex_tree, Field_Zp >;
using typeVectorVertex = std::vector< Simplex_tree::Vertex_handle >;
using namespace std;

class SimplexList{
public:
    list<string> slist;
    list<vector<long> > sv;
    
    SimplexList(string s){slist.push_back(s);}
    SimplexList(vector<long> v){sv.push_back(v);}
    ~SimplexList(){}
    
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
    map<float, SimplexList*> simplexMap;
    unordered_set<string> sNodes;
    
    unordered_set<string> indvTracker;
    unordered_set<string> keyTracker;
    unordered_map<string, float> simplexFiltrationMap;
    
    void AddToOneOrMoreSimplex(string nodeList, float filtrationValue);
    void AddToSimplexMap(string key, float &filteration);
    void CreateSimplex(set<long>* idList, float &filtration);
    void CreateSimplexForGudhi(set<long>* idList, float &filtration);
    
public:
    SimplicialComplex();
    ~SimplicialComplex();
    
    void AddSimplicialComplex(Phenotype* ph, float &filtration);
    void clearTracker();
    string PrintSimplex();
    float getPersistentOverlap();
};

#endif /* SimplicialComplex_h */
