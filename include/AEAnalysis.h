#ifndef AE_ANALYSIS
#define AE_ANALYSIS

#include "BasicAnalysis.h"
#include "AEAnalysisLatticeNode.h"
#include "llvm/IR/Constants.h"
#include <string>
#include <sstream>

using namespace llvm;
using namespace std;

class AEAnalysis: public BasicAnalysis{

public:

    AEAnalysis(Function &F):BasicAnalysis(F){
        errs()<<"AEAnalysis start.\n";
        createCFG(F);
    };


    LatticeNode *runFlowFunc(LatticeNode *in, CFGNode *currentNode){
        errs()<<"============================>>AEAnalysis:: runFlowFunc\n";
        AEAnalysisLatticeNode *inputIn = static_cast<AEAnalysisLatticeNode *>(in);
        AEAnalysisLatticeNode *returnIn;

        Instruction *currentInst = currentNode->inst;

        string opName = currentInst->getOpcodeName();
        errs()<< "string name: " << opName << '\n';
        if(opName == "add" || opName == "sub" || opName == "mul") {
            returnIn = analyzeAOpB(inputIn, currentInst);
        } else if (opName == "other") {
            //what should do here?
        } else { //there's no Operator? >>  F x:= y (in) ==
            return in;
        } 

     //left hand side of the statement
        //string currentNodeRightOp = "";

        return returnIn;
    }


    LatticeNode *latticeNodeInit(){
        /* * ae = new AELatticeNode();
        ae->val = new map<string, string>;*/
        return new AEAnalysisLatticeNode(BOTTOM);
    }

    
    ~AEAnalysis(){}


private:
    AEAnalysisLatticeNode *analyzeAOpB(AEAnalysisLatticeNode *in, Instruction *currentInst){
        errs() << ">>  AEAnalysis::analyzeAOpB  <<" << '\n' ;
        //map<string, string> value;
        AEAnalysisLatticeNode * newIn = new AEAnalysisLatticeNode(in);

        string opName = currentInst->getOpcodeName();

        Value* leftOperandValue = currentInst->getOperand(0);
        string leftOperandStr = string(leftOperandValue->getName());

        //errs() << "This left operand string is: " << leftOperandValue->getName() << '\n';

        Value* rightOperandValue = currentInst->getOperand(1);
        string rightOperandStr = string(rightOperandValue->getName());
        string currentInstName = currentInst->getName(); //get inst name e.g. $add
        currentInstName = "%" + currentInstName;
        //errs() << "This instruction is: " << opName << " This node is: " << currentInstName << " = " << leftOperandStr << " " << opName << " " << rightOperandStr << "\n";

        // Checking if left operand is a constant
        if (ConstantInt *CILeft = dyn_cast<ConstantInt>(leftOperandValue)){
            float leftVal = CILeft->getZExtValue();
            stringstream ss (stringstream::in | stringstream::out);
            ss << leftVal;
            string leftValStr = ss.str();
            errs() << "This leftVal is: " << leftVal << " The leftValStr is " << leftValStr <<'\n';

            if (ConstantInt* CIRight = dyn_cast<ConstantInt>(rightOperandValue)) {
                float rightVal = CIRight->getZExtValue();
                stringstream ss (stringstream::in | stringstream::out);
                ss << rightVal;
                string rightValStr = ss.str();
                //errs() << "This rightVal is: " << rightValStr << '\n';
                errs() << "This instruction is: " << opName << " This node is: " << currentInstName << " = " << leftValStr << " " << opName << " " << rightValStr << "\n";
                
                if (newIn->val.find(opName)==newIn->val.end() && 
                    newIn->val.find(leftValStr)==newIn->val.end() &&
                    newIn->val.find(rightValStr)==newIn->val.end()) {
                    errs() << " ADD op has not been found +=======+==+++++++++===++==+++=+++==++==++==+++===" << '\n';
                    
                    AEAnalysisLatticeNode* temp_1 = new AEAnalysisLatticeNode();
                    newIn->val[currentInstName] = leftValStr + opName + rightValStr;
                    temp_1->val = newIn->val;

                    AEAnalysisLatticeNode* temp_2 = static_cast<AEAnalysisLatticeNode*>(temp_1->joinWith(newIn));
                    delete temp_1;
                    delete newIn;
                    newIn = temp_2;
                    errs() << " FINISH ADD op has not been found +=======+==+++++++++===++==+++=+++==++==++==+++===" << '\n';
                }

            } else { //left is constant but not right side one

            }

        } else { //both are not constant

        }
       
        return newIn;
    }

};


#endif



        //http://llvm.org/docs/doxygen/html/namespacellvm_1_1mdconst.html
        /*
        if (currentInstName != 0) {
        //if ((ConstantInt* CILeft = dyn_cast<ConstantInt>(leftOperandValue)) && (ConstantInt* CIRight = dyn_cast<ConstantInt>(rightOperandValue))) {
        //both left right sides of op are constants

            for(map<string, string>::iterator it = newIn->val.begin(); it != newIn->val.end(); it++) {
                //string inNodeRightOp = it->second;

            } 
        } else {// if { //both sides are all variables
            //first  in - {x->*}
            for(map<string, string>::iterator it = newIn->val.begin(); it != newIn->val.end(); it++) {
                //string inNodeRightOp = it->second;
                if( currentInstName == it->first ) { // x->* found in the latticeNode, remove it!
                    newIn->val.erase(it);

                }else if( rightOperandStr == it->second || leftOperandStr == it->second ) { //same key different value found //delete everything x assign to.
                    newIn->val.erase(it);

                }else { // ok to merge
                    newIn->val[currentInstName] = "";
                }
            } 
            //second in - {*->x}

        }
        */
