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

#include "config.h"

short Config::GENOME = 1;
short Config::FILTER_1 = 0;
short Config::FILTER_2 = 3;
short Config::CLUSTER_ATTRIBUTE = 4;
short Config::CLUSTER_DENSITY = 2;
short Config::ENV_ATTR = 4;
bool Config::PLANT_ID_LIST = true;
float Config::CLUSTER_RADIUS = 0.8;
float Config::OVERLAP = 0.92;
short Config::WINDOW_X = 20;
short Config::WINDOW_Y = 5;
bool Config::PRINT_ALL_PATHS = true;
bool Config::ASSIGN_PATH_COLOR_MANUAL = false;
string Config::COORDINATE_FILE_NAME = "";
string Config::FILTER_NAMES[]={"",""};
string Config::DATA_FILE_NAME = "";
list<short> Config::PATH_LIST;
list<string> Config::PATH_COLOR;
float Config::OVERLAP_X = 0.0;
float Config::OVERLAP_Y = 0.0;
float Config::PERIF_WEIGHT = 0.5;
float Config::CORE_BY_PREIF = 2.0;
float Config::CORE_POINT_WEIGHT = 1;
short Config::FILTER = 2;
short Config::TOTAL_CLUSTER_IDS = 2;
float Config::NEGLIGIBLE_WEIGHT = 0.00001;
short Config::MINIMUM_PATH_LENGTH = 3;
float Config::DELTA_CHANGE = 0.0;
bool Config::INCREASE_DAP = false;
bool Config::MATCH_SIGNATURE = true;
bool Config::PRINT_BARCODE = false;
bool Config::REFERENCE_PHENOTYPE = true;
short Config::REFERENCE_ENV_INDEX = -1;
short Config::EDGE_WIDTH = 12;
string Config::EDGE_COLOR = "#a4a4a4";
float Config::NODE_SIZE_MAX = 30.0;
float Config::NODE_SIZE_MIN = 15.0;
short Config::QUAD_TREE_CHILDREN = 2;
bool Config::HEATMAP_EACH_CC = false;
bool Config::PH_JAVA_PLEX = false;

void Config::setAttributes(){
    
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
                OVERLAP = stof(value);
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
        }else if(flag.compare("REFERENCE_PHENOTYPE")==0){
            try {
                if(value.compare("true")==0){
                    REFERENCE_PHENOTYPE = true;
                }else if(value.compare("false")==0){
                    REFERENCE_PHENOTYPE = false;
                }else{
                    REFERENCE_PHENOTYPE = true;
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
    
}
