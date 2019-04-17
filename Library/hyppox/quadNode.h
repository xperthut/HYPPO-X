
/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: quadNode.h
 Objective: Class holds data of a node of a quad tree
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              02/21/2016      Initial version
 
 **************************************************************************************************/

#ifndef quadNode_h
#define quadNode_h

#include "dataPoint.h"

// NodePosType is either float or double
namespace hyppox {
    namespace quadtree{
        template<typename DPType, typename NodePosType>
        class QuadNode{
            public:
            //using QNType = QuadNode<DPType,NodePosType>;
            //using std::pair<std::vector<NodePosType>, std::vector<NodePosType> > = std::pair<std::vector<NodePosType>, std::vector<NodePosType> >;
            
            QuadNode(std::vector<NodePosType> pos, std::vector<NodePosType> len);
            ~QuadNode();
            
            static int totalChildren;
            
            // Getter methods
            NodePosType GetPosition(short index);
            NodePosType GetLength(short index);
            QuadNode<DPType,NodePosType>* GetChild(unsigned int index);
            void SetChild(unsigned int index, QuadNode<DPType,NodePosType>* child);
            bool IsThisLeafNode();
            DPType* GetNodeData();
            int GetTotalChildren();
            void ResetChildren();
            NodePosType GetNodePosition(short index);
            
            // Setter
            void SetArea();
            
            //Other methods
            void AddDataToNode(DPType *data);
            void setData(DPType* dp);
            bool SearchDataInArea(std::vector<NodePosType> &_start, std::vector<NodePosType> &_end, DPType* query);
            
            private:
            std::vector<NodePosType> pos; //position of root
            std::vector<NodePosType> start; // starting position of this block
            std::vector<NodePosType> length; // length of this block
            
            std::vector<std::vector<NodePosType> > childLocations;
            std::vector<QuadNode<DPType,NodePosType> *> children;
            
            DPType* data;
            bool hasChildren;
            int childCount;
            
            void AddDataToNode(int childIndex, DPType *data);
            bool SearchInArea(DPType* query);
            bool BelongsIn(std::vector<NodePosType> &_start, std::vector<NodePosType> &_end);
            std::pair<std::vector<NodePosType>, std::vector<NodePosType> > FindResolution(QuadNode<DPType,NodePosType>* child, DPType* query, std::vector<NodePosType> _start, std::vector<NodePosType> _len);
            std::pair<std::vector<NodePosType>, std::vector<NodePosType> > FindResolution(DPType* query1, DPType* query2, std::vector<NodePosType> _start, std::vector<NodePosType> _len);
            int InsertNewNodes(QuadNode<DPType,NodePosType>* newNode, QuadNode<DPType,NodePosType>* child);
        };
        
        template<typename DPType, typename NodePosType>
        int QuadNode<DPType,NodePosType>::totalChildren = 0;
        
        template<typename DPType, typename NodePosType>
        QuadNode<DPType,NodePosType>::QuadNode(std::vector<NodePosType> pos, std::vector<NodePosType> len):start(pos),length(len){
            this->children.assign(hyppox::Config::QUAD_TREE_CHILDREN, nullptr);
            this->childLocations.assign(hyppox::Config::QUAD_TREE_CHILDREN, pos);
            
            this->data = nullptr;
            this->hasChildren = false;
            this->childCount = 0;
            this->pos.assign(hyppox::Config::FILTER, 0.0);
            
            this->SetArea();
        }
        
        template<typename DPType, typename NodePosType>
        QuadNode<DPType,NodePosType>::~QuadNode(){
            this->data = nullptr;
        }
        
        //#pragma Getter
        template<typename DPType, typename NodePosType>
        NodePosType QuadNode<DPType,NodePosType>::GetPosition(short index){return this->pos[index];}
        
        template<typename DPType, typename NodePosType>
        NodePosType QuadNode<DPType,NodePosType>::GetLength(short index){return this->length[index];}
        
