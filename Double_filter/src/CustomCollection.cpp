/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: CustomCollection.cpp
 Objective: Implementation of member functions
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              02/16/2017      Initial version
 
 **************************************************************************************************/

#include "CustomCollection.h"

#pragma Custom_List_Class
template<class T>
CustomList<T>::CustomList(){}

template<class T>
CustomList<T>::~CustomList(){}

template<class T>
void CustomList<T>::addToList(T a){
    cList.push_back(a);
}

template<class T>
list<T>& CustomList<T>::getList(){
    return cList;
}

template<class T>
bool CustomList<T>::hasItem(T a){
    if(cList.find(a) != cList.end){
        return true;
    }
    
    return false;
}

template<class T>
size_t CustomList<T>::getSize(){
    return cList.size();
}

#pragma Custom_Set_Class
template<class T>
CustomSet<T>::CustomSet(){}

template<class T>
CustomSet<T>::~CustomSet(){}

template<class T>
void CustomSet<T>::addToSet(T a){
    cSet.insert(a);
}

template<class T>
set<T>& CustomSet<T>::getSet(){
    return cSet;
}

template<class T>
bool CustomSet<T>::hasItem(T a){
    if(cSet.find(a) != cSet.end){
        return true;
    }
    
    return false;
}

template<class T>
size_t CustomSet<T>::getSize(){
    return cSet.size();
}

#pragma Custom_Map_Class
template<class K, class T>
CustomMap<K,T>::CustomMap(){}

template<class K, class T>
CustomMap<K,T>::~CustomMap(){}

template<class K, class T>
void CustomMap<K,T>::addToMap(K k,T a){
    if(cMap.find(k) == cMap.end()){
        cMap.insert(pair<K,T>(k,a));
    }
    
}

template<class K, class T>
map<K,T>& CustomMap<K,T>::getMap(){
    return cMap;
}

template<class K, class T>
T& CustomMap<K,T>::getValue(K k){
    
    return (cMap.find(k))->second;
    
}

template<class K, class T>
bool CustomMap<K,T>::hasItem(K k){
    if(cMap.find(k) != cMap.end()){
        return true;
    }
    
    return false;
}

template<class K, class T>
size_t CustomMap<K,T>::getSize(){
    return cMap.size();
}

#pragma Custom_HashSet_Class
template<class T>
CustomHashSet<T>::CustomHashSet(){}

template<class T>
CustomHashSet<T>::~CustomHashSet(){}

template<class T>
void CustomHashSet<T>::addToSet(T a){
    cSet.insert(a);
}

template<class T>
unordered_set<T>& CustomHashSet<T>::getSet(){
    return cSet;
}

template<class T>
bool CustomHashSet<T>::hasItem(T a){
    if(cSet.find(a) != cSet.end){
        return true;
    }
    
    return false;
}

template<class T>
void CustomHashSet<T>::removeItem(T a){
    cSet.erase(a);
}

template<class T>
T CustomHashSet<T>::getElement(){
    auto itr = cSet.begin();
    return *itr;
}

template<class T>
size_t CustomHashSet<T>::getSize(){
    return cSet.size();
}

#pragma Custom_HashMap_Class
template<class K, class T>
CustomHashMap<K,T>::CustomHashMap(){}

template<class K, class T>
CustomHashMap<K,T>::~CustomHashMap(){}

template<class K, class T>
void CustomHashMap<K,T>::addToMap(K k,T a){
    if(cMap.find(k) == cMap.end()){
        cMap.insert(pair<K,T>(k,a));
    }
    
}

template<class K, class T>
void CustomHashMap<K,T>::removeFromMap(K k){
    if(cMap.find(k) != cMap.end()){
        cMap.erase(k);
    }
}

template<class K, class T>
unordered_map<K,T>& CustomHashMap<K,T>::getMap(){
    return cMap;
}

template<class K, class T>
T& CustomHashMap<K,T>::getValue(K k){
    
    return (cMap.find(k))->second;

}

template<class K, class T>
bool CustomHashMap<K,T>::hasItem(K k){
    if(cMap.find(k) != cMap.end()){
        return true;
    }
    
    return false;
}

template<class K, class T>
void CustomHashMap<K,T>::clear(){
    cMap.clear();
}

template<class K, class T>
size_t CustomHashMap<K,T>::getSize(){
    return cMap.size();
}
