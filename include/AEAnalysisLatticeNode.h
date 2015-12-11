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
#include <string>
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
        errs()<< ">>>>>>>>>> LatticeNode * joinWith start <<<<<<<<<<\n";
        //errs()<< "otherNode size () = " << otherNode->val.size() << '\n';
        //errs()<< "otherNode size () = " << otherNode->basic << '\n';

        if (this->basic == TOP || otherNode->basic == TOP) { 
            "I just entered 4... \n";
            return new AEAnalysisLatticeNode(TOP); 
        }

        if (this->basic == BOTTOM && otherNode->basic == BOTTOM) { 
            errs()<< "I just entered 1... \n"; 
            return new AEAnalysisLatticeNode(BOTTOM); 
        }
        
        AEAnalysisLatticeNode *newNode = new AEAnalysisLatticeNode();
        if (this->basic == BOTTOM) { 
            "I just entered 2... \n"; 
            return new AEAnalysisLatticeNode(otherNode); 
        }
        if (otherNode->basic == BOTTOM) { 
            "I just entered 3... \n";
            return new AEAnalysisLatticeNode(this);
        }
        //common situation
        
        newNode->val = this->val;
        errs()<< "size of newNode-val :>>" << newNode->val.size() << " \n";


    for (map<string, string>::iterator it = this->val.begin(); it != this->val.end(); it++) {

        if (otherNode->val.find(it->first) == otherNode->val.end()) {
            newNode->val[it->first] = this->val.find(it->first)->second;

        } else {

            string otherNodeVal = otherNode->val.find(it->first)->second;
            string thisVal = this->val.find(it->first)->second;

            if (otherNodeVal == thisVal)

                newNode->val[it->first] = otherNodeVal;

        }
    }

    for (map<string, string>::iterator it = otherNode->val.begin(); it != otherNode->val.end(); it++) {

        if (this->val.find(it->first) == this->val.end()) {
            newNode->val[it->first] = otherNode->val.find(it->first)->second;
        } else {
            string thisVal = this->val.find(it->first)->second;
            string otherNodeVal = otherNode->val.find(it->first)->second;

            if (otherNodeVal == thisVal)
                newNode->val[it->first] = otherNodeVal;
        }
    }
        errs()<< "Check newNode :>>" << newNode->val.size() << " \n";
        return newNode;
    }

    AEAnalysisLatticeNode():LatticeNode(){} // init
    AEAnalysisLatticeNode(string s):LatticeNode(s){}// initialize with TOP or BOTTOM
    AEAnalysisLatticeNode(AEAnalysisLatticeNode *node):LatticeNode(AEAnalysisLatticeNode(node->basic)){
        //errs()<<"00AEAnalysisLatticeNode::AEAnalysisLatticeNode good!!\n";
        this->basic = node->basic;
        
        this->val = node->val;
        //errs()<< "this->val size: " << this->val.size() << '\n';
    }
    
    ~AEAnalysisLatticeNode(){}


    //fns
    //void printErr();

    //bool isTopOrBottom();

};

#endif 


        // for(map<string, string>::iterator it = newNode->val.begin(); it != newNode->val.end(); it++) {
        //     if(otherNode->val.find(it->first) == otherNode->val.end()) { 
        //         //no same key found, merge(intersect) both nodes to empty!
        //         //newNode->val[it->first] = it->second;
        //         newNode->val.erase(it);
        //     }else if(otherNode->val.find(it->first)->second == it->second) { //same key same val found
        //         //newNode->val[it->first] = it->second;
        //         errs() << " excuted line 54, AEAnalysisLatticeNode.cpp" <<'\n';
        //     }else { //same key found, check contents
        //         newNode->val.erase(it);
        //     }
        //     /*
        //     if(otherNode->val.find(it->first)->second != it->second){ //if key is matching but contents
        //             newNode->val.erase(it);
        //             //return false; 
        //     }
        //     */
        //     //if(otherNode->val.find(it->second)->second != it->second) { return false; }
        // }
