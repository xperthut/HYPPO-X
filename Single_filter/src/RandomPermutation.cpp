//
//  RandomPermutation.cpp
//  FastCompressedQuadTree
//
//  Created by Md Kamruzzaman on 5/9/16.
//  Copyright © 2016 WSU. All rights reserved.
//

#include "RandomPermutation.hpp"

RandomPermutation::RandomPermutation(long minValue, long maxValue){
    this->minValue = minValue;
    this->maxValue = maxValue;
}

RandomPermutation::~RandomPermutation(){}

void RandomPermutation::GetValue(long totalFold, list<list<long>> *foldList){
    long total = maxValue-minValue+1;
    long ilist[total];
    long binSize = (long)(total/totalFold);
    
    for (long i=1; i<=total; ++i) {
        ilist[i] = minValue-1+i;
    }
    
    unsigned short seed = time(0);
    seed48(&seed);
    
    for (long i=total; i>=2; --i) {
        long j = i * drand48() + 1;
        j = j - j%1;
        long dummy = ilist[i];
        ilist[i] = ilist[j];
        ilist[j] = dummy;
    }
    
    long tmpFC = 0;
    long recordCount = 1;
    list<long> tmpList;
    
    for (long i=1; i<=total; ++i) {
        
        if(tmpFC < totalFold){
            tmpList.push_back(ilist[i]);
            
            if(recordCount>binSize){
                recordCount=0;
                tmpFC ++;
                
                tmpList.sort();
                foldList->push_back(tmpList);
                
                tmpList.clear();
            }
            
            recordCount++;
        
        }else{
            tmpList.push_back(ilist[i]);
        
        }
    }
    
    tmpList.sort();
    foldList->push_back(tmpList);
    
    list<list<long>>::iterator fitr;
    list<long>::iterator litr;
    
    for(fitr=foldList->begin(); fitr!=foldList->end();fitr++){
        list<long> l = *fitr;
        
        for(litr = l.begin(); litr!=l.end(); litr++){
            cout<<*litr<<",";
        }
        
        cout<<"\n";
    }
    
    cout<<endl;
}