        template<typename DPType, typename NodePosType>
        DPType* QuadNode<DPType,NodePosType>::GetNodeData(){return this->data;}
        
        template<typename DPType, typename NodePosType>
        int QuadNode<DPType,NodePosType>::GetTotalChildren(){return this->childCount;}
        
        template<typename DPType, typename NodePosType>
        NodePosType QuadNode<DPType,NodePosType>::GetNodePosition(short index){return this->start[index];}
        
        template<typename DPType, typename NodePosType>
        void QuadNode<DPType,NodePosType>::setData(DPType *dp){this->data = dp;}
        
        template<typename DPType, typename NodePosType>
        void QuadNode<DPType,NodePosType>::ResetChildren(){
            for(unsigned int i=0; i<hyppox::Config::QUAD_TREE_CHILDREN; i++){
                this->children[i] = nullptr;
            }
        }
        
        template<typename DPType, typename NodePosType>
        void QuadNode<DPType,NodePosType>::SetChild(unsigned int index, QuadNode<DPType,NodePosType> *child){
            this->children[index] = child;
        }
        
        template<typename DPType, typename NodePosType>
        QuadNode<DPType,NodePosType>* QuadNode<DPType,NodePosType>::GetChild(unsigned int index){
            if(index<hyppox::Config::QUAD_TREE_CHILDREN){
                return this->children[index];
            }
            return nullptr;
        }
        
        template<typename DPType, typename NodePosType>
        bool QuadNode<DPType,NodePosType>::IsThisLeafNode(){
            return (this->data != nullptr);
        }
        
        //#pragma Setter
        template<typename DPType, typename NodePosType>
        void QuadNode<DPType,NodePosType>::SetArea(){
            
            /*cout<<"(";
             for(short i=0; i<hyppox::Config::FILTER; i++){
             cout<<this->start[i];
             if(i<hyppox::Config::FILTER-1) cout<<",";
             }
             cout<<")"<<endl;*/
            
            for(short i=0; i<hyppox::Config::FILTER; i++){
                this->pos[i] = this->start[i] + (this->length[i]/2);
            }
            
            // Set starting position of each child
            for(unsigned int i=0; i<hyppox::Config::QUAD_TREE_CHILDREN; i++){
                unsigned int k = i;
                // For each position identify the bit 1 position, make that position
                for(short j=0;j<hyppox::Config::FILTER; j++,k>>=1){
                    if(k&1){
                        this->childLocations[i][j] = this->start[j] + (this->length[j]/2.0);
                    }else{
                        this->childLocations[i][j] = this->start[j];
                    }
                }
            }
        }
        
        //#pragma Other_Methods
        template<typename DPType, typename NodePosType>
        void QuadNode<DPType,NodePosType>::AddDataToNode(DPType *data){
            std::vector<float> _start(hyppox::Config::FILTER, 0.0), _end(hyppox::Config::FILTER, 0.0), _len(hyppox::Config::FILTER, 0.0);
            
            // Initially check all children of root to insert data
            // It no blank node found then propagate with existing node where it fits
            for(unsigned int i=0; i<hyppox::Config::QUAD_TREE_CHILDREN; i++){
                bool m = true;
                
                for(short j=0; j<hyppox::Config::FILTER; j++){
                    _start[j] = this->childLocations[i][j];
                    _len[j] = this->length[j]/2;
                    _end[j] = _start[j] + _len[j];
                    
                    m &= (_start[j]<=data->getPosition(j) && data->getPosition(j)<=_end[j]);
                    
                    if(!m) break;
                }
                
                if(m){
                    if(this->children[i] == nullptr){
                        this->children[i] = new QuadNode(_start, _len);
                        this->children[i]->setData(data);
                        
                        this->childCount++;
                        
                        totalChildren++;
                        
                    }else{
                        // Explore i-th child of root
                        this->AddDataToNode(i, data);
                    }
                    
                    break;
                }
            }
        }
        
