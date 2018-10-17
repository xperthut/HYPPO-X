
/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: dataPoint.h
 Objective: Holds a data point informaiton with its performance
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              02/21/2016      Initial version
 Md. Kamruzzaman              05/25/2018      Converting the class to template class
 **************************************************************************************************/


#ifndef dataPoint_h
#define dataPoint_h

#include <math.h>
#include <map>
#include <set>
#include <list>
#include <vector>
#include "performance.h"

namespace hyppox {
    namespace mapper{
        template<typename PerfType, typename PosType>
        class DataPoint{
            public:
            //using DPType = DataPoint<PerfType,PosType>;
            
            DataPoint(std::vector<PosType> &pos);
            ~DataPoint() = default;
            
            void addPerformanceList(std::list<PerfType>* phList);
            void addPerformanceList(PerfType& ph);
            PosType getPosition(short index);
            
            void setPosition(PosType value, short index);
            std::list<PerfType>& getPhList();
            void clearList();
            void setBoxId(size_t _id);
            size_t getBoxId();
            
            float GetAveragePerformanceData();
            
            bool ComparePoint(DataPoint<PerfType, PosType>& dp);
            float GetTotalPerfValue();
            
            private:
            std::vector<PosType> position;
            size_t boxID;
            std::list<PerfType> performanceList;
        };
        
        template<typename PerfType, typename PosType>
        DataPoint<PerfType,PosType>::DataPoint(std::vector<PosType> &pos):boxID(0){
            this->position.assign(pos.begin(), pos.end());
        }
        
        template<typename PerfType, typename PosType>
        void DataPoint<PerfType,PosType>::addPerformanceList(std::list<PerfType>* phList){
            this->performanceList.insert(this->performanceList.end(), phList->begin(), phList->end());
        }
        
        template<typename PerfType, typename PosType>
        void DataPoint<PerfType,PosType>::addPerformanceList(PerfType& ph){
            this->performanceList.push_back(ph);
        }
        
        template<typename PerfType, typename PosType>
        PosType DataPoint<PerfType,PosType>::getPosition(short index){return this->position[index];}
        
        template<typename PerfType, typename PosType>
        void DataPoint<PerfType,PosType>::setPosition(PosType value, short index){this->position[index]=value;}
        
        template<typename PerfType, typename PosType>
        std::list<PerfType>& DataPoint<PerfType,PosType>::getPhList(){return this->performanceList;}
        
        template<typename PerfType, typename PosType>
        void DataPoint<PerfType,PosType>::setBoxId(size_t _id){this->boxID = _id;}
        
        template<typename PerfType, typename PosType>
        size_t DataPoint<PerfType,PosType>::getBoxId(){return this->boxID;}
        
        template<typename PerfType, typename PosType>
        void DataPoint<PerfType,PosType>::clearList(){this->performanceList.clear();}
        
        template<typename PerfType, typename PosType>
        float DataPoint<PerfType,PosType>::GetAveragePerformanceData(){
            float avgValue = 0.0;
            size_t size = performanceList.size();
            
            for(auto itr = performanceList.begin(); itr!=performanceList.end(); itr++){
                PerfType ph = *itr;
                
                avgValue += ph->getClusterValue(hyppox::Config::REFERENCE_PH_INDEX);
            }
            
            avgValue /= size;
            
            return avgValue;
        }
        
        template<typename PerfType, typename PosType>
        bool DataPoint<PerfType,PosType>::ComparePoint(DataPoint<PerfType,PosType> &dp){
            bool m=true;
            
            for(short i=0; i<hyppox::Config::FILTER; i++){
                m &= (this->position[i]==dp.getPosition(i));
            }
            
            return m;
        }
        
        template<typename PerfType, typename PosType>
        float DataPoint<PerfType,PosType>::GetTotalPerfValue(){
            float value = 0.0;
            
            for(auto itr = performanceList.begin(); itr != performanceList.end(); itr++){
                PerfType ph = *itr;
                value += ph.getClusterValue(hyppox::Config::REFERENCE_PH_INDEX);
            }
            
            return value;
        }
    }
}

#endif /* DataPoint_h */
