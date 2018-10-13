
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

#include "external/Gudhi/graph_simplicial_complex.h"
#include "external/Gudhi/Simplex_tree.h"
#include "external/Gudhi/Persistent_cohomology.h"

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
#include "performance.h"

using Simplex_tree = Gudhi::Simplex_tree<>;
using Filtration_value = Simplex_tree::Filtration_value;
using Field_Zp = Gudhi::persistent_cohomology::Field_Zp;
using Persistent_cohomology = Gudhi::persistent_cohomology::Persistent_cohomology<Simplex_tree, Field_Zp >;
using typeVectorVertex = std::vector<Simplex_tree::Vertex_handle>;

namespace hyppox{
    namespace mapper{
        template<typename SLType>
        class SimplexList{
            public:
            std::list<std::string> slist;
            std::list<std::vector<SLType> > sv;
            
            SimplexList(std::string s){slist.push_back(s);}
            SimplexList(std::vector<SLType> v){sv.push_back(v);}
            ~SimplexList() = default;
            
            std::string GetAllData(){
                std::string str = "";
                
                for(auto sitr = slist.begin(); sitr != slist.end(); sitr++){
                    if(str.length() > 0) str += "\n";
                    
                    str += *sitr;
                }
                
                return str;
            }
        };
        
        template<typename PerfType, typename ClusterIDType>
        class SimplicialComplex{
            public:
            //using SCType = SimplicialComplex<PerfType,ClusterIDType>;
            SimplicialComplex()=default;
            ~SimplicialComplex()=default;
            
            void AddSimplicialComplex(PerfType* ph, float &filtration);
            void clearTracker();
            std::string PrintSimplex();
            float getPersistentOverlap();
            
            private:
            std::unordered_map<ClusterIDType, float> zeroSimplex;
            std::unordered_map<std::string, float> oneOrMoreSimplex;
            std::map<float, SimplexList<long>*> simplexMap;
            std::unordered_set<std::string> sNodes;
            
            std::unordered_set<size_t> indvTracker;
            std::unordered_set<std::string> keyTracker;
            std::unordered_map<std::string, float> simplexFiltrationMap;
            
            void AddToOneOrMoreSimplex(std::string nodeList, float filtrationValue);
            void AddToSimplexMap(std::string key, float &filteration);
            void CreateSimplex(std::set<ClusterIDType>* idList, float &filtration);
            void CreateSimplexForGudhi(std::set<ClusterIDType>* idList, float &filtration);
        };
        
        template<typename PerfType, typename ClusterIDType>
        void SimplicialComplex<PerfType,ClusterIDType>::AddSimplicialComplex(PerfType *ph, float &filtration){
            if(ph!=nullptr){
                size_t indv = ph->getID();
                
                // New individual
                if(indvTracker.find(indv)==indvTracker.end()){
                    std::vector<ClusterIDType> idList(Config::TOTAL_CLUSTER_IDS, 0);
                    ph->GetIDList(idList);
                    std::set<ClusterIDType> ids;
                    //list<long> lid;
                    
                    for(short i=0; i<Config::TOTAL_CLUSTER_IDS; i++){
                        if(idList[i]>0){
                            ids.insert(idList[i]);
                        }
                    }
                    
                    if(Config::PH_JAVA_PLEX){
                        this->CreateSimplex(&ids, filtration);
                    }else{
                        this->CreateSimplexForGudhi(&ids, filtration);
                    }
                }
            }
        }
        
        template<typename PerfType, typename ClusterIDType>
        void SimplicialComplex<PerfType,ClusterIDType>::clearTracker(){indvTracker.clear();}
        
        template<typename PerfType, typename ClusterIDType>
        std::string SimplicialComplex<PerfType,ClusterIDType>::PrintSimplex(){
            std::string simplex = "";
            
            for(auto sitr = this->simplexMap.begin(); sitr != this->simplexMap.end(); sitr++){
                if(simplex.length()>0) simplex += "\n";
                
                simplex += sitr->second->GetAllData();
            }
            
            simplex += "\n";
            
            
            return simplex;
        }
        
