/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: hyppox.h
 Objective: Entry class
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              04/23/2018      Initial version
 
 **************************************************************************************************/


#ifndef hyppox_h
#define hyppox_h

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <stdlib.h>
#include "mapper.h"
#include "interface.h"

std::string trim(const std::string& str,
                 std::string whitespace = " \t"){
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos) return ""; // no content
    
    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;
    
    return str.substr(strBegin, strRange);
}

const int compare(std::string s1, std::string s2){
    return s1.compare(s2);
}

using RowIDType = size_t;
using ClusterIDType = size_t;
using Gentype = std::string;
using DatetimeType = std::string;
using ClusterType = float;
using FilterType = float;

namespace hyppox {
    class Hyppox:public HInterface{
        public:
        using PerfType = mapper::Performance<RowIDType,ClusterIDType,Gentype,DatetimeType,ClusterType,FilterType>;
        using FHType = mapper::FileHandler<PerfType,float,float,RowIDType,ClusterIDType,Gentype,DatetimeType,ClusterType,FilterType>;
        using GType = graph::Graph<PerfType,FHType>;
        using MType = mapper::Mapper<PerfType,mapper::DataPoint<PerfType*,float>,quadtree::QuadTree<PerfType*,float,float>,GType,FHType,ClusterIDType,RowIDType,ClusterType>;
        
        Hyppox()=default;
        ~Hyppox()=default;
        
        std::string getD3GraphObject(int argc, const char * argv[]);
        std::string getD3GraphObject(short genotypeCol, std::vector<std::string> filterGen, std::vector<short> locationCol, std::vector<short> dateTimeCol, std::vector<short> otherInfoCol, std::vector<short> indvCol, std::vector<short> filterCol, std::vector<short> clusterCol, short clusterIndexForEdgeDirection, std::string clusterName, std::vector<float> clusterParams, std::vector<short> windows, std::vector<float> overlap, std::string readDir, std::string writeDir, short minPathLen, float deltaChanges, bool increaseTime, bool signatureMatch, bool printBarcode, bool printBarcodeUsingJavaplex, bool refPerformance, short filterRefIndex, short intPathWidth, std::string edgeColor, std::vector<float> nodeSizeRange, bool heatmapEachConComp, std::string fileName, short sortFilterIndex, std::vector<short> filterSignatureMatching);
        
        private:
        GType graph;
        
        void printHelpMessage();
        void parseIndex(std::string s, std::vector<short> &colIndices);
        void parseFloat(std::string s, std::vector<float> &val);
        int parseArgs(int argc, const char * argv[],
                      short &genotypeCol,
                      std::vector<std::string> &filterGen,
                      std::vector<short> &locationCol,
                      std::vector<short> &dateTimeCol,
                      std::vector<short> &otherInfoCol,
                      std::vector<short> &indvCol,
                      std::vector<short> &filterCol,
                      std::vector<short> &clusterCol,
                      short &clusterIndexForEdgeDirection,
                      std::string &clusterName,
                      std::vector<float> &clusterParams,
                      std::vector<short> &windows,
                      std::vector<float> &overlap,
                      std::string &readDir,
                      std::string &writeDir,
                      short &minPathLen,
                      float &deltaChanges,
                      bool &increaseTime,
                      bool &signatureMatch,
                      bool &printBarcode,
                      bool &printBarcodeUsingJavaplex,
                      bool &refPerformance,
                      short &filterRefIndex,
                      short &intPathWidth,
                      std::string &edgeColor,
                      std::vector<float> &nodeSizeRange,
                      bool &heatmapEachConComp,
                      std::string &fileName,
                      short& sortFilterIndex,
                      std::vector<short>& filterSignatureMatching);
        std::string createMapper();
        std::string visualization();
    };
    
