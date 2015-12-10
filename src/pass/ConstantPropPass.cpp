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
		//ConstantPropPass() : ModulePass(ID) {}
		virtual bool runOnFunction(Function &F) {
		//virtual bool runOnModule(Module &M) {
			//for (Module::iterator func = M.begin(); func != M.end(); func++) {
			cpa.push_back(new ConstantPropAnalysis(F));
			//for (unsigned int i = 0; i < cpa.size(); i++)
			cpa[cpa.size()-1]->runWorkList();
				//errs() << cpa.size() << "\n";
			//for (unsigned int i = 0; i < cpa.size(); i++) {
			errs() << "Run on Function "<< F.getName() << "\n";
				for (unsigned int j = 0; j < cpa[cpa.size()-1]->CFGEdges.size(); j++) {
					ConstantPropAnalysisLatticeNode *tmp = static_cast<ConstantPropAnalysisLatticeNode*>(cpa[cpa.size()-1]->CFGEdges[j]->latticeNode);
					errs() << "Output of Node #" << j << "\n";
					for (map<string, double>::iterator it = tmp->value.begin(); it != tmp->value.end(); it++) {
						errs() << it->first << " = " << it->second << "\n";
					}
					errs() << "\n";
				}
			//}
			errs() << "Done!" << "\n";
		//}
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
