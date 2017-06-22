
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
 Md. Kamruzzaman              06/20/2017      Added a flag to control connected component coloring 
 **************************************************************************************************/


// File selection
// 0: Combined with genome A and B
// 1: Consider only genome B
#define GENOME 1

// Define the filter
// 0: filter by DAP, this is fixed.
#define FILTER_1 0

// We have option to change any of this three as a second filter
// 1: filter by Temperature
// 2: filter by solar radiation
// 3: filter by humidity
#define FILTER_2 3

// 0.6 for sr,temp
// 0.95 for sr, temp
// 10X10 for sr, temp
// dap,hum, (A), {1,1,10,1,2,3,4,5,9,11,13,14}

#if GENOME==0
    // Specify full path of the data file
    #define DATA_FILE_NAME "gxe_pat.csv"
    #if FILTER_2==1
        #define CLUSTER_RADIUS 0.9
        #define OVERLAP 0.93
        #define ENV_NAMES {"dap","temp"}
        #define WINDOW_X 30
        #define WINDOW_Y 5
        #define PRINT_ALL_PATHS true
        #define ASSIGN_PATH_COLOR_MANUAL false
        #define PATH_LIST {}
        #define PATH_COLOR {}
        // Specify full path of the data file
        #define COORDINATE_FILE_NAME "gxe_node_position_2D_dap_temp_9.csv"
    #elif FILTER_2==2
        #define CLUSTER_RADIUS 0.7
        #define OVERLAP 0.95
        #define ENV_NAMES {"dap","sr"}
        #define WINDOW_X 20
        #define WINDOW_Y 5
        #define PRINT_ALL_PATHS true
        #define ASSIGN_PATH_COLOR_MANUAL false
        #define PATH_LIST {}
        #define PATH_COLOR {}
        // Specify full path of the data file
        #define COORDINATE_FILE_NAME "gxe_node_position_dap_sr_7.csv"
    #elif FILTER_2==3
        #define CLUSTER_RADIUS 0.8
        #define OVERLAP 0.92
        #define ENV_NAMES {"dap","hum"}
        #define WINDOW_X 20
        #define WINDOW_Y 5
        #define PRINT_ALL_PATHS true
        #define ASSIGN_PATH_COLOR_MANUAL false
        #define PATH_LIST {}
        #define PATH_COLOR {}
        // Specify full path of the data file
        #define COORDINATE_FILE_NAME "gxe_node_position_dap_hum_8.csv"
    #endif
#else
    // Specify full path of the data file
    #define DATA_FILE_NAME "gxe_pat_B.csv"
    #if FILTER_2==1
        #define CLUSTER_RADIUS 0.6
        #define OVERLAP 0.92
        #define ENV_NAMES {"dap","temp"}
        #define WINDOW_X 30
        #define WINDOW_Y 5
        #define PRINT_ALL_PATHS false
        #define ASSIGN_PATH_COLOR_MANUAL true
        #define PATH_LIST {2,2,4,1,2,3,4,4,4,1,2,3,4}
        #define PATH_COLOR {"","","","#da2577","#b7a511","#0eafc9","#0000ff","","","#ff0000","#8e44ad","#0ec92d","#fead54"}
        // Specify full path of the data file
        #define COORDINATE_FILE_NAME "gxe_node_position_2D_dap_temp_B_6.csv"
    #elif FILTER_2==2
        #define CLUSTER_RADIUS 0.6
        #define OVERLAP 0.78
        #define ENV_NAMES {"dap","sr"}
        #define WINDOW_X 30
        #define WINDOW_Y 5
        #define PRINT_ALL_PATHS false
        #define ASSIGN_PATH_COLOR_MANUAL true
        #define PATH_LIST {1,1,9,1,2,3,4,5,6,7,8,11}
        #define PATH_COLOR {"","","","#ff0000","#da2577","#75a920","#0eafc9","#0000ff","#8e44ad","#8e44ad","#8e44ad","#8e44ad"}
        // Specify full path of the data file
        #define COORDINATE_FILE_NAME "gxe_node_position_2D_dap_sr_B_6.csv"
    #elif FILTER_2==3
        #define CLUSTER_RADIUS 0.7
        #define OVERLAP 0.92
        #define ENV_NAMES {"dap","hum"}
        #define WINDOW_X 30
        #define WINDOW_Y 5
        #define PRINT_ALL_PATHS false
        #define ASSIGN_PATH_COLOR_MANUAL true
        #define PATH_LIST {1,2,10,1,2,3,4,5,6,7,9,10,12}
        #define PATH_COLOR {"","","","#ff0000","#da2577","#b7a511","#9EB927","#0ec92d","#0eafc9","#0000ff","#8e44ad","#cead45","#fead54"}
        // Specify full path of the data file
        #define COORDINATE_FILE_NAME "gxe_node_position_2D_dap_hum_B_7.csv"
    #endif
