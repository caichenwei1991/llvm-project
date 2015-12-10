#ifndef CONSTANT_PROP_ANALYSIS_H
#define CONSTANT_PROP_ANALYSIS_H

#include "BasicAnalysis.h"
#include "ConstantPropAnalysisLatticeNode.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

using namespace std;
using namespace llvm;

class ConstantPropAnalysis: public BasicAnalysis {
private:

public:
	ConstantPropAnalysis(Function &F):BasicAnalysis() {
		errs()<<"##### Constant Propagation Analysis starts #####\n";
		createCFG(F);	
	}

	~ConstantPropAnalysis() {}

	LatticeNode *latticeNodeInit() {
		//errs() << "ConstantAnalysis::latticeNodeInit\n";
		//return new ConstantPropAnalysisLatticeNode(ConstantPropAnalysisLatticeNode::BOTTOM); 
		return new ConstantPropAnalysisLatticeNode(BOTTOM); 
	}

	LatticeNode *runFlowFunc(LatticeNode *in, CFGNode *curNode) {
		ConstantPropAnalysisLatticeNode *newIn = static_cast<ConstantPropAnalysisLatticeNode *>(in);
		ConstantPropAnalysisLatticeNode *out;
		Instruction *inst = curNode->inst;
		//check $ROOTCSE231/llvm/src/include/llvm/IR/Instruction.def
		//errs() << inst->getOpcode() << "\n";
		switch(inst->getOpcode()) {
		case 8: // Add 
		case 10: // Sub
			out = executeOpInst(newIn, inst, inst->getOpcode());
			break;
		//case 26:
		//case 27:
		case 28:
			//out = executeStore(inst);
			break;
			
		default: 
			out = new ConstantPropAnalysisLatticeNode();
			out->basic = newIn->basic;
			out->value = newIn->value; 
			break;
		}
		return out;	
	}
	/*
	ConstantPropAnalysisLatticeNode *executeStore(ConstantPropAnalysisLatticeNode *in, Instruction *inst) {
		Value *left = inst->getOperand(0);
		Value *right = inst->getOperand(1);
		map<string, float> value;
                ConstantPropAnalysisLatticeNode *f = new ConstantPropAnalysisLatticeNode();
                f->basic = in->basic;
                f->value = in->value;
        	return f;
	}*/

	float calcOpInst(float left, float right, unsigned opcode) {
		float result;
		switch(opcode) {
		case 8 : // Add
			result = left + right;
			break;
		case 10 : //Sub
			result = left - right;
			break;
		case 12 :
			result = left * right;
			break;
		case 15 :
			result = left / right;
			break;
		}
		return result;
	}

	ConstantPropAnalysisLatticeNode *executeOpInst(ConstantPropAnalysisLatticeNode *in, Instruction *inst, unsigned opcode) {
		Value *left = inst->getOperand(0);
		Value *right = inst->getOperand(1);
		map<string, float> value;
		ConstantPropAnalysisLatticeNode *f = new ConstantPropAnalysisLatticeNode();
		f->basic = in->basic;
		f->value = in->value;
		Value *k = inst;
		if (ConstantInt *CIleft = dyn_cast<ConstantInt>(left)) {
			if (ConstantInt *CIright = dyn_cast<ConstantInt>(right)) {
				float leftval = CIleft->getZExtValue();
				float rightval = CIright->getZExtValue();
				float result = calcOpInst(leftval, rightval, opcode);
				ConstantPropAnalysisLatticeNode *ff = new ConstantPropAnalysisLatticeNode();
				value[k->getName()] = result;
				ff->value = value;
				ConstantPropAnalysisLatticeNode *t = static_cast<ConstantPropAnalysisLatticeNode*>(ff->joinWith(f));
				//errs() << inst->getName() << " : " << t->value[inst->getName()] << "\n";
				return t;
			} else {
				if (f->value.find(right->getName()) != f->value.end()) {
				float leftval = CIleft->getZExtValue(); 
               	 	        float rightval = f->value.find(right->getName())->second;
               	        	float result = calcOpInst(leftval, rightval, opcode);
				ConstantPropAnalysisLatticeNode *ff = new ConstantPropAnalysisLatticeNode();
				value[k->getName()] = result;
				ff->value = value;
				ConstantPropAnalysisLatticeNode *t = static_cast<ConstantPropAnalysisLatticeNode*>(ff->joinWith(f));
				return t;
				}
			}
		} else {
			if (ConstantInt *CIright = dyn_cast<ConstantInt>(right)) {			
				float rightval = CIright->getZExtValue(); 
	                        float leftval = f->value.find(left->getName())->second;
        	                float result = calcOpInst(leftval, rightval, opcode);
				ConstantPropAnalysisLatticeNode *ff = new ConstantPropAnalysisLatticeNode();
				value[k->getName()] = result;
				ff->value = value;
				ConstantPropAnalysisLatticeNode *t = static_cast<ConstantPropAnalysisLatticeNode*>(ff->joinWith(f));
				return t;
			} else {
				float rightval = f->value.find(right->getName())->second;  
                       		float leftval = f->value.find(left->getName())->second;
                        	float result = calcOpInst(leftval, rightval, opcode);
				ConstantPropAnalysisLatticeNode *ff = new ConstantPropAnalysisLatticeNode();
				value[k->getName()] = result;
				ff->value = value;
				ConstantPropAnalysisLatticeNode *t = static_cast<ConstantPropAnalysisLatticeNode*>(ff->joinWith(f));
				return t;
			}
		}
		return f;
	}
};



#endif
