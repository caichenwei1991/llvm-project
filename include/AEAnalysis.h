#ifndef AE_ANALYSIS
#define AE_ANALYSIS

#include "BasicAnalysis.h"
#include "AEAnalysisLatticeNode.h"
#include "llvm/IR/Constants.h"
#include <string>


using namespace llvm;
using namespace std;

class AEAnalysis: public BasicAnalysis{

public:
    AEAnalysis(Function &F):BasicAnalysis(F){
        errs()<<"============================>>AEAnalysis start.\n";
        createCFG(F);
    };


    LatticeNode *runFlowFunc(LatticeNode *in, CFGNode *currentNode){
        errs()<<"============================>>AEAnalysis:: runFlowFunc\n";
        AEAnalysisLatticeNode *inputIn = static_cast<AEAnalysisLatticeNode *>(in);
        AEAnalysisLatticeNode *returnIn;

        Instruction *currentInst = currentNode->inst;

        string opName = currentInst->getOpcodeName();

        if(opName == "add" || opName == "sub" || opName == "mul") {
            returnIn = analyzeAOpB(inputIn, currentInst);
        } else if (opName == "other") {
            //what should do here?
        } else { //there's no Operator? >>  F x:= y (in) ==
            //do sth here.
        } 

     //left hand side of the statement
        //string currentNodeRightOp = "";

        return returnIn;
    }


    LatticeNode *latticeNodeInit(){
        return new LatticeNode();
    }

    
    ~AEAnalysis(){}


private:
    AEAnalysisLatticeNode *analyzeAOpB(AEAnalysisLatticeNode *in, Instruction *currentInst){
        errs() << "entering AEAnalysis::analyzeAOpB ==========================================<<" << '\n' ;

        AEAnalysisLatticeNode * newIn = new AEAnalysisLatticeNode(in);

        string opName = currentInst->getOpcodeName();

        Value* leftOperandValue = currentInst->getOperand(0);
        string leftOperandStr = string(leftOperandValue->getName());

        Value* rightOperandValue = currentInst->getOperand(1);
        string rightOperandStr = string(rightOperandValue->getName());
        
        string currentInstLeftside = currentInst->getName();

        errs() << "This node is: " << currentInstLeftside << " = " << leftOperandStr << " " << opName << " " << rightOperandStr << "\n";

        //http://llvm.org/docs/doxygen/html/namespacellvm_1_1mdconst.html

        if (currentInstLeftside != 0) {
        //if ((ConstantInt* CILeft = dyn_cast<ConstantInt>(leftOperandValue)) && (ConstantInt* CIRight = dyn_cast<ConstantInt>(rightOperandValue))) {
        //both left right sides of op are constants

            for(map<string, string>::iterator it = newIn->val.begin(); it != newIn->val.end(); it++) {
                //string inNodeRightOp = it->second;

            } 
        } else {// if { //both sides are all variables
            //first  in - {x->*}
            for(map<string, string>::iterator it = newIn->val.begin(); it != newIn->val.end(); it++) {
                //string inNodeRightOp = it->second;
                if( currentInstLeftside == it->first ) { // x->* found in the latticeNode, remove it!
                    newIn->val.erase(it);

                }else if( rightOperandStr == it->second || leftOperandStr == it->second ) { //same key different value found //delete everything x assign to.
                    newIn->val.erase(it);

                }else { // ok to merge
                    newIn->val[currentInstLeftside] = "";
                }
            } 
            //second in - {*->x}

        }

        
        return newIn;
    }

};



#endif