    // This handler parse and handle terminal requests
    std::string Hyppox::getD3GraphObject(int argc, const char * argv[]){
        short genotypeCol ;
        std::vector<std::string> filterGen;
        std::vector<short> locationCol;
        std::vector<short> dateTimeCol;
        std::vector<short> otherInfoCol;
        std::vector<short> indvCol;
        std::vector<short> filterCol;
        std::vector<short> clusterCol;
        short clusterIndexForEdgeDirection;
        std::string clusterName;
        std::vector<float> clusterParams;
        std::vector<short> windows;
        std::vector<float> overlap;
        std::string readDir;
        std::string writeDir;
        short minPathLen;
        float deltaChanges;
        bool increaseTime;
        bool signatureMatch;
        bool printBarcode;
        bool printBarcodeUsingJavaplex;
        bool refPerformance;
        short filterRefIndex;
        short intPathWidth;
        std::string edgeColor;
        std::vector<float> nodeSizeRange;
        bool heatmapEachConComp;
        std::string fileName;
        short sortFilterIndex;
        std::vector<short> filterSignatureMatching;
        
        // Datafile: PlantHeight.csv
        // Columns: pID,DAP,Genotype,Location,PlantID,Date,GrowthRate,Temperature,Solar.Rad,Humidity,Rainfall
        // Index:    1   2    3        4         5      6      7          8         9          10      11
        
        /*short genotypeCol = 2;
         vector<string> filterGen = {};//{"ONH1","TXH1","TXH2"};
         vector<short> locationCol = {4,5,6,7};
         short dateTimeCol = 12;
         vector<short> otherInfoCol = {};
         vector<short> indvCol = {2};
         vector<short> filterCol = {13};
         vector<short> clusterCol = {8,9,10,11};
         short clusterIndexForEdgeDirection = 3;
         string clusterName = "DBSCAN";
         vector<float> clusterParams = {120,2};
         vector<short> windows = {60,1};
         vector<float> overlap = {45};
         string readDir = "/Users/methun/self/Research/Publications/Code/TDA_Phenomics/Extension/read/";
         string writeDir = "/Users/methun/self/Research/Publications/Code/TDA_Phenomics/Extension/write";
         short minPathLen = 3;
         float deltaChanges = 0.00;
         bool increaseTime = false;
         bool signatureMatch = true;
         bool printBarcode = false;
         bool printBarcodeUsingJavaplex = false;
         bool refPerformance = true;
         short filterRefIndex = -1;
         short intPathWidth = 12;
         string edgeColor = "#000000";//"#a4a4a4";
         vector<float> nodeSizeRange = {30.0,15.0};
         bool heatmapEachConComp = true;
         string fileName = "Green2Field.csv";*/
        
        // For dap only
        // Overlap 35, density 2, rad: 100, window: 60
        
        // For temperature only
        // Overlap 45, density 2, rad: 120, window: 60
        
        // Datafile: Green2Field.csv/Green2Field_4Loc.csv
        // Columns: "pID","Field.Location","Genotype","Panel","Year","DAP","State","City","Block","Plot","Pollen.DAP","Plant.height","Ear.height","Yield",
        // Index:     1          2            3         4       5     6       7      8       9       10       11          12             13         14
        //          "CGDD","GDD","Temperature","Dew.point","Relative.humidity","Solar.radiation","Rainfall"
        // Index:    15     16        17          18               19                20               21
        
        short ret = this->parseArgs(argc, argv, genotypeCol, filterGen, locationCol, dateTimeCol, otherInfoCol, indvCol, filterCol, clusterCol, clusterIndexForEdgeDirection, clusterName, clusterParams, windows, overlap, readDir, writeDir, minPathLen, deltaChanges, increaseTime, signatureMatch, printBarcode, printBarcodeUsingJavaplex, refPerformance, filterRefIndex, intPathWidth, edgeColor, nodeSizeRange, heatmapEachConComp, fileName, sortFilterIndex, filterSignatureMatching);
        
        if(ret==-1){
            return "Format missing.";
        }else if(ret==2){
            return "";
        }
        
        return this->getD3GraphObject(genotypeCol, filterGen, locationCol, dateTimeCol, otherInfoCol, indvCol, filterCol, clusterCol, clusterIndexForEdgeDirection, clusterName, clusterParams, windows, overlap, readDir, writeDir, minPathLen, deltaChanges, increaseTime, signatureMatch, printBarcode, printBarcodeUsingJavaplex, refPerformance, filterRefIndex, intPathWidth, edgeColor, nodeSizeRange, heatmapEachConComp, fileName, sortFilterIndex, filterSignatureMatching);
    }
    
