#ifndef POINTER_ANALYSIS
#define POINTER_ANALYSIS

#include "BasicAnalysis.h"
#include "PointerAnalysisLatticeNode.h"
#include "llvm/IR/Constants.h"

typedef map<string, set<string> >::iterator pa_map_it;

class PointerAnalysis: public BasicAnalysis{
public:

    PointerAnalysis(Function &F): BasicAnalysis(){
        //errs()<<"Pointer Analysis start.\n";
        createCFG(F);
    };

    LatticeNode *runFlowFunc(LatticeNode *in, CFGNode *curNode){
        //errs()<<"PointerAnalysis:: Enter into runFlowFunc\n";

        PointerAnalysisLatticeNode *newIn = static_cast<PointerAnalysisLatticeNode *>(in);
        Instruction *curInst = curNode->inst;
        string opName = curInst->getOpcodeName();
        int opCode = curInst->getOpcode();
        PointerAnalysisLatticeNode *rlt;

        ////errs()<<"RangeAnalysis:: runFlowFunc in->val size"<<newIn->val.size()<<"\n";

        ////errs()<<"RangeAnalysis:: good!\n";

        if(opCode == 28){//for A = &B
            rlt = visit_A_refB(newIn, curInst);
        }else if(opCode == 27){//load instruction
            //errs()<<"<<<load instruction!\n";
            Instruction *nextInst = curInst->getNextNode();
            Instruction *nextNextInst = nextInst->getNextNode();

            if(isa<StoreInst>(nextInst)){
                rlt = visit_A_B(newIn, curInst);
                return rlt;
            }

            if(isa<LoadInst>(nextInst) && isa<StoreInst>(nextNextInst)){
                if(!curInst->getOperand(0)->getName().empty() && !nextInst->getOperand(0)->getName().empty()){
                    rlt = visit_starA_B(newIn, curInst);
                }else if(!curInst->getOperand(0)->getName().empty() && !nextNextInst->getOperand(1)->getName().empty()){
                    rlt = visit_A_starB(newIn, curInst);
                }else{
                    return in;
                }
            }else{
                return in;
            }
        }

        return rlt;
          

        // if(opName == "add" || opName == "sub" || opName == "mul" || opName == "phi"){
        // //if(opName == "add" || opName == "sub" || opName == "mul"){
        //     rlt = visitAOpB(newIn, curInst);
        //     //errs()<<"PointerAnalysis:: val size: "<<rlt->val.size()<<"\n";
        //     //for(int i = 0; rlt->val.size();i++){
        //     for(map_it it = rlt->val.begin(); it!=rlt->val.end(); it++){
        //         //errs()<<it->first<<" "<<it->second->toString()<<"\n";
        //     }
        //     //errs()<<"PointerAnalysis:: runFlowFunc op will return \n";

        //     return rlt;

        // }else{
        //     //what should do here?
        //     return in;
        // }
    
        
    }

    LatticeNode *latticeNodeInit(){
        PointerAnalysisLatticeNode *rlt = new PointerAnalysisLatticeNode(BOTTOM);
        return rlt;
    }

    ~PointerAnalysis(){}
private:

    PointerAnalysisLatticeNode *visit_A_refB(PointerAnalysisLatticeNode *in, Instruction *inst){
        //errs()<<"PointerAnalysis::visit_A_refB enter!\n";
        StoreInst* storeInst = static_cast<StoreInst*>(inst);
        // X = &Y

        //if operand(1) is pointer
        if (storeInst->getOperand(1)->getType()->isPointerTy()) {
            if(!storeInst->getOperand(0)->getType()->isPointerTy())//also shoud be pointer
                return in;
            if(storeInst->getOperand(1)->getName()=="" || storeInst->getOperand(0)->getName() == ""){ 
                //a bit strange... NO NAME?
                return in;
            }
            ////errs()<<"PointerAnalysis::visit_A_refB enter! is pointer \n";
            PointerAnalysisLatticeNode* rlt = new PointerAnalysisLatticeNode();
            rlt->val = in->val;
            rlt->val[storeInst->getOperand(1)->getName()].insert(storeInst->getOperand(0)->getName());
            ////errs()<<"PointerAnalysis::visit_A_refB enter! is pointer. size:"<<rlt->val.size()<<"\n";
            return rlt;
        }else{
            return in;
        }
    }

