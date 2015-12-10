#ifndef BASIC_ANALYSIS
#define BASIC_ANALYSIS

#include "CFGUtils.h"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instruction.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/IR/BasicBlock.h"

#include "LatticeNode.h"
#include "ConstantPropAnalysisLatticeNode.h"
#include <vector>
#include <queue>
#include <map>

using namespace llvm;
using namespace std;

class BasicAnalysis{
public:

  BasicAnalysis() {}
  BasicAnalysis(Function &F) {
    errs()<<"Basic Analysis start.\n";
    createCFG(F);
  }
  void createCFG(Function &F){
    //adopt BFS to build the CFG
    //errs()<<"1234";
    errs()<<"BasicAnalysis::createCFG Creating CFG..."<<"\n";
    
    int idx = 0;
    map<Instruction *, CFGNode *> inst_node;
    
    for(Function::iterator BB = F.begin(); BB != F.end(); BB++){
      for(BasicBlock::iterator inst = BB->begin(); inst != BB->end(); inst++){
        CFGNode *cur_node;
        if(inst_node.find(inst) != inst_node.end())
          cur_node = inst_node[inst];
        else{
          cur_node = new CFGNode(++idx, inst);
          inst_node[inst] = cur_node;
        }
        //head node
        if(BB == F.begin() && inst == BB->begin())
          this->CFGHead = cur_node;
        //get the successor instructions
        vector<Instruction *> nextInsts;
        if(!inst->isTerminator()){  //has only one successor
          BasicBlock::iterator tmp = inst;
          nextInsts.push_back(++tmp);
        }else{  //many successors
          const TerminatorInst* terminator = BB->getTerminator();
          for(size_t i=0; i < terminator->getNumSuccessors(); i++){
            Instruction *next_inst = terminator->getSuccessor(i)->begin();
            nextInsts.push_back(next_inst);
          }
        }

        //create node and edge
        for(size_t i=0; i<nextInsts.size(); i++){
          Instruction *next_inst = nextInsts[i];
          CFGNode *next_node;
          if(inst_node.find(next_inst) != inst_node.end())
            next_node = inst_node[next_inst];
          else{
            next_node = new CFGNode(++idx, next_inst);
            inst_node[next_inst] = next_node;
          }

          CFGEdge *edge = new CFGEdge(cur_node, next_node);
          cur_node->outEdges.push_back(edge);
          next_node->inEdges.push_back(edge);
          this->CFGEdges.push_back(edge);
        }
      }
    }
    //add into F's CFGNodes and CFGEdges
    map<Instruction *, CFGNode *>::iterator it = inst_node.begin();
    for(;it!=inst_node.end(); it++){
      this->CFGNodes.push_back(it->second);
    }
    //errs()<<"Size of all nodes:"<<CFGNodes.size()<<"\n";


    //test the createCFG:

    /*
    for(size_t i=0; i<this->CFGEdges.size(); i++){
      CFGEdge *e = this->CFGEdges[i];
      errs()<<e->srcNode->idx<<" "<<e->dstNode->idx<<"\n";
    }

     errs()<<"head is "<<this->CFGHead->idx;*/
     errs()<<"CFGcreate done!\n";
    
  }
  void runWorkList(){
    //errs()<<"Running BasicAnalysis::runWorkList..."<<"\n";
    queue<CFGNode *>workList;
    for(size_t i=0; i < CFGEdges.size(); i++){
        CFGEdges[i]->latticeNode = latticeNodeInit();
    }
    for(size_t i=0; i < CFGNodes.size(); i++){
        workList.push(CFGNodes[i]);
        //errs()<<"runWorkList::good!!"<<workList.size()<<"\n";
    }
    //errs()<<"runWorkList::good!";
    //errs()<<"runWorkList::good!!"<<workList.size()<<"\n";
    //run the worklist algorithm
    //int  aa = 0;
    while(!workList.empty()){
        //errs()<<"runWorkList::good!!"<<workList.size()<<"\n";
        CFGNode *curNode = workList.front();
        workList.pop();
        //join all the latticeNodes on curNode's inEdges.
        LatticeNode *in = latticeNodeInit();
        for(size_t i = 0; i < curNode->inEdges.size(); i++){
            CFGEdge *e = curNode->inEdges[i];
            LatticeNode *tmp = in->joinWith(e->latticeNode);
            delete in;
            in = tmp;
        }

        //run the flow function and get the new lattice node
        LatticeNode *out = runFlowFunc(in, curNode);
	//ConstantPropAnalysisLatticeNode *newout = static_cast<ConstantPropAnalysisLatticeNode*>(out);
	//errs() << "add" << " : " << newout->value["add"] << "\n";
	//errs() << "sub" << " : " << newout->value["sub"] << "\n";
        //check the out changed or not
        for(size_t i =0; i < curNode->outEdges.size(); i++){
            CFGEdge *e = curNode->outEdges[i];
            LatticeNode *newOut = out->joinWith(e->latticeNode);
            e->latticeNode = newOut;
            if(!out->equalsTo(newOut)){
                delete e->latticeNode;
                e->latticeNode = newOut;
                workList.push(e->dstNode);
            }
        }
        delete out;
    }
    errs()<<"Size of nodes:"<<this->CFGNodes.size()<<"\n";
    errs()<<"Worklist Done!\n";
};
  virtual LatticeNode *runFlowFunc(LatticeNode *in, CFGNode *curNode){}
  virtual LatticeNode *latticeNodeInit(){
    //return new LatticeNode(LatticeNode::BOTTOM);
    return new LatticeNode(BOTTOM);
  }// Can write in the createCFG func or not?
  virtual ~BasicAnalysis(){}


  vector<CFGNode *> CFGNodes;
  vector<CFGEdge *> CFGEdges;
  CFGNode *CFGHead;
private:

};
#endif


