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

<img width="2364" height="1735" alt="image" src="https://github.com/user-attachments/assets/b77c2e6f-4709-4cde-941c-6a778a8a398a" />
<img width="563" height="815" alt="image" src="https://github.com/user-attachments/assets/86708ce6-5106-4e2c-b72f-ce02050dbec9" />


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