    // Request handler method, it handles public request
    std::string Hyppox::getD3GraphObject(short genotypeCol, std::vector<std::string> filterGen, std::vector<short> locationCol, std::vector<short> dateTimeCol, std::vector<short> otherInfoCol, std::vector<short> indvCol, std::vector<short> filterCol, std::vector<short> clusterCol, short clusterIndexForEdgeDirection, std::string clusterName, std::vector<float> clusterParams, std::vector<short> windows, std::vector<float> overlap, std::string readDir, std::string writeDir, short minPathLen, float deltaChanges, bool increaseTime, bool signatureMatch, bool printBarcode, bool printBarcodeUsingJavaplex, bool refPerformance, short filterRefIndex, short intPathWidth, std::string edgeColor, std::vector<float> nodeSizeRange, bool heatmapEachConComp, std::string fileName, short sortFilterIndex, std::vector<short> filterSignatureMatching){
        
        if(readDir[readDir.length()-1]!='/') readDir += "/";
        if(writeDir.length()>0){
            if(writeDir[writeDir.length()-1]!='/') writeDir += "/";
            
            size_t pos = fileName.find(".");
            if(pos!=std::string::npos){
                writeDir += fileName.substr(0, pos) + "/";
            }
        }
        
        hyppox::Config::setParams(genotypeCol, filterGen, locationCol, dateTimeCol, otherInfoCol, indvCol, filterCol, clusterCol, clusterIndexForEdgeDirection, clusterName, clusterParams, windows, overlap, readDir, writeDir, minPathLen, deltaChanges, increaseTime, signatureMatch, printBarcode, printBarcodeUsingJavaplex, refPerformance, filterRefIndex, intPathWidth, edgeColor, nodeSizeRange, heatmapEachConComp, fileName, sortFilterIndex,filterSignatureMatching);
        
        this->graph.initGraph();
        
        std::string s = this->createMapper();
        if(!hyppox::Config::PRINT_BARCODE && s.length()==0){
            s = this->visualization();
            
            std::cout<<"We are done\n\n";
        }
        
        return s;
    }
    
    // Private methods
    //
    /*
     -help: show all messages
     -GC: genotype column
     -FG: Filter genotype using any value
     -LC: Location columns
     -DTC: datetime column
     -EC: Extra information column
     -IC: Individual columns
     -FC: Filter columns
     -EFC: Which filter signature is used for edge direction matching. O means all filter signatues will be used for edge direction matching (default value is 0). For instance, if -EFC=[1,3] for filter=5 then the signature matching will be signature&00101=signature&5.
     -CC: Cluster columns
     -EDC: Edge direction determining cluster
     -CN: Cluster method name (Default DBSCAN)
     -CP: Cluster parameters (Default Eps, Density for DBSCAN)
     -WX: Number of buckets along each dimension/filter
     -GX: Gain or overlap along each dimension/filter
     -RD: Csv file's absolute path
     -WD: Absolute path to write the path, directory must have write permission and this directory contains a folder whose name will be the same as the file name without extension. This folder also needs write permission.
     -MPL: Minimum interesting path length (Default: 3)
     -DC: Delta changes for signature (Default: 0)
     -IT: Path directoction through the increase of time (Default: false)
     -SM: Match signature during propagating interesting path (Default: true)
     -PBC: Run code to compute the persistant overlap interval (default false)
     -JPX: Generate java code to print barcode (Default: false, false means compute persistant overlap using GUDHI library)
     -RP: Edge direction is based on performance (Default: true)
     -FRI: Reference filter index (Default: -s1, when -RP=true. If -RP=false then place filter index starting 0)
     -PW: Interesting path width (Default: 12)
     -ECOL: Edge color (Default: "#a4a4a4")
     -NSR: Node size range (default: [30.0, 15.0])
     -HM: Use heatmap color to each connected component separately or not. For true: Nodes of a connected component will be colored from blue to red based on low and high mean value of the nodes of that connected component. For false: Nodes of the whole graph will be colored from blue to red based on the min and max node value of the whole graph. (Default: false)
     -FN: CSV file name
     */
    
