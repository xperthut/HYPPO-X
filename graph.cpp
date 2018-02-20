/**************************************************************************************************
 Copyright © 2016-2018 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: Graph.cpp
 Objective: Class to generate graph
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              02/16/2017      Initial version
 Md. Kamruzzaman              06/20/2017      Added logic to color connected components, either all components under same coloring scheme or separate coloring scheme for each connect component, controlled by a flag
 Md. Kamruzzaman              01/06/2018      Code revision, fixing obscure logic issue
 **************************************************************************************************/

#include "graph.h"

/////////////////// Global methods ///////////////////////////////////
string fixPrecision(float value, int precision){
    string pValue = to_string(value);
    size_t pos = 0;
    
    if((pos = pValue.find(".")) != string::npos){
        
        if(pValue.length()-pos-1 > precision){
            return pValue.substr(0, pos+1+precision);
        }
    }
    
    return pValue;
}

bool comparePaths(InterestingPath& p1, InterestingPath& p2){
    return (p1.getScore()>p2.getScore());
}

/////////////////// InterestingPath class methods ///////////////////////
InterestingPath::InterestingPath(){
    this->rank = 0;
    this->score = 0.0;
}

InterestingPath::~InterestingPath(){}

void InterestingPath::setScore(float s){this->score = s;}
float InterestingPath::getScore(){return this->score;}
void InterestingPath::setRank(unsigned long r){this->rank = r;}
unsigned long InterestingPath::getRank(){return this->rank;}
void InterestingPath::addToPath(string edge){this->path.addToSet(edge);}
unordered_set<string>& InterestingPath::getPath(){return this->path.getSet();}
void InterestingPath::setColor(string c){this->color = c;}
string InterestingPath::getColor(){return this->color;}
bool InterestingPath::PathContainsEdge(string eID){return this->path.hasItem(eID);}

/////////////////// CostTableCell class methods ///////////////////////
CostTableCell::CostTableCell(){
    this->score = 0.0;
    this->maxPred = NULL;
    this->edgeID = "";
}

CostTableCell::~CostTableCell(){}

void CostTableCell::setScore(float s){this->score = s;}
void CostTableCell::setCell(CostTableCell *mp){this->maxPred = mp;}
float CostTableCell::getScore(){return this->score;}
CostTableCell* CostTableCell::getCell(){return this->maxPred;}
void CostTableCell::setEdgeID(string eID){this->edgeID = eID;}
string CostTableCell::getEdgeID(){return this->edgeID;}

/////////////////// Property class methods ///////////////////////////

Property::Property(){
    this->weight = (float*)malloc(sizeof(float)*(Config::FILTER+1));
    this->depth = 0;
}

Property::~Property(){
    free(this->weight);
}

float Property::getWeight(short index){return this->weight[index];}
void Property::setWeight(float value, short index){this->weight[index]=value;}
unsigned long Property::getDepth(){return this->depth;}
void Property::setDepth(unsigned long d){this->depth=d;}

/////////////////// rNode class methods /////////////////////////
rNode::rNode(unsigned long id){
    this->id = id;
    this->size = 0.0;
    this->in_degree = this->out_degree = 0;
    this->isSource = false;
    this->isMarked = false;
    this->nextNodes.clear();
    this->prevNodes.clear();
    this->setWeight(0.0, 0);
    for(short i=1; i<=Config::FILTER; i++){
        this->setWeight(0.0, i);
    }
}

rNode::~rNode(){
    this->nextNodes.clear();
    this->prevNodes.clear();
}

// Getter
unsigned long rNode::getID(){return id;}
float rNode::getSize(){return size;}
unsigned long rNode::getTotalInDegree(){return in_degree;}
unsigned long rNode::getTotalOutDegree(){return out_degree;}
bool rNode::IsThisNodeASource(){return isSource;}

// Setter
void rNode::setSize(float size){this->size += size;}
void rNode::addInDegree(){this->in_degree++;}
void rNode::addOutDegree(){this->out_degree++;}
void rNode::setSource(bool source){this->isSource = source;}

void rNode::createLinkNodes(){
    if(this->out_degree>0){
        this->nextNodes.assign(this->out_degree, NULL);//(rNode**)malloc(sizeof(rNode*)*this->out_degree);
    }
    
    if(this->in_degree>0){
        this->prevNodes.assign(this->in_degree, NULL);// = (rNode**)malloc(sizeof(rNode*)*this->in_degree);
    }
}

void rNode::addNextNode(rNode *nextNode){
    for(int i=0; i<this->out_degree; i++){
        if(this->nextNodes[i] == NULL){
            this->nextNodes[i] = nextNode;
            break;
        }
    }
}

void rNode::addPrevNode(rNode *prevNode){
    for(int i=0; i<this->in_degree; i++){
        if(this->prevNodes[i] == NULL){
            this->prevNodes[i] = prevNode;
            break;
        }
    }
}

rNode* rNode::getNextNode(unsigned long index){
    return nextNodes.at(index);
}

rNode* rNode::getPrevNode(unsigned long index){
    return prevNodes.at(index);
}

void rNode::removeNextNode(rNode* n){
    for(vector<rNode*>::iterator itr = this->nextNodes.begin(); itr!=this->nextNodes.end(); itr++){
        rNode* tmpNode = *itr;
        
        if(n->getID()==tmpNode->getID()){
            this->nextNodes.erase(itr);
            this->out_degree--;
            break;
        }
    }
}
void rNode::removePrevNode(rNode* n){
    for(vector<rNode*>::iterator itr = this->prevNodes.begin(); itr!=this->prevNodes.end(); itr++){
        rNode* tmpNode = *itr;
        
        if(n->getID()==tmpNode->getID()){
            this->prevNodes.erase(itr);
            this->in_degree--;
            break;
        }
    }
}

void rNode::resetLinkNodes(){
    
    this->nextNodes.clear();
    this->prevNodes.clear();
    
    this->in_degree = this->out_degree = 0;
}

void rNode::addPoint(Phenotype *ph){
    unsigned long totalPoints = this->points.getSize();
    
    string key = ph->getGenotype()+"#"+ph->GetSite()+"#"+to_string(ph->getDAP());
    
    if(!points.hasItem(key)){
        float avg = this->getWeight(0);
        float tw = (avg*totalPoints)+ph->GetValue();
        this->setWeight(tw/(totalPoints+1), 0);
        
        for(short i=1;i<=Config::FILTER;i++){
            avg = this->getWeight(i);
            tw = (avg*totalPoints)+ph->getCoOrdinate(i-1);
            this->setWeight(tw/(totalPoints+1), i);
        }
        
        points.addToMap(key, ph);
        this->setSize(ph->GetWeight(this->getID()));
    }
    
}

