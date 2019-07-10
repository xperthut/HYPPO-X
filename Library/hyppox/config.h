
/**************************************************************************************************
 Copyright © 2016-2019 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: config.h
 Objective: Configuration file
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              02/16/2017      Initial version
 Md. Kamruzzaman              06/20/2017      Added a flag to control connected component coloring
 Md. Kamruzzaman              01/03/2018      Converting to a class with static members
 **************************************************************************************************/

#include <iostream>
#include <string>
#include <list>
#include <algorithm>
#include <vector>
#include <fstream>

#define BIT_SET_SIZE 10
#define COEFF_FIELD_CHARACTERISTICS 13 // Any prime number
#define MIN_PERSISTENCE 0.0 // Always set to zero
#define ABS(x) (x<0?x*-1:x)
#define MAX_ALLOW_FILTER 2

//enum OUTPUT {__STD=0, __FILE, __PHP};

// @brief Compute time and unit and return them separately
// Input is a time difference
template<typename T>
std::vector<std::string> getTime(T t){
    if(t<1){
        return {std::to_string(t*1000),"ms"};
    }else if(t>=1 && t<60){
        return {std::to_string(t),"sec"};
    }else if(t>=60 && t<3600){
        return {std::to_string(t/60),"min"};
    }else if(t>=3600 && t<86400){
        return {std::to_string(t/3600),"hr"};
    }else{
        float d = t/86400;
        
        if(d>1&&d<30) return {std::to_string(d),"days"};
        else if(d>=30&&d<365) return {std::to_string(d/30),"mn"};
        else{
            return {std::to_string(d/365),"yr"};
        }
    }
}

template <typename CType>
CType convert_to (const std::string &str){
    try{
        std::istringstream cs(str);
        CType val;
        cs >> val;
        return val;
    }catch (std::exception &e) {
        std::cout<<"\nInput file dataformat error. Please check data type of the column based on instruction.\nError description: "<<e.what()<<std::endl;
        exit(-1);
    }
}

/****
 Get current date/time, format is YYYY-MM-DD_HH:mm:ss
 Copied from: http://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
 ***/
const std::string GetCurrentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    
    strftime(buf, sizeof(buf), "%Y-%m-%d_%X", &tstruct);
    
    return buf;
}

// @brief Convert floating point number to a string with fixed number of precision value
const std::string fixPrecision(float value, int precision){
    std::string pValue = std::to_string(value);
    size_t pos = 0;
    
    if((pos = pValue.find(".")) != std::string::npos){
        
        if(pValue.length()-pos-1 > precision){
            if(precision>0) return pValue.substr(0, pos+1+precision);
            return pValue.substr(0, pos);
        }
    }
    
    return pValue;
}

// @brief html colors
const std::string getHTMLColor(size_t index){
    const std::vector<std::string> s={"#ff0000", "#EED6C4", "#5CD4EC", "#DDDDCB", "#68466E", "#EBD7DE", "#7843DF", "#5EB8E7", "#BEE967", "#3AF0B3", "#E3B330", "#C4F1A5", "#E7F5F7", "#D1BBC7", "#EABAAB", "#5299E2", "#ED90B9", "#B07490", "#E7ACD2", "#E7F3A1", "#98E1E7", "#73F142", "#EFA9AC", "#C1EFCA", "#A6B6EA", "#C1C593", "#D63EEB", "#BD5E4F", "#42E885", "#E568A5", "#DCE872", "#E47885", "#B49FBB", "#A8CBAB", "#DD3090", "#379CB0", "#7AC5B9", "#F03CC7", "#A0D1EB", "#9B9AEF", "#71A883", "#AA65E0", "#CBF0E4", "#E56DD2", "#6FF1F1", "#9FE67D", "#E39267", "#D1E5B5", "#D1E0F4", "#8AA1B1", "#59ECDD", "#DBF240", "#B8CED3", "#C79FDD", "#D4B890", "#E4C6EB", "#ECB66B", "#F097DF", "#7E806C", "#BFE6EE", "#AF2DAB", "#B78983", "#ABF5ED", "#E63F6C", "#EEEFB5", "#446FC5", "#F0E7C2", "#44EF76", "#9C4893", "#58BE6B", "#E1D62D", "#66C73C", "#8DF275", "#96EAC5", "#7CEDB2", "#87CE93", "#F1E264", "#5F4CAC", "#7AC0D1", "#EE612D", "#579C55", "#ADAD9B", "#B0A943", "#DAC87F", "#E46FEC", "#8179E6", "#F4F1E9", "#B4C8BD", "#5FE69A", "#E8D879", "#2A5637", "#878A46", "#99DE39", "#DB9132", "#55E5C0", "#C787E2", "#8876AF", "#6183AF", "#B5C57E", "#A1F7AC", "#3BC5CA"};
    
    return s[index%s.size()];
}

// @brief Generate a list of html colors based on the requirement
const std::string getHTMLColorList(size_t size){
    std::string as="[";
    for(size_t i=0; i<size; i++){
        if(as.length()>1) as += ",";
        as += getHTMLColor(i);
    }
    return as+"]";
}

namespace hyppox{
    
