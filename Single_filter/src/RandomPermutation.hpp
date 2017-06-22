//
//  RandomPermutation.hpp
//  FastCompressedQuadTree
//
//  Created by Md Kamruzzaman on 5/9/16.
//  Copyright © 2016 WSU. All rights reserved.
//

#ifndef RandomPermutation_hpp
#define RandomPermutation_hpp

#include <iostream>
#include <list>

using namespace std;

class RandomPermutation{
private:
    long minValue;
    long maxValue;
    
public:
    RandomPermutation(long minValue, long maxValue);
    ~RandomPermutation();
    void GetValue(long totalFold, list<list<long>> *foldList);
};

#endif /* RandomPermutation_hpp */
