# if your instrumentation code calls into LLVM libraries, then comment out the above and use these instead:
CPPFLAGS=`llvm-config --cppflags`
LLVMLIBS=`llvm-config --libs`
LDFLAGS=`llvm-config --ldflags`

## compile the instrumentation module to bitcode
clang $CPPFLAGS -O0 -emit-llvm -c range.cpp -o range.bc

##make the .bc human readable
llvm-dis range.bc

## compile to native object file
llc -filetype=obj range.bc -o=range.o

## generate native executable
g++ range.o $LLVMLIBS $LDFLAGS -o range.exe

#./range.exe

opt -mem2reg < range.bc > range.mem2reg.bc
llvm-dis range.mem2reg.bc