    void Hyppox::printHelpMessage(){
        std::cout<<"-help: show all messages"<<std::endl
        <<"You can pass single or multiple column indices enclosed by square bracket. For instance, [indices separated by comma]. For range of indices you can also use [a:b] which means start from index a and end at index b and take all the indices from a to b inclusively.\nUse 0 for false, 1 for true.\n\nRequired paramets:"<<std::endl
        <<"-GC: genotype column, single column index, column index starts from 1"<<std::endl
        <<"-FG: Filter genotype using any value. For instance, if genotypes are A,B,C,D and you place here [\"B\", \"D\"] then the dataset will be filter by these two genotypes only. Defailt value is empty."<<std::endl
        <<"-LC: Location columns, place single or multiple column indices in a square bracket. e.g. [2,5]"<<std::endl
        <<"-DTC: datetime column, place single or multiple column indices in a square bracket. e.g. [2,5]"<<std::endl
        <<"-EC: Extra information column, single column index. Does not support for multiple column indices"<<std::endl
        <<"-IC: Individual columns"<<std::endl
        <<"-FC: Filter columns"<<std::endl
        <<"-EFC: Which filter signature is used for edge direction matching. O means all filter signatues will be used for edge direction matching (default value is 0). For instance, if -EFC=[1,3] for filter=5 then the signature matching will be signature&00101=signature&5."<<std::endl
        <<"-SFI: Sort the nodes in the graph based on the mean value of the indexed filter"<<std::endl
        <<"-CC: Cluster columns"<<std::endl
        <<"-EDC: Edge direction determining cluster"<<std::endl
        <<"-CN: Cluster method name (Default DBSCAN)"<<std::endl
        <<"-CP: Cluster parameters (Default Eps, Density for DBSCAN)"<<std::endl
        <<"-WX: Number of buckets along each dimension/filter"<<std::endl
        <<"-GX: Gain or overlap along each dimension/filter"<<std::endl
        <<"-RD: Csv file's absolute path. If you do not want to pass any value then simply do not use this variable. Then it will seek the file in the directory where the executable file exists."<<std::endl
        <<"-WD: Absolute path to write the path, directory must have write permission and this directory contains a folder whose name will be the same as the file name without extension. This folder also needs write permission. (You can ommit this variable to use then the file will write in the directory where the executable file exists.)"<<std::endl
        <<"-MPL: Minimum interesting path length (Default: 3)"<<std::endl
        <<"-DC: Delta changes for signature (Default: 0)"<<std::endl
        <<"-IT: Path directoction through the increase of time (Default: false)"<<std::endl
        <<"-SM: Match signature during propagating interesting path (Default: true)"<<std::endl
        <<"-PBC: Run code to compute the persistant overlap interval (default false)"<<std::endl
        <<"-JPX: Generate java code to print barcode (Default: false, false means compute persistant overlap using GUDHI library)"<<std::endl
        <<"-RP: Edge direction is based on performance (Default: true)"<<std::endl
        <<"-FRI: Reference filter index (Default: -1, when -RP=true. If -RP=false then place filter index starting 0)"<<std::endl
        <<"-PW: Interesting path width (Default: 12)"<<std::endl
        <<"-ECOL: Edge color (Default: \"#a4a4a4\")"<<std::endl
        <<"-NSR: Node size range, [min,max] (default: [15.0, 90.0])"<<std::endl
        <<"-HM: Use heatmap color to each connected component separately or not. For true: Nodes of a connected component will be colored from blue to red based on low and high mean value of the nodes of that connected component. For false: Nodes of the whole graph will be colored from blue to red based on the min and max node value of the whole graph. (Default: false)"<<std::endl
        <<"-FN: CSV file name"<<std::endl;
    }
    
