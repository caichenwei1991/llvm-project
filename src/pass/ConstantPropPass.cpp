#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include <string>
#include <vector>
#include "../../include/ConstantPropAnalysis.h"
#include "../../include/ConstantPropAnalysisLatticeNode.h"
#include <iostream>

using namespace std;
using namespace llvm;

namespace {
	struct ConstantPropPass : public FunctionPass {
	//struct ConstantPropPass : public ModulePass {
		static char ID;
		vector<ConstantPropAnalysis *> cpa;
		ConstantPropPass() : FunctionPass(ID) {}
		virtual bool runOnFunction(Function &F) {
			cpa.push_back(new ConstantPropAnalysis(F));
			cpa[cpa.size()-1]->runWorkList();
			errs() << "\n==== The analysis result of function "<< F.getName()<<" (START) ====" << "\n";
			for (unsigned int j = 0; j < cpa[cpa.size()-1]->CFGEdges.size(); j++) {
				CFGNode *src = cpa[cpa.size()-1]->CFGEdges[j]->srcNode;
				CFGNode *dst = cpa[cpa.size()-1]->CFGEdges[j]->dstNode;
				ConstantPropAnalysisLatticeNode *tmp = static_cast<ConstantPropAnalysisLatticeNode*>(cpa[cpa.size()-1]->CFGEdges[j]->latticeNode);
				errs()<<"\n"<<j<<". "<<"FROM INSTRUCTION "<<*src->inst<<" TO "<<*dst->inst<<"\n";
				for (map<string, double>::iterator it = tmp->value.begin(); it != tmp->value.end(); it++) {
					errs() << it->first << " = " << it->second << "\n";
				}
				errs() << "\n";
			}
			errs() << "\n==== The analysis result of function "<< F.getName()<<" (END) ====" << "\n";
			return false;
		}
/*
		void print (raw_ostream &OS, const Module*) const {
			for (unsigned int i = 0; i < cpa.size(); i++)
				cpa[i]->runWorkList();
			errs() << "Done!" << "\n";
		}*/
	};
}


char ConstantPropPass::ID = 0;
static RegisterPass<ConstantPropPass> X("ConstantProp", "Constant Propagation Analysis Pass", false, false);
