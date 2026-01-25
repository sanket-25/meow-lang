# Meow Lang - A Simple Programming Language

**Meow Lang** is a lightweight, production-ready programming language compiled to C. It combines a simple, readable syntax with the performance and portability of C.

## Quick Links

- 📖 **[Getting Started](GETTING_STARTED.md)** - Tutorials and examples
- 🔧 **[Installation Guide](INSTALL.md)** - Setup instructions for all platforms
- 📝 **[Project Summary](PROJECT_SUMMARY.md)** - Architecture and overview
- 🤝 **[Contributing Guide](CONTRIBUTING.md)** - How to contribute

## Features

- ✅ **Simple Syntax** - Easy-to-read, minimalist language design
- ✅ **Compiled to C** - Leverages GCC/Clang for optimized binary output
- ✅ **Fast Execution** - Native performance via C compilation
- ✅ **Cross-Platform** - Runs on Windows, macOS, and Linux
- ✅ **Type Inference** - Automatic variable type detection
- ✅ **Functions** - First-class function support with parameters
- ✅ **Control Flow** - if/else, while, for loops with break/continue
- ✅ **Operators** - Full arithmetic, logical, and bitwise operations

## Installation

### Prerequisites
- GCC or Clang compiler
- CMake 3.10+
- Make or Ninja build system

### Build from Source

**Option 1: Using build scripts (Recommended)**

On Linux/macOS:
```bash
git clone https://github.com/yourusername/meow-lang.git
cd meow-lang
chmod +x build.sh
./build.sh
sudo ./build.sh install
```

On Windows (requires GCC):
```bash
git clone https://github.com/yourusername/meow-lang.git
cd meow-lang
build.bat
```

**Option 2: Using CMake**

```bash
git clone https://github.com/yourusername/meow-lang.git
cd meow-lang
mkdir build
cd build
cmake ..
make
sudo make install
```

**Option 3: Manual compilation**

```bash
cd meow-lang
gcc -c -Iinclude src/token.c -o token.o
gcc -c -Iinclude src/lexer.c -o lexer.o
gcc -c -Iinclude src/ast.c -o ast.o
gcc -c -Iinclude src/parser.c -o parser.o
gcc -c -Iinclude src/codegen.c -o codegen.o
gcc -c -Iinclude src/main.c -o main.o
gcc token.o lexer.o ast.o parser.o codegen.o main.o -o meow -lm
```

## Usage

### Basic Syntax

```bash
meow program.meow
```

The compiler will:
1. Parse the `.meow` file
2. Generate C code
3. Compile with GCC
4. Execute the binary

## Language Guide

### Variables

```meow
var x = 42;
var name = "Fluffy";
var pi = 3.14159;
```

### Functions

```meow
func add(a, b) {
    return a + b;
}

func greet(name) {
    print("Hello, " + name);
}
```

### Control Flow

#### if/else
```meow
if (x > 10) {
    print("x is greater than 10");
} else {
    print("x is 10 or less");
}
```

#### while loop
```meow
var i = 0;
while (i < 5) {
    print(i);
    i = i + 1;
}
```

#### for loop
```meow
for (var i = 0; i < 10; i = i + 1) {
    print(i);
}
```

### Arrays

```meow
var numbers = [1, 2, 3, 4, 5];
var first = numbers[0];
```

### Operators

#### Arithmetic
- `+` Addition
- `-` Subtraction
- `*` Multiplication
- `/` Division
- `%` Modulo

#### Comparison
- `==` Equal
- `!=` Not equal
- `<` Less than
- `<=` Less than or equal
- `>` Greater than
- `>=` Greater than or equal

#### Logical
- `&&` AND
- `||` OR
- `!` NOT

#### Bitwise
- `&` AND
- `|` OR
- `^` XOR
- `~` NOT
- `<<` Left shift
- `>>` Right shift

### Built-in Functions

#### print(value)
Prints a value followed by a newline

```meow
print(42);
print("Hello");
```

## Examples

### Hello World
Create `hello.meow`:
```meow
print("Hello, Meow World!");
```

Run:
```bash
meow hello.meow
```

### Factorial
Create `factorial.meow`:
```meow
func factorial(n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

print(factorial(5));
```

### Fibonacci
Create `fibonacci.meow`:
```meow
func fib(n) {
    if (n <= 1) {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}

var i = 0;
while (i <= 10) {
    print(fib(i));
    i = i + 1;
}
```

### Sum Array
Create `sum.meow`:
```meow
func sum(arr, len) {
    var total = 0;
    var i = 0;
    while (i < len) {
        total = total + arr[i];
        i = i + 1;
    }
    return total;
}

var numbers = [10, 20, 30, 40, 50];
print(sum(numbers, 5));
```

## Project Structure

```
meow-lang/
├── CMakeLists.txt         # Build configuration
├── README.md              # This file
├── include/
│   ├── token.h           # Token definitions
│   ├── lexer.h           # Lexer interface
│   ├── parser.h          # Parser interface
│   ├── ast.h             # AST node definitions
│   └── codegen.h         # Code generator interface
├── src/
│   ├── token.c           # Token implementation
│   ├── lexer.c           # Lexer implementation
│   ├── parser.c          # Parser implementation
│   ├── ast.c             # AST implementation
│   ├── codegen.c         # Code generator implementation
│   └── main.c            # CLI entry point
├── examples/             # Example programs
└── lib/                  # Standard library files
```

## Architecture

Meow Lang follows a classic compiler pipeline:

```
Source Code (.meow)
    ↓
Lexer (Tokenization)
    ↓
Parser (AST Generation)
    ↓
Code Generator (C Code)
    ↓
GCC/Clang (Compilation)
    ↓
Executable Binary
```

### Lexer
Tokenizes the input source code into meaningful tokens (keywords, identifiers, operators, literals).

### Parser
Builds an Abstract Syntax Tree (AST) from the token stream, enforcing language syntax rules.

### Code Generator
Traverses the AST and emits equivalent C code that maintains the Meow Lang semantics.

### C Compilation
The generated C code is compiled using GCC/Clang for optimal performance.

## Performance

Meow Lang produces highly optimized C code:
- Direct 1:1 mapping to C constructs
- Full optimization benefits of GCC/Clang
- Minimal runtime overhead
- Suitable for performance-critical applications

## Limitations

- No built-in object-oriented programming
- No garbage collection (manual memory management)
- Limited standard library (extensible via C)
- Integer-based operations (64-bit by default)

## Contributing

Contributions are welcome! Areas for enhancement:
- String manipulation functions
- Mathematical function library
- File I/O operations
- Struct support
- Module system
- Better error messages

## License

This project is released under the MIT License.

## Future Roadmap

- [ ] String type and string manipulation
- [ ] Struct definitions
- [ ] Pattern matching
- [ ] Module system
- [ ] Built-in data structures (lists, maps)
- [ ] Improved error diagnostics
- [ ] REPL for interactive mode
- [ ] JIT compilation option

---

**Enjoy coding in Meow Lang!**