    void Hyppox::parseIndex(std::string s, std::vector<short> &colIndices){
        colIndices.clear();
        s.erase( remove( s.begin(), s.end(), ' ' ), s.end() ); // Remove all spaces
        
        if(s[0]=='['||s[0]=='{'){
            s = s.substr(1,s.length()-2);
        }
        size_t pos = s.find(",");
        while(pos!=std::string::npos){
            std::string s1 = s.substr(0, pos);
            s = s.substr(pos+1, s.length()-pos-1);
            
            size_t pos1 = s1.find(":");
            if(pos1==std::string::npos){
                colIndices.push_back(stoi(s1));
            }else{
                int a = stoi(s1.substr(0, pos1));
                s1 = s1.substr(pos1+1, s1.length()-pos1-1);
                int b = stoi(s1);
                
                for(int i=a;i<=b;i++){
                    colIndices.push_back(i);
                }
            }
            
            pos = s.find(",");
        }
        
        if(s.length()>0){
            size_t pos1 = s.find(":");
            if(pos1==std::string::npos){
                colIndices.push_back(stoi(s));
            }else{
                int a = stoi(s.substr(0, pos1));
                s = s.substr(pos1+1, s.length()-pos1-1);
                int b = stoi(s);
                
                for(int i=a;i<=b;i++){
                    colIndices.push_back(i);
                }
            }
        }
    }
    
    void Hyppox::parseFloat(std::string s, std::vector<float> &val){
        val.clear();
        s.erase( remove( s.begin(), s.end(), ' ' ), s.end() ); // Remove all spaces
        
        if(s[0]=='['||s[0]=='{'){
            s = s.substr(1,s.length()-2);
        }
        size_t pos = s.find(",");
        while(pos!=std::string::npos){
            std::string s1 = s.substr(0, pos);
            s = s.substr(pos+1, s.length()-pos-1);
            
            val.push_back(stof(s1));
            pos = s.find(",");
        }
        
        if(s.length()>0){
            val.push_back(stof(s));
        }
    }
    