string rNode::getPhListForD3(){
    string phList = "[";
    unordered_map<string, Phenotype*> phMap = this->points.getMap();
    
    for(unordered_map<string, Phenotype*>::iterator itr=phMap.begin(); itr!=phMap.end(); itr++){
        Phenotype* ph = itr->second;
        
        if(phList.length()>1) phList += ",";
        
        string weight = "["+fixPrecision(ph->GetValue(), 2);
        for(short i=0; i<Config::FILTER; i++){
            if(weight.length()>1) weight +=",";
            
            weight += fixPrecision(ph->getEnvValue(i), 2);
        }
        weight += "]";
        
        phList += "{\"Gen\":\"" + ph->getGenotype() + "\",\"Loc\":\"" + ph->GetSite() + "\",\"DT\":\"" + ph->getDate() +
        "\",\"Weight\":" + weight + "}";
    }
    phList += "]";
    
    return phList;
}

///////////////////// rEdge class methods ////////////////////////////
rEdge::rEdge(rNode* node1, rNode* node2){
    if(node1->getWeight(0) > node2->getWeight(0)){
        this->fromNode = node2;
        this->toNode = node1;
    }else{
        this->fromNode = node1;
        this->toNode = node2;
    }
    
    this->id = to_string(this->fromNode->getID()) + "#" + to_string(this->toNode->getID());
    
    // 10->11->10
    /*if(this->id.compare("106#81")==0||this->id.compare("81#107")==0||this->id.compare("107#82")==0){
        cout<<"";
    }*/
    
    this->signature.reset();
    this->isMarked = false;
    this->refEdge = NULL;
    this->ipRank = 0;
    this->color = Config::EDGE_COLOR;
    
    float w1,w2,w;
    
    // Edge weight based on phenotypic value
    w1 = this->fromNode->getWeight(0);
    w2 = this->toNode->getWeight(0);
    w = ABS(w2-w1)>0?ABS(w2-w1):Config::NEGLIGIBLE_WEIGHT;
    
    this->setWeight(w, 0);
    
    for(int i=1; i<=Config::FILTER; i++){
        w1 = this->fromNode->getWeight(i);
        w2 = this->toNode->getWeight(i);
        w = ABS(w2-w1)>0?ABS(w2-w1):Config::NEGLIGIBLE_WEIGHT;
        
        this->setWeight(w, i);
        
        w = w2-w1;
        
        if(w<0){
            this->setSignatue(false, i);
        }else{
            this->setSignatue(true, i);
        }
    }
    
    this->cells = NULL;
    
    // 10->11->10
    /*if(this->id.compare("106#81")==0||this->id.compare("81#107")==0||this->id.compare("107#82")==0){
        unsigned long tmp;
        this->getSignature(tmp);
        
        cout<<tmp<<endl;
    }*/
}

rEdge::~rEdge(){
    if(this->cells!=NULL) delete [] this->cells;
}

// Getter
string rEdge::getID(){return this->id;}
rNode* rEdge::getFromNode(){return this->fromNode;}
rNode* rEdge::getToNode(){return this->toNode;}
bool rEdge::getMark(){return this->isMarked;}
void rEdge::getSignature(unsigned long& sig){sig = signature.to_ulong();}
void rEdge::getSignature(string &sig){sig = signature.to_string().substr(BIT_SET_SIZE-Config::FILTER, Config::FILTER);}
rEdge* rEdge::getRefEdge(){return this->refEdge;}
unsigned long rEdge::getIPRank(){return this->ipRank;}
string rEdge::getColor(){return this->color;}

// Setter
void rEdge::setMark(bool mark){isMarked = mark;}
void rEdge::setSignatue(bool value, short index){
    if(index>0 && index<=Config::FILTER){
        signature.set(Config::FILTER-index,value);
    }
}
void rEdge::setRefEdge(rEdge *e){refEdge=e;}
void rEdge::setIPRank(unsigned long r){this->ipRank=r;}
void rEdge::setColor(string c){this->color = c;}

void rEdge::swapNodes(){
    rNode* tmp = fromNode;
    fromNode = toNode;
    toNode = tmp;
}

bool rEdge::matchSignature(rEdge* e){
    
    unsigned long s, sig;
    this->getSignature(s);
    e->getSignature(sig);
    
    if(Config::INCREASE_DAP) return (this->getSignatureAtIndex(1) == e->getSignatureAtIndex(1) && this->getSignatureAtIndex(1)==1);
    
    if(!Config::MATCH_SIGNATURE) return true;
    
    if(Config::DELTA_CHANGE==0){
        return (s==sig);
    }else{
        if(s==sig) return true;
        else{
            bool mismatch = true;
            float wDiff = 0;
            
            for(int index=0; index<Config::FILTER; index++){
                
                // Check delta threshold
                if(this->getSignatureAtIndex(index+1) != e->getSignatureAtIndex(index+1)){
                    
                    if(Config::REFERENCE_PHENOTYPE){
                        //wDiff = ABS(getNormalizedWeight(index+1)-e->getNormalizedWeight(index+1))*100;
                        wDiff = (ABS(this->getWeight(index+1)-e->getWeight(index+1))*100)/((this->getWeight(index+1)-e->getWeight(index+1))>0?this->getWeight(index+1):e->getWeight(index+1));
                        
                        mismatch = mismatch && (wDiff<=Config::DELTA_CHANGE);
                    }else{
                        // For phenotype
                        if(index==0){
                            //wDiff = ABS(getNormalizedWeight(index)-e->getNormalizedWeight(index))*100;
                            wDiff = (ABS(this->getWeight(index)-e->getWeight(index))*100)/((this->getWeight(index)-e->getWeight(index))>0?this->getWeight(index):e->getWeight(index));
                            
                            mismatch = mismatch && (wDiff<=Config::DELTA_CHANGE);
                        }
                        
                        // For environmental attributes
                        if(index != Config::REFERENCE_ENV_INDEX){
                            
                            //wDiff = ABS(getNormalizedWeight(index+1)-e->getNormalizedWeight(index+1))*100;
                            wDiff = (ABS(this->getWeight(index+1)-e->getWeight(index+1))*100)/((this->getWeight(index+1)-e->getWeight(index+1))>0?this->getWeight(index+1):e->getWeight(index+1));
                            
                            mismatch = mismatch && (wDiff<=Config::DELTA_CHANGE);
                        }
                    }
                }
            }
            
            return mismatch;
        }
    }
    
    return false;
}

