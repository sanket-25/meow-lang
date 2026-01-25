# Meow Lang - Building on Windows

Your WSL environment is minimal and doesn't have GCC installed. Here are your options:

## Option 1: Install GCC on Windows via MinGW (RECOMMENDED)

### Step 1: Download MinGW-w64
1. Visit: https://www.mingw-w64.org/
2. Click "Downloads" 
3. Download the installer for your system (64-bit recommended)

### Step 2: Install MinGW
1. Run the installer
2. Choose installation directory: `C:\mingw64`
3. Select:
   - Architecture: x86_64
   - Threads: win32
4. Finish installation

### Step 3: Add to PATH
1. Open "Edit the system environment variables" (Windows Search)
2. Click "Environment Variables" button
3. Edit "PATH" and add: `C:\mingw64\bin`
4. Click OK and restart your terminal

### Step 4: Build Meow Lang
```bash
cd c:\Users\Admin\Documents\GitHub\meow-lang
build.bat
```

### Step 5: Run Programs
```bash
build\meow.exe examples\hello.meow
```

---

## Option 2: Install Ubuntu on WSL

If you prefer to stay in WSL:

```bash
wsl --install Ubuntu
```

Then in Ubuntu:
```bash
sudo apt update
sudo apt install build-essential
cd /mnt/c/Users/Admin/Documents/GitHub/meow-lang
./build.sh
./build/meow examples/hello.meow
```

---

## Option 3: Use Docker

If you have Docker installed:

```bash
docker run -it -v c:\Users\Admin\Documents\GitHub\meow-lang:/app gcc:latest bash
cd /app
chmod +x build.sh
./build.sh
```

---

## Quick Build Command (Windows PowerShell)

```powershell
cd c:\Users\Admin\Documents\GitHub\meow-lang
build.bat
build\meow.exe examples\hello.meow
```

---

## Troubleshooting

**"gcc: command not found"** 
→ Install MinGW following Option 1 above

**"build.bat not found"**
→ Make sure you're in the meow-lang directory

**"Permission denied"**
→ Try running terminal as Administrator

---

Choose Option 1 (MinGW) for the easiest setup on Windows!