    class Config{
        private:
        // Stop creating object of this class
        Config()=default;
        ~Config()=default;
        
        public:
        static short COL_GENOTYPE;
        static std::vector<std::string> FILTER_GENOTYPE;
        static std::vector<short> COL_LOCATION;
        static std::vector<short> COL_DATETIME;
        static std::vector<short> COL_OTHER;
        static std::vector<short> COL_INDIVIDUAL;
        static std::vector<short> COL_FILTER;
        static std::vector<short> COL_CLUSTER;
        static std::string CLUSTER_METHOD;
        static std::vector<float> CLUSTER_PARAM;
        static std::vector<short> WINDOWS;
        static std::vector<float> GAIN;
        static std::string READ_DIR;
        static std::string WRITE_DIR;
        static std::vector<std::string> HEADER_NAMES;
        
        // Sort the nodes in the graph based on this filter values
        static short SORT_FILTER_INDEX;
        
        // Considering each edge length is 1
        static short MINIMUM_PATH_LENGTH;
        
        // A threshold under which we will allow to grow the path if any bit of the signature does not match
        static float DELTA_CHANGE;
        
        // Flag forces to build path along increasing DAP
        static bool INCREASE_DAP;
        
        // Flag to enable/disable signature matching
        static bool MATCH_SIGNATURE;
        
        // Print all graphs for barcode
        static bool PRINT_BARCODE;
        
        // Generate code fit for javaplex simplex
        static bool PH_JAVA_PLEX;
        
        // Residual graph
        static bool REFERENCE_PERFORMANCE;
        static short REFERENCE_PH_INDEX; // Edge direction based on this indexed phenotypic value
        static short REFERENCE_ENV_INDEX;
        // End
        
        // Edge width on a path
        static short EDGE_WIDTH;
        
        // Edge color of nin path
        static std::string EDGE_COLOR;
        
        // Node size range
        static std::vector<float> NODE_SIZE_RANGE;
        
        // true: heatmap coloring scheme at each connected compoment (CC) separately
        // false: apply gloal heatmap coloring scheme for all connected component
        static bool HEATMAP_EACH_CC;
        
        // Weight of periferal points during clusting
        static float PERIF_WEIGHT;
        
        // Weight fector of core points with respect to periferal points
        static float CORE_BY_PREIF;
        
        // Weight of core points during clustering
        static float CORE_POINT_WEIGHT;
        
        // The number of filters
        static short FILTER;
        
        // The number of cluster attribites
        static short CLUSTER;
        
        // Maximum number of possible cluster ids in a overlap
        static short TOTAL_CLUSTER_IDS;
        
        // If edge weight is zero then replace zero with this value
        static float NEGLIGIBLE_WEIGHT;
        
        // Quad tree child
        static short QUAD_TREE_CHILDREN;
        
        // Name of the filter attribute
        static std::vector<std::string> FILTER_NAMES;
        
        // Name of the filter attribute
        static std::vector<std::string> CLUSTER_NAMES;
        
        // Phenomics data file name with absolute path
        static std::string DATA_FILE_NAME;
        
        // Which filter is used for edge direction matching
        static short FILTER_SIGNATURE_MATCHING;
        
        static void setParams(short genotypeCol, std::vector<std::string> filterGen, std::vector<short> locationCol, std::vector<short> dateTimeCol, std::vector<short> otherInfoCol, std::vector<short> _indvCol, std::vector<short> filterCol, std::vector<short> clusterCol, short clusterIndexForEdgeDirection, std::string clusterName, std::vector<float> clusterParams, std::vector<short> windows, std::vector<float> overlap, std::string readDir, std::string writeDir, short minPathLen, float deltaChanges, bool increaseTime, bool signatureMatch, bool printBarcode, bool printBarcodeUsingJavaplex, bool refPerformance, short filterRefIndex, short intPathWidth, std::string edgeColor, std::vector<float> nodeSizeRange, bool heatmapEachConComp, std::string fileName, short sortFilterIndex, std::vector<short> filterSignatureMatching);
        
        static void printAllConfig();
        
        template<typename IType, typename OType>
        static void printOP(short terminal){}
        
    };
    