bool rEdge::strictMatchSignature(rEdge *e){
    
    unsigned long s, sig;
    getSignature(s);
    e->getSignature(sig);
    
    return (s==sig);
}

bool rEdge::getSignatureAtIndex(short index){
    if(index>0 && index<=Config::FILTER){
        return this->signature[Config::FILTER-index];
    }
    
    return false;
}

void rEdge::assignScore(rEdge *eMax){
    
    if(eMax!=NULL){
        this->setDepth(eMax->getDepth()+1);
    }
}

void rEdge::createCells(){
    unsigned long depth = this->getDepth();
    
    this->cells = new CostTableCell[depth];
    for(unsigned long i=0;i<depth;i++){
        this->cells[i].setEdgeID(this->id);
    }
}

void rEdge::destroyCells(){
    if(this->cells!=NULL)delete [] this->cells;
    this->cells = NULL;
}

float rEdge::getScoreOfCell(unsigned long index){
    if(index<this->getDepth()){
        return this->cells[index].getScore();
    }
    
    return -1.0;
}

void rEdge::setScoreOfCell(float score, unsigned long index){
    if(index<this->getDepth()){
        this->cells[index].setScore(score);
    }
}

void rEdge::setCostTableCellOfIndex(CostTableCell *cell, unsigned long index){
    if(index<this->getDepth()){
        this->cells[index].setCell(cell);
    }
}

CostTableCell* rEdge::getCostTableCellOfIndex(unsigned long index){
    if(index<this->getDepth()){
        return &this->cells[index];
    }
    
    return NULL;
}

////////////////// IsolatedSubGraph class methods //////////////////////
IsolatedSubGraph::IsolatedSubGraph(){
    this->nodeWeightRange = (float**)malloc(sizeof(float*)*(Config::FILTER+1));
    this->globalNodeWeightRange = (float**)malloc(sizeof(float*)*(Config::FILTER+1));
    
    for(short i=0;i<=Config::FILTER;i++){
        this->nodeWeightRange[i] = (float*)malloc(sizeof(float)*2);
        this->globalNodeWeightRange[i] = (float*)malloc(sizeof(float)*2);
        
        this->nodeWeightRange[i][0]=this->nodeWeightRange[i][1]=0.0;
        this->globalNodeWeightRange[i][0]=this->globalNodeWeightRange[i][1]=0.0;
    }
    
    this->envName = new string[Config::FILTER];
    this->maxDepth = 0;
}

IsolatedSubGraph::~IsolatedSubGraph(){
    for(short i=0;i<2;i++){
        free(this->nodeWeightRange[i]);
        free(this->globalNodeWeightRange[i]);
    }
    
    free(this->nodeWeightRange);
    free(this->globalNodeWeightRange);
    delete[] this->envName;
}

bool IsolatedSubGraph::hasNode(unsigned long id){
    return nodes.hasItem(id);
}

rNode* IsolatedSubGraph::getNodeById(unsigned long id){
    if(nodes.hasItem(id)) return nodes.getValue(id);
    return NULL;
}

void IsolatedSubGraph::addNode(unsigned long id){
    if(!nodes.hasItem(id)){
        rNode* aNode = new rNode(id);
        nodes.addToMap(id, aNode);
    }
}

void IsolatedSubGraph::setRange(){
    unordered_map<unsigned long, rNode*> nodeMap = this->nodes.getMap();
    bool first = true;
    
    for(unordered_map<unsigned long, rNode*>::iterator itr=nodeMap.begin(); itr!=nodeMap.end(); itr++){
        rNode* n = itr->second;
        
        if(first){
            this->nodeSizeRange[0] = this->nodeSizeRange[1] = n->getSize();
            this->nodeWeightRange[0][0] = this->nodeWeightRange[0][1] = n->getWeight(0);
            
            for(short i=1; i<=Config::FILTER; i++){
                this->nodeWeightRange[i][0] = this->nodeWeightRange[i][1] = n->getWeight(i);
            }
            
            first = false;
        }else{
            float w = n->getSize();
            
            if(w<this->nodeSizeRange[0]) this->nodeSizeRange[0] = w;
            if(w>this->nodeSizeRange[1]) this->nodeSizeRange[1] = w;
            
            w = n->getWeight(0);
            if(w<this->nodeWeightRange[0][0]) this->nodeWeightRange[0][0] = w;
            if(w>this->nodeWeightRange[0][1]) this->nodeWeightRange[0][1] = w;
            
            for(short i=1; i<=Config::FILTER; i++){
                w = n->getWeight(i);
                if(w<this->nodeWeightRange[i][0]) this->nodeWeightRange[i][0] = w;
                if(w>this->nodeWeightRange[i][1]) this->nodeWeightRange[i][1] = w;
            }
        }
    }
}

rEdge* IsolatedSubGraph::getMaxPredecessor(rEdge* e){
    rNode* toNode = e->getFromNode();
    rEdge* eMax = NULL;
    
    // Predicessor edges
    for(int i=0; i<toNode->getTotalInDegree(); i++){
        unsigned long maxDepth = 0;
        rNode* fNode = toNode->getPrevNode(i);
        
        string key = to_string(fNode->getID())+"#"+to_string(toNode->getID());
        rEdge* e1 = this->edges.getValue(key);
        
        if(e1->getDepth()>0){
            
            rEdge* refEdge = e1->getRefEdge();
            if(refEdge == NULL) refEdge=e1;
            
            if(refEdge->matchSignature(e)){
                // Changes is here
                // Compute which predecessor maximizes my score
                if(e->getDepth() < e1->getDepth()+1){
                    e->setDepth(e1->getDepth()+1);
                }
                
                if(maxDepth<e1->getDepth()+1){
                    maxDepth = e1->getDepth();
                    eMax = e1;
                }
            }
        }
    }
    
    return eMax;
}

bool IsolatedSubGraph::checkTerminalEdge(rEdge *e){
    rNode* fromNode = e->getToNode();
    
    for(int i=0; i<fromNode->getTotalOutDegree(); i++){
        rNode* toNode = fromNode->getNextNode(i);
        rEdge* edge = this->edges.getValue(to_string(fromNode->getID())+"#"+to_string(toNode->getID()));
        
        rEdge* refEdge = e->getRefEdge();
        if(refEdge==NULL) refEdge=e;
        
        if(refEdge->matchSignature(edge)){
            return false;
        }
    }
    
    return true;
}

