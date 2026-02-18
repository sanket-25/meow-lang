# MeowLang

A tiny, structured lexer-only prototype for MeowLang.

## Build (manual, Linux/macOS)

```
g++ src/*.cpp -o meow
```

## Build (manual, Windows MinGW)

```
g++ src/*.cpp -o meow.exe
```

## Build (CMake)

```
cmake -S . -B build
cmake --build build
```

## Run (Linux/macOS)

```
./meow examples/hello.meow
```

## Run (Windows)

```
.\build\meow.exe examples\hello.meow
```

PowerShell does not execute binaries from the current directory unless you prefix with `.\`.

- Works in current folder:

	```
	.\meow.exe examples\hello.meow
	```

- To run `meow` from anywhere, install it to your User PATH:

	```
	powershell -ExecutionPolicy Bypass -File scripts/install_windows.ps1
	```

	Then open a **new** terminal and run:

	```
	meow --version
	meow examples\hello.meow
	```

## Public release (GitHub)

1. Ensure generated binaries are not tracked in git:

	```
	git rm --cached meow meow-lang.deb meow-lang_1.0_amd64.snap meow-lang/usr/bin/meow
	git commit -m "chore: stop tracking generated release binaries"
	```

2. Push your latest main branch changes.

3. Create and push a version tag:

	```
	git tag v1.0.0
	git push origin v1.0.0
	```

4. The GitHub Actions workflow `.github/workflows/release.yml` will:
	- build MeowLang on Windows and Linux
	- package archives into `dist/`
	- publish a GitHub Release with attached artifacts

### Local packaging (optional)

- Windows:

  ```
  powershell -ExecutionPolicy Bypass -File scripts/package_windows.ps1 -Version v1.0.0
  ```

- Linux:

  ```
  bash scripts/package_linux.sh v1.0.0
  ```
