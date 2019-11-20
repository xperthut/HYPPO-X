
/**************************************************************************************************
 Copyright © 2016-2019 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: fileHandler.h
 Objective: Class to handle file management like read, write, etc
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              02/21/2016      Initial version

 **************************************************************************************************/


#ifndef fileHandler_h
#define fileHandler_h

#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <typeinfo>
#include <stdlib.h>
#include "quadTree.h"

#if defined(_WIN32)

#include <windows.h>
#include <tchar.h>

#define BUFSIZE 4096
#endif

namespace hyppox {
    namespace mapper{
        template<typename PerfType, typename PosType, typename NodePosType, typename RowIDType, typename ClusterIDType, typename GenType, typename DatetimeType, typename ClusterType, typename FilterType>
        class FileHandler{
            public:
            using DPType = DataPoint<PerfType*,PosType>;
            using QTType = quadtree::QuadTree<PerfType*,PosType,NodePosType>;
            //using FHType = FileHandler<PerfType,PosType,NodePosType,RowIDType,ClusterIDType,GenType,DatetimeType,ClusterType,FilterType>;

            FileHandler(std::string fileName);
            ~FileHandler() = default;
            int ReadFileData(QTType* tree, std::vector<NodePosType> &_minPos, std::vector<NodePosType> &_maxPos, std::unordered_map<std::string, size_t>& mapMap, std::unordered_map<std::string, size_t>& pieMap);
            void readNodePosition(std::string fileName, std::unordered_map<ClusterIDType, float*> *positionMap);
            std::string WriteDataToFile(std::string fileName, std::string extention, std::string data, bool addTimeStemp);

            private:
            std::string fileName;
            std::string currentDateTime;
            std::string getFileWithPath(std::string fileName);
        };

        template<typename PerfType, typename PosType, typename NodePosType, typename RowIDType, typename ClusterIDType, typename GenType, typename DatetimeType, typename ClusterType, typename FilterType>
        FileHandler<PerfType,PosType,NodePosType,RowIDType,ClusterIDType,GenType,DatetimeType,ClusterType,FilterType>::FileHandler(std::string fileName):fileName(fileName), currentDateTime(GetCurrentDateTime()){}

        //#pragma Public_Method
        template<typename PerfType, typename PosType, typename NodePosType, typename RowIDType, typename ClusterIDType, typename GenType, typename DatetimeType, typename ClusterType, typename FilterType>
        void FileHandler<PerfType,PosType,NodePosType,RowIDType,ClusterIDType,GenType,DatetimeType,ClusterType,FilterType>::readNodePosition(std::string _fileName, std::unordered_map<ClusterIDType, float*> *positionMap){

            try {
                std::ifstream fileReader(_fileName);

                if(fileReader.is_open()){

                    std::string line;
                    ClusterIDType nodeID;

                    // Read the header
                    getline(fileReader, line);

                    while (getline(fileReader, line)) {
                        size_t pos = 0;
                        float position[]={0,0};

                        // Store data
                        pos = line.find(",");
                        nodeID = stol(line.substr(0,pos));
                        line=line.substr(pos+1,line.length()-pos-1);

                        pos = line.find(",");
                        position[0] = stof(line.substr(0,pos));
                        line=line.substr(pos+1,line.length()-pos-1);

                        pos = line.find("\r");

                        if(pos==std::string::npos){
                            pos=line.length();
                        }
                        position[1] = 777-stof(line.substr(0,pos));

                        if(positionMap->find(nodeID) == positionMap->end()){
                            positionMap->insert(std::make_pair(nodeID, position));
                        }
                    }

                    fileReader.close();

                }else{
                    std::cout<<"\nError to read file. Please check the constant [COORDINATE_FILE_NAME] contained file name and path in config.h\n"<<std::endl;
                    exit(0);
                }

            } catch (std::exception &e) {
                std::cout<<"Error to read file: "<<e.what()<<std::endl;
            }

        }

