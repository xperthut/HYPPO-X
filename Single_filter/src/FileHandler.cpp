
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: FileHandler.cpp
 Objective: Implementation of member functions
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              02/21/2016      Initial version
 
 **************************************************************************************************/

#include "FileHandler.h"

FileHandler::FileHandler(string fileName){
    this->fileName = fileName;
    this->currentDateTime = GetCurrentDateTime();
}

FileHandler::~FileHandler(){}

//#pragma Public_Method
void FileHandler::readNodePosition(string _fileName, unordered_map<long, float*> *positionMap){
    
    try {
        ifstream fileReader(_fileName);
        
        if(fileReader.is_open()){
            
            string line;
            long nodeID;
            
            // Read the header
            getline(fileReader, line);
            
            while (getline(fileReader, line)) {
                size_t pos = 0;
                float *position = new float[2];
                
                // Store data
                pos = line.find(",");
                nodeID = stol(line.substr(0,pos));
                line=line.substr(pos+1,line.length()-pos-1);
                
                pos = line.find(",");
                position[0] = stof(line.substr(0,pos));
                line=line.substr(pos+1,line.length()-pos-1);
                
                pos = line.find("\r");
                
                if(pos==string::npos){
                    pos=line.length();
                }
                position[1] = 777-stof(line.substr(0,pos));
                
                if(positionMap->find(nodeID) == positionMap->end()){
                    positionMap->insert(pair<long, float*>(nodeID, position));
                }
            }
            
            fileReader.close();
            
        }else{
            cout<<"\nError to read file. Please check the constant [COORDINATE_FILE_NAME] contained file name and path in config.h\n"<<endl;
            exit(0);
        }
        
    } catch (exception &e) {
        cout<<"\nError to read file: "<<e.what()<<endl;
        exit(0);
    }

}

void FileHandler::ReadFileData(list<DataPoint*> *dataPointList){
    string line;
    //DataPoint *dataPoint = NULL;
    int lineCounter=0;
    string compLast="",compThis, key;
    unordered_map<float, list<Phenotype*> > pointMap;
    unordered_map<float, list<Phenotype*> >::iterator pItr;
    
    try {
        ifstream fileReader(this->fileName);
        
        if(fileReader.is_open()){
            
            //string lastX = "", lastY = "";
            string genotype,dap="-1",site,date="",time="",dateTime="",variety="0",value="0",rate;
            string env[ENV_ATTR],dist;
            
            // Read the header
            getline(fileReader, line);
            
            while (getline(fileReader, line)) {
                lineCounter++;
                
                size_t pos = 0;
                
                // Store data
                pos = line.find(",");
                dap = line.substr(0,pos);
                line=line.substr(pos+1,line.length()-pos-1);
                
                pos = line.find(",");
                genotype = line.substr(0,pos);
                line=line.substr(pos+1,line.length()-pos-1);
                
                pos = line.find(",");
                site = line.substr(0,pos);
                line=line.substr(pos+1,line.length()-pos-1);
                
                pos = line.find(",");
                variety = line.substr(0,pos);
                line=line.substr(pos+1,line.length()-pos-1);
                
                pos = line.find(",");
                date = line.substr(0,pos);
                line=line.substr(pos+1,line.length()-pos-1);
                
                pos = line.find(",");
                value = line.substr(0,pos);
                line=line.substr(pos+1,line.length()-pos-1);
                
                pos = line.find(",");
                env[0] = line.substr(0,pos);
                line=line.substr(pos+1,line.length()-pos-1);
                
                pos = line.find(",");
                env[1] = line.substr(0,pos);
                line=line.substr(pos+1,line.length()-pos-1);
                
                pos=line.length();
                pos = line.find("\r");
                env[2] = line.substr(0,pos);
                
                // Buld the comparison string
                switch (FILTER_1) {
                    case 0:
                        key = dap;
                        break;
                    case 1:
                        key = env[0];
                        break;
                    case 2:
                        key = env[1];
                        break;
                    case 3:
                        key = env[2];
                        break;
                    default:
                        key = dap;
                        break;
                }
                
                
                Phenotype *phType = NULL;
                if(variety.find("#")==string::npos){
                    phType = new Phenotype(stoi(variety), genotype, site, stof(value));
                }else{
                    phType = new Phenotype(variety, genotype, site, stof(value));
                }
                phType->setCoOrdinate(stof(key), 0);
                phType->setDateTime(dateTime);
                phType->setDAP(stoi(dap));
                phType->setDate(date);
                phType->setTime(time);
                
                for(int kk=0; kk<ENV_ATTR; kk++){
                    phType->setEnvValue(kk, stof(env[kk]));
                }
                
                if((pItr=pointMap.find(stof(key)))==pointMap.end()){
                    list<Phenotype*> phList;
                    phList.push_back(phType);
                    
                    pointMap.insert(pair<float, list<Phenotype*> >(stof(key), phList));
                }else{
                    pItr->second.push_back(phType);
                }
                
            }
            
            for(pItr = pointMap.begin(); pItr!=pointMap.end(); pItr++){
                
                float posA[] = {pItr->first};
                
                DataPoint *dataPoint = new DataPoint(posA);
                dataPoint->clearList();
                dataPoint->addPhenotypeList(pItr->second);
                
                dataPointList->push_back(dataPoint);
                
            }
            
            fileReader.close();
            
        }else{
            cout<<"\nError to read file. Please check the constant [DATA_FILE_NAME] contained file name and path in config.h\n"<<endl;
            exit(0);
        }
        
    }catch (runtime_error &e) {
        cout<<"\nError to read file: "<<e.what()<<endl;
        exit(0);
    }catch (exception &e) {
        cout<<"\nError to read file: "<<e.what()<<endl;
        exit(0);
    }catch(...){
        cout<<"\nError to read file: "<<endl;
        exit(0);
    }
}

string FileHandler::WriteDataToFile(string fileName, string extention, string data, bool addTimeStemp){
    
    /* Add Time stemp */
    if(addTimeStemp){
        
        fileName += "_" + this->currentDateTime;
    }
    
    /* Check file extention */
    if(extention.find(".") == string::npos){
        extention = "." + extention;
    }
    
    /* Generate full file name */
    fileName += extention;
    
    /* Start writing to file */
    try {
        ofstream output(fileName, ios::out);
        
        output<<data<<endl;
        
        output.close();
        
    } catch (exception& e) {
        cout<<"Error: "<<e.what()<<endl;
        return "";
    }
    
    return realpath(fileName.c_str(), NULL);
}

//#pragma Private methods

/****
 Get current date/time, format is YYYY-MM-DD_HH:mm:ss
 Copied from: http://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
 ***/
const string FileHandler::GetCurrentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    
    strftime(buf, sizeof(buf), "%Y-%m-%d_%X", &tstruct);
    
    return buf;
}

