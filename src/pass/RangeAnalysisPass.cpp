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

#include "../../include/RangeAnalysis.h"
//#include "RangeAnalysisLatticeNode.h"

using namespace std;
using namespace llvm;
  

namespace {
    struct RangeAnalysisPass : public FunctionPass {
    static char ID;
    vector<RangeAnalysis *> rangeAnalysisVec;
    RangeAnalysisPass() : FunctionPass(ID) {}
    
    bool runOnFunction(Function &F){
        RangeAnalysis *RA = new RangeAnalysis(F);
        RA->runWorkList();
        return true;
    }

    void print(llvm::raw_ostream &O, const Module *M) const{
        // size_t i =0;
        // for(; i<rangeAnalysisVec.size(); i++){
        //     rangeAnalysisVec[i]->runWorkList();
        // }
    }
  };
}

char RangeAnalysisPass::ID = 0;
static RegisterPass<RangeAnalysisPass> X("rangeAnalysis", "Range Analysis",false, false);