        template<typename PerfType, typename PosType, typename NodePosType, typename RowIDType, typename ClusterIDType, typename GenType, typename DatetimeType, typename ClusterType, typename FilterType>
        int FileHandler<PerfType,PosType,NodePosType,RowIDType,ClusterIDType,GenType,DatetimeType,ClusterType,FilterType>::ReadFileData(QTType* tree, std::vector<NodePosType> &_minPos, std::vector<NodePosType> &_maxPos, std::unordered_map<std::string, size_t>& memMap, std::unordered_map<std::string, size_t>& pieMap){

            std::string line;
            size_t lineCounter=0;
            std::string compLast="",compThis, key;
            std::unordered_map<std::string, std::list<PerfType*> > pointMap;
            bool first = true;
            memMap.clear();
            pieMap.clear();
            //size_t gen_f = 0;

            try {
                std::ifstream fileReader(hyppox::Config::READ_DIR+this->fileName);

                std::cout<<"File name with path: "<<hyppox::Config::READ_DIR+this->fileName<<std::endl;

                if(fileReader.is_open()){

                    GenType genotype;
                    std::vector<DatetimeType> datetime;
                    std::vector<FilterType> _filter;
                    std::vector<ClusterType> _clsValue;
                    std::vector<std::string> _location, _otherInfo;
                    RowIDType pID=0;
                    int col=0;
                    std::string memKey="", pieKey="";

                    // Read the header
                    getline(fileReader, line);

                    std::string s="";
                    bool start = false;
                    for(char ch:line){
                        if(ch=='\n'||ch=='\r'){
                            break;
                        }else if(ch=='"'){
                            if(start) start=false;
                            else start = true;
                        }else if(ch==','){
                            if(start) s+= ch;
                            else{
                                col++;

                                for(int fc:hyppox::Config::COL_FILTER){
                                    if(fc==col){
                                        hyppox::Config::FILTER_NAMES.push_back(s);
                                        break;
                                    }
                                }

                                for(int fc:hyppox::Config::COL_CLUSTER){
                                    if(fc==col){
                                        hyppox::Config::CLUSTER_NAMES.push_back(s);
                                        break;
                                    }
                                }

                                hyppox::Config::HEADER_NAMES.push_back(s);
                                s="";
                            }
                        }else{
                            s += ch;
                        }
                    }

                    if(s.length()>0){
                        col++;

                        for(int fc:hyppox::Config::COL_FILTER){
                            if(fc==col){
                                hyppox::Config::FILTER_NAMES.push_back(s);
                                break;
                            }
                        }

                        for(int fc:hyppox::Config::COL_CLUSTER){
                            if(fc==col){
                                hyppox::Config::CLUSTER_NAMES.push_back(s);
                                break;
                            }
                        }

                        hyppox::Config::HEADER_NAMES.push_back(s);
                        s="";
                    }

                    size_t memIndex = 0, pieIndex = 0;

                    while (getline(fileReader, line)) {
                        s = "";
                        col=0;
                        _filter.clear();
                        _otherInfo.clear();
                        _clsValue.clear();
                        memKey="";
                        pieKey="";
                        _location.clear();
                        datetime.clear();

                        lineCounter++;

                        bool start = false;
                        bool validRec = true;
                        size_t chPos = 0;
                        for(char ch:line){
                            chPos++;
                            if(ch=='\n'||ch=='\r'){
                                break;
                            }else if(ch=='"'){
                                if(start) start=false;
                                else start = true;
                            }else if(ch==','){
                                if(start) s+= ch;
                                else{
                                    col++;

                                    if(col==1){
                                        if(!convert_to<RowIDType>(s, pID)){
                                            validRec = false;
                                            break;
                                        }
                                        s="";
                                        continue;
                                    }else if(col==hyppox::Config::COL_GENOTYPE){
                                        if(hyppox::Config::FILTER_GENOTYPE.size()>0){
                                            bool _m = false;
                                            for(std::string _tmpS:hyppox::Config::FILTER_GENOTYPE){
                                                if(_tmpS.compare(s)==0){
                                                    _m = convert_to<GenType>(s, genotype);
                                                }
                                            }
                                            if(!_m){
                                                validRec = false;
                                                break;
                                            }
                                        }else{
                                            if(!convert_to<GenType>(s, genotype)){
                                                validRec = false;
                                                break;
                                            }
                                        }

                                    }

                                    for(int fc:hyppox::Config::COL_DATETIME){
                                        if(fc==col){
                                            DatetimeType _dt;
                                            if(!convert_to<DatetimeType>(s, _dt)){
                                                validRec = false;
                                                break;
                                            }
                                            datetime.push_back(_dt);
                                            break;
                                        }
                                    }

                                    for(int fc:hyppox::Config::COL_MEMBERSHIP){
                                        if(fc==col && s.length() > 0){
                                            if(memKey.length()>0) memKey += "#";
                                            memKey += s;

                                            break;
                                        }
                                    }

                                    for(int fc:hyppox::Config::COL_PIECHART){
                                        if(fc==col && s.length() > 0){
                                            if(pieKey.length()>0) pieKey += "#";
                                            pieKey += s;

                                            break;
                                        }
                                    }

                                    for(int fc:hyppox::Config::COL_LOCATION){
                                        if(fc==col){
                                            _location.push_back(s);

                                            break;
                                        }
                                    }

                                    for(int fc:hyppox::Config::COL_OTHER){
                                        if(fc==col){
                                            _otherInfo.push_back(s);

                                            break;
                                        }
                                    }

                                    for(int fc:hyppox::Config::COL_FILTER){
                                        if(fc==col){
                                            FilterType _ft;
                                            if(!convert_to<FilterType>(s, _ft)){
                                                validRec = false;
                                                break;
                                            }
                                            _filter.push_back(_ft);

                                            break;
                                        }
                                    }

                                    for(int fc:hyppox::Config::COL_CLUSTER){
                                        if(fc==col){
                                            ClusterType _ct;
                                            
                                            if(!convert_to<ClusterType>(s, _ct)){
                                                _ct = (ClusterType)INT_MAX;
                                            }
                                            
                                            _clsValue.push_back(_ct);

                                            break;
                                        }
                                    }

                                    s="";
                                }
                            }else{
                                s += ch;
                            }
                        }

                        if(!validRec) continue;

                        col++;

                        if(col==1){
                            if(!convert_to<RowIDType>(s, pID)) continue;
                        }else if(col==hyppox::Config::COL_GENOTYPE){
                            if(hyppox::Config::FILTER_GENOTYPE.size()>0){
                                bool _m = false;
                                for(std::string _tmpS:hyppox::Config::FILTER_GENOTYPE){
                                    if(_tmpS.compare(s)==0){
                                        _m = convert_to<GenType>(s, genotype);
                                    }
                                }
                                if(!_m) continue;
                            }else{
                                if(!convert_to<GenType>(s, genotype)) continue;
                            }
                        }

                        for(int fc:hyppox::Config::COL_DATETIME){
                            if(fc==col){
                                DatetimeType _dt;
                                if(!convert_to<DatetimeType>(s, _dt)) continue;
                                datetime.push_back(_dt);
                                break;
                            }
                        }

                        for(int fc:hyppox::Config::COL_MEMBERSHIP){
                            if(fc==col){
                                if(memKey.length()>0) memKey += "#";
                                memKey += s;

                                break;
                            }
                        }

                        for(int fc:hyppox::Config::COL_PIECHART){
                            if(fc==col){
                                if(pieKey.length()>0) pieKey += "#";
                                pieKey += s;

                                break;
                            }
                        }

                        for(int fc:hyppox::Config::COL_LOCATION){
                            if(fc==col){
                                _location.push_back(s);

                                break;
                            }
                        }

                        for(int fc:hyppox::Config::COL_OTHER){
                            if(fc==col){
                                _otherInfo.push_back(s);

                                break;
                            }
                        }

                        for(int fc:hyppox::Config::COL_FILTER){
                            if(fc==col){
                                FilterType _ft;
                                if(!convert_to<FilterType>(s, _ft))continue;
                                _filter.push_back(_ft);

                                break;
                            }
                        }

                        for(int fc:hyppox::Config::COL_CLUSTER){
                            if(fc==col){
                                ClusterType _ct;
                                if(!convert_to<ClusterType>(s, _ct))continue;
                                _clsValue.push_back(_ct);

                                break;
                            }
                        }

                        // Create member index map
                        if(memKey.length()>0 && memMap.find(memKey)==memMap.end()){
                            memIndex++;
                            memMap.insert(make_pair(memKey, memIndex));
                        }

                        // Create pie index map
                        if(pieKey.length()>0 && pieMap.find(pieKey)==pieMap.end()){
                            pieIndex++;
                            pieMap.insert(make_pair(pieKey, pieIndex));
                        }

                        // Buld the comparison string
                        key = "";
                        for(FilterType _f:_filter){
                            if(key.length()>0) key += "#";
                            key += std::to_string(_f);
                        }

                        PerfType *phType = new PerfType(pID, genotype, _location, datetime, _clsValue, _filter, _otherInfo, memKey, pieKey);
                        if(phType==nullptr) return -1;

                        auto pItr=pointMap.find(key);
                        if(pItr==pointMap.end()){
                            std::list<PerfType*> phList;
                            phList.push_back(phType);

                            pointMap.insert(std::make_pair(key, phList));
                        }else{
                            pItr->second.push_back(phType);
                        }

                        if(first){
                            if(_filter.size()<(size_t)hyppox::Config::FILTER){
                                std::cout<<"\nError to read file. Filter column is out of dataset boundary.\n"<<std::endl;
                                return -2;
                            }
                            for(int i=0; i<hyppox::Config::FILTER; i++){
                                _minPos[i] = _filter[i];
                                _maxPos[i] = _filter[i];
                            }

                            first = false;
                        }

                        for(int i=0; i<hyppox::Config::FILTER; i++){
                            if(_minPos[i] > _filter[i]) _minPos[i] = _filter[i];
                            if(_maxPos[i] < _filter[i]) _maxPos[i] = _filter[i];
                        }
                        //gen_f++;
                    }

                    // Increase search reason 1% in both sides along a dimension to encapsulate terminal points
                    std::vector<FilterType> _len(hyppox::Config::FILTER, 0);
                    for(short i=0; i<hyppox::Config::FILTER; i++){
                        NodePosType c = ((_maxPos[i] - _minPos[i])*0.01)/2;
                        _minPos[i] -= c;
                        _maxPos[i] += c;
                        _len[i] = _maxPos[i]-_minPos[i];
                    }

                    if(tree==nullptr){
                        tree = new QTType(_minPos, _len);
                    }else{
                        tree->setRoot(_minPos, _len);
                    }

                    std::vector<PosType> posA;

                    for(auto pItr = pointMap.begin(); pItr!=pointMap.end(); pItr++){
                        key = pItr->first;
                        posA.clear();

                        if(key.find("#")==std::string::npos){
                            PosType _pt;
                            convert_to<PosType>(key, _pt);
                            posA.push_back(_pt);
                        }else{
                            while(key.find("#")!=std::string::npos){
                                size_t pos = key.find("#");
                                std::string a = key.substr(0,pos);
                                key=key.substr(pos+1,key.length()-pos-1);

                                posA.push_back(stof(a));
                            }
                            std::string b = key.substr(0,key.length());
                            PosType _pt;
                            convert_to<PosType>(b, _pt);
                            posA.push_back(_pt);
                        }

                        DPType *dataPoint = new DPType(posA);
                        dataPoint->clearList();
                        dataPoint->addPerformanceList(&pItr->second);

                        tree->AddDataToNode(dataPoint);

                    }
                    //cout<<gen_f<<endl;
                    fileReader.close();

                }else{
                    std::cout<<"\nError to read file. Please check the file name and path.\n"<<std::endl;
                    return -2;
                }

            }catch (std::runtime_error &e) {
                std::cout<<"\nError to read data file (Runtime error): "<<e.what()<<std::endl;
                return -1;
            }catch (std::exception &e) {
                std::cout<<"\nError to read data file: "<<e.what()<<std::endl;
                return -1;
            }catch(...){
                std::cout<<"\nError to read file"<<std::endl;
                return -1;
            }

            return 0;
        }