bool IsolatedSubGraph::needMoreExplore(rEdge* e){
    
    rNode* toNode = e->getFromNode();
    
    // Predicessor edges
    for(int i=0; i<toNode->getTotalInDegree(); i++){
        rNode* fNode = toNode->getPrevNode(i);
        
        string key = to_string(fNode->getID())+"#"+to_string(toNode->getID());
        rEdge* edge = this->edges.getValue(key);
        
        if(edge->getDepth()==0){
            return true;
        }
    }
    
    return false;
}

void IsolatedSubGraph::exploreNode(){
    
    unsigned long id = this->exploreSet.getElement();
    this->exploreSet.removeItem(id);
    
    rNode* fromNode = this->nodes.getValue(id);
    
    for(int i=0; i<fromNode->getTotalOutDegree(); i++){
        rNode* toNode = fromNode->getNextNode(i);
        string key = to_string(fromNode->getID())+"#"+to_string(toNode->getID());
        rEdge* e = this->edges.getValue(key);
        
        rEdge* eMax = this->getMaxPredecessor(e);
        
        if(eMax == NULL){
            if(!fromNode->IsThisNodeASource() && !this->needMoreExplore(e)){
                fromNode->setSource(true);
                e->setDepth(1);
            }
        }else if(Config::DELTA_CHANGE>0){
            
            // First signature mismatch
            if(eMax->getRefEdge()==NULL){
                if(!eMax->strictMatchSignature(e))
                    e->setRefEdge(eMax);
                else{
                    e->setRefEdge(NULL);
                }
            }
            // Next edges after first mismatch
            else {
                if(!eMax->strictMatchSignature(e)){
                    e->setRefEdge(eMax->getRefEdge());
                }else{
                    e->setRefEdge(NULL);
                }
            }
        }
        
        if(e->getID().compare("15#16")==0){
            cout<<"";
        }
        
        if(!this->terminalEdgeSet.hasItem(e->getID()) && this->checkTerminalEdge(e)){
            this->terminalEdgeSet.addToSet(e->getID());
        }
    }
    
    // Add to explore list
    for(int i=0; i<fromNode->getTotalOutDegree(); i++){
        rNode* toNode = fromNode->getNextNode(i);
        
        this->exploreSet.addToSet(toNode->getID());
    }
}

void IsolatedSubGraph::identifySourceNodes(){
    this->terminalEdgeSet.clearHashSet();
    this->exploreSet.clearHashSet();
    this->sourceNodeIdSet.clearHashSet();
    
    unordered_map<unsigned long,rNode*> nList = this->nodes.getMap();
    unordered_map<unsigned long,rNode*>::iterator nitr;
    
    unordered_map<string,rEdge*> eList = this->edges.getMap();
    unordered_map<string,rEdge*>::iterator eitr;
    
    for(nitr=nList.begin(); nitr!=nList.end(); nitr++){
        rNode* n = nitr->second;
        
        if(n->getTotalInDegree() == 0){
            n->setSource(true);
            this->sourceNodeIdSet.addToSet(n->getID());
        }else{
            n->setSource(false);
        }
    }
    
    // Internal sources
    for(eitr=eList.begin(); eitr!=eList.end(); eitr++){
        rEdge* e = eitr->second;
        
        if(!e->getMark()){
            rNode* toNode = e->getFromNode();
            
            if(e->getID().compare("9#10")==0){
                cout<<"";
            }
            
            if(!toNode->IsThisNodeASource()){
                bool match = false;
                // Predicessor edges
                for(int i=0; i<toNode->getTotalInDegree(); i++){
                    rNode* fNode = toNode->getPrevNode(i);
                    string key = to_string(fNode->getID())+"#"+to_string(toNode->getID());
                    rEdge* e1 = this->edges.getValue(key);
                    
                    rEdge* refEdge = e1->getRefEdge();
                    if(refEdge == NULL) refEdge=e1;
                    
                    match |= refEdge->matchSignature(e);
                }
                
                if(!match) this->sourceNodeIdSet.addToSet(toNode->getID());
            }
        }
    }
}

void IsolatedSubGraph::resetDepth(){
    unordered_map<string, rEdge*> edgeMap = this->edges.getMap();
    
    for(unordered_map<string, rEdge*>::iterator itr=edgeMap.begin(); itr!=edgeMap.end(); itr++){
        rEdge* e = itr->second;
        if(!e->getMark()){
            e->setDepth(0);
        }
    }
}

void IsolatedSubGraph::runBFS(){
    this->resetDepth();
    this->identifySourceNodes();
    
    unordered_set<unsigned long> sourceIds = this->sourceNodeIdSet.getSet();
    
    for(unordered_set<unsigned long>::iterator itr=sourceIds.begin(); itr!=sourceIds.end(); itr++){
        unsigned long fromId = *itr;
        rNode* fromNode = this->nodes.getValue(fromId);
        
        for(int i=0; i<fromNode->getTotalOutDegree(); i++){
            rNode* toNode = fromNode->getNextNode(i);
            rEdge* e = this->edges.getValue(to_string(fromNode->getID())+"#"+to_string(toNode->getID()));
            
            e->setDepth(1);
            
            this->exploreSet.removeItem(e->getToNode()->getID());
            this->exploreSet.addToSet(e->getToNode()->getID());
        }
    }
    
    while(this->exploreSet.getSize()>0){
        this->exploreNode();
    }
}

