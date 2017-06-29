
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: QuickSort.cpp
 Objective: Implementation of member functions
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              03/11/2016      Initial version
 
 **************************************************************************************************/

#include "QuickSort.h"
#include <stdlib.h>

QuickSort::QuickSort(){
    this->phenotypeList = NULL;
    this->size = 0;
}

QuickSort::~QuickSort(){
    if(this->phenotypeList != NULL){
        free(this->phenotypeList);
        this->size = 0;
        this->phenotypeList = NULL;
    }
}

void QuickSort::Sort(list<Phenotype*> *phList){
    this->size = phList->size();
    
    if(this->size > 1){
        this->phenotypeList = (Phenotype**)malloc(sizeof(Phenotype*)*this->size);
        
        list<Phenotype*>::iterator itr;
        size_t count = 0;
        
        for (itr = phList->begin(); itr != phList->end(); itr++)
        {
            this->phenotypeList[count] = *itr;
            count++;
        }
        
        phList->clear();
        
        this->MakePartitionAndSorting(0, this->size-1);
        
        for(count = 0; count<this->size; count++){
            phList->push_back(this->phenotypeList[count]);
        }
        
        if(this->phenotypeList != NULL){
            free(this->phenotypeList);
            this->size = 0;
            this->phenotypeList = NULL;
        }
        
    }
    
}

//#pragma Private_Method
void QuickSort::Swap(long index1, long index2){
    Phenotype* tmp = this->phenotypeList[index1];
    
    this->phenotypeList[index1] = this->phenotypeList[index2];
    this->phenotypeList[index2] = tmp;
}

int QuickSort::MakePartitionAndSorting(long start, long end){
    long mid = (start+end)/2;
    
    long i = start;
    long j = end;
    float pivot = 0;
    
    if(!SORT_BY_PHENOTYPE){
        pivot = this->phenotypeList[mid]->getDAP();
    }else{
        pivot = this->phenotypeList[mid]->GetValue();
    }
    
    float value1, value2;
    
    while(start<j || i<end)
    {
        
        if(!SORT_BY_PHENOTYPE){
            value1 = this->phenotypeList[i]->getDAP();
            value2 = this->phenotypeList[j]->getDAP();
        }else{
            value1 = this->phenotypeList[i]->GetValue();
            value2 = this->phenotypeList[j]->GetValue();
        }
        
        while(i<end && value1 < pivot){
            i++;
            
            if(i<end){
                if(!SORT_BY_PHENOTYPE){
                    value1 = this->phenotypeList[i]->getDAP();
                }else{
                    value1 = this->phenotypeList[i]->GetValue();
                }
            }
        }
        while(start<j && value2 > pivot){
            j--;
            
            if(j>start){
                if(!SORT_BY_PHENOTYPE){
                    value2 = this->phenotypeList[j]->getDAP();
                }else{
                    value2 = this->phenotypeList[j]->GetValue();
                }
            }
        }
        
        if(j<start) j=start;
        if(i>end) i=end;
        
        if(i<=j){
            if(!SORT_BY_PHENOTYPE){
                value1 = this->phenotypeList[i]->getDAP();
                value2 = this->phenotypeList[j]->getDAP();
            }else{
                value1 = this->phenotypeList[i]->GetValue();
                value2 = this->phenotypeList[j]->GetValue();
            }
            
            if(value1!=value2)
                this->Swap(i,j);
            
            i++;
            j--;
            
            if(j<start) j=start;
            if(i>end) i=end;
        }
        else{
            if(start<j)
                this->MakePartitionAndSorting(start, j);
                //quicksort(arr, left, j);
            if(i<end)
                this->MakePartitionAndSorting(i, end);
                //quicksort(arr,i,right);
            return 0;
        }
    }
    
    return 0;
}


