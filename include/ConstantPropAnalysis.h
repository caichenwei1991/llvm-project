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
		case 8	: // Add 
		case 10	: // Sub
		case 12	: // Mul
		case 14	: // UDIV
		case 15	: // FDIV
			out = executeOpInst(newIn, inst, inst->getOpcode());
			break;
		case 9	: // FAdd
		case 11	: // FSub
		case 13	: // FMul
		case 16	: // FDiv
			out = executeFOpInst(newIn, inst, inst->getOpcode());
			break;
		//case 26:
		//case 27:
		/*
		case 33	:
		case 34	:
		case 35	:
		case 36	:
		case 37	:
		case 38	:
		case 39	:
		case 40	:
		case 41	:
		case 42	:
		case 43	:
		case 44	:
			out = executeCastOp(newIn, inst);
			break;
		*/
		case 47	: //Phi
			out = executePhiOp(newIn, inst);
			
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
		map<string, double> value;
                ConstantPropAnalysisLatticeNode *f = new ConstantPropAnalysisLatticeNode();
                f->basic = in->basic;
                f->value = in->value;
        	return f;
	}*/

	double calcOpInst(double left, double right, unsigned opcode) {
		double result;
		switch(opcode) {
		case 8 : // Add
		case 9 : // FAdd
			result = left + right;
			break;
		case 10 : // Sub
		case 11 : // FSub
			result = left - right;
			break;
		case 12 : // Mul
		case 13	: // FMul
			result = left * right;
			break;
		case 14	: // UDiv
		case 15 : // SDiv
		case 16	: // FDiv
			result = left / right;
			break;
		}
		return result;
	}

	ConstantPropAnalysisLatticeNode *executeOpInst(ConstantPropAnalysisLatticeNode *in, Instruction *inst, unsigned opcode) {
		Value *left = inst->getOperand(0);
		Value *right = inst->getOperand(1);
		map<string, double> value;
		ConstantPropAnalysisLatticeNode *f = new ConstantPropAnalysisLatticeNode();
		f->basic = in->basic;
		f->value = in->value;
		Value *k = inst;
		if (ConstantInt *CIleft = dyn_cast<ConstantInt>(left)) {
			if (ConstantInt *CIright = dyn_cast<ConstantInt>(right)) {
				double leftval = CIleft->getZExtValue();
				double rightval = CIright->getZExtValue();
				double result = calcOpInst(leftval, rightval, opcode);
				ConstantPropAnalysisLatticeNode *ff = new ConstantPropAnalysisLatticeNode();
				value[k->getName()] = result;
				ff->value = value;
				ConstantPropAnalysisLatticeNode *t = static_cast<ConstantPropAnalysisLatticeNode*>(ff->joinWith(f));
				for (map<string, double>::iterator it = ff->value.begin(); it != ff->value.end(); it++){
					errs() << it->first << " : " << it->second << "\n";
				}
				errs() << "ok!\n";
				return t;
			} else {
				if (f->value.find(right->getName()) != f->value.end()) {
				double leftval = CIleft->getZExtValue(); 
               	 	        double rightval = f->value.find(right->getName())->second;
               	        	double result = calcOpInst(leftval, rightval, opcode);
				ConstantPropAnalysisLatticeNode *ff = new ConstantPropAnalysisLatticeNode();
				value[k->getName()] = result;
				ff->value = value;
				ConstantPropAnalysisLatticeNode *t = static_cast<ConstantPropAnalysisLatticeNode*>(ff->joinWith(f));
				return t;
				}
			}
		} else {
			if (ConstantInt *CIright = dyn_cast<ConstantInt>(right)) {			
				if (f->value.find(left->getName()) != f->value.end()) {
				double rightval = CIright->getZExtValue(); 
	                        double leftval = f->value.find(left->getName())->second;
        	                double result = calcOpInst(leftval, rightval, opcode);
				ConstantPropAnalysisLatticeNode *ff = new ConstantPropAnalysisLatticeNode();
				value[k->getName()] = result;
				ff->value = value;
				ConstantPropAnalysisLatticeNode *t = static_cast<ConstantPropAnalysisLatticeNode*>(ff->joinWith(f));
				return t;
				}
			} else {
				if (f->value.find(left->getName()) != f->value.end() && f->value.find(right->getName()) != f->value.end()) {
				double rightval = f->value.find(right->getName())->second;  
                       		double leftval = f->value.find(left->getName())->second;
                        	double result = calcOpInst(leftval, rightval, opcode);
				ConstantPropAnalysisLatticeNode *ff = new ConstantPropAnalysisLatticeNode();
				value[k->getName()] = result;
				ff->value = value;
				ConstantPropAnalysisLatticeNode *t = static_cast<ConstantPropAnalysisLatticeNode*>(ff->joinWith(f));
				return t;
				}
			}
		}
		return f;
	}

	ConstantPropAnalysisLatticeNode *executeFOpInst(ConstantPropAnalysisLatticeNode *in, Instruction *inst, unsigned opcode) {
		Value *left = inst->getOperand(0);
		Value *right = inst->getOperand(1);
		map<string, double> value;
		ConstantPropAnalysisLatticeNode *f = new ConstantPropAnalysisLatticeNode();
		f->basic = in->basic;
		f->value = in->value;
		Value *k = inst;
		if (ConstantFP *CFPleft = dyn_cast<ConstantFP>(left)) {
			if (ConstantFP *CFPright = dyn_cast<ConstantFP>(right)) {
				double leftval = CFPleft->getValueAPF().convertToFloat();
				double rightval = CFPright->getValueAPF().convertToFloat();
				double result = calcOpInst(leftval, rightval, opcode);
				ConstantPropAnalysisLatticeNode *ff = new ConstantPropAnalysisLatticeNode();
				value[k->getName()] = result;
				ff->value = value;
				ConstantPropAnalysisLatticeNode *t = static_cast<ConstantPropAnalysisLatticeNode*>(ff->joinWith(f));
				//errs() << inst->getName() << " : " << t->value[inst->getName()] << "\n";
				return t;
			} else {
				if (f->value.find(right->getName()) != f->value.end()) {
				double leftval = CFPleft->getValueAPF().convertToFloat();
               	 	        double rightval = f->value.find(right->getName())->second;
               	        	double result = calcOpInst(leftval, rightval, opcode);
				ConstantPropAnalysisLatticeNode *ff = new ConstantPropAnalysisLatticeNode();
				value[k->getName()] = result;
				ff->value = value;
				ConstantPropAnalysisLatticeNode *t = static_cast<ConstantPropAnalysisLatticeNode*>(ff->joinWith(f));
				return t;
				}
			}
		} else {
			if (ConstantFP *CFPright = dyn_cast<ConstantFP>(right)) {			
				if (f->value.find(left->getName()) != f->value.end()) {
				double rightval = CFPright->getValueAPF().convertToFloat();
	                        double leftval = f->value.find(left->getName())->second;
        	                double result = calcOpInst(leftval, rightval, opcode);
				ConstantPropAnalysisLatticeNode *ff = new ConstantPropAnalysisLatticeNode();
				value[k->getName()] = result;
				ff->value = value;
				ConstantPropAnalysisLatticeNode *t = static_cast<ConstantPropAnalysisLatticeNode*>(ff->joinWith(f));
				return t;
				}
			} else {
				if (f->value.find(left->getName()) != f->value.end() && f->value.find(right->getName()) != f->value.end()) {
				double rightval = f->value.find(right->getName())->second;  
                       		double leftval = f->value.find(left->getName())->second;
                        	double result = calcOpInst(leftval, rightval, opcode);
				ConstantPropAnalysisLatticeNode *ff = new ConstantPropAnalysisLatticeNode();
				value[k->getName()] = result;
				ff->value = value;
				ConstantPropAnalysisLatticeNode *t = static_cast<ConstantPropAnalysisLatticeNode*>(ff->joinWith(f));
				return t;
				}
			}
		}
		return f;
	}
