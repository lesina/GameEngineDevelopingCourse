@echo off
setlocal enabledelayedexpansion

pushd %~dp0

set ROOT_DIR=%~dp0
set CMAKE_EXE="%~dp0\..\..\..\Tools\Build\CMake\bin\cmake.exe"
set CMAKE_OUTPUT_DIR="%~dp0\cmake_output\build_win64"
set LIB_DIR="%~dp0\lib\Windows"

if not exist %CMAKE_OUTPUT_DIR% (
    mkdir %CMAKE_OUTPUT_DIR%
)

%CMAKE_EXE% %ROOT_DIR% -B %CMAKE_OUTPUT_DIR% -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=%LIB_DIR% %cmake_args%
%CMAKE_EXE% --build %CMAKE_OUTPUT_DIR% --target fmod --config Debug 
%CMAKE_EXE% --build %CMAKE_OUTPUT_DIR% --target fmod --config Release

pause