        //#pragma Private_Methods
        template<typename DPType, typename NodePosType>
        void QuadNode<DPType,NodePosType>::AddDataToNode(int childIndex, DPType *data){
            
            QuadNode<DPType,NodePosType>* child = this->children[childIndex];
            QuadNode<DPType,NodePosType>* newNode = nullptr;
            std::pair<std::vector<NodePosType>, std::vector<NodePosType> > start_len_pair;
            
            std::vector<NodePosType> _start(hyppox::Config::FILTER, 0.0), _end(hyppox::Config::FILTER, 0.0), _len(hyppox::Config::FILTER, 0.0);
            
            // Store the start position and length of the child area
            // This part will need when a child and a new datapoint belongs under same child area if parent
            // but new datapoint does not belongs in that child area
            for(short j=0; j<hyppox::Config::FILTER; j++){
                _start[j] = this->childLocations[childIndex][j];
                _len[j] = this->length[j]/2;
                _end[j] = _start[j] + _len[j];
            }
            
            // If Child data has more children
            if(child->GetNodeData() == nullptr){
                
                // Search whether data will fit in this child area or not
                if(child->SearchInArea(data)){
                    // For each of the child check whether data will fit or not
                    for(unsigned int i=0; i<hyppox::Config::QUAD_TREE_CHILDREN; i++){
                        for(short j=0; j<hyppox::Config::FILTER; j++){
                            _start[j] = child->childLocations[i][j];
                            _len[j] = child->length[j]/2;
                            _end[j] = _start[j] + _len[j];
                        }
                        
                        if(SearchDataInArea(_start, _end, data)){
                            if(child->children[i] == nullptr){
                                child->children[i] = new QuadNode<DPType,NodePosType>(_start, _len);
                                child->children[i]->setData(data);
                                
                                child->childCount++;
                                
                                totalChildren++;
                                
                            }else{
                                
                                child->AddDataToNode(i, data);
                            }
                            
                            break;
                        }
                    }
                }else{
                    // If data is under parent region but not in child region
                    // There must have a node in between parent and child where data will fit
                    // Find that resolution point
                    // Create new node, make this node as a child of parent and previous child of parent will be a child
                    // of this new node.
                    
                    start_len_pair = this->FindResolution(child, data, _start, _len);
                    newNode = new QuadNode<DPType,NodePosType>(start_len_pair.first, start_len_pair.second);
                    
                    bool first = false, second = false;
                    
                    for(unsigned int i=0; i<hyppox::Config::QUAD_TREE_CHILDREN; i++){
                        for(short j=0; j<hyppox::Config::FILTER; j++){
                            _start[j] = newNode->childLocations[i][j];
                            _len[j] = newNode->length[j]/2;
                            _end[j] = _start[j] + _len[j];
                        }
                        
                        if(SearchDataInArea(_start, _end, data)){
                            newNode->children[i] = new QuadNode<DPType,NodePosType>(_start, _len);
                            
                            newNode->children[i]->setData(data);
                            
                            newNode->childCount++;
                            
                            totalChildren++;
                            
                            first = true;
                            
                        }else if(child->BelongsIn(_start, _end)){
                            newNode->children[i] = child;
                            
                            newNode->childCount++;
                            
                            second = true;
                        }
                        
                        if(first&& second)break;
                    }
                    
                    this->children[childIndex] = newNode;
                    
                    if(second == false){
                        this->InsertNewNodes(newNode, child);
                        
                        delete child;
                    }
                }
            }else{ // Child node has no children
                // Either data fit in this child
                // Or not
                if(child->SearchInArea(data)){
                    bool sameChild,firstChild, secondChild;
                    int fci=-1, sci=-1;
                    bool first = false, second = false;
                    
                    // For each of the child check whether data will fit or not
                    for(unsigned int i=0; i<hyppox::Config::QUAD_TREE_CHILDREN; i++){
                        sameChild = true;
                        firstChild = !(first);
                        secondChild = !(second);
                        
                        for(short j=0; j<hyppox::Config::FILTER; j++){
                            _start[j] = child->childLocations[i][j];
                            _len[j] = child->length[j]/2;
                            _end[j] = _start[j] + _len[j];
                            
                            if(child->GetNodeData()!=nullptr) firstChild &= (_start[j]<=child->GetNodeData()->getPosition(j)&& child->GetNodeData()->getPosition(j)<=_end[j]);
                            secondChild &= (_start[j]<=data->getPosition(j) && data->getPosition(j)<=_end[j]);
                            
                            if(!firstChild&&!secondChild) break;
                        }
                        
                        if(firstChild) fci = i;
                        if(firstChild && !secondChild){
                            
                            child->children[i] = new QuadNode<DPType,NodePosType>(_start, _len);
                            child->children[i]->setData(child->GetNodeData());
                            
                            child->childCount++;
                            
                            child->setData(nullptr);
                            
                            first = true;
                        }
                        
                        if(secondChild) sci = i;
                        if(secondChild&&!firstChild){
                            child->children[i] = new QuadNode<DPType,NodePosType>(_start, _len);
                            child->children[i]->setData(data);
                            
                            child->childCount++;
                            totalChildren++;
                            
                            second = true;
                        }
                        
                        if(fci>=0 && sci>=0){
                            if(fci==sci){
                                start_len_pair = this->FindResolution(child->GetNodeData(), data, _start, _len);
                                newNode = new QuadNode<DPType,NodePosType>(start_len_pair.first, start_len_pair.second);
                                
                                first = false; second = false;
                                
                                for(unsigned int i=0; i<hyppox::Config::QUAD_TREE_CHILDREN; i++){
                                    for(short j=0; j<hyppox::Config::FILTER; j++){
                                        _start[j] = newNode->childLocations[i][j];
                                        _len[j] = newNode->length[j]/2;
                                        _end[j] = _start[j] + _len[j];
                                    }
                                    
                                    if(SearchDataInArea(_start, _end, data)){
                                        newNode->children[i] = new QuadNode<DPType,NodePosType>(_start, _len);
                                        
                                        newNode->children[i]->setData(data);
                                        
                                        newNode->childCount++;
                                        
                                        totalChildren++;
                                        
                                        first = true;
                                        
                                    }else if(SearchDataInArea(_start, _end, child->GetNodeData())){
                                        newNode->children[i] = new QuadNode<DPType,NodePosType>(_start, _len);
                                        
                                        newNode->children[i]->setData(child->GetNodeData());
                                        
                                        newNode->childCount++;
                                        
                                        //totalChildren++;
                                        
                                        second = true;
                                    }
                                    
                                    if(first&& second)break;
                                }
                                
                                this->children[childIndex] = newNode;
                                
                                if(second == false){
                                    this->InsertNewNodes(newNode, child);
                                }
                                
                                delete child;
                            }
                            break;
                        }
                    }
                    
                }else{
                    
                    start_len_pair = this->FindResolution(child, data, _start, _len);
                    newNode = new QuadNode<DPType,NodePosType>(start_len_pair.first, start_len_pair.second);
                    
                    bool first = false, second = false;
                    
                    for(unsigned int i=0; i<hyppox::Config::QUAD_TREE_CHILDREN; i++){
                        for(short j=0; j<hyppox::Config::FILTER; j++){
                            _start[j] = newNode->childLocations[i][j];
                            _len[j] = newNode->length[j]/2;
                            _end[j] = _start[j] + _len[j];
                        }
                        
                        if(SearchDataInArea(_start, _end, data)){
                            newNode->children[i] = new QuadNode<DPType,NodePosType>(_start, _len);
                            
                            newNode->children[i]->setData(data);
                            
                            newNode->childCount++;
                            
                            totalChildren++;
                            
                            first = true;
                            
                        }else if(child->BelongsIn(_start, _end)){
                            newNode->children[i] = child;
                            
                            newNode->childCount++;
                            
                            totalChildren++;
                            
                            second = true;
                        }
                        
                        if(first&& second)break;
                    }
                    
                    this->children[childIndex] = newNode;
                    
                    if(second == false){
                        this->InsertNewNodes(newNode, child);
                        
                        delete child;
                    }
                }
            }
        }
        