void IsolatedSubGraph::computeCostTableForColumn(unsigned long index){
    unordered_map<string,rEdge*> eList = this->edges.getMap();
    
    if(index<=0){
        for(unordered_map<string,rEdge*>::iterator itr=eList.begin(); itr!=eList.end(); itr++){
            rEdge* e = itr->second;
            
            if(!e->getMark())
                e->setScoreOfCell(e->getWeight(0), index);
        }
    }else{
        for(unordered_map<string,rEdge*>::iterator itr=eList.begin(); itr!=eList.end(); itr++){
            rEdge* e = itr->second;
            
            // 10->11->10
            if(e->getID().compare("106#81")==0||e->getID().compare("81#107")==0||e->getID().compare("107#82")==0){
             cout<<"";
            }
            
            if(!e->getMark() && index<e->getDepth()){
                float score = e->getWeight(0)*log(index+2);
                rNode* toNode = e->getFromNode();
                float maxScore = 0.0;
                bool first = true;
                rEdge* eMax = NULL;
                
                // Compute max predecessor
                for(unsigned long i=0; i<toNode->getTotalInDegree(); i++){
                    rNode* fNode = toNode->getPrevNode(i);
                    
                    string key = to_string(fNode->getID())+"#"+to_string(toNode->getID());
                    rEdge* predEdge = this->edges.getValue(key);
                    
                    rEdge* refEdge = predEdge->getRefEdge();
                    if(refEdge == NULL) refEdge=predEdge;
                    
                    if(refEdge->matchSignature(e)){
                        float tmpScore = predEdge->getScoreOfCell(index-1) + score;
                        
                        if(first){ maxScore = tmpScore; first=false; eMax = predEdge;}
                        else if(maxScore<tmpScore){ maxScore = tmpScore; eMax = predEdge;}
                    }
                }
                
                e->setScoreOfCell(maxScore, index);
                e->setCostTableCellOfIndex(eMax->getCostTableCellOfIndex(index-1), index);
                
                if(index+1>=Config::MINIMUM_PATH_LENGTH){
                    if(this->maxScoredCell == NULL)this->maxScoredCell = e->getCostTableCellOfIndex(index);
                    else if(this->maxScoredCell->getScore()<maxScore) this->maxScoredCell = e->getCostTableCellOfIndex(index);
                }
            }
        }
    }
}

void IsolatedSubGraph::computeCostTable(){
    this->maxScoredCell = NULL;
    
    for(unsigned long i=0;i<this->maxDepth; i++){
        this->computeCostTableForColumn(i);
    }
}

void IsolatedSubGraph::createEdgeCostList(){
    unordered_map<string,rEdge*> eList = this->edges.getMap();
    unordered_map<string,rEdge*>::iterator eitr;
    
    for(eitr=eList.begin(); eitr!=eList.end(); eitr++){
        rEdge* e = eitr->second;
        
        e->destroyCells();
        
        if(!e->getMark()){
            e->createCells();
        }
    }
}

void IsolatedSubGraph::printTable(){
    unordered_map<string,rEdge*> eList = this->edges.getMap();
    unordered_map<string,rEdge*>::iterator eitr;
    cout<<endl<<"Cost table:\n";
    for(eitr=eList.begin(); eitr!=eList.end(); eitr++){
        rEdge* e = eitr->second;
        
        if(!e->getMark()){
            cout<<e->getID()<<": ";
            for(unsigned long i=0; i<e->getDepth(); i++){
                if(i>0)cout<<", ";
                cout<<e->getCostTableCellOfIndex(i)->getScore();
            }
            cout<<endl;
        }
    }
}

void IsolatedSubGraph::getMaximumInterestingPath(CustomList<InterestingPath>* IPs){
    //printTable();
    
    
    InterestingPath path;
    CostTableCell* cell = this->maxScoredCell;
    unsigned long pathLength = 0;
    
    // Assign max score of this interesting path
    if(cell->getCell()!=NULL)path.setScore(cell->getScore());
    
    // Recover the interesting path
    while(cell->getCell()!=NULL){
        path.addToPath(cell->getEdgeID());
        rEdge* e = this->edges.getValue(cell->getEdgeID());
        e->setMark(true);
        
        cell = cell->getCell();
        pathLength++;
    }
    
    path.addToPath(cell->getEdgeID());
    rEdge* e = this->edges.getValue(cell->getEdgeID());
    e->setMark(true);
    pathLength++;
    
    if(pathLength>=Config::MINIMUM_PATH_LENGTH) IPs->addToList(path);
     
}

bool IsolatedSubGraph::EdgeBelongsInPathList(string eID, CustomList<InterestingPath>* IPs){
    list<InterestingPath>* IPList = &IPs->getList();
    
    for(list<InterestingPath>::iterator itr=IPList->begin(); itr!=IPList->end(); itr++){
        InterestingPath* p = &(*itr);
        if(p->PathContainsEdge(eID)/*&&(p->getRank()==1||p->getRank()==2)*/) return true;
    }
    
    return false;
}

string IsolatedSubGraph::getEdgeColor(string eID, CustomList<InterestingPath>* IPs){
    list<InterestingPath>* IPList = &IPs->getList();
    
    for(list<InterestingPath>::iterator itr=IPList->begin(); itr!=IPList->end(); itr++){
        InterestingPath* p = &(*itr);
        if(p->PathContainsEdge(eID) /*&& (p->getRank()==1||p->getRank()==2)*/)return p->getColor();
    }
    
    return "";
}

void IsolatedSubGraph::addPointToNode(unsigned long id, Phenotype *ph){
    this->addNode(id);
    rNode* aNode = this->getNodeById(id);
    aNode->addPoint(ph);
}

void IsolatedSubGraph::addEdge(unsigned long id1, unsigned long id2){
    if(this->nodes.hasItem(id1) && this->nodes.hasItem(id2)){
        rEdge* e = new rEdge(this->getNodeById(id1), this->getNodeById(id2));
        string n1 = to_string(e->getFromNode()->getID());
        string n2 = to_string(e->getToNode()->getID());
        string key = n1+"#"+n2;
        
        this->edges.addToMap(key, e);
    }
}

void IsolatedSubGraph::setupLinks(){
    unordered_map<unsigned long,rNode*> nList = this->nodes.getMap();
    unordered_map<unsigned long,rNode*>::iterator nitr;
    
    unordered_map<string,rEdge*> eList = this->edges.getMap();
    unordered_map<string,rEdge*>::iterator eitr;
    
    for(nitr=nList.begin(); nitr!=nList.end(); nitr++){
        rNode* n = nitr->second;
        n->resetLinkNodes();
    }
    
    for(eitr=eList.begin(); eitr!=eList.end(); eitr++){
        rEdge* e = eitr->second;
        
        if(e->getID().compare("35#36")==0){
            cout<<"";
        }
        
        e->getFromNode()->addOutDegree();
        e->getToNode()->addInDegree();
    }
    
    for(nitr=nList.begin(); nitr!=nList.end(); nitr++){
        rNode* n = nitr->second;
        
        n->createLinkNodes();
    }
    
    for(eitr=eList.begin(); eitr!=eList.end(); eitr++){
        rEdge* e = eitr->second;
    
        e->getFromNode()->addNextNode(e->getToNode());
        e->getToNode()->addPrevNode(e->getFromNode());
    }
    
    // Adjust size and weight range
    this->setRange();
}

