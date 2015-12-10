#ifndef RANGE_ANALYSIS
#define RANGE_ANALYSIS

#include "BasicAnalysis.h"
#include "RangeAnalysisLatticeNode.h"
#include "llvm/IR/Constants.h"

typedef map<string, Range*>::iterator map_it;

class State{
public:
    //to record the instruction's state
    //bool repeat;
    RangeAnalysisLatticeNode *oldNode;
    State(){}
};


class RangeAnalysis: public BasicAnalysis{
public:
    map<Instruction *, RangeAnalysisLatticeNode *> inst_state;// to help us deal with loop

    RangeAnalysis(Function &F): BasicAnalysis(){
        errs()<<"Range Analysis start.\n";
        createCFG(F);
    };

    LatticeNode *runFlowFunc(LatticeNode *in, CFGNode *curNode){
        errs()<<"RangeAnalysis:: Enter into runFlowFunc\n";

        RangeAnalysisLatticeNode *newIn = static_cast<RangeAnalysisLatticeNode *>(in);
        Instruction *curInst = curNode->inst;
        string opName = curInst->getOpcodeName();
        RangeAnalysisLatticeNode *rlt;

        //errs()<<"RangeAnalysis:: runFlowFunc in->val size"<<newIn->val.size()<<"\n";


        //errs()<<"RangeAnalysis:: good!\n";
        if(opName == "add" || opName == "sub" || opName == "mul" || opName == "phi"){
        //if(opName == "add" || opName == "sub" || opName == "mul"){
            rlt = visitAOpB(newIn, curInst);
            errs()<<"RangeAnalysis:: val size: "<<rlt->val.size()<<"\n";
            //for(int i = 0; rlt->val.size();i++){
            for(map<string, Range*>::iterator it = rlt->val.begin(); it!=rlt->val.end(); it++){
                errs()<<it->first<<" "<<it->second->toString()<<"\n";
            }
            errs()<<"RangeAnalysis:: runFlowFunc op will return \n";


            return checkLoop(rlt, curInst);


        }else{
            //what should do here?
            return in;
        }
    
        
    }

    RangeAnalysisLatticeNode *checkLoop(RangeAnalysisLatticeNode *in, Instruction *inst){
        if(inst_state.count(inst)!=0){
            //this instruction has been met before
            errs()<<"RangeAnalysisLatticeNode::checkLoop Inst met before.\n";
            RangeAnalysisLatticeNode *old = inst_state[inst];
            RangeAnalysisLatticeNode *out = new RangeAnalysisLatticeNode();
            out->val = in->val;
            Range *oldRange = old->val[inst->getName()];
            Range *newRange = in->val[inst->getName()];
            Range *outRange;
            if(newRange->right > oldRange->right && newRange->left < oldRange->left){
                outRange = new Range(INT_MIN, INT_MAX);
            }else if(newRange->right > oldRange->right){
                outRange = new Range(oldRange->left, INT_MAX);
            }else if(newRange->left < oldRange->left){
                outRange = new Range(INT_MIN, oldRange->right);
            }else{
                outRange = new Range(oldRange->left, oldRange->right);
            }

            out->val[inst->getName()] = outRange;
            return out;
        }else{
            //first time to meet the inst
            errs()<<"RangeAnalysisLatticeNode::checkLoop Inst first time meet.";
            inst_state[inst] = new RangeAnalysisLatticeNode(in);
            errs()<<"RangeAnalysisLatticeNode::checkLoop Inst first time meet.!";
            return in;
        }
    }

    LatticeNode *latticeNodeInit(){
        
        RangeAnalysisLatticeNode *rlt = new RangeAnalysisLatticeNode(RangeAnalysisLatticeNode::BOTTOM);
        //rlt->val = new map<string, Range *>();
        return rlt;
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
        //errs()<<"RangeAnalysis::visitAOpB good!!\n";
        RangeAnalysisLatticeNode *rlt = new RangeAnalysisLatticeNode();
        rlt->val = in->val;
        rlt->val[inst->getName()] = newRange;
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
            if(in->val.count(string(operand->getName()))>0)
                range = in->val[operand->getName()];
            else{
                errs()<<"RA :: Cant find the var "<<operand->getName()<<" in the map\n";
                errs()<<"map size: "<<in->val.size()<<"\n";
                // for(map_it it = in->val.begin(); it!=in->val.end();it++){
                //     errs()<<it->first<<" "<<it->second->toString()<<"\n";
                // }
            }
        }
        return range;
    }

    Range *opRange(Range *leftRange, Range *rightRange, string opName){
        //'op' two given range, and return it
        errs()<<"RangeAnalysis::opRange\n";
        Range *newRange;
        if(opName == "phi"){
            //phi will introduce range
            newRange = new Range(min(leftRange->left, rightRange->left), max(leftRange->right, rightRange->right));
        }
        if(opName == "add"){
        //if(opName == "add"){
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