    short Config::COL_GENOTYPE;
    std::vector<std::string> Config::FILTER_GENOTYPE;
    std::vector<short> Config::COL_LOCATION;
    std::vector<short> Config::COL_DATETIME;
    std::vector<short> Config::COL_OTHER;
    std::vector<short> Config::COL_INDIVIDUAL;
    std::vector<short> Config::COL_FILTER;
    std::vector<short> Config::COL_CLUSTER;
    std::string Config::CLUSTER_METHOD;
    std::vector<float> Config::CLUSTER_PARAM;
    std::vector<short> Config::WINDOWS;
    std::vector<float> Config::GAIN;
    short Config::SORT_FILTER_INDEX=0;
    std::string Config::READ_DIR;
    std::string Config::WRITE_DIR;
    std::vector<std::string> Config::HEADER_NAMES;
    short Config::MINIMUM_PATH_LENGTH=3;
    float Config::DELTA_CHANGE=0.0;
    bool Config::INCREASE_DAP=false;
    bool Config::MATCH_SIGNATURE=true;
    bool Config::PRINT_BARCODE=false;
    bool Config::PH_JAVA_PLEX=false;
    bool Config::REFERENCE_PERFORMANCE=true;
    short Config::REFERENCE_PH_INDEX=0;
    short Config::REFERENCE_ENV_INDEX;
    short Config::EDGE_WIDTH=2;
    std::string Config::EDGE_COLOR="#a4a4a4";
    std::vector<float> Config::NODE_SIZE_RANGE;
    bool Config::HEATMAP_EACH_CC=false;
    float Config::PERIF_WEIGHT = 0.5;
    float Config::CORE_BY_PREIF = 2.0;
    float Config::CORE_POINT_WEIGHT = (PERIF_WEIGHT*CORE_BY_PREIF);
    short Config::FILTER = 2;
    short Config::CLUSTER = 1;
    std::vector<std::string> Config::CLUSTER_NAMES;
    short Config::TOTAL_CLUSTER_IDS = 2;
    float Config::NEGLIGIBLE_WEIGHT = 0.00001;
    short Config::QUAD_TREE_CHILDREN = 2;
    std::vector<std::string> Config::FILTER_NAMES;
    std::string Config::DATA_FILE_NAME = "";
    short Config::FILTER_SIGNATURE_MATCHING = 0;
    
    void Config::setParams(short genotypeCol, std::vector<std::string> filterGen, std::vector<short> locationCol, std::vector<short> dateTimeCol, std::vector<short> otherInfoCol, std::vector<short> _indvCol, std::vector<short> filterCol, std::vector<short> clusterCol, short clusterIndexForEdgeDirection, std::string clusterName, std::vector<float> clusterParams, std::vector<short> windows, std::vector<float> overlap, std::string readDir, std::string writeDir, short minPathLen, float deltaChanges, bool increaseTime, bool signatureMatch, bool printBarcode, bool printBarcodeUsingJavaplex, bool refPerformance, short filterRefIndex, short intPathWidth, std::string edgeColor, std::vector<float> nodeSizeRange, bool heatmapEachConComp, std::string fileName, short sortFilterIndex, std::vector<short> filterSignatureMatching){
        
        COL_GENOTYPE = genotypeCol;
        FILTER_GENOTYPE = filterGen;
        COL_LOCATION = locationCol;
        COL_DATETIME = dateTimeCol;
        COL_OTHER = otherInfoCol;
        COL_INDIVIDUAL = _indvCol;
        COL_FILTER = filterCol;
        COL_CLUSTER = clusterCol;
        CLUSTER_METHOD = clusterName;
        CLUSTER_PARAM = clusterParams;
        WINDOWS = windows;
        GAIN = overlap;
        READ_DIR = readDir;
        WRITE_DIR = writeDir;
        MINIMUM_PATH_LENGTH = minPathLen;
        DELTA_CHANGE = deltaChanges;
        INCREASE_DAP = increaseTime;
        MATCH_SIGNATURE = signatureMatch;
        PRINT_BARCODE = printBarcode;
        PH_JAVA_PLEX = printBarcodeUsingJavaplex;
        REFERENCE_PERFORMANCE = refPerformance;
        REFERENCE_ENV_INDEX = filterRefIndex;
        EDGE_WIDTH = intPathWidth;
        EDGE_COLOR = edgeColor;
        NODE_SIZE_RANGE = nodeSizeRange;
        HEATMAP_EACH_CC = heatmapEachConComp;
        DATA_FILE_NAME = fileName;
        REFERENCE_PH_INDEX = clusterIndexForEdgeDirection;
        SORT_FILTER_INDEX = sortFilterIndex;
        
        FILTER = COL_FILTER.size();
        CLUSTER = COL_CLUSTER.size();
        
        TOTAL_CLUSTER_IDS = 1;
        for(int i=0;i<FILTER;i++)TOTAL_CLUSTER_IDS*=2;
        
        QUAD_TREE_CHILDREN = 1;
        for(int i=0;i<FILTER;i++)QUAD_TREE_CHILDREN*=2;
        
        // Adjust gain value
        for(int i=0;i<FILTER;i++){
            if(GAIN[i]>1){
                GAIN[i] = (GAIN[i]*2)/100;
            }else{
                GAIN[i] *= 2;
                if(GAIN[i]>1) GAIN[i] /=2;
            }
        }
        
        if(filterSignatureMatching.size()>0){
            FILTER_SIGNATURE_MATCHING = 0;
            for(short d:filterSignatureMatching){
                FILTER_SIGNATURE_MATCHING += (short)pow(2, FILTER-d);
            }
        }
        
        //std::cout<<"FILTER_SIGNATURE_MATCHING:"<<FILTER_SIGNATURE_MATCHING<<std::endl;
    }
    
