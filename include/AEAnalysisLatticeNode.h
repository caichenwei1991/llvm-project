/*
\\                 May                Must
\\
\\ Forward     Reaching def.     **Available exp.**
\\
\\ Backward      Liveness        very busy exp.
\\
\\
\\  Top = empty   Botom = FS
\\
\\  merge = intersect
*/

#ifndef AE_ANALYSIS_LATTICE_NODE
#define AE_ANALYSIS_LATTICE_NODE

#include "LatticeNode.h"
#include <climits>
#include <map>
#include <algorithm>
#include "llvm/Support/raw_ostream.h"

using namespace llvm;


class AEAnalysisLatticeNode: public LatticeNode {
public:

    map<string, string> val;


    bool equalsTo(LatticeNode *other) {

        AEAnalysisLatticeNode * otherNode = static_cast<AEAnalysisLatticeNode*>(other);
        //if I am top or bottom:
        if(this->basic != "") {
            return this->basic == otherNode->basic;
        }else{ 
            //err() << "basic is null. \n"; 
        }
        
        //when it is neither yop nor bottom // compare both nodes map size
        if (this->val.size() != otherNode->val.size()) {
            return false;
        }
        //if two maps have same size, check contents
        for(map<string, string>::iterator it = this->val.begin(); it != this->val.end(); it++) {
            if(otherNode->val.find(it->first) != otherNode->val.end()) { 
                //same key found
                if(otherNode->val.find(it->first)->second != it->second){ //if key is matching but contents
                    return false; 
                }
                
            }
            //if(otherNode->val.find(it->second)->second != it->second) { return false; }
        }
        
        return true;
    }

    LatticeNode * joinWith(LatticeNode *other) {
        AEAnalysisLatticeNode * otherNode = static_cast<AEAnalysisLatticeNode*>(other);

        if (this->basic == BOTTOM && otherNode->basic == BOTTOM) { return new AEAnalysisLatticeNode(BOTTOM); }
        
        if (this->basic == TOP || otherNode->basic == TOP) { return new AEAnalysisLatticeNode(TOP); }
        
        if (this->basic == BOTTOM) { return new AEAnalysisLatticeNode(otherNode); }
        else if (otherNode->basic == BOTTOM) { return new AEAnalysisLatticeNode(this); }
        
        //common situation
        AEAnalysisLatticeNode *newNode = new AEAnalysisLatticeNode();
        newNode->val = this->val;
        
        for(map<string, string>::iterator it = newNode->val.begin(); it != newNode->val.end(); it++) {
            if(otherNode->val.find(it->first) == otherNode->val.end()) { 
                //no same key found, merge(intersect) both nodes to empty!
                //newNode->val[it->first] = it->second;
                newNode->val.erase(it);
            }else if(otherNode->val.find(it->first)->second == it->second) { //same key same value found
                //newNode->val[it->first] = it->second;
                errs() << " excuted line 54, AEAnalysisLatticeNode.cpp" <<'\n';
            }else { //same key found, check contents
                newNode->val.erase(it);
            }
            /*
            if(otherNode->val.find(it->first)->second != it->second){ //if key is matching but contents
                    newNode->val.erase(it);
                    //return false; 
            }
            */
            //if(otherNode->val.find(it->second)->second != it->second) { return false; }
        }
        return newNode;
    }

    AEAnalysisLatticeNode(){} // init
    AEAnalysisLatticeNode(string s):LatticeNode(s){}// initialize with TOP or BOTTOM
    AEAnalysisLatticeNode(AEAnalysisLatticeNode *node){
        errs()<<"AEAnalysisLatticeNode::AEAnalysisLatticeNode good!\n";
        this->basic = node->basic;
        errs()<<node->val.size();
        errs()<<"AEAnalysisLatticeNode::AEAnalysisLatticeNode good!!\n";
        this->val = node->val;
        errs()<<"AEAnalysisLatticeNode::AEAnalysisLatticeNode good!!!\n";
    }
    ~AEAnalysisLatticeNode(){}


    //fns
    //void printErr();

    //bool isTopOrBottom();

};

#endif 
