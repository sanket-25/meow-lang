# Contributing to Meow Lang

Thank you for your interest in contributing to Meow Lang! This document provides guidelines and instructions for contributing.

## Code of Conduct

- Be respectful and inclusive
- Welcome feedback and different perspectives
- Focus on the code, not the person
- Help others learn and improve

## Ways to Contribute

### 1. Report Bugs
- Check if the issue already exists
- Provide a minimal test case
- Include your OS and compiler version
- Describe the expected vs actual behavior

### 2. Suggest Features
- Explain the use case
- Provide examples of desired syntax
- Discuss potential implementation
- Be open to feedback

### 3. Improve Documentation
- Fix typos and unclear explanations
- Add more examples
- Clarify complex concepts
- Improve code comments

### 4. Write Code

## Development Setup

### Prerequisites
- GCC or Clang
- Git
- Text editor or IDE

### Build from Development Source
```bash
git clone <your-fork>
cd meow-lang
./build.sh
```

## Coding Standards

### C Code Style
- Use 4-space indentation
- Keep lines under 80 characters where possible
- Use descriptive variable names
- Add comments for complex logic
- Follow existing code patterns

### Example:
```c
// Good
int calculate_sum(int a, int b) {
    int result = a + b;
    return result;
}

// Avoid
int calc(int x, int y) {
    return x + y;  // Unclear what this calculates
}
```

### Header Files
- Include guards for all headers
- Forward declarations where needed
- Clear function prototypes
- Documentation comments

### Implementation Files
- Include necessary headers only
- Keep functions focused
- Handle errors gracefully
- Free allocated memory

## Git Workflow

### 1. Fork the Repository
```bash
# Go to GitHub and click "Fork"
git clone https://github.com/your-username/meow-lang.git
cd meow-lang
git remote add upstream https://github.com/original/meow-lang.git
```

### 2. Create a Feature Branch
```bash
git checkout -b feature/your-feature-name
# or
git checkout -b fix/your-bug-fix
```

### 3. Make Changes
```bash
# Edit files...
./build.sh              # Test your changes
```

### 4. Commit Changes
```bash
git add .
git commit -m "Clear, descriptive commit message"
# Example: "Add string concatenation support"
```

### 5. Push to Your Fork
```bash
git push origin feature/your-feature-name
```

### 6. Create Pull Request
- Go to GitHub
- Click "New Pull Request"
- Select your branch
- Describe your changes
- Reference any related issues

## Testing

### Before Submitting PR

1. **Build successfully:**
   ```bash
   ./build.sh
   ```

2. **Test existing examples:**
   ```bash
   ./build/meow examples/hello.meow
   ./build/meow examples/factorial.meow
   ./build/meow examples/fibonacci.meow
   ./build/meow examples/primes.meow
   ```

3. **Create test cases for your feature:**
   - Add example `.meow` files in `examples/`
   - Test edge cases
   - Test error conditions

### Example Test
```meow
// tests/test_addition.meow
var result = 2 + 3;
print(result);  // Should print: 5
```

## PR Requirements

### All PRs Must Include:
- ✅ Clear description of changes
- ✅ Passing builds
- ✅ Working examples/tests
- ✅ Updated documentation (if applicable)
- ✅ No breaking changes (unless discussed)

### For New Features:
- ✅ Example usage in `examples/`
- ✅ Documentation in `README.md`
- ✅ Test cases demonstrating functionality
- ✅ Backward compatible

### For Bug Fixes:
- ✅ Description of the bug
- ✅ Root cause analysis
- ✅ Test case demonstrating the bug
- ✅ Proof that fix works

## Documentation Style

### Code Comments
```c
// Use clear, concise comments
// Explain "why", not just "what"

// Good
// Use 64-bit integers to support large numbers
long long value = 0;

// Avoid
// Set value to 0
long long value = 0;
```

### File Headers
```c
// Include file purpose and key functions
// Example:
//
// lexer.c - Tokenization for Meow Lang
// Converts source code into a stream of tokens
// Main functions: lexer_new(), lexer_next_token()
```

## Areas for Contribution

### High Priority
- [ ] String type implementation
- [ ] More built-in functions (strlen, etc.)
- [ ] Better error messages
- [ ] Performance optimizations

### Medium Priority
- [ ] Struct support
- [ ] Math library functions
- [ ] File I/O
- [ ] Better documentation examples

### Nice to Have
- [ ] REPL mode
- [ ] Debugger integration
- [ ] IDE plugins
- [ ] Official package managers

## Review Process

1. **Automated Checks**
   - Code builds successfully
   - Examples run correctly

2. **Code Review**
   - Adherence to style guide
   - Logic correctness
   - Potential edge cases
   - Documentation completeness

3. **Approval & Merge**
   - At least one approval
   - All comments addressed
   - All tests passing

## Questions?

- Check existing issues and discussions
- Review documentation
- Look at similar code for examples
- Ask in pull request comments

## License

By contributing, you agree that your contributions will be licensed under the same MIT License as the project.

## Thank You!

Your contributions help make Meow Lang better for everyone. We appreciate your time and effort!

---

**Happy Contributing!** 🐱
