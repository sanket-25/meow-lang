@echo off
REM Simple build script for Meow Lang compiler on Windows

echo Building Meow Lang Compiler...

REM Create build directory
if not exist build mkdir build

REM Compile all source files
echo Compiling source files...
gcc -c -Iinclude src\token.c -o build\token.o
gcc -c -Iinclude src\lexer.c -o build\lexer.o
gcc -c -Iinclude src\ast.c -o build\ast.o
gcc -c -Iinclude src\parser.c -o build\parser.o
gcc -c -Iinclude src\codegen.c -o build\codegen.o
gcc -c -Iinclude src\main.c -o build\main.o

REM Link object files
echo Linking...
gcc build\token.o build\lexer.o build\ast.o build\parser.o build\codegen.o build\main.o -o build\meow.exe -lm

echo Build complete!
echo Meow compiler built at: build\meow.exe
