#! /bin/bash

clang -O0 -emit-llvm -c testcpphi2.cpp -o testcpphi2.bc
opt -mem2reg < testcpphi2.bc > testcpphi2_m2r.bc
llvm-dis testcpphi2_m2r.bc

