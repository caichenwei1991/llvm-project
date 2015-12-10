#ifndef RANGE_ANALYSIS_LATTICE_NODE
#define RANGE_ANALYSIS_LATTICE_NODE

#include "LatticeNode.h"
#include <climits>
#include <map>
#include <algorithm>
#include <sstream>


using namespace std;

class Range{
public:
    int left, right;
    Range(int left, int right){
        this->left = left;
        this->right = right;
    }

    Range(){
        //default bottom value with range [INT_MAX, INT_MIN]
        //BTW, top value is range[INT_MIN, INT_MAX]
        this->left = INT_MAX;
        this->right = INT_MIN;
    }

    bool equalsTo(Range* other){
        return (this->left == other->left) && (this->right == other->right);
    }
    string toString(){
        std::ostringstream s1;
        std::ostringstream s2;
        s1 << left;
        s2 << right;
        return "["+s1.str()+","+s2.str()+"]";
    }
};

class RangeAnalysisLatticeNode: public LatticeNode{
public:
    map<string, Range *> val; //KEY: map that stores var and its range
    
    RangeAnalysisLatticeNode():LatticeNode(){
        //val = new map<string, Range *>;
    }
    RangeAnalysisLatticeNode(string s):LatticeNode(s){
        //val = new map<string, Range *>;
    }

    RangeAnalysisLatticeNode(RangeAnalysisLatticeNode *node):LatticeNode(node->basic){
        errs()<<"RangeAnalysisLatticeNode::RangeAnalysisLatticeNode good!\n";
        this->basic = node->basic;
        //errs()<<node->val.size();
        //errs()<<"RangeAnalysisLatticeNode::RangeAnalysisLatticeNode good!!\n";
        this->val = node->val;
        errs()<<"RangeAnalysisLatticeNode::RangeAnalysisLatticeNode good!!!\n";
    }
    //RangeAnalysisLatticeNode(string s):LatticeNode(s){}//for top and bottom

    LatticeNode *joinWith(LatticeNode *other){
        errs()<<"RangeAnalysis:: join with called!\n";
        RangeAnalysisLatticeNode *otherNode = static_cast<RangeAnalysisLatticeNode *>(other);
        //if one of the two nodes is a bottom:
        if(this->basic == BOTTOM || otherNode->basic == BOTTOM){
            errs()<<"joinWith: One of the latticenode is bottpm\n";
            if(this->basic==BOTTOM){
                return new RangeAnalysisLatticeNode(otherNode);
            }else{
                return new RangeAnalysisLatticeNode(this);
            }
        }
    
        //if one of the two nodes is a top
        if(this->basic == TOP || this->basic == TOP){
            return new RangeAnalysisLatticeNode(TOP);
        }
    
        //common situation
        RangeAnalysisLatticeNode *newNode = new RangeAnalysisLatticeNode();
        newNode->val = this->val;
        map<string, Range*>::iterator it;
        for(it=otherNode->val.begin(); it!=otherNode->val.end();it++){
            if(newNode->val.find(it->first) != newNode->val.end()){
                //Found. Let's join the range
                Range *r1 = newNode->val[it->first];
                Range *r2 = it->second;
                r1->left = min(r1->left, r2->left);
                r1->right = max(r1->right, r2->right);
            }else{
                //Not found. just add it in the newnode
                newNode->val[it->first] = it->second;
            }
        }
        return newNode;
        
    }

    bool equalsTo(LatticeNode *other){
        //errs()<<"RangeAnalysis:: equals to called!\n";
        RangeAnalysisLatticeNode *otherNode = static_cast<RangeAnalysisLatticeNode*>(other);
        //if I am top or bottom:
        if(this->basic != ""){
            return this->basic == otherNode->basic;
        }
    
        //I am not top or bottom:
        if(this->val.size() != otherNode->val.size()) return false;
        map<string, Range*>::iterator it;
        for(it=this->val.begin(); it!=this->val.end(); it++){
            if(otherNode->val.find(it->first) != otherNode->val.end()){
                //have the same var
                Range *otherRange = otherNode->val[it->first];
                Range *myRange = it->second;
                if(!myRange->equalsTo(otherRange)){
                    errs()<<"RangeAnalysis:: equals to called! VALUE is False1!!!!!!!!!!!11\n";
                    return false;
                }
            }else{
                errs()<<"RangeAnalysis:: equals to called! VALUE is False2!!!!!!!!!!!11\n";
                return false;
            }
        }
        errs()<<"RangeAnalysis:: equals to called! VALUE is TRUE!!!!!!!!!!!11\n";
        return true;
    }

    ~RangeAnalysisLatticeNode(){}
};

#endif
