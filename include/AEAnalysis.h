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
        errs()<<">>>>>>>>>> AEAnalysis start <<<<<<<<<<\n";
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
            errs()<<">>>>>>>>>> AEAnalysis:: runFlowFunc start1111 <<<<<<<<<<\n";
            return returnIn;
        } else if (opName == "other") {
            //what should do here?
        } else { //there's no Operator? >>  F x:= y (in) ==
            errs()<<">>>>>>>>>> AEAnalysis:: runFlowFunc start2222 <<<<<<<<<<\n";
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

        errs()<< "opName ====>> : " << opName << " instName ====>> : %" << currentInstName <<'\n';
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

                    errs() << "both constants  :" << currentInstName << " = " << newIn->val[currentInstName] << '\n';

                    for (map<string, string>::iterator it = newIn->val.begin(); it != newIn->val.end(); it++) {

                         errs() << "it->second: " << it->second << " str: " << str << '\n';
                        if (it->second == str && it->first != currentInstName){
                            newIn->val[currentInstName] = it->first;
                            //newIn->val.erase(it);
                            errs() << "erase happen here \n" ;
                            errs() << "now current map = " << it->first << "," << it->second << '\n';
                        }
                    }

            } else { //left side is constant +  right side one is var

                    if (currentInstName != leftOperandStr && rightOperandStr != "") {
                        newIn->val[currentInstName] = opName + "," + leftValStr + "," + rightOperandStr;
                        errs() << " left side is constant +  right side one is var  :" << currentInstName << " = " << newIn->val[currentInstName] << '\n';
                    }

            }

        } else if (ConstantInt* CIRight = dyn_cast<ConstantInt>(rightOperandValue)) { //right side is constant +  left side one is var
                    float rightVal = CIRight->getZExtValue();
                    stringstream ss (stringstream::in | stringstream::out);
                    ss << rightVal;
                    string rightValStr = ss.str();

                    if (currentInstName != leftOperandStr && leftOperandStr != "" ) {
                        newIn->val[currentInstName] = opName + ","  + leftOperandStr + "," + rightValStr;
                        errs() << " right side is constant +  left side one is var  :" << currentInstName << " = " << newIn->val[currentInstName] << '\n';
                    }


                    
        } else { //both sides are varibles
                if (currentInstName != leftOperandStr && currentInstName!= rightOperandStr) {
                    for (map<string, string>::iterator it = newIn->val.begin(); it != newIn->val.end(); it++) {
                        if (currentInstName == it->first) {
                            newIn->val[currentInstName] = opName + "," + leftOperandStr + "," + rightOperandStr ;
                            errs() << " both sides are varibles +=======+  :" << currentInstName << " = " << newIn->val[currentInstName] << '\n';
                        } else {

                        }

                    }
                }
                    
                
        }
        errs() << " FINISH  +=======+ newIn size " << newIn->val.size() << '\n';
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