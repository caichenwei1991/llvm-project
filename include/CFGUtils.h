#ifndef CFG_UTILS
#define CFG_UTILS


#include "llvm/IR/Instructions.h"
#include "LatticeNode.h"
#include <vector>

using namespace std;
using namespace llvm;

class CFGNode;
class CFGEdge;

class CFGNode{
public:
  unsigned idx; //the index of the current instruction/
  Instruction *inst;
  vector<CFGEdge *> inEdges;
  vector<CFGEdge *> outEdges;
  CFGNode(unsigned idx, Instruction* inst){
    this->idx = idx;
    this->inst = inst;
  }
};

class CFGEdge{
public:
  CFGNode *srcNode;
  CFGNode *dstNode;
  LatticeNode *latticeNode;
  CFGEdge(CFGNode *srcNode, CFGNode *dstNode){
    this->srcNode = srcNode;
    this->dstNode = dstNode;
  }
};

#endif