/*
	ConstantPropAnalysisLatticeNode *executeCastOp(ConstantPropAnalysisLatticeNode *in, Instruction *inst) {
		ConstantPropAnalysisLatticeNode* f = new ConstantPropAnalysisLatticeNode();
		f->value = in->value;
		f->basic = in->basic;
		map<string, double> value;
        	Value *result = inst;
		CastInst *castInst = dyn_cast<CastInst>(inst);
		Value* casting = inst->getOperand(0);
		if (!dyn_cast<Constant>(result)) {
	                if (!dyn_cast<Constant>(casting)) {
                        	if (f->value.find(casting->getName()) != f->value.end()) {
                                float forwardVal = f->value.find(casting->getName())->second;
                                ConstantPropAnalysisLatticeNode* ff = new ConstantPropAnalysisLatticeNode();
                                Type* ttype = castInst->getDestTy();
                                if (ttype->isDoubleTy())
                                        forwardVal = (float) forwardVal;
                                else if(ttype->isFloatingPointTy())
                                        forwardVal = (float) forwardVal;
                                else if (ttype->isIntegerTy())
                                        forwardVal = (int) forwardVal;
				value[result->getName()] = forwardVal;
                                ff->value = value;
                                ConstantPropAnalysisLatticeNode* tmp = static_cast<ConstantPropAnalysisLatticeNode*>(ff->joinWith(f));
                                delete ff;
                                delete f;
                                f = tmp;
                                }
			} else {
				if (ConstantFP *cfp = dyn_cast<ConstantFP>(casting)) {
                                float forwardVal = cfp->getValueAPF().convertToFloat();
                                ConstantPropAnalysisLatticeNode* ff = new ConstantPropAnalysisLatticeNode();
                                value[result->getName()] = forwardVal;
                                ff->value = value;
                                ConstantPropAnalysisLatticeNode* tmp = static_cast<ConstantPropAnalysisLatticeNode*>(ff->joinWith(f));
                                delete ff;
                                delete f;
                                f = tmp;
                        	} else if (ConstantInt *cfp = dyn_cast<ConstantInt>(casting)) {
                                float forwardVal = cfp->getZExtValue();
                                ConstantPropAnalysisLatticeNode* ff = new ConstantPropAnalysisLatticeNode();
                                value[result->getName()] = forwardVal;
                                ff->value = value;
                                ConstantPropAnalysisLatticeNode* tmp = static_cast<ConstantPropAnalysisLatticeNode*>(ff->joinWith(f));
                                delete ff;
                                delete f;
                                f = tmp;
                        }
			}
		}
		return f;
	}*/
		ConstantPropAnalysisLatticeNode *executePhiOp(ConstantPropAnalysisLatticeNode *in, Instruction *inst) {
			ConstantPropAnalysisLatticeNode *f = new ConstantPropAnalysisLatticeNode();
			f->value = in->value;
			f->basic = in->basic;
			Value *left = inst->getOperand(0);
        		Value *right = inst->getOperand(1);
			map<string, double> value;
			Value *k = inst;
			if (!(f->value.find(left->getName()) == f->value.end() || f->value.find(right->getName()) == f->value.end())) {
				float leftval = f->value.find(left->getName())->second;
		                float rightval = f->value.find(right->getName())->second;
				if (leftval == rightval) {
					double result = leftval;
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
