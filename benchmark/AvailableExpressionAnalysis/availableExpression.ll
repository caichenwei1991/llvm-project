; ModuleID = 'availableExpression.bc'
target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:32:64-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32-S128"
target triple = "i386-pc-linux-gnu"

; Function Attrs: nounwind
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %aa = alloca i32, align 4
  %bb = alloca i32, align 4
  %cc = alloca i32, align 4
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %z = alloca i32, align 4
  %xx = alloca i32, align 4
  %yy = alloca i32, align 4
  %zz = alloca i32, align 4
  %zzz = alloca i32, align 4
  %v = alloca i32, align 4
  %w = alloca i32, align 4
  %t = alloca i32, align 4
  %k = alloca i32, align 4
  store i32 0, i32* %retval
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  %0 = load i32* %a, align 4
  %add = add nsw i32 %0, 2
  store i32 %add, i32* %x, align 4
  store i32 3, i32* %x, align 4
  %1 = load i32* %a, align 4
  %2 = load i32* %b, align 4
  %add1 = add nsw i32 %1, %2
  store i32 %add1, i32* %k, align 4
  %3 = load i32* %b, align 4
  %add2 = add nsw i32 3, %3
  store i32 %add2, i32* %x, align 4
  %4 = load i32* %x, align 4
  %5 = load i32* %b, align 4
  %sub = sub nsw i32 %4, %5
  store i32 %sub, i32* %y, align 4
  %6 = load i32* %a, align 4
  %7 = load i32* %b, align 4
  %add3 = add nsw i32 %6, %7
  store i32 %add3, i32* %v, align 4
  %8 = load i32* %x, align 4
  %add4 = add nsw i32 %8, 1
  store i32 %add4, i32* %w, align 4
  %9 = load i32* %w, align 4
  store i32 %9, i32* %a, align 4
  %10 = load i32* %y, align 4
  %add5 = add nsw i32 2, %10
  store i32 %add5, i32* %yy, align 4
  %11 = load i32* %a, align 4
  %12 = load i32* %b, align 4
  %mul = mul nsw i32 %11, %12
  store i32 %mul, i32* %v, align 4
  %13 = load i32* %x, align 4
  %add6 = add nsw i32 %13, 1
  store i32 %add6, i32* %z, align 4
  %14 = load i32* %a, align 4
  %15 = load i32* %b, align 4
  %add7 = add nsw i32 %14, %15
  store i32 %add7, i32* %t, align 4
  ret i32 0
}

attributes #0 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (tags/RELEASE_34/final)"}
