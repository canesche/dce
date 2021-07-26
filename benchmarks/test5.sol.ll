; ModuleID = 'benchmarks/test5.vssa.ll'
source_filename = "benchmarks/test5.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [8 x i8] c"loooll\0A\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local void @foo(i32 %a, i32 %b) #0 {
entry:
  br label %while.cond

while.cond:                                       ; preds = %while.cond1, %entry
  %b.addr.0 = phi i32 [ 60, %entry ], [ %dec, %while.cond1 ]
  %a.addr.0 = phi i32 [ 0, %entry ], [ %a.addr.1, %while.cond1 ]
  %cmp = icmp sgt i32 %b.addr.0, 53
  br i1 %cmp, label %while.body, label %while.end5

while.body:                                       ; preds = %while.cond
  %dec = add nsw i32 %b.addr.0, -1
  br label %while.cond1

while.cond1:                                      ; preds = %while.body3, %while.body
  %a.addr.1 = phi i32 [ %a.addr.0, %while.body ], [ %inc, %while.body3 ]
  %cmp2 = icmp slt i32 %a.addr.1, 52
  br i1 %cmp2, label %while.body3, label %while.cond

while.body3:                                      ; preds = %while.cond1
  %inc = add nsw i32 %a.addr.1, 1
  br label %while.cond1

while.end5:                                       ; preds = %while.cond
  ret void
}

declare dso_local i32 @printf(i8*, ...) #1

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 10.0.1 (https://github.com/llvm/llvm-project.git ef32c611aa214dea855364efd7ba451ec5ec3f74)"}
