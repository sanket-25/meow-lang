@echo off
setlocal

set "SCRIPT_DIR=%~dp0"
set "MEOW_EXE="

if exist "%SCRIPT_DIR%build\meow.exe" set "MEOW_EXE=%SCRIPT_DIR%build\meow.exe"
if exist "%SCRIPT_DIR%build\Debug\meow.exe" set "MEOW_EXE=%SCRIPT_DIR%build\Debug\meow.exe"
if exist "%SCRIPT_DIR%build\Release\meow.exe" set "MEOW_EXE=%SCRIPT_DIR%build\Release\meow.exe"

if not defined MEOW_EXE (
    echo Could not find meow.exe.
    echo Build first with:
    echo   cmake -S . -B build
    echo   cmake --build build
    exit /b 1
)

"%MEOW_EXE%" %*
