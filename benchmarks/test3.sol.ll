; ModuleID = 'benchmarks/test3.vssa.ll'
source_filename = "benchmarks/test3.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @foo(i32 %a, i32 %b) #0 {
entry:
  br label %while.cond

while.cond:                                       ; preds = %while.end, %entry
  %b.addr.0 = phi i32 [ 255, %entry ], [ %b.addr.1, %while.end ]
  %a.addr.0 = phi i32 [ 0, %entry ], [ %inc, %while.end ]
  %cmp = icmp slt i32 %a.addr.0, 255
  br i1 %cmp, label %while.body, label %while.end5

while.body:                                       ; preds = %while.cond
  %vSSA_sigma = phi i32 [ %a.addr.0, %while.cond ]
  %inc = add nsw i32 %vSSA_sigma, 1
  br label %while.cond1

while.cond1:                                      ; preds = %if.end, %while.body
  %b.addr.1 = phi i32 [ %b.addr.0, %while.body ], [ %dec, %if.end ]
  %cmp2 = icmp sgt i32 %b.addr.1, 0
  br i1 %cmp2, label %while.body3, label %while.cond1.while.end_crit_edge

while.cond1.while.end_crit_edge:                  ; preds = %while.cond1
  br label %while.end

while.body3:                                      ; preds = %while.cond1
  %vSSA_sigma1 = phi i32 [ %b.addr.1, %while.cond1 ]
  %cmp4 = icmp eq i32 %inc, %vSSA_sigma1
  br i1 %cmp4, label %if.then, label %if.end

if.then:                                          ; preds = %while.body3
  br label %while.end

if.end:                                           ; preds = %while.body3
  %vSSA_sigma2 = phi i32 [ %vSSA_sigma1, %while.body3 ]
  %dec = add nsw i32 %vSSA_sigma2, -1
  br label %while.cond1

while.end:                                        ; preds = %while.cond1.while.end_crit_edge, %if.then
  br label %while.cond

while.end5:                                       ; preds = %while.cond
  ret void
}

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 10.0.1 (https://github.com/llvm/llvm-project.git ef32c611aa214dea855364efd7ba451ec5ec3f74)"}
