/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: interface.h
 Objective: Implementation of interface methods, public access to library functions
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              01/03/2018      Initial version
 
 **************************************************************************************************/

#ifndef interface_h
#define interface_h

#include <string>
#include <vector>

namespace hyppox {
    class HInterface{
        public:
        virtual std::string getD3GraphObject(int argc, const char * argv[])=0;
        virtual std::string getD3GraphObject(short genotypeCol, std::vector<std::string> filterGen, std::vector<short> locationCol, short dateTimeCol, std::vector<short> otherInfoCol, std::vector<short> indvCol, std::vector<short> filterCol, std::vector<short> clusterCol, short clusterIndexForEdgeDirection, std::string clusterName, std::vector<float> clusterParams, std::vector<short> windows, std::vector<float> overlap, std::string readDir, std::string writeDir, short minPathLen, float deltaChanges, bool increaseTime, bool signatureMatch, bool printBarcode, bool printBarcodeUsingJavaplex, bool refPerformance, short filterRefIndex, short intPathWidth, std::string edgeColor, std::vector<float> nodeSizeRange, bool heatmapEachConComp, std::string fileName, short sortFilterIndex, std::vector<short> filterSignatureMatching)=0;
        virtual ~HInterface(){};
    };
}


#endif /* interface_h */
