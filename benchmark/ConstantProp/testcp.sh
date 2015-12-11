#! /bin/bash

clang -O0 -emit-llvm -c testcp.cpp -o testcp.bc
opt -mem2reg < testcp.bc > testcp_m2r.bc
llvm-dis testcp_m2r.bc