    void Config::printAllConfig(){
        std::cout<<"COL_GENOTYPE:"<<Config::COL_GENOTYPE<<std::endl;
        if(Config::FILTER_GENOTYPE.size()>0){
            std::cout<<"FILTER_GENOTYPE:[";
            for(auto s:Config::FILTER_GENOTYPE){
                std::cout<<s<<" ";
            }
            std::cout<<"]"<<std::endl;
        }
        if(Config::COL_LOCATION.size()>0){
            std::cout<<"COL_LOCATION:[";
            for(short s:Config::COL_LOCATION){
                std::cout<<s<<" ";
            }
            std::cout<<"]"<<std::endl;
        }
        if(Config::COL_DATETIME.size()>0){
            std::cout<<"COL_DATETIME:[";
            for(short s:Config::COL_DATETIME){
                std::cout<<s<<" ";
            }
            std::cout<<"]"<<std::endl;
        }
        if(Config::COL_OTHER.size()>0){
            std::cout<<"COL_OTHER:[";
            for(auto s:Config::COL_OTHER){
                std::cout<<s<<" ";
            }
            std::cout<<"]"<<std::endl;
        }
        if(Config::COL_INDIVIDUAL.size()>0){
            std::cout<<"COL_INDIVIDUAL:[";
            for(auto s:Config::COL_INDIVIDUAL){
                std::cout<<s<<" ";
            }
            std::cout<<"]"<<std::endl;
        }
        if(Config::COL_FILTER.size()>0){
            std::cout<<"COL_FILTER:[";
            for(auto s:Config::COL_FILTER){
                std::cout<<s<<" ";
            }
            std::cout<<"]"<<std::endl;
        }
        if(Config::COL_CLUSTER.size()>0){
            std::cout<<"COL_CLUSTER:[";
            for(auto s:Config::COL_CLUSTER){
                std::cout<<s<<" ";
            }
            std::cout<<"]"<<std::endl;
        }
        std::cout<<"CLUSTER_METHOD:"<<Config::CLUSTER_METHOD<<std::endl;
        if(Config::CLUSTER_PARAM.size()>0){
            std::cout<<"CLUSTER_PARAM:[";
            for(auto s:Config::CLUSTER_PARAM){
                std::cout<<s<<" ";
            }
            std::cout<<"]"<<std::endl;
        }
        if(Config::WINDOWS.size()>0){
            std::cout<<"WINDOWS:[";
            for(auto s:Config::WINDOWS){
                std::cout<<s<<" ";
            }
            std::cout<<"]"<<std::endl;
        }
        if(Config::GAIN.size()>0){
            std::cout<<"GAIN:[";
            for(auto s:Config::GAIN){
                std::cout<<s<<" ";
            }
            std::cout<<"]"<<std::endl;
        }
        std::cout<<"READ_DIR:"<<Config::READ_DIR<<std::endl;
        std::cout<<"WRITE_DIR:"<<Config::WRITE_DIR<<std::endl;
        std::cout<<"MINIMUM_PATH_LENGTH:"<<Config::MINIMUM_PATH_LENGTH<<std::endl;
        std::cout<<"DELTA_CHANGE:"<<Config::DELTA_CHANGE<<std::endl;
        std::cout<<"INCREASE_DAP:"<<Config::INCREASE_DAP<<std::endl;
        std::cout<<"MATCH_SIGNATURE:"<<Config::MATCH_SIGNATURE<<std::endl;
        std::cout<<"PRINT_BARCODE:"<<Config::PRINT_BARCODE<<std::endl;
        std::cout<<"PH_JAVA_PLEX:"<<Config::PH_JAVA_PLEX<<std::endl;
        std::cout<<"REFERENCE_PERFORMANCE:"<<Config::REFERENCE_PERFORMANCE<<std::endl;
        std::cout<<"REFERENCE_PH_INDEX:"<<Config::REFERENCE_PH_INDEX<<std::endl;
        std::cout<<"REFERENCE_ENV_INDEX:"<<Config::REFERENCE_ENV_INDEX<<std::endl;
        std::cout<<"EDGE_WIDTH:"<<Config::EDGE_WIDTH<<std::endl;
        std::cout<<"EDGE_COLOR:"<<Config::EDGE_COLOR<<std::endl;
        if(Config::NODE_SIZE_RANGE.size()>0){
            std::cout<<"NODE_SIZE_RANGE:[";
            for(auto s:Config::NODE_SIZE_RANGE){
                std::cout<<s<<" ";
            }
            std::cout<<"]"<<std::endl;
        }
        std::cout<<"HEATMAP_EACH_CC:"<<Config::HEATMAP_EACH_CC<<std::endl;
        std::cout<<"PERIF_WEIGHT:"<<Config::PERIF_WEIGHT<<std::endl;
        std::cout<<"CORE_BY_PREIF:"<<Config::CORE_BY_PREIF<<std::endl;
        std::cout<<"CORE_POINT_WEIGHT:"<<Config::CORE_POINT_WEIGHT<<std::endl;
        std::cout<<"FILTER:"<<Config::FILTER<<std::endl;
        std::cout<<"CLUSTER:"<<Config::CLUSTER<<std::endl;
        std::cout<<"CLUSTER:"<<Config::CLUSTER<<std::endl;
        if(Config::CLUSTER_NAMES.size()>0){
            std::cout<<"CLUSTER_NAMES:[";
            for(auto s:Config::CLUSTER_NAMES){
                std::cout<<s<<" ";
            }
            std::cout<<"]"<<std::endl;
        }
        std::cout<<"TOTAL_CLUSTER_IDS:"<<Config::TOTAL_CLUSTER_IDS<<std::endl;
        std::cout<<"NEGLIGIBLE_WEIGHT:"<<Config::NEGLIGIBLE_WEIGHT<<std::endl;
        std::cout<<"QUAD_TREE_CHILDREN:"<<Config::QUAD_TREE_CHILDREN<<std::endl;
        if(Config::FILTER_NAMES.size()>0){
            std::cout<<"FILTER_NAMES:[";
            for(auto s:Config::FILTER_NAMES){
                std::cout<<s<<" ";
            }
            std::cout<<"]"<<std::endl;
        }
        std::cout<<"DATA_FILE_NAME:"<<Config::DATA_FILE_NAME<<std::endl;
        std::cout<<"FILTER_SIGNATURE_MATCHING:"<<Config::FILTER_SIGNATURE_MATCHING<<std::endl;
    }
}