        template<typename DPType, typename NodePosType>
        int QuadNode<DPType,NodePosType>::InsertNewNodes(QuadNode<DPType,NodePosType> *newNode, QuadNode<DPType,NodePosType> *child){
            
            if(child == nullptr) return 0;
            
            for(unsigned int i=0; i<hyppox::Config::QUAD_TREE_CHILDREN; i++){
                QuadNode<DPType,NodePosType>* gChild = child->children[i];
                
                if (gChild != nullptr) {
                    if(gChild->data != nullptr){
                        totalChildren--;
                        newNode->AddDataToNode(gChild->data);
                        
                    }else{
                        this->InsertNewNodes(newNode, gChild);
                    }
                }
            }
            
            
            return 0;
        }
        
        template<typename DPType, typename NodePosType>
        bool QuadNode<DPType,NodePosType>::SearchDataInArea(std::vector<NodePosType> &_start, std::vector<NodePosType> &_end, DPType *query){
            bool m = true;
            
            for(short j=0; j<hyppox::Config::FILTER; j++){
                float p = query->getPosition(j);
                m &= (_start[j]<=p && p<=_end[j]);
            }
            
            return m;
        }
        
        template<typename DPType, typename NodePosType>
        bool QuadNode<DPType,NodePosType>::SearchInArea(DPType *query){
            bool m = true;
            
            for(short j=0; j<hyppox::Config::FILTER; j++){
                float p = query->getPosition(j),
                q = this->GetNodePosition(j),
                r = q + this->GetLength(j);
                m &= (q<=p && p<=r);
            }
            
            return m;
        }
        
