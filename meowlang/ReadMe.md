# MeowLang

A tiny, structured lexer-only prototype for MeowLang.

## Build (manual)

```
g++ src/*.cpp -o meow
```

## Build (CMake)

```
cmake -S . -B build
cmake --build build
```

## Run

```
./meow examples/hello.meow
```
