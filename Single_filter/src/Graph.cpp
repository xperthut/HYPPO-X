/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: Graph.cpp
 Objective: Implementation of member functions of all classes
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              02/16/2017      Initial version
 
 **************************************************************************************************/

#include "Graph.h"

//#pragma Environment_class
Environment::Environment(){
    for(int i=0; i<FILTER; i++){
        weight[i] = 0.0;
    }
}

Environment::~Environment(){}

float Environment::getWeight(int index){
    if(index>=0 && index<FILTER)
        return weight[index];
    
    return -1;
}

void Environment::setWeight(float value, int index){
    if(index>=0 && index<FILTER)
        weight[index]=value;
}

//#pragma rNode_class

rNode::rNode(rNode* node){
    this->id = node->id;
    this->weight = node->weight;
    this->size = node->size;
    this->in_degree = node->in_degree;
    this->out_degree = node->out_degree;
    this->isSource = node->isSource;
    this->nextNodes = node->nextNodes;
    this->prevNodes = node->prevNodes;
    this->isMarked = node->isMarked;
    
    for(int i=0; i<FILTER; i++){
        this->env.setWeight(node->getEnvWeight(i), i);
    }
}

rNode::rNode(long id, float weight, int size){
    this->id = id;
    this->weight = weight;
    this->size = size;
    in_degree = out_degree = 0;
    isSource = false;
    isMarked = false;
    nextNodes = NULL;
    prevNodes = NULL;
}

rNode::~rNode(){
    if(nextNodes != NULL) free(nextNodes);
    if(prevNodes != NULL) free(prevNodes);
}

// Getter
long rNode::getID(){return id;}
float rNode::getWeight(){return weight;}
float rNode::getSize(){return size;}
int rNode::getTotalInDegree(){return in_degree;}
int rNode::getTotalOutDegree(){return out_degree;}
float rNode::getEnvWeight(int index){return env.getWeight(index);}
bool rNode::IsThisNodeASource(){return isSource;}
short rNode::getVariations(){return variation;}
unordered_map<string, list<float> >& rNode::getPhenotyeSummary(){return phenotypeSummary.getMap();}
string rNode::getNodeJS(){
    string js = "[";
    
    unordered_map<string, list<float> > tmpSummary = phenotypeSummary.getMap();
    long index[]={0,0,0,0};
    
    for(unordered_map<string, list<float> >::iterator itr=tmpSummary.begin(); itr!=tmpSummary.end(); itr++){
        string key = itr->first;
        
        size_t pos = key.find("#");
        string gen = key.substr(0,pos);
        key = key.substr(pos+1, key.length()-pos-1);
        
        pos = key.find("#");
        string loc = key.substr(0,pos);
        key = key.substr(pos+1, key.length()-pos-1);
        
        short k = getGenLocWeight(gen, loc);
        if(k>=0&&k<4) index[k] += itr->second.size();
    }
    
    string n="";
    for(short i=3; i>=0; i--){
        if(index[i]>0){
            if(js.find("}")!=string::npos) js += ",";
            switch (i) {
                case 0:
                    js += "{label: \"AK\",value: " + to_string(index[i]) + ",color: '#0000ff'}";
                    break;
                case 1:
                    js += "{label: \"AN\",value: " + to_string(index[i]) + ",color: '#00ff00'}";
                    break;
                case 2:
                    js += "{label: \"BN\",value: " + to_string(index[i]) + ",color: '#E67E22'}";
                    break;
                case 3:
                    js += "{label: \"BK\",value: " + to_string(index[i]) + ",color: '#ff0000'}";
                    break;
                    
                default:
                    break;
            }
        }
    }
    
    return (js+"]");
}

// Setter
void rNode::setID(long id){this->id = id;}
void rNode::setWeight(float weight){this->weight = weight;}
void rNode::setSize(float size){this->size = size;}
void rNode::addInDegree(){in_degree++;}
void rNode::addOutDegree(){out_degree++;}
void rNode::setEnvWeight(float weight, int index){return env.setWeight(weight, index);}
void rNode::setSource(bool source){isSource = source;}
void rNode::setVariations(short v){variation=v;}

void rNode::setPhenotypeSummary(string genotype, string location, long plantID, int dap, float filter, float value){
    string key = genotype+"#"+location+"#"+to_string(dap)+"#"+to_string(plantID) + "#" + fixPrecision(filter, 2);
    
    unordered_map<string, list<float> > tmpSummary = phenotypeSummary.getMap();
    unordered_map<string, list<float> >::iterator itr;
    
    if(!phenotypeSummary.hasItem(key)){
        list<float> alist;
        alist.push_back(value);
        
        phenotypeSummary.addToMap(key, alist);
    }else{
        list<float> alist= phenotypeSummary.getValue(key);
        
       alist.push_back(value);
    }
}

string rNode::getGenLoc(){
    unordered_map<string, list<float> > tmpSummary = phenotypeSummary.getMap();
    long index[]={0,0,0,0};
    
    for(unordered_map<string, list<float> >::iterator itr=tmpSummary.begin(); itr!=tmpSummary.end(); itr++){
        string key = itr->first;
        
        size_t pos = key.find("#");
        string gen = key.substr(0,pos);
        key = key.substr(pos+1, key.length()-pos-1);
        
        pos = key.find("#");
        string loc = key.substr(0,pos);
        key = key.substr(pos+1, key.length()-pos-1);
        
        short k = getGenLocWeight(gen, loc);
        if(k>=0 && index[k]==0) index[k] += itr->second.size();
    }
    
    string n="";
    for(short i=4; i>=0; i--){
        if(index[i]>0){
            if(n.length()>0)n+= ":";
            switch (i) {
                case 0:
                    n += "AK";
                    break;
                case 1:
                    n += "AN";
                    break;
                case 2:
                    n += "BN";
                    break;
                case 3:
                    n += "BK";
                    break;
                    
                default:
                    break;
            }
        }
    }
    
    return n;
}

int rNode::getGenLocWeight(string gen, string loc){
    //A
    if(gen.compare("F42 x DJ7")==0||gen.compare("A")==0){
        if(loc.compare("KS")==0||loc.compare("K")==0) return 0;
        else if(loc.compare("NE")==0||loc.compare("N")==0) return 1;
    }
    //B
    else{
        if(loc.compare("NE")==0||loc.compare("N")==0) return 2;
        else if(loc.compare("KS")==0||loc.compare("K")==0) return 3;
    }
    
    return -1;
}

int rNode::getGenLocColorIndex(){
    unordered_map<string, list<float> > tmpSummary = phenotypeSummary.getMap();
    short index[]={0,0,0,0};
    
    for(unordered_map<string, list<float> >::iterator itr=tmpSummary.begin(); itr!=tmpSummary.end(); itr++){
        string key = itr->first;
        
        size_t pos = key.find("#");
        string gen = key.substr(0,pos);
        key = key.substr(pos+1, key.length()-pos-1);
        
        pos = key.find("#");
        string loc = key.substr(0,pos);
        key = key.substr(pos+1, key.length()-pos-1);
        
        short k = getGenLocWeight(gen, loc);
        if(k>=0 && index[k]==0) index[k]=1;
    }
    
    // AK=0, AN=1, BN=2,  BK=3
    // AKAN=4 AKBN=5 AKBK=6 ANBN=7 ANBK=8 BNBK=9
    // AKANBN=10 AKANBK=11 AKBNBK=12 ANBNBK=13
    // AKANBNBK=14
    
    //Weight: AK=1, AN=2, BN=3, BK=6
    
    string n="0";
    int powN=0;
    for(short i=3; i>=0; i--){
        if(index[i]==1){
            n += to_string(i);
            
            if(i==3) powN += 600;
            else if(i==2) powN += 250;
            else if(i==1) powN += 150;
            else if(i==0) powN += 100;
        }
    }
    
    //(1+n)*5
    switch (stoi(n)) {
        case 10:
            return 5*powN;
        case 20:
            return 6*powN;
        case 30:
            return 7*powN;
        case 21:
            return 8*powN;
        case 31:
            return 9*powN;
        case 32:
            return 10*powN;
        case 210:
            return 11*powN;
        case 310:
            return 12*powN;
        case 320:
            return 13*powN;
        case 321:
            return 14*powN;
        case 3210:
            return 15*powN;
        default:
            break;
    }
    
    
    return powN*(1+stoi(n));
}

void rNode::createLinkNodes(){
    if(out_degree>0){
        nextNodes = (rNode**)malloc(sizeof(rNode*)*out_degree);
        
        for(int i=0; i<out_degree; i++){
            nextNodes[i] = NULL;
        }
    }
    
    if(in_degree>0){
        prevNodes = (rNode**)malloc(sizeof(rNode*)*in_degree);
        
        for(int i=0; i<in_degree; i++){
            prevNodes[i] = NULL;
        }
    }
}

void rNode::addNextNode(rNode *nextNode){
    for(int i=0; i<out_degree; i++){
        if(nextNodes[i] == NULL){
            nextNodes[i] = nextNode;
            break;
        }
    }
}

void rNode::addPrevNode(rNode *prevNode){
    for(int i=0; i<in_degree; i++){
        if(prevNodes[i] == NULL){
            prevNodes[i] = prevNode;
            break;
        }
    }
}

rNode* rNode::getNextNode(int index){
    return nextNodes[index];
}

rNode* rNode::getPrevNode(int index){
    return prevNodes[index];
}

void rNode::resetLinkNodes(){
    
    for(int i=0; i<in_degree; i++){
        prevNodes[i] = NULL;
    }
    
    for(int i=0; i<out_degree; i++){
        nextNodes[i] = NULL;
    }
    
    if(nextNodes != NULL) free(nextNodes);
    if(prevNodes != NULL) free(prevNodes);
    
    prevNodes = NULL;
    nextNodes = NULL;
    
    in_degree = out_degree = 0;
}

//#pragma rEdge_class

rEdge::rEdge(rNode* node1, rNode* node2){
    if(node1->getWeight() > node2->getWeight()){
        fromNode = node2;
        toNode = node1;
    }else{
        fromNode = node1;
        toNode = node2;
    }
    
    signature.set();
    weight = 0.0;
    score = 0.0;
    rank = 0;
    isMarked = false;
    refEdge = NULL;
}

rEdge::~rEdge(){
    //if(fromNode!=NULL) delete fromNode;
    //if(toNode!=NULL) delete toNode;
}

// Getter
rNode* rEdge::getFromNode(){return fromNode;}
rNode* rEdge::getToNode(){return toNode;}
float rEdge::getWeight(){return weight;}
float rEdge::getScore(){return score;}
int rEdge::getRank(){return rank;}
bool rEdge::getMark(){return isMarked;}
void rEdge::getSignature(unsigned long& sig){sig = signature.to_ulong();}
void rEdge::getSignature(string &sig){sig = signature.to_string();}
float rEdge::getNormalizedWeight(int index){
    if(index>=0 && index<=FILTER){
        return normalizeWeight[index];
    }
    
    return -1;
}
float rEdge::getEdgeWeight(int index){
    if(index>=0 && index<=FILTER){
        return edgeWeight[index];
    }
    
    return -1;
}
rEdge* rEdge::getRefEdge(){return refEdge;}

