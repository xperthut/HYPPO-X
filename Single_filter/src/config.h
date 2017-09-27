
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
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
 
 **************************************************************************************************/

// Define the filter
// 0: filter by DAP
// 1: filter by Temperature
// 2: filter by solar radiation
// 3: filter by humidity

#define FILTER_1 0

#define ENV_NAMES {"",""}

// Total environmental attributes
#define ENV_ATTR 3

#define PRINT_ALL_PATHS false

// Specify full path of the data file
#define DATA_FILE_NAME "../data/input.csv"

// DBSCAN parameters
// 0.6 for DAP,SR
// 0.9 for HUM
// 0.4 for temp
// 0.8 for DAP_50

// overlap
// 0.50 FOR DAP
// 0.72 for SR
// 0.95 for HUM
// 0.8 for temp
// 0.5 for DAP_50



// 30 or 50 for DAP
// 12 for SR
// 18 for HUM
// 15 for temp
// 25 for DAP_50


#if FILTER_1==0
    #define CLUSTER_RADIUS 0.6
    #define OVERLAP 0.50
    #define SUFFIX "DAP"
    #define WINDOW_X 30
    // Specify full path of the data file
    #define COORDINATE_FILE_NAME "../data/gxe_node_position_1D_dap_6.csv"
#elif FILTER_1==1
    #define CLUSTER_RADIUS 0.4
    #define OVERLAP 0.80
    #define SUFFIX "TEMP"
    #define WINDOW_X 15
    // Specify full path of the data file
    #define COORDINATE_FILE_NAME "../data/gxe_node_position_1D_temp_4.csv"
#elif FILTER_1==2
    #define CLUSTER_RADIUS 0.6
    #define OVERLAP 0.72
    #define SUFFIX "SR"
    #define WINDOW_X 12
    // Specify full path of the data file
    #define COORDINATE_FILE_NAME "../data/gxe_node_position_single_SR_6.csv"
#elif FILTER_1==3
    #define CLUSTER_RADIUS 0.9
    #define OVERLAP 0.95
    #define SUFFIX "HUM"
    #define WINDOW_X 18
    // Specify full path of the data file
    #define COORDINATE_FILE_NAME "../data/gxe_node_position_single_HUM_9.csv"
#endif

#define CLUSTER_DENSITY 2
#define WINDOW_Y 1

// If file is pat_all_gl the enable this contant.
#define PLANT_ID_LIST true

// Sort phenotype by value or DAP, any of the two must be true
#define SORT_BY_PHENOTYPE true
#define SORT_BY_DAP false

// Split nodes by genotype or not
#define CLUSTER_BY_GENLOC false

#define OVERLAP_X OVERLAP //single linkage GR DAP
#define OVERLAP_Y OVERLAP //single linkage GR DAP

#define PERIF_WEIGHT 0.1
#define CORE_BY_PREIF 2
#define CORE_POINT_WEIGHT (PERIF_WEIGHT*CORE_BY_PREIF)

#define EDGE_THRESHOLD 2

#define FILTER 1
#define TOTAL_CLUSTER_IDS 1+(2*FILTER)

#define EDGE_OVERLAPPING false

// Print label to add edges or only on the interesting paths
#define SHOW_EDGE_LABEL_FOR_ALL_EDGES false

// Print edge label in ph,env att graph
#define PRINT_EDGE_LABEL false

// Either print weight as edge label
#define EDGE_LABEL_WEIGHT false

// If EDGE_LABEL_WEIGHT == true then print normalized weight as edge label or node weight diference as edge weight
#define EDGE_LABEL_NORM_WEIGHT false

// Or print combined edge weight, L2 distance of ph & env atts
#define EDGE_LABEL_SCORE false

// Or print signature value
#define PRINT_SIGNATURE false

#define NEGLIGIBLE_WEIGHT 0.00001

#define MINIMUM_PATH_LENGTH 0 // Considering each edge length is 1
#define DELTA_CHANGE 0.0

// Print all graphs for barcode
#define PRINT_BARCODE false

/* Any one will be true */
#define NORMALIZE_UNIFORM_DIST false
#define NORMALIZE_STANDARD_DEVIATION false
#define ABSOLUTE_DISTANCE true
/* End */

// Whether we will consider signature or not
#define MATCH_SIGNATURE false

// Residual graph
#define REFERENCE_PHENOTYPE true
#define REFERENCE_ENVIRONMENT false
#define REFERENCE_ENV_INDEX -1
// End

#define EXPLORE_PHENOTYPE_GRAPH true

// Show precision in edge label
#define SHOW_PRECISION_IN_EDGE_LABEL 3

// Whether generate path or not
#define GENERATE_PATHS true

// Graph node size range
#define NODE_SIZE_MAX 20
#define NODE_SIZE_MIN 15

// Directed graph or not
#define DIRECTED_EDGE false

// Edge color of nin path
#define EDGE_COLOR "#101010" //"a4a4a4"

// Enable to print pie chart
#define PIE_CHART_CODE false

// Always false
#define HAMMING_DISTANCE false;

// Testing flag
#define TEST false

/* Conventions
1. Edge from lower weighted node to higher weighted node
2. Normalized weight array index: 0->phenotype, n->n-th environmental attribute{n=1,...,N}
3. Bit array index: 1st env, 2nd env, ... 
4. predecessor Edge -> matchSignature(Successor edge)

*/