    int Hyppox::parseArgs(int argc, const char * argv[],
                           short &genotypeCol,
                           std::vector<std::string> &filterGen,
                           std::vector<short> &locationCol,
                           std::vector<short> &dateTimeCol,
                           std::vector<short> &otherInfoCol,
                           std::vector<short> &indvCol,
                           std::vector<short> &filterCol,
                           std::vector<short> &clusterCol,
                           short &clusterIndexForEdgeDirection,
                           std::string &clusterName,
                           std::vector<float> &clusterParams,
                           std::vector<short> &windows,
                           std::vector<float> &overlap,
                           std::string &readDir,
                           std::string &writeDir,
                           short &minPathLen,
                           float &deltaChanges,
                           bool &increaseTime,
                           bool &signatureMatch,
                           bool &printBarcode,
                           bool &printBarcodeUsingJavaplex,
                           bool &refPerformance,
                           short &filterRefIndex,
                           short &intPathWidth,
                           std::string &edgeColor,
                           std::vector<float> &nodeSizeRange,
                           bool &heatmapEachConComp,
                           std::string &fileName,
                           short& sortFilterIndex,
                           std::vector<short>& filterSignatureMatching){
        
        if(argc<2){
            std::cout<<"\nWrong arguments.\n"<<std::endl;
            printHelpMessage();
            return -1;
        }
        
        // Clear all container
        filterGen.clear();
        locationCol.clear();
        otherInfoCol.clear();
        indvCol.clear();
        dateTimeCol.clear();
        filterCol.clear();
        clusterCol.clear();
        clusterParams.clear();
        windows.clear();
        overlap.clear();
        nodeSizeRange.clear();
        filterSignatureMatching.clear();
        
        // Place default values
        clusterName = "DBSCAN";
        minPathLen = 3;
        deltaChanges = 0.00;
        increaseTime = false;
        signatureMatch = true;
        printBarcode = false;
        printBarcodeUsingJavaplex = false;
        refPerformance = true;
        filterRefIndex = -1;
        intPathWidth = 12;
        edgeColor = "#a4a4a4";
        heatmapEachConComp = false;
        sortFilterIndex = 0;
        
        int index=1;
        
        try{
            while(index<argc){
                std::string s = argv[index];
                //cout<<s<<" "<<argv[index+1]<<" ";
                if(s.compare("-help")==0){
                    printHelpMessage();
                    return 2;
                    
                }else if(s.compare("-GC")==0){
                    index++;
                    s = argv[index];
                    genotypeCol = (short)stoi(s);
                    
                }else if(s.compare("-FG")==0){
                    index++;
                    s = argv[index];
                    //s.erase( remove( s.begin(), s.end(), ' ' ), s.end() ); // Remove all spaces
                    
                    if(s[0]=='['||s[0]=='{'){
                        s = s.substr(1,s.length()-2);
                    }
                    size_t pos = s.find(",");
                    while(pos!=std::string::npos){
                        std::string s1 = s.substr(0, pos);
                        s = s.substr(pos+1, s.length()-pos-1);
                        
                        filterGen.push_back(trim(s1," \t"));
                        pos = s.find(",");
                    }
                    
                    if(s.length()>0){
                        filterGen.push_back(trim(s," \t"));
                    }
                    
                }else if(s.compare("-EFC")==0){
                    index++;
                    s = argv[index];
                    //s.erase( remove( s.begin(), s.end(), ' ' ), s.end() ); // Remove all spaces
                    
                    if(s[0]=='['||s[0]=='{'){
                        s = s.substr(1,s.length()-2);
                    }
                    
                    size_t pos = s.find(",");
                    while(pos!=std::string::npos){
                        std::string s1 = s.substr(0, pos);
                        s = s.substr(pos+1, s.length()-pos-1);
                        
                        filterSignatureMatching.push_back(convert_to<short>(trim(s," \t")));
                        pos = s.find(",");
                    }
                    
                    if(s.length()>0){
                        filterSignatureMatching.push_back(convert_to<short>(trim(s," \t")));
                    }
                    
                }else if(s.compare("-LC")==0){
                    index++;
                    s = argv[index];
                    parseIndex(s, locationCol);
                    
                }else if(s.compare("-DTC")==0){
                    index++;
                    s = argv[index];
                    //dateTimeCol = (short)stoi(s);
                    parseIndex(s, dateTimeCol);
                    
                }else if(s.compare("-EC")==0){
                    index++;
                    s = argv[index];
                    parseIndex(s, otherInfoCol);
                    
                }else if(s.compare("-IC")==0){
                    index++;
                    s = argv[index];
                    parseIndex(s, indvCol);
                    
                }else if(s.compare("-FC")==0){
                    index++;
                    s = argv[index];
                    parseIndex(s, filterCol);
                    
                }else if(s.compare("-SFI")==0){
                    index++;
                    s = argv[index];
                    sortFilterIndex = (short)stoi(s);
                    
                }else if(s.compare("-CC")==0){
                    index++;
                    s = argv[index];
                    parseIndex(s, clusterCol);
                    
                }else if(s.compare("-EDC")==0){
                    index++;
                    s = argv[index];
                    clusterIndexForEdgeDirection = (short)stoi(s);
                    
                }else if(s.compare("-CN")==0){
                    index++;
                    s = argv[index];
                    clusterName = s;
                    
                }else if(s.compare("-CP")==0){
                    index++;
                    s = argv[index];
                    //cout<<"\nDEnsity: "<<s<<endl;
                    parseFloat(s, clusterParams);
                    
                }else if(s.compare("-WX")==0){
                    index++;
                    s = argv[index];
                    parseIndex(s, windows);
                    
                }else if(s.compare("-GX")==0){
                    index++;
                    s = argv[index];
                    parseFloat(s, overlap);
                    
                }else if(s.compare("-RD")==0){
                    index++;
                    s = argv[index];
                    readDir = s;
                    
                }else if(s.compare("-WD")==0){
                    index++;
                    s = argv[index];
                    writeDir = s;
                    
                }else if(s.compare("-MPL")==0){
                    index++;
                    s = argv[index];
                    minPathLen = (short)stoi(s);
                    
                }else if(s.compare("-DC")==0){
                    index++;
                    s = argv[index];
                    deltaChanges = stof(s);
                    
                }else if(s.compare("-IT")==0){
                    index++;
                    s = argv[index];
                    increaseTime = compare(s,"0")==0?false:true;
                    
                }else if(s.compare("-SM")==0){
                    index++;
                    s = argv[index];
                    signatureMatch = compare(s,"0")==0?false:true;
                    
                }else if(s.compare("-PBC")==0){
                    index++;
                    s = argv[index];
                    printBarcode = compare(s,"0")==0?false:true;
                    
                }else if(s.compare("-JPX")==0){
                    index++;
                    s = argv[index];
                    printBarcodeUsingJavaplex = compare(s,"0")==0?false:true;
                    
                }else if(s.compare("-RP")==0){
                    index++;
                    s = argv[index];
                    refPerformance = compare(s,"0")==0?false:true;
                    
                }else if(s.compare("-FRI")==0){
                    index++;
                    s = argv[index];
                    filterRefIndex = (short)stoi(s);
                    
                }else if(s.compare("-PW")==0){
                    index++;
                    s = argv[index];
                    intPathWidth = (short)stoi(s);
                    
                }else if(s.compare("-ECOL")==0){
                    index++;
                    s = argv[index];
                    edgeColor = s;
                    
                }else if(s.compare("-NSR")==0){
                    index++;
                    s = argv[index];
                    parseFloat(s, nodeSizeRange);
                    
                }else if(s.compare("-HM")==0){
                    index++;
                    s = argv[index];
                    heatmapEachConComp = compare(s,"0")==0?false:true;
                    
                }else if(s.compare("-FN")==0){
                    index++;
                    s = argv[index];
                    fileName = s;
                    
                    if(s.length()==0) return -1;
                }else{
                    std::cout<<"\nInvalud parameters\n"<<std::endl;
                    printHelpMessage();
                    return -1;
                }
                
                index++;
            }
        }catch (std::runtime_error &e) {
            std::cout<<"\nError to read parameters (Runtime error): "<<e.what()<<std::endl;
            return -1;
        }catch (std::exception &e) {
            std::cout<<"\nError to read parameters: "<<e.what()<<std::endl;
            return -1;
        }catch(...){
            std::cout<<"\nError to read parameters"<<std::endl;
            return -1;
        }
        
        short _fSize = filterCol.size();
        
        while(windows.size()<MAX_ALLOW_FILTER){
            windows.push_back(1);
        }
        
        while(overlap.size()<_fSize){
            overlap.push_back(overlap[0]);
        }
        
        if(clusterIndexForEdgeDirection >= clusterCol.size()){
            clusterIndexForEdgeDirection = clusterCol.size()-1;
        }
        
        if(nodeSizeRange.size()==0){
            nodeSizeRange = {15.0,90.0};
        }
        //cout<<endl;
        return 0;
    }
    
    std::string Hyppox::createMapper(){
        MType* mapper = new MType();
        if(mapper==NULL){
            return "Does not have enough space to create the mapper.";
        }
        
        std::string r = mapper->createMapper(&this->graph, &this->graph.individualIndexMap);
        delete mapper;
        
        return r;
    }
    
    std::string Hyppox::visualization(){
        std::cout<<"Started constructing interesting paths...\n";
        this->graph.generateInterestingPaths();
        
        std::cout<<"Started constructing flares...\n";
        time_t t1=clock(), t2;
        if(hyppox::Config::COL_INDIVIDUAL.size()>0)this->graph.generateFlares();
        t2 = clock();
        
        float _time = (float)(t2-t1)/CLOCKS_PER_SEC;
        std::string unit = "(sec)";
        
        if(_time<1){
            _time = ((float)(t2-t1)*1000)/CLOCKS_PER_SEC;
            unit = "(ms)";
        }
        
        std::cout<<"Total time to construct all flares:"<<_time<<unit<<std::endl;
        
        return this->graph.printD3Graph();
    }
}


#endif /* hyppox_h */
