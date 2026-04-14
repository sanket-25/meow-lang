# 🐱 MeowLang

> A tiny programming language built from scratch in C++ - featuring a full compiler pipeline from **Lexer -> Parser -> AST -> Bytecode -> Virtual Machine**.

---

## 🚀 Overview

MeowLang is an **educational programming language** designed to demonstrate how a real language works internally.

It includes:

- ✅ Lexer
- ✅ Parser (recursive descent)
- ✅ AST (Abstract Syntax Tree)
- ✅ Bytecode compiler
- ✅ Stack-based Virtual Machine

> ⚠️ This is a **learning-focused project**, not production-ready.

---

## 🧠 Language Features

### ✅ Supported

- Variables (`int`, `deci`, `bool`, `string`, `char`)
- Arithmetic & logical expressions
- Print statement:

```meow
meow << expression;
```

- Conditional (`if`)
- Blocks `{ }`

---

### ❌ Not Yet Supported

- `else` branch (token exists but not implemented)
- Loops (`while`, `for`)
- Functions
- Type checking
- Garbage collection

---

## ⚙️ Architecture

```text
Source (.meow)
	↓
Lexer
	↓
Parser
	↓
AST
	↓
Compiler -> Bytecode
	↓
Virtual Machine
	↓
Output
```

### ⚠️ Current Limitations

- Global VM state (variables persist across runs)
- Undefined variables return empty values (no error)
- Runtime type coercion via `stoi` / `stod`
- Minimal error handling

---

## 📦 Build Instructions

### 🔧 Manual Build (Linux/macOS)

```bash
g++ src/*.cpp -o meow
```

### 🔧 Manual Build (Windows - MinGW)

```bash
g++ src/*.cpp -o meow.exe
```

### 🏗️ Build using CMake

```bash
cmake -S . -B build
cmake --build build
```

---

## ▶️ Run

### Linux/macOS

```bash
./meow examples/hello.meow
```

### Windows

```powershell
.\build\meow.exe examples\hello.meow
```

---

## 🧪 Example

```meow
int x = 10;
deci y = 5.5;
meow << x + y;
```

**Output:**

```text
15.5
```

---

## 🚀 Public Release (GitHub)

### 1. Remove generated binaries

```bash
git rm --cached meow meow-lang.deb meow-lang_1.0_amd64.snap meow-lang/usr/bin/meow
git commit -m "chore: stop tracking generated release binaries"
```

### 2. Push changes

```bash
git push origin main
```

### 3. Create release tag

```bash
git tag v1.0.0
git push origin v1.0.0
```

### 4. Automated Release (GitHub Actions)

The workflow:

`.github/workflows/release.yml`

Will:

- ✅ Build on Windows & Linux
- 📦 Package binaries
- 🚀 Publish GitHub Release

> ⚠️ Ensure the workflow file exists before tagging.

---

## 📦 Local Packaging (Optional)

### Windows

```powershell
powershell -ExecutionPolicy Bypass -File scripts/package_windows.ps1 -Version v1.0.0
```

### Linux

```bash
bash scripts/package_linux.sh v1.0.0
```

---

## ⚠️ Important Before Open Source

- ❗ Remove sensitive files (e.g. `credentials.txt`) from Git history
- ❗ Add a proper `LICENSE` (MIT recommended)
- ❗ Ensure `.gitignore` excludes binaries
- ❗ Fix case-sensitive file issues (`README.md`)

---

## 🛣️ Roadmap

### 🔹 Short Term

- Add `else` support
- Improve error handling
- Fix VM global state

### 🔹 Mid Term

- Add loops (`while`)
- Better runtime safety
- Parser diagnostics

### 🔹 Long Term

- Functions & call stack
- Static type checking
- Garbage collection
- Improved REPL

---

## 🤝 Contributing

Contributions are welcome.

If you want to:

- Improve the VM
- Add new language features
- Fix bugs

Feel free to open issues or submit PRs.

---

## 📜 License

> Currently not defined - add an open-source license (MIT / Apache-2.0 recommended).

---

## 💡 Why MeowLang?

This project is built to answer one question:

> *"How does a programming language actually work under the hood?"*

If you're learning compilers, interpreters, or systems programming, this is for you.
