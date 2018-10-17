
/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: dbScan.h
 Objective: Implementation of density based clustering DBSCAN algo
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              03/12/2016      Initial version
 
 **************************************************************************************************/

#ifndef cluster_h
#define cluster_h

#include "fileHandler.h"

namespace hyppox {
    namespace cluster{
        template<typename PerfType, typename ClusterIDType>
        class Cluster{
            private:
            static ClusterIDType clusterID;
            
            public:
            Cluster()=default;
            virtual ~Cluster()=default;
            
            static ClusterIDType GetNextID(){clusterID++; return clusterID;}
            static ClusterIDType GetClusterID(){return clusterID;}
            static void SetClusterID(ClusterIDType _id){clusterID = _id;}
            
            virtual void doClustering(std::list<PerfType*>*&) = 0;
        };
        
        template<typename PerfType, typename ClusterIDType>
        ClusterIDType Cluster<PerfType,ClusterIDType>::clusterID = 0;
    }
}


#endif /* DBScan_h */