unsigned long IsolatedSubGraph::computeMaxLengthPath(){
    this->runBFS();
    
    unsigned long length = 0;
    unordered_set<string> tSet = this->terminalEdgeSet.getSet();
    
    for(unordered_set<string>::iterator itr=tSet.begin(); itr!=tSet.end(); itr++){
        string eID = *itr;
        rEdge* e = this->edges.getValue(eID);
        
        if(length==0 || length<e->getDepth()) length = e->getDepth();
    }
    
    this->maxDepth = length;
    return length;
}

string IsolatedSubGraph::getGMLNodes(){
    string nodes = "";
    unordered_map<unsigned long, rNode*> nodeMap = this->nodes.getMap();
    
    RGBColor rgbColor;
    rgbColor.SetGamma(0.1);
    rgbColor.SetRange(this->globalNodeWeightRange[0][0], this->globalNodeWeightRange[0][1]);
    
    for(unordered_map<unsigned long, rNode*>::iterator itr=nodeMap.begin(); itr!=nodeMap.end(); itr++){
        rNode* n = itr->second;
        
        string color = rgbColor.GetColor(n->getWeight(0));
        string label = fixPrecision(n->getWeight(0), 2);
        
        nodes += "node\n[\n id " + to_string(n->getID()) + "\n label \"" + to_string(n->getID()) + "\" \n size " + to_string(20) + "\n color \"" + "#ffffff" + "\" \n shape \"" + "circle" + "\" \n]\n";
    }
    
    return nodes;
}

string IsolatedSubGraph::getGMLEdges(CustomList<InterestingPath>* IPs){
    string edges = "";
    unordered_map<string, rEdge*> edgeMap = this->edges.getMap();
    
    for(unordered_map<string, rEdge*>::iterator itr=edgeMap.begin(); itr!=edgeMap.end(); itr++){
        rEdge* e = itr->second;
        string sig;
        e->getSignature(sig);
        string label = sig;
        
        if(Config::FILTER==1){
            edges += "edge\n[\n source " + to_string(e->getFromNode()->getID()) + "\n target " + to_string(e->getToNode()->getID()) + "\n width " + to_string(2) + "\n label \"" + "" + "\" \n color \"" + Config::EDGE_COLOR + "\"\n]\n";
        }else{
            string color = this->getEdgeColor(e->getID(), IPs);
            bool edgeInAPath = this->EdgeBelongsInPathList(e->getID(), IPs);
            
            if(edgeInAPath){
                edges += "edge\n[\n source " + to_string(e->getFromNode()->getID()) + "\n target " + to_string(e->getToNode()->getID()) + "\n width " + to_string(Config::EDGE_WIDTH) + "\n label \"" + label + "\" \n color \"" + color + "\"\n]\n";
            }else{
                edges += "edge\n[\n source " + to_string(e->getFromNode()->getID()) + "\n target " + to_string(e->getToNode()->getID()) + "\n width " + to_string(2) + "\n label \"" + label + "\" \n color \"" + Config::EDGE_COLOR + "\"\n]\n";
            }
        }
    }
    
    return edges;
}

void IsolatedSubGraph::getNodeSizeAndWeightRange(float *size, float **weight){
    if(size!=NULL){
        if(size[0] > this->nodeSizeRange[0]) size[0]=this->nodeSizeRange[0];
        if(size[1] < this->nodeSizeRange[1]) size[1]=this->nodeSizeRange[1];
    }
    
    if(weight!=NULL){
        for(int i=0;i<=Config::FILTER; i++){
            if(weight[i]!=NULL){
                if(weight[i][0] > this->nodeWeightRange[i][0]) weight[i][0] = this->nodeWeightRange[i][0];
                if(weight[i][1] < this->nodeWeightRange[i][1]) weight[i][1] = this->nodeWeightRange[i][1];
            }
        }
    }
}

void IsolatedSubGraph::setGlobalSizeRange(float size, short index){
    this->globalNodeSizeRange[index] = size;
}

void IsolatedSubGraph::setGlobalWeightRange(float *size, short index){
    this->globalNodeWeightRange[index][0] = size[0];
    this->globalNodeWeightRange[index][1] = size[1];
}

/****** Cost table related methods *******/

void IsolatedSubGraph::computeInterestingPaths(CustomList<InterestingPath>* IPs){
    
    unordered_map<string,rEdge*> eList = this->edges.getMap();
    unordered_map<string,rEdge*>::iterator eitr;
    
    while(true){
        for(eitr=eList.begin(); eitr!=eList.end(); eitr++){
            rEdge* e = eitr->second;
            
            if(e->getMark()){
                rNode* n1 = e->getFromNode();
                rNode* n2 = e->getToNode();
                
                n1->removeNextNode(n2);
                n2->removePrevNode(n1);
            }
        }
        
        // Compute the max length path
        unsigned long pathLength = this->computeMaxLengthPath();
        
        if(pathLength<Config::MINIMUM_PATH_LENGTH) break;
        
        // Create cost table
        this->createEdgeCostList();
        
        // Compute score in cost table
        this->computeCostTable();
        
        // Generate max-IP
        this->getMaximumInterestingPath(IPs);
    }
}

void IsolatedSubGraph::recoverNodeLinks(){
    unordered_map<string,rEdge*> eList = this->edges.getMap();
    
    for(unordered_map<string,rEdge*>::iterator itr=eList.begin(); itr!=eList.end(); itr++){
        rEdge* e = itr->second;
        
        if(e->getMark()){
            rNode* fn = e->getFromNode();
            rNode* tn = e->getToNode();
            
            fn->addOutDegree();
            fn->addNextNode(tn);
            
            tn->addInDegree();
            tn->addPrevNode(fn);
        }
    }
}

bool IsolatedSubGraph::addRankColorToEdge(string edgeID, string color, unsigned long rank){
    if(this->edges.hasItem(edgeID)){
        rEdge* e = this->edges.getValue(edgeID);
        
        e->setColor(color);
        e->setIPRank(rank);
        
        return true;
    }
    
    return false;
}

