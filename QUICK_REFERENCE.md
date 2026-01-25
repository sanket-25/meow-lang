# Meow Lang - Quick Reference Card

## Basic Syntax

### Comments
```meow
// This is a comment
// Multi-line: use multiple comment lines
```

### Variables
```meow
var name = value;
var x = 42;
var text = "hello";
var flag = true;
```

### Functions
```meow
func name(param1, param2) {
    // function body
    return result;
}
```

## Data Types

| Type | Example | Notes |
|------|---------|-------|
| Number | `42`, `3.14` | 64-bit int/float |
| String | `"text"` | Text values |
| Boolean | `true`, `false` | True/false |
| Array | `[1, 2, 3]` | Indexed list |
| Null | `null` | Null value |

## Operators

### Arithmetic
| Op | Meaning | Example |
|----|---------|---------|
| `+` | Addition | `5 + 3` → 8 |
| `-` | Subtraction | `5 - 3` → 2 |
| `*` | Multiplication | `5 * 3` → 15 |
| `/` | Division | `6 / 3` → 2 |
| `%` | Modulo | `7 % 3` → 1 |

### Comparison
| Op | Meaning |
|----|---------|
| `==` | Equal |
| `!=` | Not equal |
| `<` | Less than |
| `>` | Greater than |
| `<=` | Less than or equal |
| `>=` | Greater than or equal |

### Logical
| Op | Meaning |
|----|---------|
| `&&` | AND |
| `||` | OR |
| `!` | NOT |

### Bitwise
| Op | Meaning |
|----|---------|
| `&` | AND |
| `|` | OR |
| `^` | XOR |
| `~` | NOT |
| `<<` | Left shift |
| `>>` | Right shift |

### Assignment
| Op | Meaning |
|----|---------|
| `=` | Assign |
| `+=` | Add and assign |
| `-=` | Subtract and assign |
| `*=` | Multiply and assign |
| `/=` | Divide and assign |

## Control Flow

### If Statement
```meow
if (condition) {
    // code if true
} else {
    // code if false
}
```

### While Loop
```meow
while (condition) {
    // loop body
    break;      // exit loop
    continue;   // next iteration
}
```

### For Loop
```meow
for (var i = 0; i < 10; i = i + 1) {
    // loop body
}
```

## Built-in Functions

### print()
```meow
print(42);           // Print number
print("hello");      // Print string
print(true);         // Print boolean
var sum = 5 + 3;
print(sum);          // Print variable
```

## Common Patterns

### Countdown
```meow
for (var i = 10; i >= 1; i = i - 1) {
    print(i);
}
```

### Accumulation
```meow
var sum = 0;
for (var i = 1; i <= 100; i = i + 1) {
    sum = sum + i;
}
print(sum);  // 5050
```

### Conditionals
```meow
if (age < 13) {
    print("Child");
} else if (age < 18) {
    print("Teen");
} else {
    print("Adult");
}
```

### Array Access
```meow
var arr = [10, 20, 30];
var first = arr[0];    // 10
var last = arr[2];     // 30
```

## Error Prevention

### Common Mistakes

❌ **Missing semicolon**
```meow
var x = 5  // Error: expected ;
```
✅ **Add semicolon**
```meow
var x = 5;
```

❌ **Undefined variable**
```meow
print(y);  // Error: y not defined
```
✅ **Declare first**
```meow
var y = 10;
print(y);
```

❌ **Wrong brackets**
```meow
if x > 5) {  // Error: ( expected
```
✅ **Use correct brackets**
```meow
if (x > 5) {
```

❌ **Array out of bounds**
```meow
var arr = [1, 2];
var x = arr[5];  // Error or undefined
```
✅ **Check array length**
```meow
var arr = [1, 2, 3];
var x = arr[2];  // Valid
```

## Command Line

### Build Compiler
```bash
./build.sh          # Unix/Linux/macOS
build.bat           # Windows
```

### Run Program
```bash
./build/meow program.meow
# or
meow program.meow   # If installed
```

### Examples
```bash
meow examples/hello.meow
meow examples/factorial.meow
meow examples/fibonacci.meow
meow examples/primes.meow
meow examples/sum_100.meow
```

## Complete Example

### factorial.meow
```meow
func factorial(n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

print(factorial(5));   // Output: 120
print(factorial(10));  // Output: 3628800
```

## Getting Help

- **Language Guide**: See README.md
- **Tutorials**: See GETTING_STARTED.md
- **Installation**: See INSTALL.md
- **Examples**: Check examples/ directory
- **Architecture**: See PROJECT_SUMMARY.md

---

**Meow Lang Reference Card** - Keep this handy while coding! 🐱
