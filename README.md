🐱 MeowLang

A tiny programming language built from scratch in C++ — featuring a full compiler pipeline from Lexer → Parser → AST → Bytecode → Virtual Machine.

🚀 Overview

MeowLang is an educational programming language designed to demonstrate how a real language works internally.

It includes:

✅ Lexer
✅ Parser (recursive descent)
✅ AST (Abstract Syntax Tree)
✅ Bytecode compiler
✅ Stack-based Virtual Machine

⚠️ This is a learning-focused project, not production-ready.

🧠 Language Features
✅ Supported
Variables (int, deci, bool, string, char)
Arithmetic & logical expressions

Print statement:

meow << expression;
Conditional (if)
Blocks { }
❌ Not Yet Supported
else branch (token exists but not implemented)
Loops (while, for)
Functions
Type checking
Garbage collection
⚙️ Architecture
Source (.meow)
   ↓
Lexer
   ↓
Parser
   ↓
AST
   ↓
Compiler → Bytecode
   ↓
Virtual Machine
   ↓
Output
⚠️ Current Limitations
Global VM state (variables persist across runs)
Undefined variables return empty values (no error)
Runtime type coercion via stoi / stod
Minimal error handling
📦 Build Instructions
🔧 Manual Build (Linux/macOS)
g++ src/*.cpp -o meow
🔧 Manual Build (Windows - MinGW)
g++ src/*.cpp -o meow.exe
🏗️ Build using CMake
cmake -S . -B build
cmake --build build
▶️ Run
Linux/macOS
./meow examples/hello.meow
Windows
.\build\meow.exe examples\hello.meow
🧪 Example
int x = 10;
deci y = 5.5;
meow << x + y;

Output:

15.5
🚀 Public Release (GitHub)
1. Remove generated binaries
git rm --cached meow meow-lang.deb meow-lang_1.0_amd64.snap meow-lang/usr/bin/meow
git commit -m "chore: stop tracking generated release binaries"
2. Push changes
git push origin main
3. Create release tag
git tag v1.0.0
git push origin v1.0.0
4. Automated Release (GitHub Actions)

The workflow:

.github/workflows/release.yml

Will:

✅ Build on Windows & Linux
📦 Package binaries
🚀 Publish GitHub Release

⚠️ Ensure the workflow file exists before tagging.

📦 Local Packaging (Optional)
Windows
powershell -ExecutionPolicy Bypass -File scripts/package_windows.ps1 -Version v1.0.0
Linux
bash scripts/package_linux.sh v1.0.0
