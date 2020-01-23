@a = common global [1 x i32] zeroinitializer, align 16@i = common global i32 0, align 4
@n = common global i32 0, align 4
@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1
declare i32 @scanf(i8*, ...) #1
@.str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
declare i32 @printf(i8*, ...) #1
define void @initialize() #0 {
  %1 = alloca i32, align 4
  store i32 2, i32* %1, align 4
  br label %2

; <label>:2:
  %3 = load i32, i32* %1, align 4
  %4 = icmp sle i32 %3, 100
  br i1 %4, label %5, label %13

; <label>:5:
  %6 = load i32, i32* %1, align 4
  %7 = sub nsw i32 %6, 1
  %8 = sext i32 %7 to i64
  %9 = getelementptr inbounds [1 x i32], [1 x i32]* @a, i64 0, i64 %8
  %10 = load i32, i32* %1, align 4
  store i32 %10, i32* %9, align 4
  %11 = load i32, i32* %1, align 4
  %12 = add nsw i32 %11, 1
  store i32 %12, i32* %1, align 4
  br label %2

; <label>:13:
  ret void
}

define void @check(i32) #0 {
  %2 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  %3 = alloca i32, align 4
  %4 = load i32, i32* %2, align 4
  store i32 %4, i32* %3, align 4
  br label %5

; <label>:5:
  %6 = load i32, i32* %3, align 4
  %7 = icmp sle i32 %6, 100
  br i1 %7, label %8, label %16

; <label>:8:
  %9 = load i32, i32* %3, align 4
  %10 = sub nsw i32 %9, 1
  %11 = sext i32 %10 to i64
  %12 = getelementptr inbounds [1 x i32], [1 x i32]* @a, i64 0, i64 %11
  store i32 0, i32* %12, align 4
  %13 = load i32, i32* %3, align 4
  %14 = load i32, i32* %2, align 4
  %15 = add nsw i32 %13, %14
  store i32 %15, i32* %3, align 4
  br label %5

; <label>:16:
  ret void
}

define i32 @main() #0 {
  call void @initialize()
  %1 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32* @n)
  %2 = load i32, i32* @n, align 4
  %3 = icmp sle i32 %2, 100
  br i1 %3, label %4, label %29

; <label>:4:
  store i32 2, i32* @i, align 4
  br label %5

; <label>:5:
  %6 = load i32, i32* @n, align 4
  %7 = load i32, i32* @i, align 4
  %8 = icmp sle i32 %7, %6
  br i1 %8, label %9, label %28

; <label>:9:
  %10 = load i32, i32* @i, align 4
  %11 = sub nsw i32 %10, 1
  %12 = sext i32 %11 to i64
  %13 = getelementptr inbounds [1 x i32], [1 x i32]* @a, i64 0, i64 %12
  %14 = load i32, i32* %13, align 4
  %15 = load i32, i32* @i, align 4
  %16 = icmp eq i32 %14, %15
  br i1 %16, label %17, label %25

; <label>:17:
  %18 = load i32, i32* @i, align 4
  %19 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 %18)
  %20 = load i32, i32* @i, align 4
  %21 = sub nsw i32 %20, 1
  %22 = sext i32 %21 to i64
  %23 = getelementptr inbounds [1 x i32], [1 x i32]* @a, i64 0, i64 %22
  %24 = load i32, i32* %23, align 4
  call void @check(i32 %24)
  br label %25

; <label>:25:
  %26 = load i32, i32* @i, align 4
  %27 = add nsw i32 %26, 1
  store i32 %27, i32* @i, align 4
  br label %5

; <label>:28:
  br label %29

; <label>:29:
  ret i32 0
}
