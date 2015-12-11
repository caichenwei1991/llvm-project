# llvm-project
# CSE231 final project part 2

#Benchmarks and source file of four analysis:

$ cd benchmarks/ConstantProp/
$ source testcp.sh

$ cd benchmarks/AvailableExpressionAnalysis/
$ source availableExpression.sh

$ cd benchmarks/RangeAnalysis/
$ source range.sh

$ cd benchmarks/PointerAnalyasis/
$ source pointer.sh


#To compile passes: (suggest $ source startenv.sh #environment before proceed)
$ cd $CSE231ROOT
$ make llvm
# Constant propagation pass:
$ opt -load $LLVMLIB/pass.so -ConstantProp < $BENCHMARKS 

# Available expression pass:
$ opt -load $LLVMLIB/pass.so -aeAnalysis < $BENCHMARKS 

# Range anlaysis pass:
$ opt -load $LLVMLIB/pass.so -rangeAnalysis < $BENCHMARKS 

# Pointer analysis pass:
$ opt -load $LLVMLIB/pass.so -pointerAnalysis < $BENCHMARKS 

# Passes are located under:
/src/pass/
