/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: main.cpp
 Objective: Main entry point of the code
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              01/03/2018      Initial version
 
 **************************************************************************************************/

#include <iostream>
#include <string>
#include "include/hyppox.h"

int main(int argc, const char * argv[]) {
    // Test
    //./HyppoX_Lib -GC 3 -LC {4} -DTC 6 -EC {5} -IC [3:4] -FC {9} -CC [7] -CP {0.6,2} -WX {13} -GX {46} -RD "/Applications/XAMPP/xamppfiles/htdocs/Hyppo-X/dev/Data/csv" -WD "/Applications/XAMPP/xamppfiles/htdocs/Hyppo-X/dev/Data/json" -HM 0 -FN "PlantHeight.csv"
    
    // Place datatype of <RowID, ClusterID, Genotype, Datetime, Cluster value, Filter value>
    using HType = hyppox::Hyppox<size_t, size_t, std::string, std::string, float, float>;
    
    clock_t t1, t2;
    
    t1 = clock();
    
    hyppox::HInterface* _hyppox = new HType();
    if(_hyppox==nullptr){
        std::cout<<"There has no enough memory"<<std::endl;
    }
    std::string _p = _hyppox->getD3GraphObject(argc, argv);
    delete _hyppox;
    
    t2 = clock();
    
    if(_p.length()>0) std::cout<<"More info: "<<_p<<std::endl<<std::endl;
    
    std::vector<std::string> _time = getTime((float)(t2-t1)/CLOCKS_PER_SEC);
    
    std::cout<<"Total time:"<<_time[0]<<" ("<<_time[1]<<")"<<std::endl;
    
    return 0;
}