/* Conventions
1. Edge from lower weighted node to higher weighted node
2. Normalized weight array index: 0->performance, n->n-th environmental attribute{n=1,...,N}
3. Bit array index: 1st filter, 2nd filter, ...
4. predecessor Edge -> matchSignature(Successor edge)

1. All file names must start with lower case letter
2. All class names start with upper case letter
*/
// Old constants



// File selection
// 0: Combined with genome A and B
// 1: Consider only genome B
//static short GENOME;

// Define the filter
// We have option to change any of this four as a first filter
// 0: filter by DAP
// 1: filter by Temperature
// 2: filter by solar radiation
// 3: filter by humidity
// 3: filter by growth rate
//static short FILTER_1;

// We have option to change any of this four as a second filter
// 0: filter by DAP
// 1: filter by Temperature
// 2: filter by solar radiation
// 3: filter by humidity
// 4: filter by growth rate
//static short FILTER_2;

// 0.6 for sr,temp
// 0.95 for sr, temp
// 10X10 for sr, temp
// dap,hum, (A), {1,1,10,1,2,3,4,5,9,11,13,14}

// Define the cluster value
// 0: cluster by DAP
// 1: cluster by Temperature
// 2: cluster by solar radiation
// 3: cluster by humidity
// 4: cluster by Growth rate
//static short CLUSTER_ATTRIBUTE;



// Cluster radius
//static float CLUSTER_RADIUS;

// Percentage overlapping
//static float OVERLAP;

// Number of windows for filter 1
//static short WINDOW_X;

// Number of windows for filter 2
//static short WINDOW_Y;

// Print all interesting paths
//static bool PRINT_ALL_PATHS;

// Assign color to each path manually
//static bool ASSIGN_PATH_COLOR_MANUAL;

// List of interesting path numbers based on interestingness score in decreasing order
//static list<short> PATH_LIST;

// List of color for each of the paths selected at PATH_LIST
//static list<string> PATH_COLOR;

// Absolute path of the file with contains coordinate of each vertex used to draw pie chart
//static string COORDINATE_FILE_NAME;

// Cluster density, the number of minimum points
//static short CLUSTER_DENSITY;

// If file is pat_all_gl the enable this contant.
//static bool PLANT_ID_LIST;

// Percentage overlap along filter 1
//static float OVERLAP_X;

// Percentage overlap along filter 2
//static float OVERLAP_Y;


// Total environmental attributes
//static short ENV_ATTR;

//static void setAttributes();

//static void resetSettings();

