
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
            
            Performance(RowIDType _pid, Gentype _genotype, std::vector<std::string> _location, std::vector<DatetimeType> _datetime, std::vector<ClusterType> _clsValue, std::vector<FilterType> _filterValue, std::vector<std::string> _other, std::string memShip, std::string pieChart):
            pID(_pid),genotype(_genotype),cluster_value(_clsValue),filter_value(_filterValue),other(_other),membership(memShip),pieChart(pieChart)
            {
                if(_location.size()>0)this->location.assign(_location.begin(), _location.end());
                if(_datetime.size()>0)this->datetime.assign(_datetime.begin(), _datetime.end());
            }
            
            ~Performance() = default;
            
            RowIDType getID(){return this->pID;}
            
            Gentype getGenotype(){return this->genotype;}
            
            std::string getLocation(){
                std::string _loc="";
                
                if(this->location.size()==0) return _loc;
                
                for(auto l:this->location){
                    if(_loc.length()>0) _loc += ", ";
                    _loc += l;
                }
                
                return _loc;
            }
            
            void getLocation(std::vector<std::string>& _loc){
                _loc.clear();
                _loc.assign(this->location.begin(), this->location.end());
            }
            
            std::string getDateTime(){
                std::string s = "";
                
                if(this->datetime.size()==0) return s;
                
                convert_to<std::string>(this->datetime[0], s);
                
                for(size_t i=1; i<this->datetime.size(); i++){
                    std::string _tmp;
                    convert_to<std::string>(this->datetime[i], _tmp);
                    s += ", " + _tmp;
                }
                
                return s;
            }
            
            FilterType getFilter(short index){
                if(index<0||(size_t)index>=this->filter_value.size()){
                    return -999999;
                }
                
                return this->filter_value[index];
            }
            
            void getFilter(std::vector<FilterType>& _filter){
                _filter.clear();
                _filter.insert(_filter.begin(), this->filter_value.begin(), this->filter_value.end());
            }
            
            ClusterType getClusterValue(short index){
                if(index<0||(size_t)index>=this->cluster_value.size()){
                    return -999999;
                }
                
                return this->cluster_value[index];
            }
            
            void getClusterValue(std::vector<ClusterType>& _cluster){
                _cluster.clear();
                _cluster.insert(_cluster.begin(), this->cluster_value.begin(), this->cluster_value.end());
            }
            
            std::string getClusterValue(){
                std::string s = "";
                
                for(auto _cv:this->cluster_value){
                    if(s.length()>0) s += "#";
                    s += to_string(_cv);
                }
                
                return s;
            }
            
            std::string getOtherValues(short index){
                std::string s = "";
                
                for(auto _cv:this->cluster_value){
                    if(s.length()>0) s += "#";
                    s += to_string(_cv);
                }
                
                return s;
            }
            
            void getOtherValues(std::vector<std::string>& _other){
                _other.clear();
                _other.insert(_other.begin(), this->other.begin(), this->other.end());
            }
            
            size_t getTotalFilters(){return this->filter_value.size();}
            
            size_t getTotalClusterValues(){return this->cluster_value.size();}
            
            size_t getTotalOtherValues(){return this->other.size();}
            
            std::string getMembership(){return this->membership;}
            
            std::string getPieChart(){return this->pieChart;}
            
            std::string getPoint(){return (this->genotype+"#"+this->getLocation()+"#"+this->getDateTime());}
            
            private:
            RowIDType pID;
            Gentype genotype;
            std::vector<std::string> location;
            std::vector<DatetimeType> datetime;
            std::vector<ClusterType> cluster_value;
            std::vector<FilterType> filter_value;
            std::vector<std::string> other;
            std::string membership;
            std::string pieChart;
        };
    }
}

#endif /* Performance_h */
