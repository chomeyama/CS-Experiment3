; ModuleID = 'samples/pl3b.c'
source_filename = "samples/pl3b.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

@a = common global [101 x i32] zeroinitializer, align 16
@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@n = common global i32 0, align 4
@i = common global i32 0, align 4
@.str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

; Function Attrs: noinline nounwind optnone ssp uwtable
define void @initialize() #0 {
  %1 = alloca i32, align 4
  store i32 2, i32* %1, align 4
  br label %2

; <label>:2:                                      ; preds = %9, %0
  %3 = load i32, i32* %1, align 4
  %4 = icmp sle i32 %3, 100
  br i1 %4, label %5, label %12

; <label>:5:                                      ; preds = %2
  %6 = load i32, i32* %1, align 4
  %7 = sext i32 %6 to i64
  %8 = getelementptr inbounds [101 x i32], [101 x i32]* @a, i64 0, i64 %7
  store i32 0, i32* %8, align 4
  br label %9

; <label>:9:                                      ; preds = %5
  %10 = load i32, i32* %1, align 4
  %11 = add nsw i32 %10, 1
  store i32 %11, i32* %1, align 4
  br label %2

; <label>:12:                                     ; preds = %2
  ret void
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define void @check(i32) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  %4 = load i32, i32* %2, align 4
  store i32 %4, i32* %3, align 4
  br label %5

; <label>:5:                                      ; preds = %8, %1
  %6 = load i32, i32* %3, align 4
  %7 = icmp sle i32 %6, 100
  br i1 %7, label %8, label %15

; <label>:8:                                      ; preds = %5
  %9 = load i32, i32* %3, align 4
  %10 = sext i32 %9 to i64
  %11 = getelementptr inbounds [101 x i32], [101 x i32]* @a, i64 0, i64 %10
  store i32 1, i32* %11, align 4
  %12 = load i32, i32* %3, align 4
  %13 = load i32, i32* %2, align 4
  %14 = add nsw i32 %12, %13
  store i32 %14, i32* %3, align 4
  br label %5

; <label>:15:                                     ; preds = %5
  ret void
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  call void @initialize()
  %2 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32* @n)
  %3 = load i32, i32* @n, align 4
  %4 = icmp sle i32 %3, 100
  br i1 %4, label %5, label %25

; <label>:5:                                      ; preds = %0
  store i32 2, i32* @i, align 4
  br label %6

; <label>:6:                                      ; preds = %21, %5
  %7 = load i32, i32* @i, align 4
  %8 = load i32, i32* @n, align 4
  %9 = icmp sle i32 %7, %8
  br i1 %9, label %10, label %24

; <label>:10:                                     ; preds = %6
  %11 = load i32, i32* @i, align 4
  %12 = sext i32 %11 to i64
  %13 = getelementptr inbounds [101 x i32], [101 x i32]* @a, i64 0, i64 %12
  %14 = load i32, i32* %13, align 4
  %15 = icmp eq i32 %14, 0
  br i1 %15, label %16, label %20

; <label>:16:                                     ; preds = %10
  %17 = load i32, i32* @i, align 4
  %18 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 %17)
  %19 = load i32, i32* @i, align 4
  call void @check(i32 %19)
  br label %20

; <label>:20:                                     ; preds = %16, %10
  br label %21

; <label>:21:                                     ; preds = %20
  %22 = load i32, i32* @i, align 4
  %23 = add nsw i32 %22, 1
  store i32 %23, i32* @i, align 4
  br label %6

; <label>:24:                                     ; preds = %6
  br label %25

; <label>:25:                                     ; preds = %24, %0
  %26 = load i32, i32* %1, align 4
  ret i32 %26
}

declare i32 @scanf(i8*, ...) #1

declare i32 @printf(i8*, ...) #1

attributes #0 = { noinline nounwind optnone ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sahf,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sahf,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"Apple LLVM version 10.0.0 (clang-1000.10.44.4)"}