        template<typename DPType, typename NodePosType>
        bool QuadNode<DPType,NodePosType>::BelongsIn(std::vector<NodePosType> &_start, std::vector<NodePosType> &_end){
            bool m = true;
            
            for(short j=0; j<hyppox::Config::FILTER; j++){
                float s = this->GetNodePosition(j);
                float l = this->GetLength(j);
                m &= (_start[j]<=s && (s+l)<=_end[j]);
            }
            
            return m;
        }
        
        template<typename DPType, typename NodePosType>
        std::pair<std::vector<NodePosType>, std::vector<NodePosType> > QuadNode<DPType,NodePosType>::FindResolution(QuadNode<DPType,NodePosType>* child, DPType* query, std::vector<NodePosType> _start, std::vector<NodePosType> _len){
            
            std::vector<NodePosType> t_pos(hyppox::Config::FILTER, 0.0); //position of root
            std::vector<NodePosType> t_start(_start); // starting position of this block
            std::vector<NodePosType> t_length(_len); // length of this block
            std::vector<std::vector<NodePosType> > t_childLocations(hyppox::Config::QUAD_TREE_CHILDREN, _start);
            
            for(short i=0; i<hyppox::Config::FILTER; i++){
                t_pos[i] = t_start[i] + (t_length[i]/2);
            }
            
            // Set starting position of each child
            for(unsigned int i=0; i<hyppox::Config::QUAD_TREE_CHILDREN; i++){
                unsigned int k = i;
                // For each position identify the bit 1 position, make that position
                for(short j=0;j<hyppox::Config::FILTER; j++,k>>=1){
                    if(k&1){
                        t_childLocations[i][j] = t_start[j] + (t_length[j]/2.0);
                    }else{
                        t_childLocations[i][j] = t_start[j];
                    }
                }
            }
            
            std::vector<NodePosType> _cdPos(hyppox::Config::FILTER, 0.0), _dPos(hyppox::Config::FILTER, 0.0),  _end(hyppox::Config::FILTER, 0.0);
            
            // Check this node is the root of these two datapoint or not
            for(short j=0;j<hyppox::Config::FILTER; j++){
                _cdPos[j] = child->GetPosition(j);
                _dPos[j] = query->getPosition(j);
            }
            
            bool sameChild,firstChild, secondChild;
            int fci=-1, sci=-1;
            
            for(unsigned int i=0; i<hyppox::Config::QUAD_TREE_CHILDREN; i++){
                sameChild = true;
                firstChild = true;
                secondChild = true;
                
                for(short j=0; j<hyppox::Config::FILTER; j++){
                    _start[j] = t_childLocations[i][j];
                    _len[j] = t_length[j]/2;
                    _end[j] = _start[j] + _len[j];
                    
                    firstChild &= (_start[j]<=_dPos[j] && _dPos[j]<=_end[j]);
                    secondChild &= (_start[j]<=_cdPos[j] && _cdPos[j]<=_end[j]);
                    
                    if(!firstChild&&!secondChild) break;
                }
                
                if(firstChild) fci = i;
                if(secondChild) sci = i;
                
                if(fci>=0 && sci>=0){
                    if(fci==sci){
                        return FindResolution(child, query, _start, _len);
                    }
                    break;
                }
            }
            
            return make_pair(t_start, t_length);
        }
        
