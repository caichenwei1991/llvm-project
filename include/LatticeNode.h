#ifndef LATTICE_NODE
#define LATTICE_NODE

#include <string>

using namespace std;
//using namespace llvm;

class LatticeNode{
private:
  
public:
    static string TOP;
    static string BOTTOM;
    string basic;   //to mark the node is top or bottom

    LatticeNode(){
        basic = "";
    }
    LatticeNode(string s){
        if(s!="")
        basic = s;
    }
    //Join Function: for the merge operation, should be overridden by subclasses.
    virtual LatticeNode *joinWith(LatticeNode *other){
        //most basic(conservative) join
        return new LatticeNode(TOP);
    }   
    virtual bool equalsTo(LatticeNode *other){return true;}

    virtual ~LatticeNode(){};
};

string LatticeNode::TOP = "top";
string LatticeNode::BOTTOM = "bottom";
#endif


