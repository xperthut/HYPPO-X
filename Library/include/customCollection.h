
/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: customCollection.h
 Objective: Customoze collection class like set, list, map, etc
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              02/16/2017      Initial version
 
 **************************************************************************************************/

#ifndef customCollection_h
#define customCollection_h

#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <bitset>

namespace hyppox{
    template<typename LstType>
    class CustomList{
        private:
        std::list<LstType> cList;
        
        public:
        CustomList() = default;
        ~CustomList() = default;
        
        void addToList(LstType a){
            cList.push_back(a);
        }
        
        void addToList(LstType a, bool placeBack){
            if(placeBack) cList.push_back(a);
            else cList.push_front(a);
        }
        
        std::list<LstType>& getList(){return cList;}
        
        bool hasItem(LstType a){
            return (cList.find(a) != cList.end);
        }
        
        size_t getSize(){return cList.size();}
        
        void clearList(){cList.clear();}
    };
    
    template<typename SetType>
    class CustomSet{
        private:
        std::set<SetType> cSet;
        
        public:
        CustomSet() = default;
        ~CustomSet() = default;
        
        void addToSet(SetType a){cSet.insert(a);}
        
        std::set<SetType>& getSet(){return cSet;}
        
        bool hasItem(SetType a){
            return (cSet.find(a) != cSet.end);
        }
        
        size_t getSize(){return cSet.size();}
        
        void clearSet(){cSet.clear();}
    };
    
    template<class KeyType, class ValType>
    class CustomMap{
        private:
        std::map<KeyType,ValType> cMap;
        
        public:
        CustomMap() = default;
        ~CustomMap() = default;
        
        void addToMap(KeyType& k, ValType& a){
            if(cMap.find(k) == cMap.end()){
                cMap.insert(std::make_pair(k,a));
            }
        }
        
        std::map<KeyType,ValType>& getMap(){
            return cMap;
        }
        
        bool hasItem(KeyType k){
            return (cMap.find(k) != cMap.end());
        }
        
        ValType& getValue(KeyType k){
            return (cMap.find(k))->second;
        }
        
        size_t getSize(){return cMap.size();}
        
        void clearMap(){cMap.clear();}
    };
    
    template<typename SetType>
    class CustomHashSet{
        private:
        std::unordered_set<SetType> cSet;
        
        public:
        CustomHashSet() = default;
        ~CustomHashSet() = default;
        
        void addToSet(SetType a){
            cSet.insert(a);
        }
        
        std::unordered_set<SetType>& getSet(){
            return cSet;
        }
        
        bool hasItem(SetType a){
            return (cSet.find(a) != cSet.end());
        }
        
        void removeItem(SetType a){cSet.erase(a);}
        
        SetType getElement(){
            auto itr = cSet.begin();
            return *itr;
        }
        
        size_t getSize(){return cSet.size();}
        
        void clearHashSet(){cSet.clear();}
        
        void copyHashSet(CustomHashSet<SetType>& _set){
            auto _cSet = _set.getSet();
            for(SetType s:_cSet){
                cSet.insert(s);
            }
        }
    };
    
    template<class KeyType, class ValType>
    class CustomHashMap{
        private:
        std::unordered_map<KeyType,ValType> cMap;
        
        public:
        CustomHashMap() = default;
        ~CustomHashMap() = default;
        
        // New insertion only
        void addToMap(KeyType k, ValType a){
            if(cMap.find(k) == cMap.end()){
                cMap.insert(std::make_pair(k,a));
            }
        }
        
        // Override the old value
        void insertToMap(KeyType k, ValType a){
            auto itr = cMap.find(k);
            
            if(itr == cMap.end()){
                cMap.insert(std::make_pair(k,a));
            }else{
                itr->second = a;
            }
        }
        
        void removeFromMap(KeyType k){
            if(cMap.find(k) != cMap.end()){
                cMap.erase(k);
            }
        }
        
        std::unordered_map<KeyType,ValType>& getMap(){
            return cMap;
        }
        
        bool hasItem(KeyType k){
            return (cMap.find(k) != cMap.end());
        }
        
        ValType& getValue(KeyType k){
            return (cMap.find(k))->second;
        }
        
        void clear(){cMap.clear();}
        
        size_t getSize(){return cMap.size();}
        
        void copyHashMap(CustomHashMap<KeyType,ValType>& _map){
            auto tmap = _map.getMap();
            
            for(auto itr=tmap.begin(); itr!=tmap.end(); itr++){
                this->insertToMap(itr->first, itr->second);
            }
        }
    };
}

#endif /* CustomCollection_h */
