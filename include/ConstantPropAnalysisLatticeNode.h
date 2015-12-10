#ifndef CONTANT_PROP_ANALYSIS_LATTICE_NODE_H
#define CONTANT_PROP_ANALYSIS_LATTICE_NODE_H
#include <string>
#include <map>
#include "llvm/Support/raw_ostream.h"
#include "LatticeNode.h"

using namespace std;
using namespace llvm;
/*
 * Must-point-to Analysis
 *   
 */

class ConstantPropAnalysisLatticeNode: public LatticeNode {
private:

public:
 	// initialize with an empty map
	ConstantPropAnalysisLatticeNode() {}

	// initialize with TOP or BOTTOM
	ConstantPropAnalysisLatticeNode(string input): LatticeNode(input) {}
 
        ~ConstantPropAnalysisLatticeNode() {}

        bool equalsTo(LatticeNode *othersuper) {
		ConstantPropAnalysisLatticeNode *other = static_cast<ConstantPropAnalysisLatticeNode*>(othersuper);
	    	// either this or other is "top" and "bottom"
	    	if ((this->basic!="") || (other->basic!="")) return (this->basic == other->basic);

		if (this->value.size() != other->value.size()) return false;

	    	// for the same key name, compare their values
		for (map<string, double>::iterator it = this->value.begin(); it != this->value.end(); it++) {
			string key = it->first;
        		float thisval = it->second;
 	       		// cannot find this key
       			if (other->value.find(key) == other->value.end()) return false;
       			// find this key in other but cannot match
        		if (other->value.find(key)->second != thisval) return false;
		}
    		return true;
	}
	LatticeNode *joinWith(LatticeNode *othersuper) {
		ConstantPropAnalysisLatticeNode *other = static_cast<ConstantPropAnalysisLatticeNode*>(othersuper);
		if (this->basic == TOP || other->basic == TOP) return new ConstantPropAnalysisLatticeNode(TOP);
		if (this->basic == BOTTOM && other->basic == BOTTOM ) return new ConstantPropAnalysisLatticeNode(BOTTOM);

		ConstantPropAnalysisLatticeNode *f = new ConstantPropAnalysisLatticeNode();
		if (this->basic == BOTTOM) {
			f->value = other->value;
			f->basic = other->basic;
			return f;
    		}

	        if (other->basic == BOTTOM) {
			f->value = this->value;
			f->basic = this->basic;
			return f;
    		}

		for (map<string, double>::iterator it = this->value.begin(); it != this->value.end(); it++) {
			if (other->value.find(it->first) == other->value.end()) {
	         	f->value[it->first] = this->value.find(it->first)->second;
		} else if (it->second == other->value.find(it->first)->second) 
	    		f->value[it->first] = this->value.find(it->first)->second;
    		}

	        for (map<string, double>::iterator it = other->value.begin(); it != other->value.end(); it++) {
			if (this->value.find(it->first) == this->value.end()) {
	    		f->value[it->first] = other->value.find(it->first)->second;
		} else if (it->second == this->value.find(it->first)->second) 
		        f->value[it->first] = other->value.find(it->first)->second;
    		}

	        return f;
	}

	map<string, double> value;
};
#endif 