void IsolatedSubGraph::getD3Nodes(CustomList<string> *nodeListJS){
    
    for(int i=0; i<=Config::FILTER; i++){
        cout<<this->globalNodeWeightRange[i][0]<<","<<this->globalNodeWeightRange[i][1]<<endl;
    }
    
    unordered_map<unsigned long, rNode*> nodeMap = this->nodes.getMap();
    
    RGBColor *rgbColor = new RGBColor[Config::FILTER+1];
    string filter_name = "[\"phenotype\"";
    for(short i=0; i<=Config::FILTER; i++){
        rgbColor[i].SetGamma(0.1);
        
        if(Config::HEATMAP_EACH_CC){
            rgbColor[i].SetRange(this->nodeWeightRange[i][0], this->nodeWeightRange[i][1]);
        }else{
            rgbColor[i].SetRange(this->globalNodeWeightRange[i][0], this->globalNodeWeightRange[i][1]);
        }
        
        if(i>0)filter_name += ",\"" + Config::FILTER_NAMES[i-1] + "\"";
    }
    filter_name += "]";
    
    for(unordered_map<unsigned long, rNode*>::iterator itr=nodeMap.begin(); itr!=nodeMap.end(); itr++){
        rNode* n = itr->second;
        
        string color = "[", label = "[";
        for(short i=0; i<=Config::FILTER; i++){
            if(color.length()>1) color += ",";
            if(label.length()>1) label += ",";

            color += "\"" + rgbColor[i].GetColor(n->getWeight(i)) + "\"";
            label += "\"" + fixPrecision(n->getWeight(i), 2) + "\"";
        }
        color += "]";
        label += "]";
        
        string node = "{\"Id\":" + to_string(n->getID()) + ",\"Attr\":" + filter_name + ",\"Size\":" +
            fixPrecision(adjustSize(n->getSize()), 2) + ",\"Color\":" + color + ",\"Label\":" + label + ",\"Ph\":" +
            n->getPhListForD3() + "}";
        
        nodeListJS->addToList(node);
    }
    
    delete [] rgbColor;
}

void IsolatedSubGraph::getD3Edges(CustomList<string> *edgeListJS, CustomList<InterestingPath>* IPs){
    unordered_map<string, rEdge*> edgeMap = this->edges.getMap();
    
    for(unordered_map<string, rEdge*>::iterator itr=edgeMap.begin(); itr!=edgeMap.end(); itr++){
        rEdge* e = itr->second;
        string label;
        e->getSignature(label);
        string edges = "";
        
        if(Config::FILTER==1){
            edges = "{\"source\": "+to_string(e->getFromNode()->getID())+", \"target\":" + to_string(e->getToNode()->getID())+
                ",\"L\":\"" + label + "\", \"C\":\"" + Config::EDGE_COLOR + "\",\"W\":2}";
        }else{
            string color = e->getColor(); //this->getEdgeColor(e->getID(), IPs);
            size_t rank = e->getIPRank(); //this->EdgeBelongsInPathList(e->getID(), IPs);
            
            if(rank>0){
                edges = "{\"source\": "+to_string(e->getFromNode()->getID())+", \"target\":" + to_string(e->getToNode()->getID())+
                ",\"L\":\"" + label + "\", \"C\":\"" + color + "\",\"W\":" + to_string(Config::EDGE_WIDTH) + ",\"R\":" + to_string(rank) + "}";
            }else{
                edges = "{\"source\": "+to_string(e->getFromNode()->getID())+", \"target\":" + to_string(e->getToNode()->getID())+
                    ",\"L\":\"" + label + "\", \"C\":\"" + Config::EDGE_COLOR + "\",\"W\":2,\"R\":0}";
            }
        }
        
        edgeListJS->addToList(edges);
    }
}

float IsolatedSubGraph::adjustSize(float size){
    float max = Config::NODE_SIZE_MAX, min = Config::NODE_SIZE_MIN;
    
    float alpha = (this->nodeSizeRange[1]==this->nodeSizeRange[0])?0:(size-this->nodeSizeRange[0])/(this->nodeSizeRange[1]-this->nodeSizeRange[0]);
    
    // Special case for small CCs
    if((this->nodeSizeRange[1]-this->nodeSizeRange[0])<1) alpha=0;
    
    return (alpha*(max-min)+min);
}

///////////////////////////// Graph class methods ////////////////////////////
Graph::Graph(short totalCC){
    this->totalCC = totalCC;
    this->connectedComponent = new IsolatedSubGraph[this->totalCC];
    
    this->globalNodeWeightRange = (float**)malloc(sizeof(float*)*(Config::FILTER+1));
    
    for(short i=0;i<=Config::FILTER;i++){
        this->globalNodeWeightRange[i] = (float*)malloc(sizeof(float)*2);
        
        this->globalNodeWeightRange[i][0]=this->globalNodeWeightRange[i][1]=0.0;
    }
}

Graph::~Graph(){
    delete [] this->connectedComponent;
    
    for(short i=0;i<=Config::FILTER;i++){
        free(this->globalNodeWeightRange[i]);
    }
    free(this->globalNodeWeightRange);
}

IsolatedSubGraph* Graph::getConnectedComponent(short index){
    return &this->connectedComponent[index];
}

