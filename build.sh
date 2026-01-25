#!/bin/bash
# Simple build script for Meow Lang compiler

set -e

echo "Building Meow Lang Compiler..."

# Create build directory
mkdir -p build

# Compile all source files
echo "Compiling source files..."
gcc -c -Iinclude src/token.c -o build/token.o
gcc -c -Iinclude src/lexer.c -o build/lexer.o
gcc -c -Iinclude src/ast.c -o build/ast.o
gcc -c -Iinclude src/parser.c -o build/parser.o
gcc -c -Iinclude src/codegen.c -o build/codegen.o
gcc -c -Iinclude src/main.c -o build/main.o

# Link object files
echo "Linking..."
gcc build/token.o build/lexer.o build/ast.o build/parser.o build/codegen.o build/main.o -o build/meow -lm

echo "Build complete!"
echo "Meow compiler built at: build/meow"

# Optional: Install to system path
if [ "$1" == "install" ]; then
    echo "Installing to /usr/local/bin..."
    sudo cp build/meow /usr/local/bin/meow
    chmod +x /usr/local/bin/meow
    echo "Installation complete!"
fi
