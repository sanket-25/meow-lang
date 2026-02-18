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
