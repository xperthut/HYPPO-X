
/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: simplicialComplex.cpp
 Objective: Implement member functions
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              08/17/2016      Initial version
 Md. Kamruzzaman              04/11/2017      Implement bit logic to create simplex
 
 **************************************************************************************************/

#include "simplicialComplex.h"

SimplicialComplex::SimplicialComplex(){}

SimplicialComplex::~SimplicialComplex(){}

void SimplicialComplex::AddSimplicialComplex(Phenotype *ph, float &filtration){
    if(ph!=NULL){
        string indv = ph->getIndividualId();
        
        // New individual
        if(indvTracker.find(indv)==indvTracker.end()){
            unsigned long *idList = (unsigned long*)calloc(Config::TOTAL_CLUSTER_IDS, sizeof(unsigned long));
            
            ph->GetIDList(idList);
            set<long> ids;
            list<long> lid;
            
            for(short i=0; i<Config::TOTAL_CLUSTER_IDS; i++){
                if(idList[i]>0){
                    ids.insert(idList[i]);
                    lid.push_back(idList[i]);
                }
            }
            
            if(lid.size()>1){
                cout<<"";
            }
            
            free(idList);
            
            if(Config::PH_JAVA_PLEX)
                this->CreateSimplex(&ids, filtration);
            else
                this->CreateSimplexForGudhi(&ids, filtration);
        }
    }
}

void SimplicialComplex::clearTracker(){indvTracker.clear();}

string SimplicialComplex::PrintSimplex(){
    string simplex = "";
    
    for(map<float, SimplexList*>::iterator sitr = this->simplexMap.begin(); sitr != this->simplexMap.end(); sitr++){
        if(simplex.length()>0) simplex += "\n";
        
        simplex += sitr->second->GetAllData();
    }
    
    simplex += "\n";
    
    
    return simplex;
}

float SimplicialComplex::getPersistentOverlap(){
    Simplex_tree st;
    
    for(map<float, SimplexList*>::iterator itr=this->simplexMap.begin(); itr!=this->simplexMap.end(); itr++){
        for(list<vector<long> >::iterator lItr=itr->second->sv.begin(); lItr!=itr->second->sv.end(); lItr++){
            vector<long> tv = *lItr;
            typeVectorVertex SimplexVector = tv;
            st.insert_simplex_and_subfaces(SimplexVector, itr->first);
        }
    }
    
    st.set_dimension(Config::FILTER);
    
    /*
     cout << "\nThe complex contains " << st.num_simplices() << " simplices - " << st.num_vertices() << " vertices "
    << endl;
    cout << "   - dimension " << st.dimension() << endl;
    std::cout << std::endl << std::endl << "Iterator on Simplices in the filtration, with [filtration value]:"
    << std::endl;
    std::cout << "**************************************************************" << std::endl;
    std::cout << "strict graph G { " << std::endl;
    
    for (auto f_simplex : st.filtration_simplex_range()) {
        std::cout << "   " << "[" << st.filtration(f_simplex) << "] ";
        for (auto vertex : st.simplex_vertex_range(f_simplex)) {
            std::cout << static_cast<int>(vertex) << " -- ";
        }
        std::cout << ";" << std::endl;
    }
    
    std::cout << "}" << std::endl;
    std::cout << "**************************************************************" << std::endl<<"Press any key to continue...";
    */
     
    // Compute the persistence diagram of the complex
    Persistent_cohomology pcoh(st);
    // initializes the coefficient field for homology
    pcoh.init_coefficients(COEFF_FIELD_CHARACTERISTICS);
    
    pcoh.compute_persistent_cohomology(MIN_PERSISTENCE);
    
    // Output the diagram in filediag
    //pcoh.output_diagram();
    
    return pcoh.getPersistentFiltrationValue(Config::FILTER);
}

/////////////// Private Methods ////////////////////

void SimplicialComplex::CreateSimplexForGudhi(set<long> *idList, float &filtration){
    if(idList->size()>0){
        // Sort in ascending order
        //idList->sort();
        
        vector<long> v;
        v.insert(v.begin(), idList->begin(), idList->end());
        
        string s="{";
        for(set<long>::iterator itr=idList->begin(); itr!=idList->end(); itr++){
            if(s.length()>1) s+=",";
            s += to_string(*itr);
        }
        s +="}";
        
        if(sNodes.find(s)==sNodes.end()){
            sNodes.insert(s);
            
            map<float, SimplexList*>::iterator itr;
            if((itr=simplexMap.find(filtration))==simplexMap.end()){
                SimplexList *sl = new SimplexList(v);
                
                simplexMap.insert(pair<float, SimplexList*>(filtration, sl));
            }else{
                itr->second->sv.push_back(v);
            }
        }
    }
}

void SimplicialComplex::CreateSimplex(set<long> *idList, float &filtration){
    long k = idList->size();
    
    if(k>0){
        // Sort in ascending order
        //idList->sort();
        
        bitset<BIT_SET_SIZE> idBitSet;
        long max = pow(2, k)-1;
        unordered_map<string, float>::iterator sItr;
        
        for(long i=1; i<=max; i++){
            idBitSet = i;
            
            
            // If has at least one 1
            if(idBitSet.count()>0){
                
                // Made key with all 1 position ids
                string key = "";
                for(long j=0;j<k; j++){
                    
                    // Go to the bit position, similar to the id position in the list
                    if(idBitSet[j]==1){
                        
                        long tmp=0;
                        for(set<long>::iterator itr=idList->begin(); itr!=idList->end(); itr++){
                            
                            // Match the position of list to the bit set
                            if(j == tmp){
                                if(key.length()>0) key += ",";
                                key += to_string(*itr);
                                
                                break;
                            }
                            
                            tmp++;
                        }
                    }
                }
                
                if((sItr=simplexFiltrationMap.find(key)) == simplexFiltrationMap.end()){
                    simplexFiltrationMap.insert(pair<string, float>(key, filtration));
                    AddToSimplexMap(key, filtration);
                }else{
                    if(sItr->second == 0.00 && filtration > 0){
                        sItr->second = filtration;
                        AddToSimplexMap(key, filtration);
                    }
                }
            }
        }
    }
}

void SimplicialComplex::AddToSimplexMap(string key, float &filtration){
    string simplexText = "";
    map<float, SimplexList*>::iterator sitr;
    
    if(key.find(",")==string::npos){
        if(filtration==0)
            simplexText = "stream.addVertex(" + key + ", " + to_string(filtration).substr(0,5) + ");";
    }else{
        simplexText = "stream.addElement(new int[]{" + key + "},  " + to_string(filtration).substr(0,5) + ");";
    }
    
    if(simplexText.length()>0 && keyTracker.find(simplexText) == keyTracker.end()){
        keyTracker.insert(simplexText);
        
        if((sitr=this->simplexMap.find(filtration)) == this->simplexMap.end()){
            SimplexList *sl = new SimplexList(simplexText);
            
            this->simplexMap.insert(pair<float, SimplexList*>(filtration, sl));
        }else{
            sitr->second->slist.push_back(simplexText);
        }
    }
}
