
/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: clusterInformation.h
 Objective: Class holds clustering information of an individual
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              09/20/2016      Initial version
 Md. Kamruzzaman              01/04/2017      Adjust code with modified config file
 **************************************************************************************************/

#ifndef clusterInformation_h
#define clusterInformation_h

#include <string>
#include <stdlib.h>
#include <vector>
#include "config.h"

namespace hyppox {
    namespace mapper{
        template <typename ClusterIDType>
        //using CType = ClusterInformation<ClusterIDType>;
        class ClusterInformation{
            public:
            
            ClusterInformation();
            ~ClusterInformation() = default;
            
            void AddClusterID(ClusterIDType _id);
            std::string GetClusterIdList();
            void SetStatus(bool status);
            bool GetStatus();
            void GetIDList(std::vector<ClusterIDType> &idList);
            void GetTypeList(std::vector<std::string> &typeList);
            bool HasClusterID(ClusterIDType cID);
            short GetTotalIdCount();
            void SetType(std::string s);
            std::string GetType();
            std::string GetType(short index1, short index2);
            ClusterIDType GetUniqueID();
            void SetUniqueId(ClusterIDType oid);
            void UpdateClusterID(short index, ClusterIDType _id);
            void UpdateType(short index, std::string ch);
            void DecreaseClusterIndex(short index);
            float GetWeight();
            float GetWeight(short index1, short index2);
            float GetWeight(ClusterIDType clusterID);
            void ResetClusterInformation();
            
            private:
            std::vector<ClusterIDType> clusterID;
            bool status;
            std::vector<std::string> type;
            ClusterIDType uniqueID;
            
        };
        
        template <typename ClusterIDType>
        ClusterInformation<ClusterIDType>::ClusterInformation():status(true){
            this->clusterID.clear();
            this->type.clear();
        }
        
        template <typename ClusterIDType>
        ClusterIDType ClusterInformation<ClusterIDType>::GetUniqueID(){return this->uniqueID;}
        
        template <typename ClusterIDType>
        void ClusterInformation<ClusterIDType>::SetUniqueId(ClusterIDType oid){this->uniqueID = oid;}
        
        template <typename ClusterIDType>
        void ClusterInformation<ClusterIDType>::SetStatus(bool status){this->status = status;}
        
        template <typename ClusterIDType>
        bool ClusterInformation<ClusterIDType>::GetStatus(){return this->status;}
        
        template <typename ClusterIDType>
        void ClusterInformation<ClusterIDType>::SetType(std::string s){
            if(this->type.size()+1 == this->clusterID.size()){
                this->type.push_back(s);
            }
        }
        
        template <typename ClusterIDType>
        std::string ClusterInformation<ClusterIDType>::GetType(){
            std::string idList = "";
            
            for(short i=0; i<=this->type.size(); i++){
                if(idList.length() > 0){
                    idList += ",";
                }
                
                idList += this->type[i];
            }
            
            return idList;
        }
        
        template <typename ClusterIDType>
        std::string ClusterInformation<ClusterIDType>::GetType(short index1, short index2){
            std::string idList = "";
            
            if(index1>=0 && index2>=0 && index1 < this->type.size() && index2 < this->type.size()){
                idList = this->type[index1] + "," + this->type[index2];
            }
            
            return idList;
        }
        
        template <typename ClusterIDType>
        void ClusterInformation<ClusterIDType>::AddClusterID(ClusterIDType _id){
            this->clusterID.push_back(_id);
            this->status = true;
        }
        
        template <typename ClusterIDType>
        void ClusterInformation<ClusterIDType>::DecreaseClusterIndex(short index){
            if(index>=0 && (size_t)index<this->clusterID.size()){
                this->clusterID.erase(this->clusterID.begin()+index);
                this->type.erase(this->type.begin()+index);
            }
        }
        
