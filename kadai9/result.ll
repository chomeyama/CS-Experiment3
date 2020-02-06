@x = common global i32 0, align 4
@.str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
declare i32 @printf(i8*, ...) #1
define i32 @main() #0 {
  store i32 10, i32* @x, align 4
  %1 = load i32, i32* @x, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 %1)
  %3 = load i32, i32* @x, align 4
  %4 = shl i32 %3, 3
  store i32 %4, i32* @x, align 4
  %5 = load i32, i32* @x, align 4
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 %5)
  %7 = load i32, i32* @x, align 4
  %8 = ashr i32 %7, 2
  store i32 %8, i32* @x, align 4
  %9 = load i32, i32* @x, align 4
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 %9)
  %11 = load i32, i32* @x, align 4
  %12 = shl i32 %11, 5
  store i32 %12, i32* @x, align 4
  %13 = load i32, i32* @x, align 4
  %14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 %13)
  ret i32 0
}
