# if your instrumentation code calls into LLVM libraries, then comment out the above and use these instead:
CPPFLAGS=`llvm-config --cppflags`
LLVMLIBS=`llvm-config --libs`
LDFLAGS=`llvm-config --ldflags`

## compile the instrumentation module to bitcode
clang $CPPFLAGS -O0 -emit-llvm -c availableExpression.cpp -o availableExpression.bc

##make the .bc human readable
llvm-dis availableExpression.bc

## compile to native object file
#llc -filetype=obj availableExpression.bc -o=availableExpression.o

## generate native executable
#g++ availableExpression.o $LLVMLIBS $LDFLAGS -o range.exe

#./range.exe

opt -mem2reg < availableExpression.bc > availableExpression.mem2reg.bc

llvm-dis availableExpression.mem2reg.bc
