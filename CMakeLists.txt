cmake_minimum_required(VERSION 3.12)
project(meowlang LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(meow
    src/main.cpp
    src/lexer.cpp
    src/token.cpp
    src/parser.cpp
    src/compiler.cpp
    src/vm.cpp
)

if (MSVC)
    target_compile_options(meow PRIVATE /W4 /permissive- /utf-8)
else()
    target_compile_options(meow PRIVATE -Wall -Wextra -Wpedantic)
endif()

install(TARGETS meow RUNTIME DESTINATION bin)
