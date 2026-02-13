cmake_minimum_required(VERSION 3.12)
project(meowlang)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(meow
    src/main.cpp
    src/lexer.cpp
    src/token.cpp
    src/parser.cpp
    src/compiler.cpp
)
