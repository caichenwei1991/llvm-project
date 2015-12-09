#ifndef RANGE_ANALYSIS
#define RANGE_ANALYSIS

#include "BasicAnalysis.h"
#include "RangeAnalysisLatticeNode.h"
#include "llvm/IR/Constants.h"

class RangeAnalysis: public BasicAnalysis{
public:
    RangeAnalysis(Function &F): BasicAnalysis(){
        errs()<<"Range Analysis start.\n";
        createCFG(F);

    };

    LatticeNode *runFlowFunc(LatticeNode *in, CFGNode *curNode){
        errs()<<"RangeAnalysis:: runFlowFunc\n";

        RangeAnalysisLatticeNode *newIn = static_cast<RangeAnalysisLatticeNode *>(in);
        Instruction *curInst = curNode->inst;
        string opName = curInst->getOpcodeName();
        RangeAnalysisLatticeNode *rlt;

        errs()<<"RangeAnalysis:: runFlowFunc in->val size"<<newIn->val.size()<<"\n";


        //errs()<<"RangeAnalysis:: good!\n";
        if(opName == "add" || opName == "sub" || opName == "mul" || opName == "phi"){
            rlt = visitAOpB(newIn, curInst);
            errs()<<"RangeAnalysis:: runFlowFunc return \n";
            return rlt;
        }else{
            //what should do here?
            return in;
        }
    
        
    }
    LatticeNode *latticeNodeInit(){
        return new LatticeNode();
    }
    ~RangeAnalysis(){}
private:

    RangeAnalysisLatticeNode *visitAOpB(RangeAnalysisLatticeNode *in, Instruction *inst){
        errs()<<"RangeAnalysis::visitAOpB meet a sensitive op:"<<inst->getOpcodeName()<<"\n";

        Value *leftOperand = inst->getOperand(0);
        Value *rightOperand = inst->getOperand(1);
        string opName = inst->getOpcodeName();
    
        errs()<<"RangeAnalysis::visitAOpB good!\n";
        //RangeAnalysisLatticeNode *tmp = new RangeAnalysisLatticeNode();
        Range *leftRange, *rightRange, *newRange;
        leftRange = getOperandRange(leftOperand, in);
        rightRange = getOperandRange(rightOperand, in);
    
        //op the range
        newRange = opRange(leftRange, rightRange, opName);
    
        //create new node and return it
        errs()<<"RangeAnalysis::visitAOpB good!!\n";
        RangeAnalysisLatticeNode *rlt = new RangeAnalysisLatticeNode(in);
        errs()<<"RangeAnalysis::visitAOpB good!!!\n";
        rlt->val[inst->getName()] = newRange;
        errs()<<"RangeAnalysis::visitAOpB good!!!!\n";
        return rlt;

}
    Range *getOperandRange(Value *operand, RangeAnalysisLatticeNode *in){
        //return the range of a given operand
        errs()<<"RangeAnalysis::getOperandRange\n";
        Range *range;
        if(isa<ConstantInt>(operand)){
            //is a constant
            errs()<<"RangeAnalysis::getOperandRange is a constant\n";
            ConstantInt *tmp = dyn_cast<ConstantInt>(operand);
            int value = tmp->getSExtValue();
            range = new Range(value, value);
        }else{
            //is a var
            errs()<<"RangeAnalysis::getOperandRange is a var\n";
            if(in->val.count(operand->getName()) != 0)
                range = in->val[operand->getName()];
            else
                errs()<<"RA :: Cant find the var "<<operand->getName()<<"in the map\n";
        }
        return range;
    }

    Range *opRange(Range *leftRange, Range *rightRange, string opName){
        //'op' two given range, and return it
        errs()<<"RangeAnalysis::opRange\n";
        Range *newRange;
        if(opName == "add" || opName == "phi"){
            errs()<<"RangeAnalysis::opRange add or phi\n";
            newRange = new Range(leftRange->left + rightRange->left, leftRange->right + rightRange->right);
            errs()<<"RangeAnalysis::opRange good!\n";
        }
        if(opName == "sub"){
            newRange = new Range(leftRange->left - rightRange->right, leftRange->right - rightRange->left);
        }
        if(opName == "mul"){
            int tmp1 = leftRange->left * rightRange->right;
            int tmp2 = leftRange->left * rightRange->left;
            int tmp3 = leftRange->right * rightRange->left;
            int tmp4 = leftRange->right * rightRange->right;
            int minValue = min(tmp4, min(tmp3, min(tmp2, tmp1)));
            int maxValue = max(tmp4, max(tmp3, max(tmp2, tmp1)));
    
            newRange = new Range(minValue, maxValue);
        }
        errs()<<"RangeAnalysis::opRange return!\n";
        return newRange;
    }
    
};



#endif
