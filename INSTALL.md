# Installation Guide for Meow Lang

## System Requirements

- **Compiler**: GCC 4.8+ or Clang 3.1+
- **Build Tools**: Make or equivalent
- **Memory**: 256MB minimum (1GB recommended)
- **Disk Space**: 50MB for build

## Platform-Specific Installation

### Linux (Ubuntu/Debian)

#### 1. Install Build Tools
```bash
sudo apt-get update
sudo apt-get install build-essential git
```

#### 2. Clone and Build
```bash
git clone https://github.com/yourusername/meow-lang.git
cd meow-lang
chmod +x build.sh
./build.sh
```

#### 3. Install Globally
```bash
sudo cp build/meow /usr/local/bin/meow
sudo chmod +x /usr/local/bin/meow
```

#### 4. Verify Installation
```bash
meow --help
meow examples/hello.meow
```

### macOS

#### 1. Install Xcode Command Line Tools
```bash
xcode-select --install
```

#### 2. Clone and Build
```bash
git clone https://github.com/yourusername/meow-lang.git
cd meow-lang
chmod +x build.sh
./build.sh
```

#### 3. Install Globally
```bash
sudo cp build/meow /usr/local/bin/meow
sudo chmod +x /usr/local/bin/meow
```

#### 4. Verify Installation
```bash
meow examples/hello.meow
```

### Windows

#### Option A: Using MinGW (Recommended)

1. **Download and Install MinGW-w64**
   - Visit: https://www.mingw-w64.org/
   - Download the installer for your system (32-bit or 64-bit)
   - Run the installer and select:
     - Architecture: x86_64 (for 64-bit)
     - Threads: win32
     - Installation path: `C:\mingw64`

2. **Add MinGW to PATH**
   - Open System Properties → Environment Variables
   - Edit `PATH` and add: `C:\mingw64\bin`
   - Click OK

3. **Clone and Build**
   ```bash
   git clone https://github.com/yourusername/meow-lang.git
   cd meow-lang
   build.bat
   ```

4. **Add to PATH (Optional)**
   - Add the `meow-lang\build` directory to your PATH
   - Or use the full path: `C:\path\to\meow-lang\build\meow.exe`

5. **Verify Installation**
   ```bash
   meow examples\hello.meow
   ```

#### Option B: Using MSVC (Visual Studio)

1. **Install Visual Studio Community**
   - Download from: https://visualstudio.microsoft.com/vs/community/
   - Install with "Desktop development with C++"

2. **Clone Repository**
   ```bash
   git clone https://github.com/yourusername/meow-lang.git
   cd meow-lang
   ```

3. **Open Developer Command Prompt**
   - Search for "Developer Command Prompt for Visual Studio"

4. **Compile and Link**
   ```bash
   cl /Iinclude src\token.c src\lexer.c src\ast.c src\parser.c src\codegen.c src\main.c /Fe:build\meow.exe
   ```

### Docker (Cross-Platform)

1. **Create a Dockerfile:**
   ```dockerfile
   FROM gcc:11
   WORKDIR /app
   COPY . .
   RUN chmod +x build.sh && ./build.sh
   ENTRYPOINT ["./build/meow"]
   ```

2. **Build Docker Image**
   ```bash
   docker build -t meow-lang .
   ```

3. **Run Meow Programs**
   ```bash
   docker run -v $(pwd):/workspace meow-lang /workspace/program.meow
   ```

## Manual Compilation

If build scripts fail, compile manually:

```bash
# Create output directory
mkdir -p build

# Compile object files
gcc -c -Iinclude src/token.c -o build/token.o
gcc -c -Iinclude src/lexer.c -o build/lexer.o
gcc -c -Iinclude src/ast.c -o build/ast.o
gcc -c -Iinclude src/parser.c -o build/parser.o
gcc -c -Iinclude src/codegen.c -o build/codegen.o
gcc -c -Iinclude src/main.c -o build/main.o

# Link object files
gcc build/token.o build/lexer.o build/ast.o build/parser.o build/codegen.o build/main.o -o build/meow -lm

# On Windows, replace the last line with:
# gcc build/token.o build/lexer.o build/ast.o build/parser.o build/codegen.o build/main.o -o build/meow.exe -lm
```

## Verification

Test your installation:

### 1. Check Executable
```bash
# Linux/macOS
ls -la build/meow

# Windows
dir build\meow.exe
```

### 2. Run a Simple Program
Create `test.meow`:
```meow
print(42);
```

Run:
```bash
meow test.meow
```

Expected output:
```
42
```

### 3. Run Examples
```bash
meow examples/hello.meow
meow examples/factorial.meow
```

## Uninstallation

### Linux/macOS
```bash
sudo rm /usr/local/bin/meow
```

### Windows
- Remove from PATH environment variable
- Delete the cloned directory

## Troubleshooting

### "gcc: command not found"
- Install GCC: See platform-specific instructions above
- Verify PATH is set correctly

### Build fails with "Permission denied"
- On Linux/macOS: Run `chmod +x build.sh` before executing

### Cannot find header files
- Ensure you're in the meow-lang directory
- Verify all files are present: `ls include/` (Linux/macOS) or `dir include` (Windows)

### Linker errors
- Ensure all `.c` files compile to `.o` files
- Check for typos in filenames
- Verify math library is linked: Add `-lm` to final link command

### Runtime errors after compilation
- Check that your Meow program is syntactically correct
- Review error messages from the compiler
- See [GETTING_STARTED.md](GETTING_STARTED.md) for language examples

## Next Steps

1. Read [README.md](README.md) for language reference
2. Check [GETTING_STARTED.md](GETTING_STARTED.md) for tutorials
3. Explore [examples/](examples/) directory for sample programs

## Support

- File issues on GitHub
- Check existing documentation
- Review example programs for patterns
