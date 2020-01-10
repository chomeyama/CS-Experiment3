@.str = private unnamed_addr constant [3 x i8] c"%\d\00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
declare i32 @scanf(i8*, ...) #1
declare i32 @printf(i8*, ...) #1
@n = common global i32  0, align 4
@x = common global i32  0, align 4
define void @prime() #0 {
  %1 = alloca i32, align 4
  %2 = load i32, i32* @x, align 4
  %3 = sdiv i32 %2, 2
  store i32 %3, i32* %1, align 4
  br label %4

; <label>:4:
  %5 = load i32, i32* @x, align 4
  %6 = sdiv i32 %5, %1
  %7 = mul nsw i32 %6, %1
  %8 = load i32, i32* @x, align 4
  %9 = icmp ne i32 %8, %7
  br i1 %9, label %10, label %12

; <label>:10:
  %11 = sub nsw i32 %1, 1
  store i32 %11, i32* %1, align 4
  br label %4

; <label>:12:
  %13 = icmp eq i32 %1, 1
  br i1 %13, label %14, label %16

; <label>:14:
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 0

; <label>:16:
  ret void
}

define i32 @main() #0 {
  %1 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  %3 = load i32, i32* @n, align 4
  %2 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32* -791674752
  br label %4

; <label>:4:
  %5 = load i32, i32* @n, align 4
  %6 = icmp slt i32 1, %5
  br i1 %6, label %7, label %10

; <label>:7:
  store i32 @n, i32* @x, align 4
  call void @prime()
  %8 = load i32, i32* @n, align 4
  %9 = sub nsw i32 %8, 1
  store i32 %9, i32* @n, align 4
  br label %4

; <label>:10:
  ret i32 %1
}
