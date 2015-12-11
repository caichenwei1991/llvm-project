#ifndef POINTER_ANALYSIS_LATTICE_NODE
#define POINTER_ANALYSIS_LATTICE_NODE

#include "LatticeNode.h"
#include <climits>
#include <map>
#include <algorithm>
#include <sstream>
#include <set>


using namespace std;

class PointerAnalysisLatticeNode: public LatticeNode{
public:
    map<string, set<string> > val; //KEY: map that stores var and its range
    
    PointerAnalysisLatticeNode():LatticeNode(){
        //val = new map<string, Range *>;
    }
    PointerAnalysisLatticeNode(string s):LatticeNode(s){
        //val = new map<string, Range *>;
    }

    PointerAnalysisLatticeNode(PointerAnalysisLatticeNode *node):LatticeNode(node->basic){
        //errs()<<"PointerAnalysisLatticeNode::PointerAnalysisLatticeNode() good!\n";
        this->basic = node->basic;
        //errs()<<node->val.size();
        //errs()<<"RangeAnalysisLatticeNode::RangeAnalysisLatticeNode good!!\n";
        this->val = node->val;
        //errs()<<"RangeAnalysisLatticeNode::RangeAnalysisLatticeNode good!!!\n";
    }
    //RangeAnalysisLatticeNode(string s):LatticeNode(s){}//for top and bottom

    LatticeNode *joinWith(LatticeNode *other){
        //errs()<<"PointerAnalysis:: join with called!\n";
        PointerAnalysisLatticeNode *otherNode = static_cast<PointerAnalysisLatticeNode *>(other);
        //if one of the two nodes is a bottom:
        if(this->basic == BOTTOM || otherNode->basic == BOTTOM){
            //errs()<<"joinWith: One of the latticenode is bottom\n";
            if(this->basic==BOTTOM){
                return new PointerAnalysisLatticeNode(otherNode);
            }else{
                return new PointerAnalysisLatticeNode(this);
            }
        }
    
        //if one of the two nodes is a top
        if(this->basic == TOP || this->basic == TOP){
            return new PointerAnalysisLatticeNode(TOP);
        }
    
        //common situation
        PointerAnalysisLatticeNode *newNode = new PointerAnalysisLatticeNode();
        newNode->val = this->val;
        map<string, set<string> >::iterator it;
        for(it=otherNode->val.begin(); it!=otherNode->val.end();it++){
            if(newNode->val.find(it->first) != newNode->val.end()){
                errs()<<"Found. Let's join(union) the pointee.\n";
                set<string> otherSet = it->second;
                set<string> newSet = newNode->val[it->first];
                set<string>::iterator set_it = otherSet.begin();
                for(;set_it!=otherSet.end(); set_it++){
                    newSet.insert(*set_it);
                }
                newNode->val[it->first] = newSet;
            }else{
                errs()<<"Not found. just add it in the newnode.\n";
                newNode->val[it->first] = it->second;
            }
        }
        return newNode;
        
    }

    bool equalsTo(LatticeNode *other){
        //errs()<<"RangeAnalysis:: equals to called!\n";
        PointerAnalysisLatticeNode *otherNode = static_cast<PointerAnalysisLatticeNode*>(other);
        //if I am top or bottom:
        if(this->basic != ""){
            return this->basic == otherNode->basic;
        }
    
        //I am not top or bottom:
        if(this->val.size() != otherNode->val.size()) return false;
        map<string, set<string> >::iterator it = this->val.begin();
        //map<string, set<string>>::iterator it2 = other->val.begin();
        for(; it!=this->val.end(); it++){
            if(otherNode->val.find(it->first) != otherNode->val.end()){
                //have the same var
                set<string> a = it->second;
                set<string> b = otherNode->val[it->first];

                if(a.size()!=b.size()) return false;
                else{
                    for(set<string>::iterator it2 = a.begin(); it2!=a.end(); it2++){
                        if(b.find(*it2) == b.end()){
                            return false;
                        }
                    }
                }
            }else{
                errs()<<"PointerAnalysis:: equals to called! VALUE is False2!!!!!!!!!!!11\n";
                return false;
            }
        }
        errs()<<"PointerAnalysis:: equals to called! VALUE is TRUE!!!!!!!!!!!11\n";
        return true;
    }

    ~PointerAnalysisLatticeNode(){}
};

#endif
