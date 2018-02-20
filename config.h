
/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
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
#include <fstream>
#include <algorithm>

#define BIT_SET_SIZE 10
#define COEFF_FIELD_CHARACTERISTICS 13 // Any prime number
#define MIN_PERSISTENCE 0.0 // Always set to zero
#define ABS(x) (x<0?x*-1:x)

using namespace std;

class Config{
private:
    // Stop creating object of this class
    Config(){}
    ~Config(){}
    
public:
    // File selection
    // 0: Combined with genome A and B
    // 1: Consider only genome B
    static short GENOME;
    
    // Define the filter
    // We have option to change any of this four as a first filter
    // 0: filter by DAP
    // 1: filter by Temperature
    // 2: filter by solar radiation
    // 3: filter by humidity
    // 3: filter by growth rate
    static short FILTER_1;
    
    // We have option to change any of this four as a second filter
    // 0: filter by DAP
    // 1: filter by Temperature
    // 2: filter by solar radiation
    // 3: filter by humidity
    // 4: filter by growth rate
    static short FILTER_2;
    
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
    static short CLUSTER_ATTRIBUTE;
    
    // Name of the filter attribute
    static string FILTER_NAMES[BIT_SET_SIZE];
    
    // Phenomics data file name with absolute path
    static string DATA_FILE_NAME;
    
    // Cluster radius
    static float CLUSTER_RADIUS;
    
    // Percentage overlapping
    static float OVERLAP;
    
    // Number of windows for filter 1
    static short WINDOW_X;
    
    // Number of windows for filter 2
    static short WINDOW_Y;
    
    // Print all interesting paths
    static bool PRINT_ALL_PATHS;
    
    // Assign color to each path manually
    static bool ASSIGN_PATH_COLOR_MANUAL;
    
    // List of interesting path numbers based on interestingness score in decreasing order
    static list<short> PATH_LIST;
    
    // List of color for each of the paths selected at PATH_LIST
    static list<string> PATH_COLOR;
    
    // Absolute path of the file with contains coordinate of each vertex used to draw pie chart
    static string COORDINATE_FILE_NAME;
    
    // Cluster density, the number of minimum points
    static short CLUSTER_DENSITY;
    
    // Total environmental attributes
    static short ENV_ATTR;
    
    // If file is pat_all_gl the enable this contant.
    static bool PLANT_ID_LIST;
    
    // Percentage overlap along filter 1
    static float OVERLAP_X;
    
    // Percentage overlap along filter 2
    static float OVERLAP_Y;
    
    // Weight of periferal points during clusting
    static float PERIF_WEIGHT;
    
    // Weight fector of core points with respect to periferal points
    static float CORE_BY_PREIF;
    
    // Weight of core points during clustering
    static float CORE_POINT_WEIGHT;
    
    // The number of filters
    static short FILTER;
    
    // Maximum number of possible cluster ids in a overlap
    static short TOTAL_CLUSTER_IDS;
    
    // If edge weight is zero then replace zero with this value
    static float NEGLIGIBLE_WEIGHT;
    
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
    
    // Residual graph
    static bool REFERENCE_PHENOTYPE;
    static short REFERENCE_ENV_INDEX;
    // End
    
    // Edge width on a path
    static short EDGE_WIDTH;
    
    // Edge color of nin path
    static string EDGE_COLOR;
    
    // Node size range
    static float NODE_SIZE_MAX;
    static float NODE_SIZE_MIN;
    
    // Quad tree child
    static short QUAD_TREE_CHILDREN;
    
    // true: heatmap coloring scheme at each connected compoment (CC) separately
    // false: apply gloal heatmap coloring scheme for all connected component
    static bool HEATMAP_EACH_CC;
    
    // Generate code fit for javaplex simplex
    static bool PH_JAVA_PLEX;
    
    static void setAttributes();
    
    static void resetSettings();
};

/* Conventions
1. Edge from lower weighted node to higher weighted node
2. Normalized weight array index: 0->phenotype, n->n-th environmental attribute{n=1,...,N}
3. Bit array index: 1st filter, 2nd filter, ...
4. predecessor Edge -> matchSignature(Successor edge)

1. All file names must start with lower case letter
2. All class names start with upper case letter
*/