        template<typename PerfType, typename PosType, typename NodePosType, typename RowIDType, typename ClusterIDType, typename GenType, typename DatetimeType, typename ClusterType, typename FilterType>
        std::string FileHandler<PerfType,PosType,NodePosType,RowIDType,ClusterIDType,GenType,DatetimeType,ClusterType,FilterType>::WriteDataToFile(std::string fileName, std::string extention, std::string data, bool addTimeStemp){

            /* Add Time stemp */
            if(addTimeStemp){

                fileName += "_" + this->currentDateTime;
            }

            /* Check file extention */
            if(extention.find(".") == std::string::npos){
                extention = "." + extention;
            }

            /* Generate full file name */
            fileName += extention;

            /* Start writing to file */
            try {
                std::ofstream output(fileName, std::ios::out);

                output<<data<<std::endl;

                output.close();

            } catch (std::exception& e) {
                std::cout<<"Error: "<<e.what()<<std::endl;
                return "Can not write the file. Please check file location and the permission.";
            }

            //std::cout<<"Saving file: "<<fileName<<std::endl;

            return fileName;
        }

        template<typename PerfType, typename PosType, typename NodePosType, typename RowIDType, typename ClusterIDType, typename GenType, typename DatetimeType, typename ClusterType, typename FilterType>
        std::string FileHandler<PerfType,PosType,NodePosType,RowIDType,ClusterIDType,GenType,DatetimeType,ClusterType,FilterType>::getFileWithPath(std::string fileName){
#if defined(_WIN32)
            TCHAR full_path[BUFSIZE];
            if (GetFullPathName(_T(fileName.c_str()), BUFSIZE, full_path, nullptr)) {
                std::string filePath = full_path;
                return filePath;
            }
#else
            return realpath(fileName.c_str(), nullptr);
#endif

            return fileName;
        }
    }
}

#endif /* FileHandler_h */
