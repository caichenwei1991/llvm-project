#include "../../include/RangeAnalysis.h"
/*

LatticeNode *RangeAnalysis::runFlowFunc(LatticeNode *in, CFGNode *curNode){
    RangeAnalysisLatticeNode *newIn = static_cast<RangeAnalysisLatticeNode *>(in);
    Instruction *curInst = curNode->inst;
    string opName = curInst->getOpcodeName();
    RangeAnalysisLatticeNode *rlt;

    if(opName == "add" || opName == "sub" || opName == "mul" || opName == "phi"){
        rlt = visitAOpB(newIn, curInst);
    }else{
        //what should do here?
    }

    return rlt;
}


LatticeNode *latticeNodeInit(){
    return new LatticeNode();
}

RangeAnalysisLatticeNode *RangeAnalysis::visitAOpB(RangeAnalysisLatticeNode *in, Instruction *inst){
    Value *leftOperand = inst->getOperand(0);
    Value *rightOperand = inst->getOperand(1);
    string opName = inst->getOpcodeName();

    //RangeAnalysisLatticeNode *tmp = new RangeAnalysisLatticeNode();
    Range *leftRange, *rightRange, *newRange;

    leftRange = getOperandRange(leftOperand, in);
    rightRange = getOperandRange(rightOperand, in);

    //op the range
    newRange = opRange(leftRange, rightRange, opName);

    //create new node and return it
    RangeAnalysisLatticeNode *rlt = new RangeAnalysisLatticeNode(in);
    rlt->val[inst->getName()] = newRange;
    return rlt;

}

Range *RangeAnalysis::getOperandRange(Value *operand, RangeAnalysisLatticeNode *in){
    //return the range of a given operand
    Range *range;
    if(isa<ConstantInt>(operand)){
        //is a constant
        ConstantInt *tmp = dyn_cast<ConstantInt>(operand);
        int value = tmp->getSExtValue();
        range = new Range(value, value);
    }else{
        //is a var
        if(in->val.count(operand->getName()) != 0)
            range = in->val[operand->getName()];
        else
            errs()<<"RA :: Cant find the var "<<operand->getName()<<"in the map\n";
    }
    return range;
}

Range *RangeAnalysis::opRange(Range *leftRange, Range *rightRange, string opName){
    //'op' two given range, and return it
    Range *newRange;
    if(opName == "add" || opName == "phi"){
        newRange = new Range(leftRange->left + rightRange->left, leftRange->right + rightRange->right);
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

    return newRange;
}

RangeAnalysis::~RangeAnalysis(){}
*/