#endif

#define CLUSTER_DENSITY 2

// Total environmental attributes
#define ENV_ATTR 4

// If file is pat_all_gl the enable this contant.
#define PLANT_ID_LIST true

// Sort phenotype by value or DAP, any of the two must be true
#define SORT_BY_PHENOTYPE true
#define SORT_BY_DAP false

// Split nodes by genotype or not
#define CLUSTER_BY_GENLOC false

#define OVERLAP_X OVERLAP
#define OVERLAP_Y OVERLAP

#define PERIF_WEIGHT 0.5
#define CORE_BY_PREIF 2
#define CORE_POINT_WEIGHT (PERIF_WEIGHT*CORE_BY_PREIF)

#define EDGE_THRESHOLD 2

#define FILTER 2
#define TOTAL_CLUSTER_IDS (2*FILTER)

#define EDGE_OVERLAPPING false

// Print label to add edges or only on the interesting paths
#define SHOW_EDGE_LABEL_FOR_ALL_EDGES true

// Print edge label in ph,env att graph
#define PRINT_EDGE_LABEL false

// Either print weight as edge label
#define EDGE_LABEL_WEIGHT false

// If EDGE_LABEL_WEIGHT == true then print normalized weight as edge label or node weight diference as edge weight
#define EDGE_LABEL_NORM_WEIGHT false

// Or print combined edge weight, L2 distance of ph & env atts
#define EDGE_LABEL_SCORE false

// Or print signature value
#define PRINT_SIGNATURE true

#define NEGLIGIBLE_WEIGHT 0.00001

#define MINIMUM_PATH_LENGTH 3 // Considering each edge length is 1
#define DELTA_CHANGE 0.0
#define INCREASE_DAP false

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

#define EXPLORE_PHENOTYPE_GRAPH false

// Edge width on a path
#define EDGE_WIDTH 12

// Show precision in edge label
#define SHOW_PRECISION_IN_EDGE_LABEL 3

// Whether generate path or not
#define GENERATE_PATHS true

// Edge color of nin path
#define EDGE_COLOR "#a4a4a4"

// Node size range
#define NODE_SIZE_MAX 30.0
#define NODE_SIZE_MIN 15.0

// Quad tree child
#define QUAD_TREE_CHILDREN 2*FILTER

// Directed graph or not
#define DIRECTED_EDGE true

// Whether consider only MAX cc or all ccs
#define MAX_CONNECTED_COMPONENT_ONLY false

// true: Enable to print pie chart
#define PIE_CHART_CODE false

// true: heatmap coloring scheme at each connected compoment (CC) separately
// false: apply gloal heatmap coloring scheme for all connected component
#define HEATMAP_EACH_CC false

// Always false here
#define HAMMING_DISTANCE false;

// Testing flag
#define TEST false

/* Conventions
1. Edge from lower weighted node to higher weighted node
2. Normalized weight array index: 0->phenotype, n->n-th environmental attribute{n=1,...,N}
3. Bit array index: 1st filter, 2nd filter, ...
4. predecessor Edge -> matchSignature(Successor edge)

*/
