; ModuleID = 'testcpphi2_m2r.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %add = add nsw i32 2, 3
  %cmp = icmp slt i32 2, 3
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %add1 = add nsw i32 2, 2
  %add2 = add nsw i32 %add1, 3
  %add3 = add nsw i32 %add1, 4
  br label %if.end

if.else:                                          ; preds = %entry
  %add4 = add nsw i32 2, 5
  %add5 = add nsw i32 %add4, 6
  %add6 = add nsw i32 %add4, 7
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %b.0 = phi i32 [ %add1, %if.then ], [ %add4, %if.else ]
  %cmp7 = icmp eq i32 5, %b.0
  br i1 %cmp7, label %if.then8, label %if.else11

if.then8:                                         ; preds = %if.end
  %add9 = add nsw i32 5, 1
  %add10 = add nsw i32 %b.0, 2
  br label %if.end14

if.else11:                                        ; preds = %if.end
  %add12 = add nsw i32 5, 2
  %add13 = add nsw i32 %b.0, 3
  br label %if.end14

if.end14:                                         ; preds = %if.else11, %if.then8
  ret i32 0
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (tags/RELEASE_34/final)"}
