#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instruction.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include <map>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "llvm/IR/IRBuilder.h"

#include "../../include/AEAnalysis.h"

namespace {
    struct AEAnalysisPass : public FunctionPass {
        static char ID;
        vector<AEAnalysis *> AEAnalysisVec;
        AEAnalysisPass() : FunctionPass(ID) {}

        virtual bool runOnFunction(Function &F) {
            AEAnalysisVec.push_back(new AEAnalysis(F));
            for (unsigned int i = 0; i < AEAnalysisVec.size(); i++)
                AEAnalysisVec[i]->runWorkList();
                //errs() << AEAnalysisVec.size() << "\n";
                errs() << "Inst = op, LtO, RtO"<< "\n";

            for (unsigned int i = 0; i < AEAnalysisVec.size(); i++){
                for (unsigned int j = 0; j < AEAnalysisVec[i]->CFGEdges.size(); j++) {
                    AEAnalysisLatticeNode *tmp = static_cast<AEAnalysisLatticeNode*>(AEAnalysisVec[i]->CFGEdges[j]->latticeNode);
                    errs() << "Output of Node #" << j << "\n";
                    for (map<string, string>::iterator it = tmp->val.begin(); it != tmp->val.end(); it++) {
                        
                        errs() << it->first << " = " << it->second << "\n";
                    }
                    errs() << "\n";
                }
            }
            errs() << "Done!" << "\n";
        //}
            return false;
        }





    };
}

char AEAnalysisPass::ID = 0;
static RegisterPass<AEAnalysisPass> X("aeAnalysis", "AE Analysis", false, false);

    /*
    void print(llvm::raw_ostream &O, const Module *M) const{
         errs() << "AEpass::print\n";
        //size_t i =0;
        for(unsigned int i = 0; i<AEAnalysisVec.size(); i++){
            AEAnalysisVec[i]->runWorkList();
        }
    }*/