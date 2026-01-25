# Getting Started with Meow Lang

## Prerequisites

Before you begin, ensure you have:
- GCC or Clang compiler installed
- Bash (for Unix-like systems) or Command Prompt (for Windows)

### Installing Prerequisites

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get update
sudo apt-get install build-essential
```

**macOS:**
```bash
# Install Xcode Command Line Tools
xcode-select --install
```

**Windows:**
- Download and install MinGW from https://www.mingw-w64.org/
- Or use MSVC compiler if you have Visual Studio installed

## Building Meow Lang

### Step 1: Clone the Repository

```bash
git clone https://github.com/yourusername/meow-lang.git
cd meow-lang
```

### Step 2: Build the Compiler

**Linux/macOS:**
```bash
chmod +x build.sh
./build.sh
```

**Windows:**
```bash
build.bat
```

**Verify the build:**
```bash
./build/meow --help  # Linux/macOS
build\meow.exe       # Windows (should show usage)
```

### Step 3: Install (Optional)

**Linux/macOS:**
```bash
sudo cp build/meow /usr/local/bin/
```

**Windows:**
Add the `build` directory to your PATH environment variable.

## Your First Program

### 1. Create a file `hello.meow`:

```meow
print("Hello, Meow World!");
```

### 2. Run it:

```bash
./build/meow hello.meow
```

Output:
```
Hello, Meow World!
```

## More Examples

### Variables and Arithmetic

Create `math.meow`:
```meow
var x = 10;
var y = 20;
var sum = x + y;
var product = x * y;

print(sum);
print(product);
```

Run:
```bash
./build/meow math.meow
```

### Functions

Create `functions.meow`:
```meow
func add(a, b) {
    return a + b;
}

func multiply(a, b) {
    return a * b;
}

var result1 = add(5, 3);
var result2 = multiply(4, 7);

print(result1);
print(result2);
```

### Loops

Create `loops.meow`:
```meow
var i = 1;
while (i <= 5) {
    print(i);
    i = i + 1;
}

for (var j = 10; j >= 1; j = j - 1) {
    print(j);
}
```

### Conditionals

Create `conditions.meow`:
```meow
var age = 25;

if (age < 13) {
    print("Child");
} else if (age < 18) {
    print("Teenager");
} else {
    print("Adult");
}
```

## Language Features

### Comments

```meow
// Single line comment
var x = 42;

// Multi-line comments not yet supported, use multiple single-line comments
// This is comment line 1
// This is comment line 2
```

### Data Types

Meow Lang uses a simple type system:
- **Numbers**: 64-bit integers and floats
- **Arrays**: `[1, 2, 3]`
- **Strings**: `"hello"`
- **Booleans**: `true` or `false`

```meow
var integer = 42;
var floating = 3.14;
var text = "Meow!";
var flag = true;
var list = [1, 2, 3, 4, 5];
```

### Operators

**Arithmetic:**
```meow
var a = 10 + 5;   // 15
var b = 10 - 5;   // 5
var c = 10 * 5;   // 50
var d = 10 / 5;   // 2
var e = 10 % 3;   // 1
```

**Comparison:**
```meow
if (x == y) { print("Equal"); }
if (x != y) { print("Not equal"); }
if (x < y) { print("Less than"); }
if (x > y) { print("Greater than"); }
if (x <= y) { print("Less or equal"); }
if (x >= y) { print("Greater or equal"); }
```

**Logical:**
```meow
if (x > 0 && y > 0) { print("Both positive"); }
if (x > 0 || y > 0) { print("At least one positive"); }
if (!flag) { print("Flag is false"); }
```

### Function Declaration

```meow
func functionName(param1, param2) {
    var result = param1 + param2;
    return result;
}
```

### Control Flow

**If Statement:**
```meow
if (condition) {
    // code
} else {
    // code
}
```

**While Loop:**
```meow
while (condition) {
    // code
    break;      // Exit loop
    continue;   // Skip to next iteration
}
```

**For Loop:**
```meow
for (var i = 0; i < 10; i = i + 1) {
    print(i);
}
```

## Troubleshooting

### Compilation Errors

If you see a compilation error, check:
1. Syntax errors in your `.meow` file
2. Missing semicolons at end of statements
3. Matching braces and parentheses

### Runtime Errors

- Ensure all variables are declared before use
- Check function names are correct
- Verify array indices are within bounds

### Build Issues

If the build fails:
1. Check that GCC/Clang is installed: `gcc --version`
2. Ensure you're in the correct directory
3. Try manual compilation (see README)

## Next Steps

1. Read the [README.md](README.md) for full language documentation
2. Explore the [examples](examples/) directory for more programs
3. Contribute to the project on GitHub

## Getting Help

- Check the [README.md](README.md) for language reference
- Review [examples](examples/) for code patterns
- File an issue on GitHub for bugs or feature requests

Happy coding with Meow Lang! 🐱
