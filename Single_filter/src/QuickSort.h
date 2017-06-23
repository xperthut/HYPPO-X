
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: QuickSort.h
 Objective: Class to implement quick sort for custom data
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              03/11/2016      Initial version
 
 **************************************************************************************************/

#ifndef QuickSort_h
#define QuickSort_h

#include "Phenotype.h"

class QuickSort{
private:
    Phenotype** phenotypeList;
    long size;
    
    int MakePartitionAndSorting(long start, long end);
    void Swap(long index1, long index2);
    
public:
    QuickSort();
    ~QuickSort();
    void Sort(list<Phenotype*> *phList);
};

#endif /* QuickSort_h */
