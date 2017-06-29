
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: CustomCollection.h
 Objective: Customoze collection class like set, list, map, etc
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              02/16/2017      Initial version
 
 **************************************************************************************************/

#ifndef CustomCollection_h
#define CustomCollection_h

#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <bitset>

using namespace std;

template<class T>
class CustomList{
private:
    list<T> cList;
    
public:
    CustomList();
    ~CustomList();
    
    void addToList(T a);
    list<T>& getList();
    bool hasItem(T a);
    size_t getSize();
};

template<class T>
class CustomSet{
private:
    set<T> cSet;
    
public:
    CustomSet();
    ~CustomSet();
    
    void addToSet(T a);
    set<T>& getSet();
    bool hasItem(T a);
    size_t getSize();
};

template<class K, class T>
class CustomMap{
private:
    map<K,T> cMap;
    
public:
    CustomMap();
    ~CustomMap();
    
    void addToMap(K k, T a);
    map<K,T>& getMap();
    bool hasItem(K k);
    T& getValue(K k);
    size_t getSize();
};

template<class T>
class CustomHashSet{
private:
    unordered_set<T> cSet;
    
public:
    CustomHashSet();
    ~CustomHashSet();
    
    void addToSet(T a);
    unordered_set<T>& getSet();
    bool hasItem(T a);
    void removeItem(T a);
    T getElement();
    size_t getSize();
};

template<class K, class T>
class CustomHashMap{
private:
    unordered_map<K,T> cMap;
    
public:
    CustomHashMap();
    ~CustomHashMap();
    
    void addToMap(K k, T a);
    void removeFromMap(K k);
    unordered_map<K,T>& getMap();
    bool hasItem(K k);
    T& getValue(K k);
    void clear();
    size_t getSize();
};

#endif /* CustomCollection_h */
