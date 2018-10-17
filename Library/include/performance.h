
/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: performance.h
 Objective: Each point carries a phenotypic information, which is uniquely identified by <genotype,
            location,date/time>
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              02/21/2016      Initial version
 Md. Kamruzzaman              04/25/2018      Change the members of the class
 **************************************************************************************************/

#ifndef performance_h
#define performance_h

#include <string>
#include <list>
#include <vector>
#include "clusterInformation.h"

namespace hyppox {
    namespace mapper{
        template <typename RowIDType, typename ClusterIDType, typename Gentype, typename DatetimeType, typename ClusterType, typename FilterType>
        class Performance:public ClusterInformation<ClusterIDType>{
            public:
            //using PerfType = Performance<RowIDType,ClusterIDType,Gentype,DatetimeType,ClusterType,FilterType>;
            
            Performance(RowIDType _pid, Gentype _genotype, std::vector<std::string> _location, DatetimeType _datetime, std::vector<ClusterType> _clsValue, std::vector<FilterType> _filterValue, std::vector<std::string> _other, std::string _indv);
            ~Performance() = default;
            
            RowIDType getID();
            Gentype getGenotype();
            std::string getLocation();
            void getLocation(std::vector<std::string>& _loc);
            DatetimeType getDateTime();
            FilterType getFilter(short index);
            ClusterType getClusterValue(short index);
            std::string getOtherValues(short index);
            void getFilter(std::vector<FilterType>& _filter);
            void getClusterValue(std::vector<ClusterType>& _cluster);
            std::string getClusterValue();
            void getOtherValues(std::vector<std::string>& _other);
            size_t getTotalFilters();
            size_t getTotalClusterValues();
            size_t getTotalOtherValues();
            std::string getIndividualId();
            std::string getPoint();
            
            private:
            RowIDType pID;
            Gentype genotype;
            std::vector<std::string> location;
            DatetimeType datetime;
            std::vector<ClusterType> cluster_value;
            std::vector<FilterType> filter_value;
            std::vector<std::string> other;
            std::string indv;
        };
        
        template <typename RowIDType, typename ClusterIDType, typename Gentype, typename DatetimeType, typename ClusterType, typename FilterType>
        Performance<RowIDType,ClusterIDType,Gentype,DatetimeType,ClusterType,FilterType>::Performance(RowIDType _pid, Gentype _genotype, std::vector<std::string> _location, DatetimeType _datetime, std::vector<ClusterType> _clsValue, std::vector<FilterType> _filterValue, std::vector<std::string> _other, std::string _indv):
        pID(_pid),genotype(_genotype),datetime(_datetime),cluster_value(_clsValue),filter_value(_filterValue),other(_other),indv(_indv)
        {this->location.assign(_location.begin(), _location.end());}
        
        template <typename RowIDType, typename ClusterIDType, typename Gentype, typename DatetimeType, typename ClusterType, typename FilterType>
        RowIDType Performance<RowIDType,ClusterIDType,Gentype,DatetimeType,ClusterType,FilterType>::getID(){return this->pID;}
        
        template <typename RowIDType, typename ClusterIDType, typename Gentype, typename DatetimeType, typename ClusterType, typename FilterType>
        Gentype Performance<RowIDType,ClusterIDType,Gentype,DatetimeType,ClusterType,FilterType>::getGenotype(){return this->genotype;}
        
        template <typename RowIDType, typename ClusterIDType, typename Gentype, typename DatetimeType, typename ClusterType, typename FilterType>
        std::string Performance<RowIDType,ClusterIDType,Gentype,DatetimeType,ClusterType,FilterType>::getLocation(){
            std::string _loc="";
            for(auto l:this->location){
                if(_loc.length()>0) _loc += ", ";
                _loc += l;
            }
            
            return _loc;
        }
        
        template <typename RowIDType, typename ClusterIDType, typename Gentype, typename DatetimeType, typename ClusterType, typename FilterType>
        void Performance<RowIDType,ClusterIDType,Gentype,DatetimeType,ClusterType,FilterType>::getLocation(std::vector<std::string> &_loc){
            _loc.clear();
            _loc.assign(this->location.begin(), this->location.end());
        }
        
        template <typename RowIDType, typename ClusterIDType, typename Gentype, typename DatetimeType, typename ClusterType, typename FilterType>
        DatetimeType Performance<RowIDType,ClusterIDType,Gentype,DatetimeType,ClusterType,FilterType>::getDateTime(){return this->datetime;}
        
