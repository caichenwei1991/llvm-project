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

namespace {
    struct AEAnalysisPass : public FunctionPass {
        static char ID;
        vector<AEAnalysis *> AEAnalysisVec;
        AEAnalysisPass() : FunctionPass(ID) {}

        
    bool runOnFunction(Function &F){
        AEAnalysis *ae = new AEAnalysis(F);
        ae->runWorkList();
        //print
        errs()<<"\n==== The analysis result of function "<<F.getName()<<" (START) ====\n";
        for(size_t i = 0; i < ae->CFGEdges.size(); i++){
            CFGNode *src = ae->CFGEdges[i]->srcNode;
            CFGNode *dst = ae->CFGEdges[i]->dstNode;
            LatticeNode *ln = ae->CFGEdges[i]->latticeNode;
            errs()<<"\n"<<i<<". "<<"FROM INSTRUCTION "<<*src->inst<<" TO "<<*dst->inst<<"\n";
            
            AEAnalysisLatticeNode *paln = static_cast<AEAnalysisLatticeNode *>(ln);
            map<string, string >::iterator it = paln->val.begin();
            for(;it!=paln->val.end();it++){

                errs() << it->first << " = " << it->second << "\n";
                //errs()<<"\n";
            }
        
        }
        errs()<<"\n==== The analysis result of function "<<F.getName()<<" (END) ====\n\n\n";
        return true;
    }


    };
}

char AEAnalysisPass::ID = 0;
static RegisterPass<AEAnalysisPass> X("aeAnalysis", "AE Analysis", false, false);