        template<typename PerfType, typename ClusterIDType>
        float SimplicialComplex<PerfType,ClusterIDType>::getPersistentOverlap(){
            Simplex_tree st;
            
            for(auto itr=this->simplexMap.begin(); itr!=this->simplexMap.end(); itr++){
                for(auto lItr=itr->second->sv.begin(); lItr!=itr->second->sv.end(); lItr++){
                    typeVectorVertex SimplexVector = *lItr;
                    st.insert_simplex_and_subfaces(SimplexVector, itr->first);
                }
            }
            
            st.set_dimension(Config::FILTER+1);
            
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
            
            return pcoh.getPersistentFiltrationValue(Config::FILTER+1);
        }
        
        /////////////// Private Methods ////////////////////
        
        template<typename PerfType, typename ClusterIDType>
        void SimplicialComplex<PerfType,ClusterIDType>::CreateSimplexForGudhi(std::set<ClusterIDType> *idList, float &filtration){
            if(idList->size()>0){
                // Sort in ascending order
                //idList->sort();
                
                std::vector<long> v;
                v.insert(v.begin(), idList->begin(), idList->end());
                
                std::string s="{";
                for(auto itr=idList->begin(); itr!=idList->end(); itr++){
                    if(s.length()>1) s+=",";
                    s += std::to_string(*itr);
                }
                s +="}";
                
                if(sNodes.find(s)==sNodes.end()){
                    sNodes.insert(s);
                    
                    auto itr=simplexMap.find(filtration);
                    
                    if(itr==simplexMap.end()){
                        SimplexList<long> *sl = new SimplexList<long>(v);
                        
                        simplexMap.insert(std::make_pair(filtration, sl));
                    }else{
                        itr->second->sv.push_back(v);
                    }
                }
            }
        }
        
        template<typename PerfType, typename ClusterIDType>
        void SimplicialComplex<PerfType,ClusterIDType>::CreateSimplex(std::set<ClusterIDType> *idList, float &filtration){
            ClusterIDType k = idList->size();
            
            if(k>0){
                // Sort in ascending order
                //idList->sort();
                
                std::bitset<BIT_SET_SIZE> idBitSet;
                ClusterIDType max = pow(2, k)-1;
                
                for(ClusterIDType i=1; i<=max; i++){
                    idBitSet = i;
                    
                    
                    // If has at least one 1
                    if(idBitSet.count()>0){
                        
                        // Made key with all 1 position ids
                        std::string key = "";
                        for(ClusterIDType j=0;j<k; j++){
                            
                            // Go to the bit position, similar to the id position in the list
                            if(idBitSet[j]==1){
                                
                                ClusterIDType tmp=0;
                                for(auto itr=idList->begin(); itr!=idList->end(); itr++){
                                    
                                    // Match the position of list to the bit set
                                    if(j == tmp){
                                        if(key.length()>0) key += ",";
                                        key += std::to_string(*itr);
                                        
                                        break;
                                    }
                                    
                                    tmp++;
                                }
                            }
                        }
                        
                        auto sItr=simplexFiltrationMap.find(key);
                        if(sItr == simplexFiltrationMap.end()){
                            simplexFiltrationMap.insert(std::make_pair(key, filtration));
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
        
        template<typename PerfType, typename ClusterIDType>
        void SimplicialComplex<PerfType,ClusterIDType>::AddToSimplexMap(std::string key, float &filtration){
            std::string simplexText = "";
            
            if(key.find(",")==std::string::npos){
                if(filtration==0)
                simplexText = "stream.addVertex(" + key + ", " + std::to_string(filtration).substr(0,5) + ");";
            }else{
                simplexText = "stream.addElement(new int[]{" + key + "},  " + std::to_string(filtration).substr(0,5) + ");";
            }
            
            if(simplexText.length()>0 && keyTracker.find(simplexText) == keyTracker.end()){
                keyTracker.insert(simplexText);
                
                auto sitr=this->simplexMap.find(filtration);
                if(sitr == this->simplexMap.end()){
                    SimplexList<long> *sl = new SimplexList<long>(simplexText);
                    
                    this->simplexMap.insert(std::make_pair(filtration, sl));
                }else{
                    sitr->second->slist.push_back(simplexText);
                }
            }
        }
    }
}


#endif /* SimplicialComplex_h */