/*void Config::setAttributes(){
    
    if(GENOME==0){
        DATA_FILE_NAME = "input.csv";
        if(FILTER==1){
            WINDOW_Y = 1;
            PRINT_ALL_PATHS = true;
            ASSIGN_PATH_COLOR_MANUAL = false;
            PATH_LIST.clear();
            PATH_COLOR.clear();
            
            if(FILTER_1==0){
                CLUSTER_RADIUS = 0.6;
                OVERLAP = 0.50;
                WINDOW_X = 30;
                COORDINATE_FILE_NAME = "gxe_node_position_1D_dap_6.csv";
            }else if(FILTER_1==1){
                CLUSTER_RADIUS = 0.4;
                OVERLAP = 0.8;
                WINDOW_X = 15;
                COORDINATE_FILE_NAME = "gxe_node_position_1D_temp_4.csv";
            }else if(FILTER_1==2){
                CLUSTER_RADIUS = 0.6;
                OVERLAP = 0.72;
                WINDOW_X = 12;
                COORDINATE_FILE_NAME = "gxe_node_position_single_SR_6.csv";
            }else if(FILTER_1==3){
                CLUSTER_RADIUS = 0.9;
                OVERLAP = 0.95;
                WINDOW_X = 18;
                COORDINATE_FILE_NAME = "gxe_node_position_single_HUM_9.csv";
            }else if(FILTER_1==4){
                CLUSTER_RADIUS = 1.0;
                OVERLAP = 0.4;
                WINDOW_X = 10;
                COORDINATE_FILE_NAME = "gxe_node_position_1D_gr_1_10.csv";
            }else{
                CLUSTER_RADIUS = 0.6;
                OVERLAP = 0.50;
                WINDOW_X = 30;
                COORDINATE_FILE_NAME = "gxe_node_position_1D_dap_6.csv";
            }
        }else{
            if(FILTER_2==1){
                CLUSTER_RADIUS = 0.9;
                OVERLAP = 0.93;
                WINDOW_X = 30;
                WINDOW_Y = 5;
                PRINT_ALL_PATHS = true;
                ASSIGN_PATH_COLOR_MANUAL = false;
                PATH_LIST.clear();
                PATH_COLOR.clear();
                COORDINATE_FILE_NAME = "gxe_node_position_2D_dap_temp_9.csv";
            }else if(FILTER_2==2){
                CLUSTER_RADIUS = 0.7;
                OVERLAP = 0.95;
                WINDOW_X = 20;
                WINDOW_Y = 5;
                PRINT_ALL_PATHS = true;
                ASSIGN_PATH_COLOR_MANUAL = false;
                PATH_LIST.clear();
                PATH_COLOR.clear();
                COORDINATE_FILE_NAME = "gxe_node_position_dap_sr_7.csv";
            }else if(FILTER_2==3){
                CLUSTER_RADIUS = 0.8;
                OVERLAP = 0.92;
                WINDOW_X = 20;
                WINDOW_Y = 5;
                PRINT_ALL_PATHS = true;
                ASSIGN_PATH_COLOR_MANUAL = false;
                PATH_LIST.clear();
                PATH_COLOR.clear();
                COORDINATE_FILE_NAME = "gxe_node_position_dap_hum_8.csv";
            }else if(FILTER_2==4){
                CLUSTER_RADIUS = 0.8;
                OVERLAP = 0.92;
                WINDOW_X = 20;
                WINDOW_Y = 5;
                PRINT_ALL_PATHS = true;
                ASSIGN_PATH_COLOR_MANUAL = false;
                PATH_LIST.clear();
                PATH_COLOR.clear();
                COORDINATE_FILE_NAME = "gxe_node_position_dap_hum_8.csv";
            }else{
                CLUSTER_RADIUS = 0.8;
                OVERLAP = 0.92;
                WINDOW_X = 20;
                WINDOW_Y = 5;
                PRINT_ALL_PATHS = true;
                ASSIGN_PATH_COLOR_MANUAL = false;
                PATH_LIST.clear();
                PATH_COLOR.clear();
                COORDINATE_FILE_NAME = "gxe_node_position_dap_hum_8.csv";
            }
        }
    }else{
        DATA_FILE_NAME = "input_B.csv";
        
        if(FILTER_2==1){
            CLUSTER_RADIUS = 0.6;
            OVERLAP = 0.92;
            WINDOW_X = 30;
            WINDOW_Y = 5;
            PRINT_ALL_PATHS = false;
            ASSIGN_PATH_COLOR_MANUAL = true;
            int a[] = {2,2,4,1,2,3,4,4,4,1,2,3,4};
            PATH_LIST.clear();
            PATH_LIST.insert(PATH_LIST.begin(), a, a+13);
            string b[] = {"","","","#da2577","#b7a511","#0eafc9","#0000ff","","","#ff0000","#8e44ad","#0ec92d","#fead54"};
            PATH_COLOR.clear();
            PATH_COLOR.insert(PATH_COLOR.begin(), b, b+13);
            COORDINATE_FILE_NAME = "gxe_node_position_2D_dap_temp_B_6.csv";
        }else if(FILTER_2==2){
            CLUSTER_RADIUS = 0.6;
            OVERLAP = 0.78;
            WINDOW_X = 30;
            WINDOW_Y = 5;
            PRINT_ALL_PATHS = false;
            ASSIGN_PATH_COLOR_MANUAL = true;
            int a[] = {1,2,9,1,2,3,4,5,6,7,8,11};
            PATH_LIST.clear();
            PATH_LIST.insert(PATH_LIST.begin(), a, a+12);
            string b[] = {"","","","#ff0000","#da2577","#75a920","#0eafc9","#0000ff","#8e44ad","#8ead44","#3effad","#ad448e"};
            PATH_COLOR.clear();
            PATH_COLOR.insert(PATH_COLOR.begin(), b, b+12);
            COORDINATE_FILE_NAME = "gxe_node_position_2D_dap_sr_B_6.csv";
        }else if(FILTER_2==3){
            CLUSTER_RADIUS = 0.7;
            OVERLAP = 0.92;
            WINDOW_X = 30;
            WINDOW_Y = 5;
            PRINT_ALL_PATHS = false;
            ASSIGN_PATH_COLOR_MANUAL = true;
            int a[] = {1,2,10,1,2,3,4,5,6,7,9,10,12};
            PATH_LIST.clear();
            PATH_LIST.insert(PATH_LIST.begin(), a, a+13);
            string b[] = {"","","","#ff0000","#da2577","#b7a511","#9EB927","#0ec92d","#0eafc9","#0000ff","#8e44ad","#cead45","#fead54"};
            PATH_COLOR.clear();
            PATH_COLOR.insert(PATH_COLOR.begin(), b, b+13);
            COORDINATE_FILE_NAME = "gxe_node_position_2D_dap_hum_B_7.csv";
        }else if(FILTER_2==4){
            CLUSTER_RADIUS = 0.7;
            OVERLAP = 0.92;
            WINDOW_X = 30;
            WINDOW_Y = 5;
            PRINT_ALL_PATHS = false;
            ASSIGN_PATH_COLOR_MANUAL = true;
            int a[] = {1,2,10,1,2,3,4,5,6,7,9,10,12};
            PATH_LIST.clear();
            PATH_LIST.insert(PATH_LIST.begin(), a, a+13);
            string b[] = {"","","","#ff0000","#da2577","#b7a511","#9EB927","#0ec92d","#0eafc9","#0000ff","#8e44ad","#cead45","#fead54"};
            PATH_COLOR.clear();
            PATH_COLOR.insert(PATH_COLOR.begin(), b, b+13);
            COORDINATE_FILE_NAME = "gxe_node_position_2D_dap_hum_B_7.csv";
        }else{
            CLUSTER_RADIUS = 0.8;
            OVERLAP = 0.92;
            WINDOW_X = 20;
            WINDOW_Y = 5;
            PRINT_ALL_PATHS = true;
            ASSIGN_PATH_COLOR_MANUAL = false;
            PATH_LIST.clear();
            PATH_COLOR.clear();
            COORDINATE_FILE_NAME = "gxe_node_position_dap_hum_8.csv";
        }
    }
}

void Config::resetSettings(){
    
    ifstream read("param.config");
    
    if(!read.is_open()){
        cout<<"Can't read config file.."<<endl<<endl<<"Press any key to exit..."<<endl;
        cin.get();
        
        exit(0);
    }
    
    string line, flag, value;
    size_t pos;
    
    // Read line that is neither empty nor a comment
    while(getline(read, line)){
        if(line.length()==0 || line[0]=='#') continue;
        // Remove all spaces from the line
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        
        // Remove ending ;
        line.erase(std::remove(line.begin(), line.end(), ';'),line.end());
        
        pos = line.find("=");
        if(pos!=string::npos){
            flag = line.substr(0, pos);
            value = line.substr(pos+1, line.length()-pos-1);
        }
        
        transform(flag.begin(), flag.end(), flag.begin(), ::toupper);
        
        if(flag.compare("DATA_FILE_NAME")==0){
            try {
                if(value.length()==0){
                    cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                    cin.get();
                    
                    exit(0);
                }
                DATA_FILE_NAME = value;
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("GENOME")==0){
            try {
                GENOME = stoi(value);
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("FILTER")==0){
            try {
                FILTER = stoi(value);
                if(FILTER<0 || FILTER>2) FILTER = 2;
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("FILTER_1")==0){
            try {
                FILTER_1 = stoi(value);
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("FILTER_2")==0){
            try {
                FILTER_2 = stoi(value);
            } catch (exception e) {
                if(FILTER>1){
                    cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                    cin.get();
                    
                    exit(0);
                }
            }
        }else if(flag.compare("CLUSTER_ATTRIBUTE")==0){
            try {
                CLUSTER_ATTRIBUTE = stoi(value);
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("CLUSTER_DENSITY")==0){
            try {
                CLUSTER_DENSITY = stoi(value);
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("CLUSTER_RADIUS")==0){
            try {
                CLUSTER_RADIUS = stof(value);
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("OVERLAP")==0){
            try {
                OVERLAP = (stof(value)*2)/100;
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("WINDOW_X")==0){
            try {
                WINDOW_X = stoi(value);
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("WINDOW_Y")==0){
            try {
                WINDOW_Y = stoi(value);
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("ENV_ATTR")==0){
            try {
                ENV_ATTR = stoi(value);
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("PLANT_ID_LIST")==0){
            try {
                if(value.compare("true")==0){
                    PLANT_ID_LIST = true;
                }else if(value.compare("false")==0){
                    PLANT_ID_LIST = false;
                }else{
                    cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                    cin.get();
                    
                    exit(0);
                }
                
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("PERIF_WEIGHT")==0){
            try {
                PERIF_WEIGHT = stof(value);
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("CORE_BY_PREIF")==0){
            try {
                CORE_BY_PREIF = stof(value);
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("NEGLIGIBLE_WEIGHT")==0){
            try {
                NEGLIGIBLE_WEIGHT = stof(value);
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("MINIMUM_PATH_LENGTH")==0){
            try {
                MINIMUM_PATH_LENGTH = stoi(value);
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("DELTA_CHANGE")==0){
            try {
                DELTA_CHANGE = stof(value);
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("INCREASE_DAP")==0){
            try {
                if(value.compare("true")==0){
                    INCREASE_DAP = true;
                }else if(value.compare("false")==0){
                    INCREASE_DAP = false;
                }else{
                    INCREASE_DAP = false;
                }
                
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("MATCH_SIGNATURE")==0){
            try {
                if(value.compare("true")==0){
                    MATCH_SIGNATURE = true;
                }else if(value.compare("false")==0){
                    MATCH_SIGNATURE = false;
                }else{
                    MATCH_SIGNATURE = true;
                }
                
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("PRINT_BARCODE")==0){
            try {
                if(value.compare("true")==0){
                    PRINT_BARCODE = true;
                }else if(value.compare("false")==0){
                    PRINT_BARCODE = false;
                }else{
                    PRINT_BARCODE = false;
                }
                
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("PH_JAVA_PLEX")==0){
            try {
                if(value.compare("true")==0){
                    PH_JAVA_PLEX = true;
                }else if(value.compare("false")==0){
                    PH_JAVA_PLEX = false;
                }else{
                    PH_JAVA_PLEX = false;
                }
                
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("REFERENCE_PERFORMANCE")==0){
            try {
                if(value.compare("true")==0){
                    REFERENCE_PERFORMANCE = true;
                }else if(value.compare("false")==0){
                    REFERENCE_PERFORMANCE = false;
                }else{
                    REFERENCE_PERFORMANCE = true;
                }
                
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("REFERENCE_ENV_INDEX")==0){
            try {
                REFERENCE_ENV_INDEX = stoi(value);
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("EDGE_WIDTH")==0){
            try {
                EDGE_WIDTH = stoi(value);
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("EDGE_COLOR")==0){
            try {
                if(value.length()==0||value[0]!='#'||value.length()<4||value.length()>7){
                    cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                    cin.get();
                    
                    exit(0);
                }
                EDGE_COLOR = value;
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("NODE_SIZE_MAX")==0){
            try {
                NODE_SIZE_MAX = stof(value);
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("NODE_SIZE_MIN")==0){
            try {
                NODE_SIZE_MIN = stof(value);
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }else if(flag.compare("HEATMAP_EACH_CC")==0){
            try {
                if(value.compare("true")==0){
                    HEATMAP_EACH_CC = true;
                }else if(value.compare("false")==0){
                    HEATMAP_EACH_CC = false;
                }else{
                    HEATMAP_EACH_CC = false;
                }
                
            } catch (exception e) {
                cout<<"The value of "<<flag<<" in param.config file is wrong."<<endl<<endl<<"Press any key to exit..."<<endl;;
                cin.get();
                
                exit(0);
            }
        }
    }
    
    if(FILTER==1) GENOME=0;
    else GENOME = 1;
    
    if(FILTER_1==0){
        FILTER_NAMES[0] = "DAP";
        if(FILTER>1){
            if(FILTER_2==1){
                FILTER_NAMES[1] = "temp";
            }else if(FILTER_2==2){
                FILTER_NAMES[1] = "sr";
            }else if(FILTER_2==3){
                FILTER_NAMES[1] = "Humidity";
            }else if(FILTER_2==4){
                FILTER_NAMES[1] = "gr";
            }
        }
    }else if(FILTER_1==1){
        FILTER_NAMES[0] = "temp";
        if(FILTER>1){
            if(FILTER_2==0){
                FILTER_NAMES[1] = "dap";
            }else if(FILTER_2==2){
                FILTER_NAMES[1] = "sr";
            }else if(FILTER_2==3){
                FILTER_NAMES[1] = "hum";
            }else if(FILTER_2==4){
                FILTER_NAMES[1] = "gr";
            }
        }
    }else if(FILTER_1==2){
        FILTER_NAMES[0] = "sr";
        if(FILTER>1){
            if(FILTER_2==0){
                FILTER_NAMES[1] = "dap";
            }else if(FILTER_2==1){
                FILTER_NAMES[1] = "temp";
            }else if(FILTER_2==3){
                FILTER_NAMES[1] = "hum";
            }else if(FILTER_2==4){
                FILTER_NAMES[1] = "gr";
            }
        }
    }else if(FILTER_1==3){
        FILTER_NAMES[0] = "hum";
        if(FILTER>1){
            if(FILTER_2==0){
                FILTER_NAMES[1] = "dap";
            }else if(FILTER_2==1){
                FILTER_NAMES[1] = "temp";
            }else if(FILTER_2==2){
                FILTER_NAMES[1] = "sr";
            }else if(FILTER_2==4){
                FILTER_NAMES[1] = "gr";
            }
        }
    }else if(FILTER_1==4){
        FILTER_NAMES[0] = "gr";
        if(FILTER>1){
            if(FILTER_2==0){
                FILTER_NAMES[1] = "dap";
            }else if(FILTER_2==1){
                FILTER_NAMES[1] = "temp";
            }else if(FILTER_2==2){
                FILTER_NAMES[1] = "sr";
            }else if(FILTER_2==3){
                FILTER_NAMES[1] = "hum";
            }
        }
    }
    
    OVERLAP_X = OVERLAP_Y = OVERLAP;
    CORE_POINT_WEIGHT = (PERIF_WEIGHT*CORE_BY_PREIF);
    TOTAL_CLUSTER_IDS = 1;
    for(int i=0;i<FILTER;i++)TOTAL_CLUSTER_IDS*=2;
    
    QUAD_TREE_CHILDREN = 1;
    for(int i=0;i<FILTER;i++)QUAD_TREE_CHILDREN*=2;
}*/

