cmake_minimum_required(VERSION 3.10)
project(meow_lang C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/token.c
    src/lexer.c
    src/ast.c
    src/parser.c
    src/codegen.c
    src/main.c
)

# Create executable
add_executable(meow ${SOURCES})

# Link math library on Unix
if(UNIX)
    target_link_libraries(meow m)
endif()

# Installation
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    install(TARGETS meow RUNTIME DESTINATION bin)
else()
    install(TARGETS meow RUNTIME DESTINATION /usr/local/bin)
endif()