// Setter
void rEdge::setScore(float score){this->score = score;}
void rEdge::setRank(int rank){this->rank = rank;}
void rEdge::setWeight(float weight){this->weight = weight;}
void rEdge::setMark(bool mark){isMarked = mark;}
void rEdge::setSignatue(bool value, int index){
    if(index>=0 && index<FILTER){
        signature.set(FILTER-index-1,value);
    }
}
void rEdge::setNormalizedWeight(float value, int index){
    if(index>=0 && index<=FILTER){
        normalizeWeight[index]=value;
    }
}
void rEdge::setEdgeWeight(float value, int index){
    if(index>=0 && index<=FILTER){
        edgeWeight[index]=value;
    }
}
void rEdge::setRefEdge(rEdge *e){refEdge=e;}

void rEdge::swapNodes(){
    rNode* tmp = fromNode;
    fromNode = toNode;
    toNode = tmp;
}

bool rEdge::matchSignature(rEdge* e){
    
    unsigned long s, sig;
    getSignature(s);
    e->getSignature(sig);
    
    if(!MATCH_SIGNATURE) return true;
    
    if(DELTA_CHANGE==0){
        return (s==sig);
    }else{
        if(s==sig) return true;
        else{
            bool mismatch = true;
            float wDiff = 0;
            
            for(int index=0; index<FILTER; index++){
                
                // Check delta threshold
                if(getSignatureAtIndex(index) != e->getSignatureAtIndex(index)){
                    
                    if(REFERENCE_PHENOTYPE){
                        wDiff = abs(getNormalizedWeight(index+1)-e->getNormalizedWeight(index+1))*100;
                        
                        mismatch = mismatch && (wDiff<=DELTA_CHANGE);
                    }else{
                        // For phenotype
                        if(index==0){
                            wDiff = abs(getNormalizedWeight(index)-e->getNormalizedWeight(index))*100;
                            
                            mismatch = mismatch && (wDiff<=DELTA_CHANGE);
                        }
                        
                        // For environmental attributes
                        if(index != REFERENCE_ENV_INDEX){
                            
                            wDiff = abs(getNormalizedWeight(index+1)-e->getNormalizedWeight(index+1))*100;
                            
                            mismatch = mismatch && (wDiff<=DELTA_CHANGE);
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
    
    if(!MATCH_SIGNATURE) return true;
    
    return (s==sig);
}

bool rEdge::getSignatureAtIndex(int index){
    if(index>=0 && index<FILTER){
        return signature[FILTER-index-1];
    }
    
    return false;
}

//#pragma Path_class
Path::Path(){
    score = 0.0;
    color = "#000000";
    width = 3;
}

Path::~Path(){}
void Path::addToPath(string id){
    path.addToSet(id);
}

unordered_set<string>& Path::getPath(){
    return path.getSet();
}

string Path::getNodeIds(){
    string str = "";
    
    unordered_set<string> p = path.getSet();
    
    for(unordered_set<string>::iterator itr=p.begin(); itr!=p.end(); itr++){
        if(str.length()>0) str += ",";
        
        str += *itr;
    }
    
    return str;
}

void Path::setScore(float score){this->score = score;}
float Path::getScore(){return score;}
void Path::setColor(string color){this->color = color;}
string Path::getColor(){return color;}
bool Path::hasNodes(string id){
    unordered_set<string> p = path.getSet();
    
    if(p.find(id) == p.end()){
        return false;
    }
    
    return true;
}

list<long> Path::getAllNodesInAPath(){
    unordered_set<string> nlist = path.getSet();
    list<long> pathNodeOrder;
    
    unordered_set<string>::iterator itr1=nlist.begin();
    string str = *itr1;
    size_t pos = str.find("#");
    
    long n1 = stol(str.substr(0,pos));
    long n2 = stol(str.substr(pos+1,str.length()-pos-1));
    
    
    if(pathNodeOrder.size()==0){
        pathNodeOrder.push_back(n1);
        pathNodeOrder.push_back(n2);
    }
    
    while(pathNodeOrder.size()<nlist.size()+1){
    
        for(unordered_set<string>::iterator itr2=nlist.begin(); itr2!=nlist.end(); itr2++){
            string str2 = *itr2;
            size_t pos2 = str2.find("#");
            
            long n_1 = stol(str2.substr(0,pos2));
            long n_2 = stol(str2.substr(pos2+1,str2.length()-pos2-1));
            
            if(n2==n_1){
                pathNodeOrder.push_back(n_2);
                n2=n_2;
            }else if(n_2==n1){
                pathNodeOrder.push_front(n_1);
                n1=n_1;
            }
        }
    }
    
    return pathNodeOrder;
}

//#pragma RGB_class
RGB::RGB(){}
RGB::RGB(short R, short G, short B){this->R=R;this->G=G;this->B=B;}
RGB::~RGB(){}

short RGB::getR(){return R;}
short RGB::getG(){return G;}
short RGB::getB(){return B;}

void RGB::setR(short R){this->R=R;}
void RGB::setG(short G){this->G=G;}
void RGB::setB(short B){this->B=B;}

//#pragma Residual_Graph_class

CompositeGraph::CompositeGraph(){
    pathCounter = 0;
}

CompositeGraph::~CompositeGraph(){}

bool CompositeGraph::hasNode(long id){
    return nodeList.hasItem(id);
}

float CompositeGraph::adjustSize(float size){
    
    int max = NODE_SIZE_MAX, min = NODE_SIZE_MIN;
    
    float alpha = (nodeSizeRange[1]==nodeSizeRange[0])?0:(size-nodeSizeRange[0])/(nodeSizeRange[1]-nodeSizeRange[0]);
    
    // Special case for small CCs
    if((nodeSizeRange[1]-nodeSizeRange[0])<1) alpha=0;
    
    return (alpha*(max-min)+min);
    
}

bool CompositeGraph::hasNode(long id1, long id2){
    
    return (nodeList.hasItem(id1) && nodeList.hasItem(id2));
    
}

rNode* CompositeGraph::getNodeById(long id){
    
    if(nodeList.hasItem(id)){
        return nodeList.getValue(id);
    }
    
    return NULL;
}

void CompositeGraph::addNode(rNode *node){
    
    nodeList.addToMap(node->getID(), node);
}

void CompositeGraph::addEdge(long id1, long id2){

    if(nodeList.hasItem(id1) && nodeList.hasItem(id2)){
        rEdge* e = new rEdge(getNodeById(id1), getNodeById(id2));
        string n1 = to_string(e->getFromNode()->getID());
        string n2 = to_string(e->getToNode()->getID());
        string key = n1+"#"+n2;
        
        edgeMap.addToMap(key, e);
    }
    
}

void CompositeGraph::updateStorageRange(float *nsRange, float *nwRange, float (*ewRange)[2]){
    nodeSizeRange[0] = nsRange[0];
    nodeSizeRange[1] = nsRange[1];
    
    nodeWeightRange[0] = nwRange[0];
    nodeWeightRange[1] = nwRange[1];
    
    for(int i=0; i<FILTER; i++){
        envWeightRange[i][0]=ewRange[i][0];
        envWeightRange[i][1]=ewRange[i][1];
    }
}

void CompositeGraph::storeRange(float *nsRange, float *nwRange, float (*ewRange)[2], string ewName[FILTER]){
    nodeSizeRange[0] = nsRange[0];
    nodeSizeRange[1] = nsRange[1];
    
    nodeWeightRange[0] = nwRange[0];
    nodeWeightRange[1] = nwRange[1];
    
    for(int i=0; i<FILTER; i++){
        envWeightRange[i][0]=ewRange[i][0];
        envWeightRange[i][1]=ewRange[i][1];
        envName[i] = ewName[i];
    }
}

void CompositeGraph::getWidthAndColor(long fromID, long toID, string *property){
    property[0] = "2";
    property[1] = EDGE_COLOR;
    string id1=to_string(fromID) + "#" + to_string(toID);
    string id2=to_string(toID) + "#" + to_string(fromID);
    
    for(int i=0,c=1; i<pathCounter; i++){
        Path* p = interestingPaths.getValue(i);
        
        if(p->getPath().size() >= MINIMUM_PATH_LENGTH){
            if(PRINT_ALL_PATHS){
                if(p->hasNodes(id1)||p->hasNodes(id2)){
                    property[0] = to_string(8);
                    property[1] = p->getColor();
                }
            }else{
                // Print only these paths
                if(c==100 || c==800){
                    
                    if(p->hasNodes(id1)||p->hasNodes(id2)){
                        property[0] = to_string(8);
                        property[1] = p->getColor();
                        
                        // Change the color
                        //Path* tmpP = interestingPaths.getValue(i+1);
                        
                        //property[1] = tmpP->getColor();
                    }
                }
            }
            
            c++;
        }
    }
}

bool CompositeGraph::getEdgeDirection(rEdge *edge, int index){
    float phW1 = edge->getFromNode()->getWeight();
    float phW2 = edge->getToNode()->getWeight();
    
    float envW1 = edge->getFromNode()->getEnvWeight(index);
    float envW2 = edge->getToNode()->getEnvWeight(index);
    
    if(phW1-phW2>0){
        if(envW1-envW2>0) return true;
    }else{
        if(envW1-envW2<=0) return true;
    }
    
    return false;
}

string CompositeGraph::printGraph(bool isPhenotype, bool isEnv, int index, string fileNameSuffix){
    string nodes = "";
    string edges = "";
    string gmlData = "";
    string type = "";
    
    unordered_map<long,rNode*> nList = nodeList.getMap();
    unordered_map<long,rNode*>::iterator nitr;
    
    unordered_map<string,rEdge*> eList = edgeMap.getMap();
    unordered_map<string,rEdge*>::iterator eitr;
    
    unordered_map<long, float*> nodePos;
    unordered_map<long, float*>::iterator npItr;
    
    FileHandler* fileHandler = new FileHandler("");
    fileHandler->readNodePosition(COORDINATE_FILE_NAME, &nodePos);
    delete fileHandler;
    
    string nodeJS="[";
    string tmpNodeJS="";
    string tmpPosJs="";
    
    RGBColor c(nodeWeightRange[0], nodeWeightRange[1], 0.1);

    for(nitr=nList.begin(); nitr!=nList.end(); nitr++){
        rNode *n = nitr->second;
        
        if(isPhenotype){
            string label = fixPrecision(n->getWeight(), 2);
            string shape = "circle";
            string size = fixPrecision(adjustSize(n->getSize()), 2);
            string nColor = c.GetColor(n->getWeight());
            
            if(PRINT_BARCODE){
                label = to_string(n->getID());
                size = "15";
                nColor = "#ffffff";
            }
            
            nodes += "node\n[\n id " + to_string(n->getID()) + "\n label \"" + label + "\" \n size " + size + "\n color \"" + nColor + "\" \n shape \"" + shape + "\" \n]\n";
            
            //nodes += "node\n[\n id " + to_string(n->getID()) + "\n label \"" + to_string(n->getID()) + "\" \n size " + fixPrecision(adjustSize(n->getSize()), 2) + "\n color \"#ffffff\" \n shape \"circle\" \n]\n";
            
            if((npItr=nodePos.find(n->getID())) != nodePos.end()){
                if(nodeJS.find("}")!=string::npos) nodeJS += ",";
                nodeJS += "{id:"+ to_string(n->getID()) + ", d:{label: 'P',value: " + label+ ",color: '" + nColor + "'}}";
            }
        }else if(isEnv){
            RGBColor ce(envWeightRange[index][0], envWeightRange[index][1], 0.1);
            string label = fixPrecision(n->getEnvWeight(index), 2);
            string shape = "circle";
            string size = fixPrecision(adjustSize(n->getSize()), 2);
            string nColor = ce.GetColor(n->getEnvWeight(index));
            
            if(n->IsThisNodeASource()){
                shape = "oval";
            }
            
            if(PRINT_BARCODE){
                label = to_string(n->getID());
                size = "15";
                nColor = "#ffffff";
            }
            
            nodes += "node\n[\n id " + to_string(n->getID()) + "\n label \"" + label + "\" \n size " + size + "\n color \"" + nColor + "\" \n shape \"" + shape + "\" \n]\n";
            
            //nodes += "node\n[\n id " + to_string(n->getID()) + "\n label \"" + to_string(n->getID()) + "\" \n size " + fixPrecision(adjustSize(n->getSize()), 2) + "\n color \"#ffffff\" \n shape \"circle\" \n]\n";
            
            if((npItr=nodePos.find(n->getID())) != nodePos.end()){
                if(nodeJS.find("}")!=string::npos) nodeJS += ",";
                nodeJS += "{id:"+ to_string(n->getID()) + ", d:{label: '" + (index==0?"T":"S")+ "',value: " + label + ",color: '" + nColor + "'}}";
            }
        }
        
    }
    
    nodeJS += "]";
    
    if(PIE_CHART_CODE&&false){
        if(isPhenotype)
            cout<<"\n\nvar ph="<<nodeJS<<";\n\n";
        else{
            cout<<"\n\nvar "<<envName[index]<<"="<<nodeJS<<";\n\n";
        }
    }
    
    string wc[2];
    long fID, tID;
    
    for(eitr=eList.begin(); eitr!=eList.end(); eitr++){
        rEdge* e = eitr->second;
        getWidthAndColor(e->getFromNode()->getID(), e->getToNode()->getID(), wc);
        
        string label = fixPrecision(e->getScore(), SHOW_PRECISION_IN_EDGE_LABEL);
        
        if(PRINT_EDGE_LABEL){
            if(EDGE_LABEL_WEIGHT){
                if(isPhenotype)
                    if(EDGE_LABEL_NORM_WEIGHT){
                        label = fixPrecision(e->getNormalizedWeight(0), SHOW_PRECISION_IN_EDGE_LABEL);
                    }else{
                        label = fixPrecision(e->getEdgeWeight(0), SHOW_PRECISION_IN_EDGE_LABEL);
                    }
                else if(isEnv){
                    if(EDGE_LABEL_NORM_WEIGHT){
                        label = fixPrecision(e->getNormalizedWeight(index+1), SHOW_PRECISION_IN_EDGE_LABEL);
                    }else{
                        label = fixPrecision(e->getEdgeWeight(index+1), SHOW_PRECISION_IN_EDGE_LABEL);
                    }
                }
            }else if(EDGE_LABEL_SCORE){
                label = fixPrecision(e->getScore(), SHOW_PRECISION_IN_EDGE_LABEL);
                
            }else if(PRINT_SIGNATURE){
                e->getSignature(label);
            }
        }else{
            label = "";
        }
        
        if(!SHOW_EDGE_LABEL_FOR_ALL_EDGES && stoi(wc[0])==1) label="";
        
        if(PRINT_BARCODE) label = "";
        
        fID = e->getFromNode()->getID();
        tID = e->getToNode()->getID();
        
        if(isEnv){
            if(!getEdgeDirection(e, index)){
                tID = e->getFromNode()->getID();
                fID = e->getToNode()->getID();
                if(label.length()>0) label="-"+label;
            }
        }
        
        edges += "edge\n[\n source " + to_string(fID) + "\n target " + to_string(tID) + "\n width " + wc[0] + "\n label \"" + label + "\" \n color \"" + wc[1] + "\"\n]\n";
        
    }
    
    gmlData = nodes+edges;
    
    return gmlData;
}

string CompositeGraph::calculateNodeDegree(bool isPhenotype, bool isEnv, int index){
    
    unordered_map<long,rNode*> nList = nodeList.getMap();
    unordered_map<long,rNode*>::iterator nitr;
    
    unordered_map<string,rEdge*> eList = edgeMap.getMap();
    unordered_map<string,rEdge*>::iterator eitr;
    
    for(nitr=nList.begin(); nitr!=nList.end(); nitr++){
        rNode* n = nitr->second;
        n->resetLinkNodes();
    }
    
    for(eitr=eList.begin(); eitr!=eList.end(); eitr++){
        rEdge* e = eitr->second;
        
        string oldID = to_string(e->getFromNode()->getID()) + "#" + to_string(e->getToNode()->getID());
        
        if(isPhenotype){
            if(e->getFromNode()->getWeight() > e->getToNode()->getWeight()){
                e->swapNodes();
                e->setWeight(e->getToNode()->getWeight()-e->getFromNode()->getWeight());
                
                edgeMap.removeFromMap(oldID);
                
                string newID = to_string(e->getFromNode()->getID()) + "#" + to_string(e->getToNode()->getID());
                edgeMap.addToMap(newID, e);
            }else{
                e->setWeight(e->getToNode()->getWeight()-e->getFromNode()->getWeight());

            }
        }else if(isEnv){
            float weight1 = e->getFromNode()->getEnvWeight(index);
            float weight2 = e->getToNode()->getEnvWeight(index);
            
            if(weight1>weight2){
                e->swapNodes();
                e->setWeight(e->getToNode()->getEnvWeight(index)-e->getFromNode()->getEnvWeight(index));
                edgeMap.removeFromMap(oldID);
                
                string newID = to_string(e->getFromNode()->getID()) + "#" + to_string(e->getToNode()->getID());
                edgeMap.addToMap(newID, e);
                
                //newIdSet.insert(newID);
            }else{
                e->setWeight(e->getToNode()->getEnvWeight(index)-e->getFromNode()->getEnvWeight(index));
            }
        }
        
        e->getFromNode()->addOutDegree();
        e->getToNode()->addInDegree();
    }
    
    string str = "\n";
    for(nitr=nList.begin(); nitr!=nList.end(); nitr++){
        rNode* n = nitr->second;
        
        n->createLinkNodes();
        
        if(n->getTotalInDegree() == 0){
            n->setSource(true);
            sourceNodeIds.addToSet(n->getID());
        }
        
        //str += to_string(n->getID()) + "[" + (n->IsThisNodeASource()?"S,":"") + to_string(n->getWeight()) + ", " + to_string(n->getTotalInDegree()) + ", " + to_string(n->getTotalOutDegree()) + "]\n";
    }
    
    return str;
}

void CompositeGraph::constructGraph(bool isPhenotype, bool isEnv, int index){
    calculateNodeDegree(isPhenotype, isEnv, index);
    
    unordered_map<string,rEdge*> eList = edgeMap.getMap();
    unordered_map<string,rEdge*>::iterator eitr;
    
    for(eitr=eList.begin(); eitr!=eList.end(); eitr++){
        string id = eitr->first;
        rEdge* e = eitr->second;
        
        e->getFromNode()->addNextNode(e->getToNode());
        e->getToNode()->addPrevNode(e->getFromNode());
        
    }
}

string CompositeGraph::printSourceNodes(){
    string str = "";
    
    unordered_set<long> srcNodes = sourceNodeIds.getSet();
    for(unordered_set<long>::iterator itr=srcNodes.begin(); itr!=srcNodes.end(); itr++){
        if(str.length()>0) str += ",";
        str += to_string(*itr);
    }
    
    srcNodes = additionalSrcNodeIds.getSet();
    for(unordered_set<long>::iterator itr=srcNodes.begin(); itr!=srcNodes.end(); itr++){
        if(str.length()>0) str += ",";
        str += to_string(*itr);
    }

    return str;
}

rEdge* CompositeGraph::getMaxRank(long id){
    float maxScore = -1;
    int maxRank = 0;
    
    rNode* toNode = nodeList.getValue(id);
    rEdge* eMax = NULL;
    
    if(!toNode->IsThisNodeASource()){
        for(int i=0; i<toNode->getTotalInDegree(); i++){
            rNode* fNode = toNode->getPrevNode(i);
            
            string key = to_string(fNode->getID())+"#"+to_string(id);
            rEdge* e = edgeMap.getValue(key);
            
            if(maxScore < e->getScore()){
                maxRank = e->getRank();
                maxScore = e->getScore();
                
                eMax = e;
            }
        }
    }
    
    
    return eMax;
}

rEdge* CompositeGraph::getMaxRank(long id, rEdge* edge){
    float maxScore = -1;
    int maxRank = 0;
    
    rNode* toNode = nodeList.getValue(id);
    rEdge* eMax = NULL;
    
    
    
    // Predicessor edges
    for(int i=0; i<toNode->getTotalInDegree(); i++){
        rNode* fNode = toNode->getPrevNode(i);
        
        string key = to_string(fNode->getID())+"#"+to_string(id);
        rEdge* e = edgeMap.getValue(key);
        
        if(e->getRank()>0){
            rEdge* refEdge = e->getRefEdge();
            if(refEdge == NULL) refEdge=e;
            
            if(refEdge->matchSignature(edge)){
                if(maxScore < e->getScore()){
                    maxRank = e->getRank();
                    maxScore = e->getScore();
                    
                    eMax = e;
                }
            }
        }
    }
    
    return eMax;
}

float CompositeGraph::calculateInterestingnessScore(float weight, int rank){
    
    return (weight*log2(rank));
    
}

bool CompositeGraph::willFollowEdge(long fromID, long toID, rEdge* eMax){
    rEdge* edge = edgeMap.getValue(to_string(fromID)+"#"+to_string(toID));
    rNode* fNode = nodeList.getValue(fromID);
    
    float score = 0.0, prevScore = 0.0;
    int prevRank = 0;
    
    if(fNode->IsThisNodeASource()){
        edge->setRank(2);
        edge->setScore(calculateInterestingnessScore(edge->getWeight(), edge->getRank()));
        
        return true;
        
    }else{
        prevScore = eMax->getScore();
        prevRank = eMax->getRank();
        
        if(edge->getScore()==0){
            edge->setRank(prevRank+1);
            edge->setScore(prevScore + calculateInterestingnessScore(edge->getWeight(), edge->getRank()));
            
            return true;
        }else if(EDGE_OVERLAPPING){
            score = prevScore+calculateInterestingnessScore(edge->getWeight(), prevRank+1);
            
            if(edge->getScore() < score){
                edge->setRank(prevRank+1);
                edge->setScore(score);
                
                return true;
            }
        }
    }
    
    return false;
}

void CompositeGraph::assignScore(long fromID, long toID, rEdge *eMax){
    rEdge* edge = edgeMap.getValue(to_string(fromID)+"#"+to_string(toID));
    
    float prevScore = 0.0;
    int prevRank = 0;
    
    if(eMax==NULL){
        edge->setRank(2);
        edge->setScore(calculateInterestingnessScore(edge->getWeight(), edge->getRank()));
        
    }else{
        prevScore = eMax->getScore();
        prevRank = eMax->getRank();
        
        edge->setRank(prevRank+1);
        edge->setScore(prevScore + calculateInterestingnessScore(edge->getWeight(), edge->getRank()));
    }
}

bool CompositeGraph::checkTerminalEdge(long toID, rEdge* e){
    rNode* fromNode = nodeList.getValue(toID);
    
    for(int i=0; i<fromNode->getTotalOutDegree(); i++){
        rNode* toNode = fromNode->getNextNode(i);
        rEdge* edge = edgeMap.getValue(to_string(fromNode->getID())+"#"+to_string(toNode->getID()));
        
        rEdge* refEdge = e->getRefEdge();
        if(refEdge==NULL) refEdge=e;
        
        if(refEdge->matchSignature(edge)){
            return false;
        }
    }
    
    return true;
}

bool CompositeGraph::needMoreExplore(rEdge* e){
    
    rNode* toNode = e->getFromNode();
    
    // Predicessor edges
    for(int i=0; i<toNode->getTotalInDegree(); i++){
        rNode* fNode = toNode->getPrevNode(i);
        
        string key = to_string(fNode->getID())+"#"+to_string(toNode->getID());
        rEdge* edge = edgeMap.getValue(key);
        
        if(edge->getScore()==0){
            return true;
        }
    }
    
    return false;
}

bool CompositeGraph::exploreNode(){
    
    if(exploreList.getSize() == 0) return false;
    
    long id = exploreList.getElement();
    exploreList.removeItem(id);
    
    rNode* fromNode = nodeList.getValue(id);
 
    for(int i=0; i<fromNode->getTotalOutDegree(); i++){
        rNode* toNode = fromNode->getNextNode(i);
        string key = to_string(fromNode->getID())+"#"+to_string(toNode->getID());
        rEdge* e = edgeMap.getValue(key);
        
        if(TEST){
            if(e->getToNode()->getID()==32&&(e->getFromNode()->getID()==24||e->getFromNode()->getID()==45||e->getFromNode()->getID()==46)){
                cout<<"";
            }
        }
        
        rEdge* eMax = getMaxRank(fromNode->getID(), e);
        
        if(eMax == NULL){
            if(!fromNode->IsThisNodeASource() && !needMoreExplore(e)){
                fromNode->setSource(true);
                
                // Add new source of next path and target of last path
                additionalSrcNodeIds.addToSet(fromNode->getID());
            }
        }else if(DELTA_CHANGE>0){
            
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
        
        if(checkTerminalEdge(toNode->getID(), e)){
            terminalEdges.addToSet(e);
        }
        
        if(eMax==NULL && !needMoreExplore(e))
            assignScore(fromNode->getID(), toNode->getID(), eMax);
        else if(eMax!=NULL)
            assignScore(fromNode->getID(), toNode->getID(), eMax);
    }
    
    // Add to explore list
    for(int i=0; i<fromNode->getTotalOutDegree(); i++){
        rNode* toNode = fromNode->getNextNode(i);
        
        exploreList.addToSet(toNode->getID());
    }
    
    return exploreNode();
}

void CompositeGraph::runBFS(){
    unordered_set<long> sourceIds = sourceNodeIds.getSet();
    unordered_set<long>::iterator itr;
    
    for(itr=sourceIds.begin(); itr!=sourceIds.end(); itr++){
        long fromId = *itr;
        rNode* fromNode = nodeList.getValue(fromId);
        
        for(int i=0; i<fromNode->getTotalOutDegree(); i++){
            rNode* toNode = fromNode->getNextNode(i);
            rEdge* e = edgeMap.getValue(to_string(fromNode->getID())+"#"+to_string(toNode->getID()));
            
            // Assign score
            assignScore(fromId, toNode->getID(), NULL);
            
            exploreList.removeItem(e->getToNode()->getID());
            exploreList.addToSet(e->getToNode()->getID());
        }
    }
    
    exploreNode();
}

rEdge* CompositeGraph::getEdgeOfRank(rEdge* edge){
    
    long id=edge->getFromNode()->getID();
    int rank=edge->getRank()-1;
    float weight = edge->getWeight();
    float score=edge->getScore();
    
    rNode* toNode = nodeList.getValue(id);
    rEdge* newEdge = NULL;
    
    for(int i=0; i<toNode->getTotalInDegree(); i++){
        rNode* fNode = toNode->getPrevNode(i);
        
        string key = to_string(fNode->getID())+"#"+to_string(id);
        rEdge* e = edgeMap.getValue(key);
        
        if(rank == e->getRank()){
            
            float newScore = e->getScore() + calculateInterestingnessScore(weight, rank+1);
            
            if(EXPLORE_PHENOTYPE_GRAPH)
                newScore = e->getScore() + edge->getNormalizedWeight(0);
            
            if(newScore == score){
                newEdge = e;
                break;
            }
        }
    }
    
    return newEdge;

}

rEdge* CompositeGraph::getMaxRank(rEdge* edge){
    return getEdgeOfRank(edge);
}

bool comparePaths(Path *p1, Path *p2){
    if(p1->getScore() > p2->getScore()) return true;
    
    return false;
}

bool compareEdgeScore(rEdge* e1, rEdge* e2){
    if(e1->getScore() > e2->getScore()) return true;
    
    return false;
}

string getGenLocColor(string gen, string loc){
    //A
    if(gen.compare("F42 x DJ7")==0||gen.compare("A")==0){
        if(loc.compare("KS")==0||loc.compare("K")==0) return "#0000ff";
        else if(loc.compare("NE")==0||loc.compare("N")==0) return "#00ff00";
    }
    //B
    else{
        if(loc.compare("NE")==0||loc.compare("N")==0) return "#E67E22";
        else if(loc.compare("KS")==0||loc.compare("K")==0) return "#ff0000";
    }
    
    return "";
}

string getPlantIdColor(int plantID){
    
    switch (plantID) {
        case 1:
            return "#cdd31f";
            
        case 2:
            return "#419031";
            
        case 3:
            return "#27e6af";
            
        case 4:
            return "#1eb3f4";
            
        case 5:
            return "#b146dc";
            
        case 6:
            return "#ec67f0";
            
        default:
            break;
    }
    
    return "#a5a5a5";
}

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

void CompositeGraph::assignPathColor(){

    list<Path*> tmpPath;
    unordered_set<float> scoreSet;
    
    for(int i=0; i<pathCounter; i++){
        Path* p = interestingPaths.getValue(i);
        
        if(p->getPath().size() >= MINIMUM_PATH_LENGTH){
            tmpPath.push_back(p);
            scoreSet.insert(p->getScore());
        }
    }
    
    tmpPath.sort(comparePaths);
    
    unsigned long totalPaths=tmpPath.size();
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
                
                float d1 = abs(ta-t1);
                float d2 = abs(ta-i);
                short gray = ceil((255*d2)/d1);
                
                RGB r(255,gray,0);
                rgb[counter] = r;
                
            }else if(counter>interval && counter<(2*interval)){
                
                float d1 = abs(t1-tb);
                float d2 = abs(tb-i);
                short red = ceil((255*d2)/d1);
                
                RGB r(red,255,0);
                rgb[counter] = r;
            }else if(counter>(2*interval) && counter<(3*interval)){
                
                float d1 = abs(t2-tb);
                float d2 = abs(tb-i);
                short blue = ceil((255*d2)/d1);
                
                RGB r(0,255,blue);
                rgb[counter] = r;
            }else if(counter>(3*interval) && counter<totalPaths-1){
                
                float d1 = abs(t2-tc);
                float d2 = abs(tc-i);
                short grey = ceil((255*d2)/d1);
                
                RGB r(0,grey,255);
                rgb[counter] = r;
            }
        }
    }
    
    interestingPaths.clear();
    pathCounter=0;
    RGBColor c;
    
    for(list<Path*>::iterator pitr=tmpPath.begin(); pitr!=tmpPath.end(); pitr++, pathCounter++){
        RGB r = rgb[pathCounter];
        short R=r.getR(),G=r.getG(), B=r.getB();
        
        Path* p = *pitr;
        p->setColor(c.RGBtoHex(R,G,B));
        
        interestingPaths.addToMap(pathCounter, p);
    }
    
    free(rgb);
}

bool CompositeGraph::exploreEdge(rEdge* edge){
    //e->getFromNode()->getID(), e->getRank(), e->getWeight(), e->getScore(), s
    rEdge* eMax = getMaxRank(edge);
    
    if(eMax != NULL){
        
        if(eMax->getMark() == true){
            return false;
        }
        
        Path* p = interestingPaths.getValue(pathCounter);
        p->addToPath(to_string(eMax->getFromNode()->getID())+"#"+to_string(eMax->getToNode()->getID()));
        
        eMax->setMark(true);
        exploreEdge(eMax);
    }
    
    return true;
}

void CompositeGraph::reconstructPaths(){
    computeMaxTerminalEdges();
    
    list<rEdge*> tmpEdgeList;
    unordered_map<string, rEdge*> terminalEdges = maxTermEdges.getMap();
    
    for(unordered_map<string, rEdge*>::iterator itr=terminalEdges.begin(); itr!=terminalEdges.end(); itr++){
        tmpEdgeList.push_back(itr->second);
    }
    
    tmpEdgeList.sort(compareEdgeScore);
    
    pathCounter = 0;
    
    for(list<rEdge*>::iterator itr=tmpEdgeList.begin(); itr!=tmpEdgeList.end(); itr++){
        
        rEdge* e = *itr;
        e->setMark(true);
        
        if(TEST){
            if(e->getFromNode()->getID()==30&& e->getToNode()->getID()==61){
                cout<<"";
            }
        }
        
        Path* newPath = new Path();
        newPath->addToPath(to_string(e->getFromNode()->getID()) + "#" + to_string(e->getToNode()->getID()));
        newPath->setScore(e->getScore());
        interestingPaths.addToMap(pathCounter, newPath);
        
        if(exploreEdge(e)){
            pathCounter++;
        }else{
            interestingPaths.removeFromMap(pathCounter);
        }
    }
    
    assignPathColor();
}

string CompositeGraph::printPaths(){
    string str = "";
    
    for(int i=0,c=1; i<pathCounter; i++){
        Path* p = interestingPaths.getValue(i);
        if(p->getPath().size() >= MINIMUM_PATH_LENGTH){
        
            if(str.length() > 0)str += "\n\n";
        
            str += "Path_" + to_string(c) + "_[" + to_string(p->getScore()) + "," + p->getColor() + "] : " + p->getNodeIds();
            c++;
        }
    }
    
    return str;
}

void CompositeGraph::normalizeByUniformDistribution(){
    unordered_map<string, rEdge*> eList = edgeMap.getMap();
    float range[3] = {0};
    float pw1,pw2,pw,w1,w2,w;
    float min[3]={0}, max[3]={0};
    bool first[3] = {true};
    
    for(unordered_map<string, rEdge*>::iterator itr=eList.begin(); itr!=eList.end(); itr++){
        rEdge* e = itr->second;
        rNode* fromNode = e->getFromNode();
        rNode* toNode = e->getToNode();
        
        pw1 = fromNode->getWeight();
        pw2 = toNode->getWeight();
        pw = abs(pw2-pw1)>0?abs(pw2-pw1):NEGLIGIBLE_WEIGHT;
        
        if(first[0]){
            first[0]=false;
            
            min[0]=max[0]=pw;
        }
        
        if(min[0]>pw) min[0]=pw;
        if(max[0]<pw) max[0]=pw;
        
        for(int i=0; i<FILTER; i++){
            w1 = fromNode->getEnvWeight(i);
            w2 = toNode->getEnvWeight(i);
            w = abs(w2-w1)>0?abs(w2-w1):NEGLIGIBLE_WEIGHT;
            
            if(first[i+1]){
                first[i+1]=false;
                
                min[i+1]=max[i+1]=w;
            }
            
            if(min[i+1]>w) min[i+1]=w;
            if(max[i+1]<w) max[i+1]=w;
        }
        
    }
    
    for(int i=0; i<FILTER+1; i++){
        range[i] = max[i]-min[i];
        if(range[i]==0) range[i]=1;
    }
    
    for(unordered_map<string, rEdge*>::iterator itr=eList.begin(); itr!=eList.end(); itr++){
        rEdge* e = itr->second;
        rNode* fromNode = e->getFromNode();
        rNode* toNode = e->getToNode();
        
        if(TEST){
            if(fromNode->getID()==52 && (toNode->getID()==55||toNode->getID()==53)){
                cout<<"";
            }
        }
        
        pw1 = fromNode->getWeight();
        pw2 = toNode->getWeight();
        pw = abs(pw2-pw1)>0?abs(pw2-pw1):NEGLIGIBLE_WEIGHT;
        
        e->setNormalizedWeight(pw/range[0], 0);
        e->setEdgeWeight(pw, 0);
        
        for(int i=0; i<FILTER; i++){
            w1 = fromNode->getEnvWeight(i);
            w2 = toNode->getEnvWeight(i);
            w = abs(w2-w1)>0?abs(w2-w1):NEGLIGIBLE_WEIGHT;
            
            e->setNormalizedWeight(w/range[i+1], i+1);
            e->setEdgeWeight(w, i+1);
        }
        
        float wgt[FILTER+1];
        
        if(REFERENCE_PHENOTYPE){
            
            wgt[0] = e->getNormalizedWeight(0);
            
            for(int i=0; i<FILTER; i++){
                wgt[i+1] = e->getNormalizedWeight(i+1);
                
                w1 = e->getFromNode()->getEnvWeight(i);
                w2 = e->getToNode()->getEnvWeight(i);
                w = w2-w1;
                
                if(w<0){
                    e->setSignatue(false, i);
                }else{
                    e->setSignatue(true, i);
                }
            }
            
        }else{
            
            int cIndex=0;
            
            // Avg phenotype weight
            wgt[0] = e->getNormalizedWeight(0);
            
            w1 = e->getFromNode()->getWeight();
            w2 = e->getToNode()->getWeight();
            w = w2-w1;
            
            if(w<0){
                e->setSignatue(false, cIndex);
            }else{
                e->setSignatue(true, cIndex);
            }
            
            cIndex++;
            
            for(int i=0; i<FILTER; i++){
                wgt[i+1] = e->getNormalizedWeight(i+1);
                
                if(i!= REFERENCE_ENV_INDEX){
                    
                    w1 = e->getFromNode()->getEnvWeight(i);
                    w2 = e->getToNode()->getEnvWeight(i);
                    w = w2-w1;
                    
                    if(w<0){
                        e->setSignatue(false, cIndex);
                    }else{
                        e->setSignatue(true, cIndex);
                    }
                    
                    cIndex++;
                }
            }
        }
        
        float euWeight = 0.0;
        
        for(int i=0; i<FILTER+1; i++){
            euWeight += (wgt[i]*wgt[i]);
        }
        
        e->setWeight(sqrt(euWeight));
    }
    
}

void CompositeGraph::normalizeByStandardDeviation(){
    unordered_map<string, rEdge*> eList = edgeMap.getMap();
    float mean[3] = {0.0}, SD[3]={0.0}, w1,w2,w;
    size_t counter=eList.size();
    
    for(unordered_map<string, rEdge*>::iterator itr=eList.begin(); itr!=eList.end(); itr++){
        rEdge* e = itr->second;
        rNode* fromNode = e->getFromNode();
        rNode* toNode = e->getToNode();
        
        w1 = fromNode->getWeight();
        w2 = toNode->getWeight();
        w = abs(w2-w1)>0?abs(w2-w1):NEGLIGIBLE_WEIGHT;
        
        mean[0] += w;
    }
    
    mean[0] /= counter;
    
    for(unordered_map<string, rEdge*>::iterator itr=eList.begin(); itr!=eList.end(); itr++){
        rEdge* e = itr->second;
        rNode* fromNode = e->getFromNode();
        rNode* toNode = e->getToNode();
        
        w1 = fromNode->getWeight();
        w2 = toNode->getWeight();
        w = abs(w2-w1)>0?abs(w2-w1):NEGLIGIBLE_WEIGHT;
        
        SD[0] += ((w-mean[0])*(w-mean[0]));
    }
    
    SD[0] = sqrt(SD[0]/counter);
    
    for(unordered_map<string, rEdge*>::iterator itr=eList.begin(); itr!=eList.end(); itr++){
        rEdge* e = itr->second;
        rNode* fromNode = e->getFromNode();
        rNode* toNode = e->getToNode();
        
        w1 = fromNode->getWeight();
        w2 = toNode->getWeight();
        w = abs(w2-w1)>0?abs(w2-w1):NEGLIGIBLE_WEIGHT;
        
        e->setNormalizedWeight((w-mean[0])/SD[0], 0);
        e->setEdgeWeight(w, 0);
    }
    
    // For environmental attributes
    for(int i=0; i<FILTER; i++){
        
        for(unordered_map<string, rEdge*>::iterator itr=eList.begin(); itr!=eList.end(); itr++){
            rEdge* e = itr->second;
            rNode* fromNode = e->getFromNode();
            rNode* toNode = e->getToNode();
            
            w1 = fromNode->getEnvWeight(i);
            w2 = toNode->getEnvWeight(i);
            w = abs(w2-w1)>0?abs(w2-w1):NEGLIGIBLE_WEIGHT;
            
            mean[i+1] += w;
        }
        
        mean[i+1] /= counter;
        
        for(unordered_map<string, rEdge*>::iterator itr=eList.begin(); itr!=eList.end(); itr++){
            rEdge* e = itr->second;
            rNode* fromNode = e->getFromNode();
            rNode* toNode = e->getToNode();
            
            w1 = fromNode->getEnvWeight(i);
            w2 = toNode->getEnvWeight(i);
            w = abs(w2-w1)>0?abs(w2-w1):NEGLIGIBLE_WEIGHT;
            
            SD[i+1] += ((w-mean[i+1])*(w-mean[i+1]));
        }
        
        SD[i+1] = sqrt(SD[i+1]/counter);
        
        for(unordered_map<string, rEdge*>::iterator itr=eList.begin(); itr!=eList.end(); itr++){
            rEdge* e = itr->second;
            rNode* fromNode = e->getFromNode();
            rNode* toNode = e->getToNode();
            
            w1 = fromNode->getEnvWeight(i);
            w2 = toNode->getEnvWeight(i);
            w = abs(w2-w1)>0?abs(w2-w1):NEGLIGIBLE_WEIGHT;
            
            e->setNormalizedWeight((w-mean[i+1])/SD[i+1], i+1);
            e->setEdgeWeight(w, i+1);
        }
    }
    
    for(unordered_map<string, rEdge*>::iterator itr=eList.begin(); itr!=eList.end(); itr++){
        rEdge* e = itr->second;
        
        float wgt[FILTER+1]={0};
        
        if(REFERENCE_PHENOTYPE){
            wgt[0] = e->getNormalizedWeight(0);
            
            for(int i=0; i<FILTER; i++){
                wgt[i] = e->getNormalizedWeight(i+1);
                
                w1 = e->getFromNode()->getEnvWeight(i);
                w2 = e->getToNode()->getEnvWeight(i);
                w = w2-w1;
                
                if(w<0){
                    e->setSignatue(false, i);
                }else{
                    e->setSignatue(true, i);
                }
            }
        }else{
    
            int cIndex=0;
            
            // Avg phenotype weight
            wgt[0] = e->getNormalizedWeight(0);
            w1 = e->getFromNode()->getWeight();
            w2 = e->getToNode()->getWeight();
            w = w2-w1;
            
            if(w<0){
                e->setSignatue(false, cIndex);
            }else{
                e->setSignatue(true, cIndex);
            }
            
            cIndex++;
            
            
            for(int i=0; i<FILTER; i++){
               
                wgt[i+1] = e->getNormalizedWeight(i+1);
                
                if(i!= REFERENCE_ENV_INDEX){
                    
                    w1 = e->getFromNode()->getEnvWeight(i);
                    w2 = e->getToNode()->getEnvWeight(i);
                    w = w2-w1;
                    
                    if(w<0){
                        e->setSignatue(false, cIndex);
                    }else{
                        e->setSignatue(true, cIndex);
                    }
                    
                    cIndex++;
                }
            }
        }
        
        float euWeight = 0.0;
        
        for(int i=0; i<FILTER+1; i++){
            euWeight += (wgt[i]*wgt[i]);
        }
        
        e->setWeight(sqrt(euWeight));
    }

}

void CompositeGraph::normalizeWeight(){
    if(NORMALIZE_UNIFORM_DIST){
        normalizeByUniformDistribution();
    }else if(NORMALIZE_STANDARD_DEVIATION){
        normalizeByStandardDeviation();
    }
}

void CompositeGraph::absoluteDistanceWeight(){
    unordered_map<string, rEdge*> eList = edgeMap.getMap();
    float pw1,pw2,pw;
    float w1,w2,w;

    for(unordered_map<string, rEdge*>::iterator itr=eList.begin(); itr!=eList.end(); itr++){
        rEdge* e = itr->second;
        rNode* fromNode = e->getFromNode();
        rNode* toNode = e->getToNode();
        
        if(TEST){
            if(fromNode->getID()==52 && (toNode->getID()==55||toNode->getID()==53)){
                cout<<"";
            }
        }
        
        pw1 = fromNode->getWeight();
        pw2 = toNode->getWeight();
        pw = abs(pw2-pw1)>0?abs(pw2-pw1):NEGLIGIBLE_WEIGHT;
        
        e->setNormalizedWeight(pw, 0);
        e->setEdgeWeight(pw, 0);
        e->setWeight(pw);
        
        for(int i=0; i<FILTER; i++){
            w1 = fromNode->getEnvWeight(i);
            w2 = toNode->getEnvWeight(i);
            w = abs(w2-w1)>0?abs(w2-w1):NEGLIGIBLE_WEIGHT;
            
            e->setNormalizedWeight(w, i+1);
            e->setEdgeWeight(w, i+1);
        }
        
        if(REFERENCE_PHENOTYPE){
            
            for(int i=0; i<FILTER; i++){
                
                w1 = e->getFromNode()->getEnvWeight(i);
                w2 = e->getToNode()->getEnvWeight(i);
                w = w2-w1;
                
                if(w<0){
                    e->setSignatue(false, i);
                }else{
                    e->setSignatue(true, i);
                }
            }
            
        }else{
            
            int cIndex=0;
            
            w1 = e->getFromNode()->getWeight();
            w2 = e->getToNode()->getWeight();
            w = w2-w1;
            
            if(w<0){
                e->setSignatue(false, cIndex);
            }else{
                e->setSignatue(true, cIndex);
            }
            
            cIndex++;
            
            for(int i=0; i<FILTER; i++){
                
                if(i!= REFERENCE_ENV_INDEX){
                    
                    w1 = e->getFromNode()->getEnvWeight(i);
                    w2 = e->getToNode()->getEnvWeight(i);
                    w = w2-w1;
                    
                    if(w<0){
                        e->setSignatue(false, cIndex);
                    }else{
                        e->setSignatue(true, cIndex);
                    }
                    
                    cIndex++;
                }
            }
        }
        
    }
}

void CompositeGraph::setEdgeWeightAndSignature(){
    // Normalize weight
    
    if(ABSOLUTE_DISTANCE){
        absoluteDistanceWeight();
    }else{
        normalizeWeight();
    }
}

string CompositeGraph::printOriginarGraphs(string fileNameSuffix, bool isPhenotype, bool isEnv, int index){
    
    string gmlData="";
    
    if(isPhenotype){
        constructGraph(isPhenotype, isEnv, index);
        gmlData = printGraph(isPhenotype, isEnv, index, fileNameSuffix);
    }
    else if(!PRINT_BARCODE){
        constructGraph(isPhenotype, isEnv, index);
        gmlData = printGraph(isPhenotype, isEnv, index, fileNameSuffix);
        
    }
    
    return gmlData;
}

void CompositeGraph::constructCompositeGraph(){
    // Reference 
    constructGraph(REFERENCE_PHENOTYPE, REFERENCE_ENVIRONMENT, REFERENCE_ENV_INDEX);
    
    // Adjust weight and sign
    setEdgeWeightAndSignature();
}

string CompositeGraph::printCompositeGraph(bool withSignature, string fileNameSuffix){
    string nodes = "";
    string edges = "";
    string gmlData = "";
    string type = "";
    
    unordered_map<long,rNode*> nList = nodeList.getMap();
    unordered_map<long,rNode*>::iterator nitr;
    
    unordered_map<string,rEdge*> eList = edgeMap.getMap();
    unordered_map<string,rEdge*>::iterator eitr;
    
    unordered_map<long, float*> nodePos;
    unordered_map<long, float*>::iterator npItr;
    
    FileHandler* fileHandler = new FileHandler("");
    fileHandler->readNodePosition(COORDINATE_FILE_NAME, &nodePos);
    delete fileHandler;
    
    string nodeJS="[";
    string tmpNodeJS="";
    string tmpPosJs="";
    
    //RGBColor c(1, 17000, 0.1);
    for(nitr=nList.begin(); nitr!=nList.end(); nitr++){
        rNode *n = nitr->second;
        
        string label = to_string(n->getID());//n->getGenLoc();// + ":" + to_string(n->getVariations());to_string(n->getID());//
        string shape = "pie";
        string col = "#ffffff";//c.GetColor(n->getGenLocColorIndex()); //
        string size = fixPrecision(adjustSize(n->getSize()), 2);
        
        nodes += "node\n[\n id " + to_string(n->getID()) + "\n label \"" + label + "\" \n size " + size + "\n color \"" + col + "\" \n shape \"" + shape + "\" \n]\n";
        
        if((npItr=nodePos.find(n->getID())) != nodePos.end()){
            tmpNodeJS = "d:"+n->getNodeJS();
            tmpPosJs = "p:{x:"+to_string(npItr->second[0])+",y:"+to_string(npItr->second[1])+",rad:"+size+"}";
        
            if(nodeJS.find("}")!=string::npos) nodeJS += ",";
            nodeJS += "{id:"+ to_string(n->getID()) + "," + tmpPosJs + ", " + tmpNodeJS + "}";
        }
    }
    nodeJS += "]";
    
    if(PIE_CHART_CODE){
        cout<<"\n\nvar data="<<nodeJS<<";\n\n";
    }
    
    string wc[2];
    float egt=0;
    unordered_map<string, int> signature_counter_map;
    unordered_map<string, int>::iterator sig_map;
    for(eitr=eList.begin(); eitr!=eList.end(); eitr++){
        rEdge* e = eitr->second;
        getWidthAndColor(e->getFromNode()->getID(), e->getToNode()->getID(), wc);
        egt+=e->getWeight();
        string label = fixPrecision(e->getWeight(), SHOW_PRECISION_IN_EDGE_LABEL);
        string sig;
        e->getSignature(sig);
        
        if((sig_map=signature_counter_map.find(sig)) == signature_counter_map.end()){
            signature_counter_map.insert(pair<string,int>(sig,1));
        }else{
            sig_map->second++;
        }
        
        if(withSignature){
             e->getSignature(label);
        }else if(!SHOW_EDGE_LABEL_FOR_ALL_EDGES){
            label="";
        }
        
        if(!SHOW_EDGE_LABEL_FOR_ALL_EDGES && stoi(wc[0])==1) label="";
        
        edges += "edge\n[\n source " + to_string(e->getFromNode()->getID()) + "\n target " + to_string(e->getToNode()->getID()) + "\n width " + wc[0] + "\n label \"" + label + "\" \n color \"" + wc[1] + "\"\n]\n";
        
    }
    
    /*cout<<"\n\n";
    for(sig_map=signature_counter_map.begin(); sig_map!=signature_counter_map.end(); sig_map++){
        cout<<sig_map->first<<": "<<sig_map->second<<"\n";
    }
    
    printf("\n\nTotal weight:%.2f\n\n",egt);*/
    gmlData = nodes+edges;
    
    return gmlData;
}

string CompositeGraph::printTerminalEdges(){
    string str = "";
    
    unordered_set<rEdge*> eSet = terminalEdges.getSet();
    
    for(unordered_set<rEdge*>::iterator itr = eSet.begin(); itr!=eSet.end(); itr++){
        rEdge* e = *itr;
        string s;
        e->getSignature(s);
        
        if(str.length()>0) str += "\n";
        str += to_string(e->getToNode()->getID()) + "#" + s + ": " + to_string(e->getFromNode()->getID()) + "->" + to_string(e->getToNode()->getID());
    }
    
    return str;
}

void CompositeGraph::computeMaxTerminalEdges(){
    unordered_set<rEdge*> eSet = terminalEdges.getSet();
    unordered_map<string, rEdge*> termEdges;
    unordered_map<string, rEdge*>::iterator titr;
    
    for(unordered_set<rEdge*>::iterator itr = eSet.begin(); itr!=eSet.end(); itr++){
        rEdge* e = *itr;
        string s;
        e->getSignature(s);
        
        string key = to_string(e->getFromNode()->getID()) + "#" + to_string(e->getToNode()->getID()) + "#" + s;
        
        if(EXPLORE_PHENOTYPE_GRAPH)
            key = to_string(e->getFromNode()->getID()) + "#" + to_string(e->getToNode()->getID());
        
        if(TEST){
            if(e->getToNode()->getID() == 148){
                cout<<"";
            }
        }
        
        if((titr=termEdges.find(key)) == termEdges.end()){
            termEdges.insert(pair<string, rEdge*>(key,e));
        }else{
            rEdge* eTemp = titr->second;
            
            if(eTemp->getScore()<e->getScore()){
                titr->second = e;
            }
        }
    }
    
    for(titr=termEdges.begin(); titr!=termEdges.end(); titr++){
        maxTermEdges.addToMap(titr->first, titr->second);
    }
}

rEdge* CompositeGraph::getMaxRankInPhenotypeGraph(long id){
    float maxScore = -1;
    int maxRank = 0;
    
    rNode* toNode = nodeList.getValue(id);
    rEdge* eMax = NULL;
    
    // Predicessor edges
    for(int i=0; i<toNode->getTotalInDegree(); i++){
        rNode* fNode = toNode->getPrevNode(i);
        
        string key = to_string(fNode->getID())+"#"+to_string(id);
        rEdge* e = edgeMap.getValue(key);
        
        if(e->getRank()>0){
            if(maxScore < e->getScore()){
                maxRank = e->getRank();
                maxScore = e->getScore();
                    
                eMax = e;
            }
            
        }
    }
    
    return eMax;

}

bool CompositeGraph::checkTerminalEdgeForPhenotype(long id){
    rNode* fromNode = nodeList.getValue(id);
    
    if(fromNode->getTotalOutDegree()>0)
        return false;
    
    return true;
}

void CompositeGraph::runBFSonPhenotype(){
    unordered_set<long> sourceIds = sourceNodeIds.getSet();
    unordered_set<long>::iterator itr;
    
    for(itr=sourceIds.begin(); itr!=sourceIds.end(); itr++){
        long fromId = *itr;
        rNode* fromNode = nodeList.getValue(fromId);
        
        for(int i=0; i<fromNode->getTotalOutDegree(); i++){
            rNode* toNode = fromNode->getNextNode(i);
            rEdge* e = edgeMap.getValue(to_string(fromNode->getID())+"#"+to_string(toNode->getID()));
            
            // Assign score
            e->setScore(e->getNormalizedWeight(0));
            e->setRank(1);
            
            exploreList.removeItem(e->getToNode()->getID());
            exploreList.addToSet(e->getToNode()->getID());
        }
    }
    
    exploreNodeInPhenotypeGraph();
}

bool CompositeGraph::exploreNodeInPhenotypeGraph(){
    
    if(exploreList.getSize() == 0) return false;
    
    long id = exploreList.getElement();
    exploreList.removeItem(id);
    
    rNode* fromNode = nodeList.getValue(id);
    
    if(TEST){
        if(id==18){
            cout<<"";
        }
    }
    
    for(int i=0; i<fromNode->getTotalOutDegree(); i++){
        rNode* toNode = fromNode->getNextNode(i);
        string key = to_string(fromNode->getID())+"#"+to_string(toNode->getID());
        rEdge* e = edgeMap.getValue(key);
        
        rEdge* eMax = getMaxRankInPhenotypeGraph(fromNode->getID());
        
        if(eMax == NULL){
            if(!fromNode->IsThisNodeASource()){
                fromNode->setSource(true);
                
                // Add new source of next path and target of last path
                additionalSrcNodeIds.addToSet(fromNode->getID());
            }
            
            e->setRank(1);
            e->setScore(e->getNormalizedWeight(0));
        }else{
            e->setRank(eMax->getRank()+1);
            e->setScore(e->getNormalizedWeight(0) + eMax->getScore());
        }
        
        if(checkTerminalEdge(toNode->getID(), e)){
            terminalEdges.addToSet(e);
        }
        
    }
    
    // Add to explore list
    for(int i=0; i<fromNode->getTotalOutDegree(); i++){
        rNode* toNode = fromNode->getNextNode(i);
        
        exploreList.addToSet(toNode->getID());
    }
    
    return exploreNodeInPhenotypeGraph();
}

void CompositeGraph::filterEdges(){
    unordered_map<string, rEdge*> tmpEdgeMap = edgeMap.getMap();
    
    for(unordered_map<string, rEdge*>::iterator itr=tmpEdgeMap.begin(); itr!=tmpEdgeMap.end(); itr++){
        rEdge* e = itr->second;
        rNode* n1 = e->getFromNode();
        rNode* n2 = e->getToNode();
        
        if(n1->getVariations()< 4 || n2->getVariations()<4){
            edgeMap.removeFromMap(itr->first);
        }
        
        if(n1->getVariations()<4) nodeList.removeFromMap(n1->getID());
        if(n2->getVariations()<4) nodeList.removeFromMap(n2->getID());
    }
    
    //cout<<"\nTotal Edges: "<<edgeMap.getSize()<<endl;
    
    float nodeSizeRange[2];
    float nodeWeightRange[2];
    float envWeightRange[FILTER][2];
    bool first=true;
    
    tmpEdgeMap = edgeMap.getMap();
    for(unordered_map<string, rEdge*>::iterator itr=tmpEdgeMap.begin(); itr!=tmpEdgeMap.end(); itr++){
        rEdge* e = itr->second;
        rNode* n1 = e->getFromNode();
        rNode* n2 = e->getToNode();
        
        if(first){
            first=false;
            
            nodeSizeRange[0]=nodeSizeRange[1]=n1->getSize();
            nodeWeightRange[0]=nodeWeightRange[1]=n1->getWeight();
            envWeightRange[0][0]=envWeightRange[0][1]=n1->getEnvWeight(0);
            envWeightRange[1][0]=envWeightRange[1][1]=n1->getEnvWeight(1);
        }
        
        // Adjust node size range
        if(nodeSizeRange[0]>n1->getSize()){
            nodeSizeRange[0] = n1->getSize();
        }
        
        if(nodeSizeRange[1]<n1->getSize()){
            nodeSizeRange[1]=n1->getSize();
        }
        
        if(nodeSizeRange[0]>n2->getSize()){
            nodeSizeRange[0] = n2->getSize();
        }
        
        if(nodeSizeRange[1]<n2->getSize()){
            nodeSizeRange[1]=n2->getSize();
        }
        
        // Adjust node weight range
        if(nodeWeightRange[0]>n1->getWeight()){
            nodeWeightRange[0] = n1->getWeight();
        }
        
        if(nodeWeightRange[1]<n1->getWeight()){
            nodeWeightRange[1]=n1->getWeight();
        }
        
        if(nodeSizeRange[0]>n2->getWeight()){
            nodeWeightRange[0] = n2->getWeight();
        }
        
        if(nodeWeightRange[1]<n2->getWeight()){
            nodeWeightRange[1]=n2->getWeight();
        }
        
        // Adjust environmental attribute #1
        if(envWeightRange[0][0]>n1->getEnvWeight(0)){
            envWeightRange[0][0] = n1->getEnvWeight(0);
        }
        
        if(envWeightRange[0][1]<n1->getEnvWeight(0)){
            envWeightRange[0][1]=n1->getEnvWeight(0);
        }
        
        if(envWeightRange[1][0]>n1->getEnvWeight(1)){
            envWeightRange[1][0] = n1->getEnvWeight(1);
        }
        
        if(envWeightRange[1][1]<n1->getEnvWeight(1)){
            envWeightRange[1][1]=n1->getEnvWeight(1);
        }
        
        if(envWeightRange[0][0]>n2->getEnvWeight(0)){
            envWeightRange[0][0] = n2->getEnvWeight(0);
        }
        
        if(envWeightRange[0][1]<n2->getEnvWeight(0)){
            envWeightRange[0][1]=n2->getEnvWeight(0);
        }
        
        if(envWeightRange[1][0]>n2->getEnvWeight(1)){
            envWeightRange[1][0] = n2->getEnvWeight(1);
        }
        
        if(envWeightRange[1][1]<n2->getEnvWeight(1)){
            envWeightRange[1][1]=n2->getEnvWeight(1);
        }
    }
    
    updateStorageRange(nodeSizeRange, nodeWeightRange, envWeightRange);
    
}

string CompositeGraph::printCompositeGraphsWithPath(string fileNameSuffix){
    //filterEdges();
    
    constructCompositeGraph();
    
    if(GENERATE_PATHS){
        if(EXPLORE_PHENOTYPE_GRAPH)
            runBFSonPhenotype();
        else
            runBFS();
        
        reconstructPaths();
        
        //std::cout<<"\n\nTerminal nodes:\n"<<printPaths();
    }
    
    // false: print normalized weight, true: signature
    return printCompositeGraph(PRINT_SIGNATURE,fileNameSuffix);
}

void CompositeGraph::findInterestingPaths(string fileNameSuffix){
    
    /*if(!PRINT_BARCODE)
        printCompositeGraphsWithPath(fileNameSuffix);
    
    printOriginarGraphs(fileNameSuffix);*/
}

void CompositeGraph::analyzePaths(int begin, size_t last, bool allPaths, Graph* graph, string fileNameSuffix){
    size_t low = begin, high=last;
    
    if(begin<0) low=0;
    if(high>=interestingPaths.getSize()) high=interestingPaths.getSize()-1;
    
    if(allPaths){
        low=0;
        high=interestingPaths.getSize()-1;
    }
    
    unordered_map<int, Path*> allPathsMap = interestingPaths.getMap();
    list<Path*> pathList;
    int counter = 0;
    
    for(unordered_map<int, Path*>::iterator itr=allPathsMap.begin(); itr!=allPathsMap.end(); itr++,counter++){
        if(counter>=low && counter<=high){
            pathList.push_back(itr->second);
        }
        
        if(counter>high){
            break;
        }
    }
    
    pathList.sort(comparePaths);
    
    // key: genotype,location,id,path
    map<string,Individual*> phPathTable;
    map<string,Individual*>::iterator ppItr;
    int pathID=0;
    
    for(list<Path*>::iterator itr=pathList.begin();itr!=pathList.end(); itr++){
        Path *path = *itr;
        pathID++;
        
        list<long> nList = path->getAllNodesInAPath();
        //long pathLength = nList.size();
        unordered_set<string> AllInd;
        unordered_map<long, unordered_set<string> > nodeIndv;
        unordered_map<long, unordered_set<string> >::iterator nodeIndvItr;
        
        for(list<long>::iterator nItr=nList.begin(); nItr!=nList.end(); nItr++){
            long n = *nItr;
            unordered_set<string> tmp;
            
            unordered_map<string,list<Phenotype*> >::iterator nodeItr = graph->node.nodeList.find(to_string(n));
            list<Phenotype*> pList = nodeItr->second;
            
            for(list<Phenotype*>::iterator pItr=pList.begin(); pItr!=pList.end(); pItr++){
                Phenotype* ph = *pItr;
                
                if(PLANT_ID_LIST){
                    list<long> pIdList = ph->getPlantIdList();
                    
                    for(list<long>::iterator litr=pIdList.begin(); litr!=pIdList.end(); litr++){
                        string key = ph->getGenotype() + "#" + ph->GetSite() + "#" + to_string(*litr);
                        
                        AllInd.insert(key);
                        tmp.insert(key);
                        
                        if((ppItr=phPathTable.find(key)) == phPathTable.end()){
                            Individual* ind = new Individual(ph->getGenotype(), ph->GetSite(), ph->GetVariety());
                            ind->addNodeId(n);
                            
                            phPathTable.insert(pair<string, Individual*>(key,ind));
                        }else{
                            ppItr->second->addNodeId(n);
                        }
                    }
                }else{
                    
                    string key = ph->getGenotype() + "#" + ph->GetSite() + "#" + to_string(ph->GetVariety());
                    
                    AllInd.insert(key);
                    tmp.insert(key);
                    
                    if((ppItr=phPathTable.find(key)) == phPathTable.end()){
                        Individual* ind = new Individual(ph->getGenotype(), ph->GetSite(), ph->GetVariety());
                        ind->addNodeId(n);
                        
                        phPathTable.insert(pair<string, Individual*>(key,ind));
                    }else{
                        ppItr->second->addNodeId(n);
                    }
                }
                
            }
            
            if((nodeIndvItr=nodeIndv.find(n)) == nodeIndv.end()){
                nodeIndv.insert(pair<long, unordered_set<string> >(n, tmp));
            }
        }
        
        long totalMiss=0;
        for(nodeIndvItr=nodeIndv.begin(); nodeIndvItr!=nodeIndv.end(); nodeIndvItr++){
            //long nid = nodeIndvItr->first;
            unordered_set<string> s = nodeIndvItr->second;
            long miss=AllInd.size()-s.size();
            
            totalMiss += miss;
        }
        
        //cout<<"\nMean different in Path #"<<pathID<<": "<<((float)totalMiss/nodeIndv.size())<<"\n";
        
    }
    
    string csv = "Genotype,Location,PlantID,PathID,nodes,Total nodes,Node covered\n";
    
    for(ppItr=phPathTable.begin(); ppItr!=phPathTable.end();ppItr++){
        
        Individual* indv = ppItr->second;
        
        csv += indv->getTableInCsv();
    }
    
    FileHandler* fileHandler = new FileHandler("");
    fileHandler->WriteDataToFile("Path_stats_"+fileNameSuffix, "csv", csv, false);
    delete fileHandler;
}

float CompositeGraph::getMeanValue(list<float> val){
    size_t size = val.size();
    float sum=0;
    
    for(list<float>::iterator itr=val.begin(); itr!=val.end(); itr++){
        sum += *itr;
    }
    
    return (sum/size);
}

void CompositeGraph::analyzeAndSummarizePaths(string fileNameSuffix){
    unordered_map<int, Path*> allPathsMap = interestingPaths.getMap();
    list<Path*> pathList;
    int counter = 0;
    
    for(unordered_map<int, Path*>::iterator itr=allPathsMap.begin(); itr!=allPathsMap.end(); itr++,counter++){
        pathList.push_back(itr->second);
    }
    
    pathList.sort(comparePaths);
    
    // key: genotype,location,id,path
    map<string,Individual*> phPathTable;
    map<string,Individual*>::iterator ppItr;
    int pathID=0;
    string csv = "PathID,Location,Genotype,PlantID,DAP,nodeOrder,nodeID,Avg.Growth.Rate,plant.id.color,Loc.Gen.color\n";
    string plantColor[]={""};
    
    unordered_map<string, list<float> > pathSummary;
    unordered_map<string, list<float> >::iterator pathItr;
    
    for(list<Path*>::iterator itr=pathList.begin();itr!=pathList.end(); itr++){
        Path *path = *itr;
        pathID++;
        
        list<long> nList = path->getAllNodesInAPath();
        //long pathLength = nList.size();
        int nodeOrder = 0;
        
        for(list<long>::iterator nItr=nList.begin(); nItr!=nList.end(); nItr++){
            long nID = *nItr;
            nodeOrder++;
            
            rNode* n = nodeList.getValue(nID);
            
            unordered_map<string, list<float> > phSummary = n->getPhenotyeSummary();
            for(unordered_map<string, list<float> >::iterator phItr=phSummary.begin(); phItr!=phSummary.end(); phItr++){
                string key = phItr->first;
                list<float> phValList = phItr->second;
                
                size_t pos = key.find("#");
                string genotype = key.substr(0, pos);
                key = key.substr(pos+1, key.length()-pos-1);
                
                pos = key.find("#");
                string location = key.substr(0, pos);
                key = key.substr(pos+1, key.length()-pos-1);
                
                pos = key.find("#");
                string dap = key.substr(0, pos);
                key = key.substr(pos+1, key.length()-pos-1);
                
                string plantID = key.substr(0, key.length());
                
                string pCol = getPlantIdColor(stoi(plantID));
                
                key = to_string(pathID) + "$" + genotype + "$" + location + "$" + plantID + "$" + dap + "$" + to_string(nID) + "$" + to_string(nodeOrder) + "$" + pCol + "$" + getGenLocColor(genotype, location);
                
                if((pathItr=pathSummary.find(key)) == pathSummary.end()){
                    pathSummary.insert(pair<string, list<float> >(key, phValList));
                }else{
                    pathItr->second.merge(phValList);
                }
            }
        }
    }
    
    for(pathItr=pathSummary.begin(); pathItr!=pathSummary.end(); pathItr++){
        string key = pathItr->first;
        list<float> phValList = pathItr->second;
        
        size_t pos = key.find("$");
        string pathNumber = key.substr(0, pos);
        key = key.substr(pos+1, key.length()-pos-1);
        
        pos = key.find("$");
        string genotype = key.substr(0, pos);
        key = key.substr(pos+1, key.length()-pos-1);
        
        pos = key.find("$");
        string location = key.substr(0, pos);
        key = key.substr(pos+1, key.length()-pos-1);
        
        pos = key.find("$");
        string plantID = key.substr(0, pos);
        key = key.substr(pos+1, key.length()-pos-1);
        
        pos = key.find("$");
        string dap = key.substr(0, pos);
        key = key.substr(pos+1, key.length()-pos-1);
        
        pos = key.find("$");
        string nodeID = key.substr(0, pos);
        key = key.substr(pos+1, key.length()-pos-1);
        
        pos = key.find("$");
        string nodeOrder = key.substr(0, pos);
        key = key.substr(pos+1, key.length()-pos-1);
        
        pos = key.find("$");
        string color = key.substr(0, pos);
        key = key.substr(pos+1, key.length()-pos-1);
        
        string glCol = key.substr(0, key.length());
        
        csv += pathNumber + "," + location + "," + genotype + "," + plantID + "," + dap + "," + nodeOrder + "," + nodeID + "," + fixPrecision(getMeanValue(phValList), 2) + "," + color + "," + glCol + "\n";
    }
    
    FileHandler* fileHandler = new FileHandler("");
    fileHandler->WriteDataToFile("Path_Summary_stats_"+fileNameSuffix, "csv", csv, false);
    delete fileHandler;
}

string CompositeGraph::analyzeNodes(){

    unordered_set<string> compare;
    unordered_map<long, rNode*> nList = nodeList.getMap();
    string csv = "";
    string d="";
    
    for(unordered_map<long, rNode*>::iterator nItr=nList.begin(); nItr!=nList.end(); nItr++){
        
        rNode* node = nItr->second;
        
        long n = node->getID();
        
        unordered_map<string, list<float> > phSummary = node->getPhenotyeSummary();
        for(unordered_map<string, list<float> >::iterator phItr=phSummary.begin(); phItr!=phSummary.end(); phItr++){
            string key = phItr->first;
            list<float> phValList = phItr->second;
            
            size_t pos = key.find("#");
            string genotype = key.substr(0, pos);
            key = key.substr(pos+1, key.length()-pos-1);
            
            pos = key.find("#");
            string location = key.substr(0, pos);
            key = key.substr(pos+1, key.length()-pos-1);
            
            pos = key.find("#");
            string dap = key.substr(0, pos);
            key = key.substr(pos+1, key.length()-pos-1);
            
            pos = key.find("#");
            string plantID = key.substr(0, pos);
            key = key.substr(pos+1, key.length()-pos-1);
            
            string filter = key.substr(0, key.length());
            
            string pCol = getPlantIdColor(stoi(plantID));
            
            csv += genotype + "," + location + "," + plantID + "," + dap + "," + to_string(n) + "," + pCol + "," + getGenLocColor(genotype, location) + "," + fixPrecision(getMeanValue(phValList), 2) + "," + filter + "\n";
            
        }
        
        
    }
    
    
    return csv;
    
    
}

//#pragma Main_graph_class
MainGraph::MainGraph(int ccCounter){
    this->ccCounter = ccCounter;
    connectedComponents = new CompositeGraph[ccCounter];
}

MainGraph::~MainGraph(){
    delete []connectedComponents;
}

CompositeGraph* MainGraph::getCC(int index){
    if(index>=0&&index<ccCounter){
        return &connectedComponents[index];
    }
    
    return NULL;
}

void MainGraph::setEnvName(string *env){
    for(int i=0; i<FILTER;i++){
        envName[i] = env[i];
    }
}

void MainGraph::printCompositeGraph(string fileNameSuffix){
    string gmlData = "graph\n[\ndirected 1\nweighted 1\n";
    
    if(!DIRECTED_EDGE)
        gmlData = "graph\n[\ndirected 0\nweighted 1\n";
    
    string fileName_dag = "graph_COMPOSITE_";
    
    if(DELTA_CHANGE>0)
        fileName_dag += "ROBUST_" + fixPrecision(DELTA_CHANGE, 2) + "_";
    
    if(PRINT_SIGNATURE){
        fileName_dag += "SIGNATURE_";
    }
    
    fileName_dag += fileNameSuffix;
    
    for(int i=0;i<ccCounter; i++){
        gmlData += connectedComponents[i].printCompositeGraphsWithPath(fileNameSuffix);
    }
    
    gmlData += "]";
    
    FileHandler* fileHandler = new FileHandler("");
    fileHandler->WriteDataToFile(fileName_dag, "gml", gmlData, false);
    delete fileHandler;
}

void MainGraph::printOriginalGraph(string fileNameSuffix){
    bool isPhenotype;
    bool isEnv;
    int index=0;
    string type;
    string gmlData = "graph\n[\ndirected 1\nweighted 1\n";
    
    if(PRINT_BARCODE || !DIRECTED_EDGE)
        gmlData = "graph\n[\ndirected 0\nweighted 1\n";
    
    string fileName_dag = "graph_DAG_";
    if(EDGE_LABEL_WEIGHT){
        fileName_dag = "graph_DAG_COLOR_";
    }else if(EDGE_LABEL_SCORE){
        fileName_dag = "graph_DAG_SCORE_";
    }
    
    isPhenotype=true;
    isEnv=false;
    
    if(isPhenotype){
        type = "ph";
    }else if(isEnv){
        type = envName[index];
    }
    
    fileName_dag += type + "_";
    
    if(DELTA_CHANGE>0)
        fileName_dag += "ROBUST_" + fixPrecision(DELTA_CHANGE, 2) + "_";
    
    fileName_dag += fileNameSuffix;
    
    for(int i=0;i<ccCounter; i++){
        gmlData += connectedComponents[i].printOriginarGraphs(fileNameSuffix,isPhenotype, isEnv, index);
    }
    
    gmlData += "]";
    
    FileHandler* fileHandler = new FileHandler("");
    fileHandler->WriteDataToFile(fileName_dag, "gml", gmlData, false);
    delete fileHandler;
    
    
    if(!PRINT_BARCODE && false){
        isPhenotype=false;
        isEnv=true;
        for(int i=0; i<FILTER; i++){
            gmlData = "graph\n[\ndirected 1\nweighted 1\n";
            fileName_dag = "graph_DAG_";
            if(EDGE_LABEL_WEIGHT){
                fileName_dag = "graph_DAG_COLOR_";
            }else if(EDGE_LABEL_SCORE){
                fileName_dag = "graph_DAG_SCORE_";
            }
            if(isPhenotype){
                type = "ph";
            }else if(isEnv){
                type = envName[i];
            }
            
            fileName_dag += type + "_";
            
            if(DELTA_CHANGE>0)
                fileName_dag += "ROBUST_" + fixPrecision(DELTA_CHANGE, 2) + "_";
            
            fileName_dag += fileNameSuffix;
            
            for(int j=0;j<ccCounter; j++){
                gmlData += connectedComponents[j].printOriginarGraphs(fileNameSuffix,isPhenotype, isEnv, i);
            }
            
            gmlData += "]";
            
            fileHandler = new FileHandler("");
            fileHandler->WriteDataToFile(fileName_dag, "gml", gmlData, false);
            delete fileHandler;
        }
    }

}

void MainGraph::printAllGraphs(string fileNameSuffix){
    if(!PRINT_BARCODE)
        printCompositeGraph(fileNameSuffix);
    
    printOriginalGraph(fileNameSuffix);
}

void MainGraph::PrintAllNodeStatistics(string fileNameSuffix){
    string csv = "Genotype,Location,PlantID,DAP,NodeID,plant.id.color,Loc.Gen.color,Growth.rate,filter\n";
    
    for(int j=0;j<ccCounter; j++){
        csv += connectedComponents[j].analyzeNodes();
    }
    
    FileHandler* fileHandler = new FileHandler("");
    fileHandler->WriteDataToFile("Node_summary_"+ fileNameSuffix, "csv", csv, false);
    delete fileHandler;
}