        template <typename RowIDType, typename ClusterIDType, typename Gentype, typename DatetimeType, typename ClusterType, typename FilterType>
        FilterType Performance<RowIDType,ClusterIDType,Gentype,DatetimeType,ClusterType,FilterType>::getFilter(short index){
            if(index<0||index>=this->filter_value.size()){
                return -999999;
            }
            
            return this->filter_value[index];
        }
        
        template <typename RowIDType, typename ClusterIDType, typename Gentype, typename DatetimeType, typename ClusterType, typename FilterType>
        ClusterType Performance<RowIDType,ClusterIDType,Gentype,DatetimeType,ClusterType,FilterType>::getClusterValue(short index){
            if(index<0||index>=this->cluster_value.size()){
                return -999999;
            }
            
            return this->cluster_value[index];
        }
        
        template <typename RowIDType, typename ClusterIDType, typename Gentype, typename DatetimeType, typename ClusterType, typename FilterType>
        std::string Performance<RowIDType,ClusterIDType,Gentype,DatetimeType,ClusterType,FilterType>::getClusterValue(){
            std::string s = "";
            
            for(auto _cv:this->cluster_value){
                if(s.length()>0) s += "#";
                s += to_string(_cv);
            }
            
            return s;
        }
        
        template <typename RowIDType, typename ClusterIDType, typename Gentype, typename DatetimeType, typename ClusterType, typename FilterType>
        std::string Performance<RowIDType,ClusterIDType,Gentype,DatetimeType,ClusterType,FilterType>::getOtherValues(short index){
            if(index<0||index>=this->other.size()){
                return "NAS"; // Not a valid string
            }
            
            return this->other[index];
        }
        
        template <typename RowIDType, typename ClusterIDType, typename Gentype, typename DatetimeType, typename ClusterType, typename FilterType>
        void Performance<RowIDType,ClusterIDType,Gentype,DatetimeType,ClusterType,FilterType>::getFilter(std::vector<FilterType>& _filter){
            _filter.clear();
            _filter.insert(_filter.begin(), this->filter_value.begin(), this->filter_value.end());
        }
        
        template <typename RowIDType, typename ClusterIDType, typename Gentype, typename DatetimeType, typename ClusterType, typename FilterType>
        void Performance<RowIDType,ClusterIDType,Gentype,DatetimeType,ClusterType,FilterType>::getClusterValue(std::vector<ClusterType>& _cluster){
            _cluster.clear();
            _cluster.insert(_cluster.begin(), this->cluster_value.begin(), this->cluster_value.end());
        }
        
        template <typename RowIDType, typename ClusterIDType, typename Gentype, typename DatetimeType, typename ClusterType, typename FilterType>
        void Performance<RowIDType,ClusterIDType,Gentype,DatetimeType,ClusterType,FilterType>::getOtherValues(std::vector<std::string>& _other){
            _other.clear();
            _other.insert(_other.begin(), this->other.begin(), this->other.end());
        }
        
        template <typename RowIDType, typename ClusterIDType, typename Gentype, typename DatetimeType, typename ClusterType, typename FilterType>
        size_t Performance<RowIDType,ClusterIDType,Gentype,DatetimeType,ClusterType,FilterType>::getTotalFilters(){return this->filter_value.size();}
        
        template <typename RowIDType, typename ClusterIDType, typename Gentype, typename DatetimeType, typename ClusterType, typename FilterType>
        size_t Performance<RowIDType,ClusterIDType,Gentype,DatetimeType,ClusterType,FilterType>::getTotalClusterValues(){return this->cluster_value.size();}
        
        template <typename RowIDType, typename ClusterIDType, typename Gentype, typename DatetimeType, typename ClusterType, typename FilterType>
        size_t Performance<RowIDType,ClusterIDType,Gentype,DatetimeType,ClusterType,FilterType>::getTotalOtherValues(){return this->other.size();}
        
        template <typename RowIDType, typename ClusterIDType, typename Gentype, typename DatetimeType, typename ClusterType, typename FilterType>
        std::string Performance<RowIDType,ClusterIDType,Gentype,DatetimeType,ClusterType,FilterType>::getIndividualId(){return this->indv;}
        
        template <typename RowIDType, typename ClusterIDType, typename Gentype, typename DatetimeType, typename ClusterType, typename FilterType>
        std::string Performance<RowIDType,ClusterIDType,Gentype,DatetimeType,ClusterType,FilterType>::getPoint(){return (this->genotype+"#"+this->getLocation()+"#"+this->datetime);}
    }
}

#endif /* Performance_h */
