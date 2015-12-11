#ifndef AE_ANALYSIS
#define AE_ANALYSIS

#include "BasicAnalysis.h"
#include "AEAnalysisLatticeNode.h"
#include "llvm/IR/Constants.h"
#include <string>
#include <sstream>


class AEAnalysis: public BasicAnalysis{

public:

    AEAnalysis(Function &F):BasicAnalysis(F){
        createCFG(F);
    };


    LatticeNode *runFlowFunc(LatticeNode *in, CFGNode *currentNode){
        errs()<<"\n>\n";
        AEAnalysisLatticeNode *inputIn = static_cast<AEAnalysisLatticeNode *>(in);
        AEAnalysisLatticeNode *returnIn;

        Instruction *currentInst = currentNode->inst;

        string opName = currentInst->getOpcodeName();
        
        if(opName == "add" || opName == "sub" || opName == "mul") {
            returnIn = analyzeAOpB(inputIn, currentInst);
            return returnIn;
        } else if (opName == "other") {
            //what should do here?
        } else { //there's no Operator? >>  F x:= y (in) ==
            return in;
        } 

     //left hand side of the statement
        //string currentNodeRightOp = "";

    }


    LatticeNode *latticeNodeInit(){
        /* * ae = new AELatticeNode();
        ae->val = new map<string, string>;*/
        return new AEAnalysisLatticeNode(BOTTOM);
    }

    
    ~AEAnalysis(){}


private:
    AEAnalysisLatticeNode *analyzeAOpB(AEAnalysisLatticeNode *in, Instruction *currentInst){
        //errs() << ">>  AEAnalysis::analyzeAOpB  <<" << '\n' ;
        //map<string, string> value;
        //AEAnalysisLatticeNode * newIn = new AEAnalysisLatticeNode(in);
        AEAnalysisLatticeNode * newIn = new AEAnalysisLatticeNode();
        newIn->val = in->val;


        string opName = currentInst->getOpcodeName();

        Value* leftOperandValue = currentInst->getOperand(0);
        string leftOperandStr = string(leftOperandValue->getName());

        Value* rightOperandValue = currentInst->getOperand(1);
        string rightOperandStr = string(rightOperandValue->getName());
        string currentInstName = currentInst->getName(); //get inst name e.g. %add

        errs()<< "OperatorName >> : " << opName << "\nInstructionName >> : %" << currentInstName <<'\n';
        //currentInstName = "%" + currentInstName;
        //errs() << "This instruction is: " << opName << " This node is: " << currentInstName << " = " << leftOperandStr << " " << opName << " " << rightOperandStr << "\n";

        // Checking if left operand is a constant
        if (ConstantInt *CILeft = dyn_cast<ConstantInt>(leftOperandValue)){
            float leftVal = CILeft->getZExtValue();
            stringstream ss (stringstream::in | stringstream::out);
            ss << leftVal;
            string leftValStr = ss.str();
            //errs() << "This leftVal is: " << leftVal << " The leftValStr is " << leftValStr <<'\n';
            if (ConstantInt* CIRight = dyn_cast<ConstantInt>(rightOperandValue)) {
                float rightVal = CIRight->getZExtValue();
                stringstream ss (stringstream::in | stringstream::out);
                ss << rightVal;
                string rightValStr = ss.str();
                //errs() << "This rightVal is: " << rightValStr << '\n';

                    string str = opName + "," + leftValStr + "," + rightValStr;

                    newIn->val[currentInstName] = str;

                    errs() << "Type >> : Both operands are constant >> : " << currentInstName << " = " << newIn->val[currentInstName] << '\n';

                    for (map<string, string>::iterator it = newIn->val.begin(); it != newIn->val.end(); it++) {

                         //errs() << "it->second: " << it->second << " str: " << str << '\n';
                        if (it->second == str && it->first != currentInstName){
                            newIn->val[currentInstName] = it->first;
                            //newIn->val.erase(it);
                            //errs() << "erase happen here \n" ;
                            //errs() << "now current map = " << it->first << "," << it->second << '\n';
                        }
                    }

            } else { //left side is constant +  right side one is var

                    if (currentInstName != leftOperandStr && rightOperandStr != "") {
                        newIn->val[currentInstName] = opName + "," + leftValStr + "," + rightOperandStr;
                        errs() << "Type >> : Left operand - Constant & Right operand - varible >> : " << currentInstName << " = " << newIn->val[currentInstName] << '\n';
                    }

            }

        } else if (ConstantInt* CIRight = dyn_cast<ConstantInt>(rightOperandValue)) { //right side is constant +  left side one is var
                    float rightVal = CIRight->getZExtValue();
                    stringstream ss (stringstream::in | stringstream::out);
                    ss << rightVal;
                    string rightValStr = ss.str();

                    if (currentInstName != leftOperandStr && leftOperandStr != "" ) {
                        newIn->val[currentInstName] = opName + ","  + leftOperandStr + "," + rightValStr;
                        errs() << "Type >> : Left operand - varible & Right operand - operand >> : " << currentInstName << " = " << newIn->val[currentInstName] << '\n';
                    }


                    
        } else { //both sides are varibles
                if (currentInstName != leftOperandStr && currentInstName!= rightOperandStr) {
                    for (map<string, string>::iterator it = newIn->val.begin(); it != newIn->val.end(); it++) {
                        if (currentInstName == it->first) {
                            newIn->val[currentInstName] = opName + "," + leftOperandStr + "," + rightOperandStr ;
                            errs() << "Type >> : Both operands are varible >> : " << currentInstName << " = " << newIn->val[currentInstName] << '\n';
                        } else {

                        }

                    }
                }
                    
                
        }
        errs() << "Finishing analyzeAOpB >> newIn node size >> : " << newIn->val.size() << '\n';
        return newIn;
    }

};


#endif
