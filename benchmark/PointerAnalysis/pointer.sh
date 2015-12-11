# if your instrumentation code calls into LLVM libraries, then comment out the above and use these instead:
CPPFLAGS=`llvm-config --cppflags`
LLVMLIBS=`llvm-config --libs`
LDFLAGS=`llvm-config --ldflags`

## compile the instrumentation module to bitcode
clang $CPPFLAGS -O0 -emit-llvm -c pointer.cpp -o pointer.bc

##make the .bc human readable
llvm-dis pointer.bc

## compile to native object file
#llc -filetype=obj pointer.bc -o=pointer.o

## generate native executable
#g++ pointer.o $LLVMLIBS $LDFLAGS -o range.exe

#./range.exe

opt -mem2reg < pointer.bc > pointer.mem2reg.bc

llvm-dis pointer.mem2reg.bc
