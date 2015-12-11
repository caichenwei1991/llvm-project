; ModuleID = 'testcp.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define void @_Z4testv() #0 {
entry:
  %aa = alloca i32, align 4
  %bb = alloca i32, align 4
  %cc = alloca i32, align 4
  %dd = alloca i32, align 4
  %ee = alloca float, align 4
  %ff = alloca float, align 4
  store i32 300, i32* %aa, align 4
  store i32 4, i32* %bb, align 4
  %0 = load i32* %aa, align 4
  %1 = load i32* %bb, align 4
  %div = sdiv i32 %0, %1
  store i32 %div, i32* %cc, align 4
  %2 = load i32* %cc, align 4
  %3 = load i32* %aa, align 4
  %mul = mul nsw i32 %2, %3
  store i32 %mul, i32* %dd, align 4
  %4 = load i32* %aa, align 4
  %add = add nsw i32 %4, 500
  store i32 %add, i32* %cc, align 4
  store float 0x4003333340000000, float* %ee, align 4
  %5 = load float* %ee, align 4
  %conv = fpext float %5 to double
  %div1 = fdiv double %conv, 2.000000e-01
  %conv2 = fptrunc double %div1 to float
  store float %conv2, float* %ff, align 4
  ret void
}

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %g = alloca i32*, align 8
  %d = alloca float, align 4
  %e = alloca float, align 4
  %f = alloca float, align 4
  store i32 0, i32* %retval
  store i32 100, i32* %a, align 4
  store i32 200, i32* %b, align 4
  %0 = load i32* %a, align 4
  %1 = load i32* %b, align 4
  %sub = sub nsw i32 %0, %1
  store i32 %sub, i32* %c, align 4
  store i32* %a, i32** %g, align 8
  store float 1.500000e+00, float* %d, align 4
  store float 0x4002666660000000, float* %e, align 4
  %2 = load float* %d, align 4
  %3 = load float* %e, align 4
  %add = fadd float %2, %3
  store float %add, float* %f, align 4
  %4 = load float* %f, align 4
  %conv = fpext float %4 to double
  %add1 = fadd double %conv, 5.700000e+00
  %conv2 = fptrunc double %add1 to float
  store float %conv2, float* %d, align 4
  %5 = load i32* %c, align 4
  %6 = load i32** %g, align 8
  %7 = load i32* %6, align 4
  %add3 = add nsw i32 %5, %7
  store i32 %add3, i32* %b, align 4
  ret i32 0
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (tags/RELEASE_34/final)"}