void Graph::generatePathColor(){
    list<InterestingPath>* IPList = &this->IPs.getList();
    IPList->sort(comparePaths);
    
    unsigned long r = 1;
    for(list<InterestingPath>::iterator itr=IPList->begin(); itr!=IPList->end(); itr++, r++){
        InterestingPath *ip = &(*itr);
        ip->setRank(r);
    }
    
    unsigned long totalPaths=IPList->size();
    RGB *rgb = (RGB*)malloc(sizeof(RGB)*totalPaths);
    RGB ra(255,0,0), rb(255,255,0), rc(0,255,0), rd(0,255,255), re(0,0,255);
    
    if(totalPaths<=5){
        switch (totalPaths) {
            case 1:
                rgb[0] = ra;
                break;
                
            case 2:
                rgb[0] = ra;
                rgb[1] = re;
                break;
                
            case 3:
                rgb[0] = ra;
                rgb[1] = rc;
                rgb[2] = re;
                break;
                
            case 4:
                rgb[0] = ra;
                rgb[1] = rb;
                rgb[2] = rd;
                rgb[3] = re;
                break;
                
            case 5:
                rgb[0] = ra;
                rgb[1] = rb;
                rgb[2] = rc;
                rgb[3] = rd;
                rgb[4] = re;
                break;
                
            default:
                break;
        }
    }else{
        int a = 3*ceil(totalPaths/4);
        int b = 3*floor(totalPaths/4);
        int interval = 0;
        
        if(totalPaths-a > 4 && totalPaths-b>4)
            interval = ceil(totalPaths/4);
        else
            interval = floor(totalPaths/4);
        
        rgb[0] = ra;
        rgb[interval] = rb;
        rgb[2*interval] = rc;
        rgb[3*interval] = rd;
        rgb[totalPaths-1] = re;
        
        int counter = 0;
        float ta=0, tb=0, tc=0, t1=0, t2=0;
        for(int i=1; i<=totalPaths; i++,counter++){
            
            if(counter==0){
                ta = i;
            }else if(counter == interval){
                t1 = i;
            }else if(counter == (2*interval)){
                tb = i;
            }else if(counter == (3*interval)){
                t2 = i;
            }else if(counter == totalPaths-1){
                tc = i;
            }
        }
        
        counter=0;
        for(int i=1; i<=totalPaths; i++,counter++){
            
            if(counter>0 && counter<interval){
                
                float d1 = ABS(ta-t1);
                float d2 = ABS(ta-i);
                short gray = ceil((255*d2)/d1);
                
                RGB r(255,gray,0);
                rgb[counter] = r;
                
            }else if(counter>interval && counter<(2*interval)){
                
                float d1 = ABS(t1-tb);
                float d2 = ABS(tb-i);
                short red = ceil((255*d2)/d1);
                
                RGB r(red,255,0);
                rgb[counter] = r;
            }else if(counter>(2*interval) && counter<(3*interval)){
                
                float d1 = ABS(t2-tb);
                float d2 = ABS(tb-i);
                short blue = ceil((255*d2)/d1);
                
                RGB r(0,255,blue);
                rgb[counter] = r;
            }else if(counter>(3*interval) && counter<totalPaths-1){
                
                float d1 = ABS(t2-tc);
                float d2 = ABS(tc-i);
                short grey = ceil((255*d2)/d1);
                
                RGB r(0,grey,255);
                rgb[counter] = r;
            }
        }
    }
    
    unsigned int pathCounter=0;
    RGBColor c;
    
    for(list<InterestingPath>::iterator itr=IPList->begin(); itr!=IPList->end(); itr++, pathCounter++){
        RGB r = rgb[pathCounter];
        short R=r.getR(),G=r.getG(), B=r.getB();
        
        InterestingPath* p = &(*itr);
        p->setColor(c.RGBtoHex(R,G,B));
        
        unordered_set<string> *edgeInAPath = &p->getPath();
        for(unordered_set<string>::iterator epItr=edgeInAPath->begin(); epItr!=edgeInAPath->end(); epItr++){
            string key = *epItr;
            
            for(short i=0;i<this->totalCC;i++){
                if(this->connectedComponent[i].addRankColorToEdge(key, p->getColor(), p->getRank())) break;
            }
        }
    }
    
    free(rgb);
}

void Graph::printGMLGraph(){
    this->generatePathColor();
    
    string nodes = "", edges = "";
    string gmlData = "graph\n[\ndirected 1\nweighted 1\n";
    
    for(short i=0;i<this->totalCC;i++){
        nodes += this->connectedComponent[i].getGMLNodes();
        edges += this->connectedComponent[i].getGMLEdges(&this->IPs);
    }
    
    gmlData += nodes + "\n" + edges + "]";
    
    FileHandler* fileHandler = new FileHandler("");
    fileHandler->WriteDataToFile("GML_PathLength", "gml", gmlData, false);
    delete fileHandler;
}

void Graph::setGlobalNodeSizeAndWeightRange(float *size, float **weight){
    if(size!=NULL){
        this->globalNodeSizeRange[0] = size[0];
        this->globalNodeSizeRange[1] = size[1];
    }
    
    if(weight!=NULL){
        for(short i=0;i<=Config::FILTER; i++){
            if(weight[i]!=NULL){
                this->globalNodeWeightRange[i][0] = weight[i][0];
                this->globalNodeWeightRange[i][1] = weight[i][1];
            }
        }
    }
}

void Graph::generateInterestingPaths(){
    for(short i=0;i<this->totalCC;i++){
        this->connectedComponent[i].computeInterestingPaths(&this->IPs);
    }
}

void Graph::printD3Graph(){
    
    for(int i=0; i<=Config::FILTER; i++){
        cout<<this->globalNodeWeightRange[i][0]<<","<<this->globalNodeWeightRange[i][1]<<endl;
    }
    
    
    this->generatePathColor();
    
    string d3Data = "";
    string d3Nodes = "\"nodes\": [\n\t";
    
    CustomList<string> nodeListJS, edgeListJS;
    
    for(short i=0;i<this->totalCC;i++){
        this->connectedComponent[i].getD3Nodes(&nodeListJS);
        this->connectedComponent[i].getD3Edges(&edgeListJS, &this->IPs);
    }
    
    if(nodeListJS.getSize()>0 && edgeListJS.getSize()>0){
        string d3Data = "";
        string d3Nodes = "\"nodes\": [\n\t";
        
        list<string> d3NodeList = nodeListJS.getList();
        
        for(list<string>::iterator d3Itr=d3NodeList.begin(); d3Itr!=d3NodeList.end(); d3Itr++){
            if(d3Nodes.find("{")!=string::npos) d3Nodes += ",\n\t";
            
            d3Nodes += *d3Itr;
        }
        
        d3Nodes += "]";
        
        string d3Edges = "\"links\": [\n\t";
        
        list<string> d3EdgeList = edgeListJS.getList();
        
        for(list<string>::iterator d3Itr=d3EdgeList.begin(); d3Itr!=d3EdgeList.end(); d3Itr++){
            if(d3Edges.find("{")!=string::npos) d3Edges += ",\n\t";
            
            d3Edges += *d3Itr;
        }
        
        d3Edges += "]";
        
        d3Data = "{" + d3Nodes + ",\n" + d3Edges + "}";
        
        float ovl = (Config::OVERLAP<0.95)?Config::OVERLAP/2:0.50;
        
        FileHandler* writeToFile = new FileHandler("");
        writeToFile->WriteDataToFile("Data_"+to_string(Config::WINDOW_X)+"_"+to_string(Config::WINDOW_Y) + "_" + fixPrecision(ovl, 2), ".json", d3Data, false);
        delete writeToFile;
    }
    
    //FileHandler* fileHandler = new FileHandler("");
    //fileHandler->WriteDataToFile("GML_PathLength", "gml", gmlData, false);
    //delete fileHandler;
}
