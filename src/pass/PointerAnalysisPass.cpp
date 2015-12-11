#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instruction.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include <map>
#include <iomanip>
#include <iostream>
#include "llvm/IR/IRBuilder.h"

#include "../../include/PointerAnalysis.h"
//#include "RangeAnalysisLatticeNode.h"

using namespace std;
using namespace llvm;
   

namespace {
    struct PointerAnalysisPass : public FunctionPass {
    static char ID;
    vector<PointerAnalysis *> pointerAnalysisVec;
    PointerAnalysisPass() : FunctionPass(ID) {}
    
    bool runOnFunction(Function &F){
        PointerAnalysis *PA = new PointerAnalysis(F);
        PA->runWorkList();
        //print
        errs()<<"\n==== The analysis result of function "<<F.getName()<<" (START) ====\n";
        for(size_t i = 0; i < PA->CFGEdges.size(); i++){
            CFGNode *src = PA->CFGEdges[i]->srcNode;
            CFGNode *dst = PA->CFGEdges[i]->dstNode;
            LatticeNode *ln = PA->CFGEdges[i]->latticeNode;
            errs()<<"\n"<<i<<". "<<"FROM INSTRUCTION "<<*src->inst<<" TO "<<*dst->inst<<"\n";
            
            PointerAnalysisLatticeNode *paln = static_cast<PointerAnalysisLatticeNode *>(ln);
            map<string, set<string> >::iterator it = paln->val.begin();
            for(;it!=paln->val.end();it++){
                errs()<<it->first<<" -> ";
                set<string> s = it->second;
                for(set<string>::iterator it2 = s.begin(); it2!=s.end(); it2++){
                    errs()<<" "<<*it2<<" ";
                }
                errs()<<"\n";
            }
        
        }
        errs()<<"\n==== The analysis result of function "<<F.getName()<<" (END) ====\n\n\n";
        return true;
    }

    void print(llvm::raw_ostream &O, const Module *M) const{

    }
  };
}

char PointerAnalysisPass::ID = 0;
static RegisterPass<PointerAnalysisPass> X("pointerAnalysis", "Pointer Analysis",false, false);

