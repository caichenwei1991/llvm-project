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

#include "../../include/AEAnalysis.h"


using namespace std;
using namespace llvm;
  

namespace {
    struct AEAnalysisPass : public FunctionPass {
    static char ID;
    vector<AEAnalysis *> AEAnalysisVec;
    AEAnalysisPass() : FunctionPass(ID) {}
    
    bool runOnFunction(Function &F){
        AEAnalysisVec.push_back(new AEAnalysis(F));
        //AEAnalysis * BA = new AEAnalysis(F);
        return true;
    }

    void print(llvm::raw_ostream &O, const Module *M) const{
        size_t i =0;
        for(; i<AEAnalysisVec.size(); i++){
            AEAnalysisVec[i]->runWorkList();
        }
    }
  };
}

char AEAnalysisPass::ID = 0;
static RegisterPass<AEAnalysisPass> X("aeAnalysis", "AE Analysis", false, false);