        template <typename ClusterIDType>
        void ClusterInformation<ClusterIDType>::UpdateClusterID(short index, ClusterIDType _id){
            if(index>=0 && (size_t)index<this->clusterID.size()){
                this->clusterID[index] = _id;
            }
        }
        
        template <typename ClusterIDType>
        void ClusterInformation<ClusterIDType>::UpdateType(short index, std::string ch){
            if(index>=0 && index<this->type.size()){
                this->type[index] = ch;
            }
        }
        
        template <typename ClusterIDType>
        std::string ClusterInformation<ClusterIDType>::GetClusterIdList(){
            std::string idList = "";
            
            for(short i=0; i<this->clusterID.size(); i++){
                if(this->clusterID[i]>0){
                    if(idList.length() > 0){
                        idList += ",";
                    }
                    
                    idList += to_string(this->clusterID[i]);
                }
            }
            
            return idList;
        }
        
        template <typename ClusterIDType>
        void ClusterInformation<ClusterIDType>::GetIDList(std::vector<ClusterIDType> &idList){
            idList.clear();
            idList.insert(idList.begin(), this->clusterID.begin(), this->clusterID.end());
        }
        
        template <typename ClusterIDType>
        void ClusterInformation<ClusterIDType>::GetTypeList(std::vector<std::string> &typeList){
            typeList.clear();
            typeList.insert(typeList.begin(), this->type.begin(), this->type.end());
        }
        
        template <typename ClusterIDType>
        short ClusterInformation<ClusterIDType>::GetTotalIdCount(){return this->clusterID.size();}
        
        template <typename ClusterIDType>
        bool ClusterInformation<ClusterIDType>::HasClusterID(ClusterIDType cID){
            
            for(short i=0; i<=this->clusterID.size(); i++){
                
                if(this->clusterID[i] == cID){
                    return true;
                }
            }
            
            return false;
        }
        
        template <typename ClusterIDType>
        float ClusterInformation<ClusterIDType>::GetWeight(){
            short c=0,p=0;
            
            for(short i=0; i<this->type.size(); i++){
                
                if(this->type[i].compare("C") == 0){
                    c++;
                }else if(this->type[i].compare("P") == 0){
                    p++;
                }
            }
            
            return (c*hyppox::Config::CORE_POINT_WEIGHT) + (p*hyppox::Config::PERIF_WEIGHT);
            
        }
        
        template <typename ClusterIDType>
        float ClusterInformation<ClusterIDType>::GetWeight(ClusterIDType clusterID){
            
            short index = -1;
            
            for(short i=0; i<this->clusterID.size(); i++){
                if(this->clusterID[i] == clusterID){
                    index = i;
                    break;
                }
            }
            
            if(index >= 0 && index <this->type.size()){
                if(this->type[index].compare("C") == 0){
                    return (hyppox::Config::CORE_POINT_WEIGHT);
                }else if(this->type[index].compare("P") == 0){
                    return (hyppox::Config::PERIF_WEIGHT);
                }
            }
            
            return -1;
        }
        
        template <typename ClusterIDType>
        float ClusterInformation<ClusterIDType>::GetWeight(short index1, short index2){
            short c=0,p=0;
            
            if(index1 >= 0 && index1 < this->type.size()){
                if(this->type[index1].compare("C") == 0){
                    c++;
                }else if(this->type[index1].compare("P") == 0){
                    p++;
                }
            }
            
            if(index2 >= 0 && index2 < this->type.size()){
                if(this->type[index2].compare("C") == 0){
                    c++;
                }else if(this->type[index2].compare("P") == 0){
                    p++;
                }
            }
            
            return (c*hyppox::Config::CORE_POINT_WEIGHT) + (p*hyppox::Config::PERIF_WEIGHT);
        }
        
        template <typename ClusterIDType>
        void ClusterInformation<ClusterIDType>::ResetClusterInformation(){
            this->status = false;
            this->type.clear();
            this->clusterID.clear();
        }
    }
}


#endif /* ClusterInformation_h */