    PointerAnalysisLatticeNode *visit_A_B(PointerAnalysisLatticeNode *in, Instruction *inst){
        //errs()<<"PointerAnalysis::visit_A_B enter!\n";

        Value* B = inst->getOperand(0); 
        Value* A = inst->getNextNode()->getOperand(1); 
        if(A->getType()->isPointerTy() && B->getType()->isPointerTy()){
            if(A->getName()=="" || B->getName() == "") return in;
            PointerAnalysisLatticeNode* rlt = new PointerAnalysisLatticeNode();
            rlt->val = in->val;
            ////errs()<<"name A: "<<A->getName()<<" B name: "<<B->getName()<<" set Size "<<in->val[B->getName()].size()<<"\n";
            rlt->val[A->getName()] = in->val[B->getName()];
            // for(set<string>::iterator it = in->val[B->getName()].begin(); it!= in->val[B->getName()].end(); it++){
            //     //errs()<<"!"<<*it<<"\n";
            // }
            return rlt;
        }

        return in;
    }

    PointerAnalysisLatticeNode *visit_starA_B(PointerAnalysisLatticeNode *in, Instruction *inst){
        //errs()<<"PointerAnalysis::visit_starA_B enter!\n";

        Value* A = inst->getNextNode()->getOperand(0);
        Value* B = inst->getOperand(0);

        if(A->getType()->isPointerTy() && B->getType()->isPointerTy()){
            //errs()<<"A: "<<A->getName()<<" B "<<B->getName()<<"\n";
            if(A->getName().empty() || B->getName().empty()) return in;
            //errs()<<"PointerAnalysis::visit_starA_B enter!!\n";
            PointerAnalysisLatticeNode* rlt = new PointerAnalysisLatticeNode();
            rlt->val = in->val;

            set<string> pointedByA = in->val[A->getName()];

            set<string>::iterator it = pointedByA.begin();
            set<string>::iterator it2;
            for (; it != pointedByA.end() ; it++) {
                //union two set
                //what A point to will add what B point to
                //errs()<<"PointerAnalysis::visit_starA_B enter!!!\n";
                for(it2 = in->val[B->getName()].begin(); it2 != in->val[B->getName()].end(); it2++)
                    rlt->val[*it].insert(*it2);
            }

            return rlt;
        }


        return in;
    }

    PointerAnalysisLatticeNode *visit_A_starB(PointerAnalysisLatticeNode *in, Instruction *inst){
        //errs()<<"PointerAnalysis::visit_A_starB enter!\n";
        Value* A = inst->getNextNode()->getNextNode()->getOperand(1);
        Value* B = inst->getOperand(0);
        if(A->getType()->isPointerTy() && B->getType()->isPointerTy()){
            if(A->getName().empty() || B->getName().empty()) return in;
            PointerAnalysisLatticeNode* rlt = new PointerAnalysisLatticeNode();
            rlt->val = in->val;
            
            set<string> pointedByB = in->val[B->getName()];
            for (set<string>::iterator C = pointedByB.begin(); C != pointedByB.end() ; C++) {
                for (set<string>::iterator D = in->val[*C].begin() ; D != in->val[*C].end() ; D++) {
                    rlt->val[A->getName()].insert(*D);
                }
            }

            return rlt;
        }


        return in;
    }

    // PointerAnalysisLatticeNode *visit_A_NULL(PointerAnalysisLatticeNode *in, Instruction *in){

    // }
    
};



#endif