        template<typename DPType, typename NodePosType>
        std::pair<std::vector<NodePosType>, std::vector<NodePosType> > QuadNode<DPType,NodePosType>::FindResolution(DPType* query1, DPType* query2, std::vector<NodePosType> _start, std::vector<NodePosType> _len){
            
            std::vector<NodePosType> t_pos(hyppox::Config::FILTER, 0.0); //position of root
            std::vector<NodePosType> t_start(_start); // starting position of this block
            std::vector<NodePosType> t_length(_len); // length of this block
            std::vector<std::vector<NodePosType> > t_childLocations(hyppox::Config::QUAD_TREE_CHILDREN, _start);
            
            for(short i=0; i<hyppox::Config::FILTER; i++){
                t_pos[i] = t_start[i] + (t_length[i]/2);
            }
            
            // Set starting position of each child
            for(unsigned int i=0; i<hyppox::Config::QUAD_TREE_CHILDREN; i++){
                unsigned int k = i;
                // For each position identify the bit 1 position, make that position
                for(short j=0;j<hyppox::Config::FILTER; j++,k>>=1){
                    if(k&1){
                        t_childLocations[i][j] = t_start[j] + (t_length[j]/2.0);
                    }else{
                        t_childLocations[i][j] = t_start[j];
                    }
                }
            }
            
            std::vector<NodePosType> _cdPos(hyppox::Config::FILTER, 0.0), _dPos(hyppox::Config::FILTER, 0.0),  _end(hyppox::Config::FILTER, 0.0);
            
            // Check this node is the root of these two datapoint or not
            for(short j=0;j<hyppox::Config::FILTER; j++){
                _cdPos[j] = query1->getPosition(j);
                _dPos[j] = query2->getPosition(j);
            }
            
            bool sameChild,firstChild, secondChild;
            int fci=-1, sci=-1;
            
            for(unsigned int i=0; i<hyppox::Config::QUAD_TREE_CHILDREN; i++){
                sameChild = true;
                firstChild = true;
                secondChild = true;
                
                for(short j=0; j<hyppox::Config::FILTER; j++){
                    _start[j] = t_childLocations[i][j];
                    _len[j] = t_length[j]/2;
                    _end[j] = _start[j] + _len[j];
                    
                    firstChild &= (_start[j]<=_dPos[j] && _dPos[j]<=_end[j]);
                    secondChild &= (_start[j]<=_cdPos[j] && _cdPos[j]<=_end[j]);
                    
                    if(!firstChild&&!secondChild) break;
                }
                
                if(firstChild) fci = i;
                if(secondChild) sci = i;
                
                if(fci>=0 && sci>=0){
                    if(fci==sci){
                        return FindResolution(query1, query2, _start, _len);
                    }
                    break;
                }
            }
            
            return make_pair(t_start, t_length);
        }
    }
    
}

#endif /* QuadNode_h */
