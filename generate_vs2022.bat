@echo off
setlocal enabledelayedexpansion

pushd %~dp0

set ROOT_DIR=%~dp0
set CMAKE_OUTPUT_DIR="%~dp0\cmake_output\build_win64"
set PRESET="win_x64"

echo ===========================
echo Looking for MSBuild
echo ===========================
set "PATH=%PATH%;%ProgramFiles(x86)%\Microsoft Visual Studio\Installer"
set MSBUILD_PATH=""
for /f "tokens=* USEBACKQ" %%i in (`vswhere.exe -version "[17.0,)" -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do (
    set MSBUILD_PATH=%%i
)

if "%MSBUILD_PATH%" == """" (
    echo "VS2022 wasn't found"
    pause
    popd
    exit
)
echo "MSbuild path is: %MSBUILD_PATH%"

set CMAKE_EXE="%~dp0\Tools\Build\CMake\bin\cmake.exe"

if not exist %CMAKE_OUTPUT_DIR% (
    mkdir %CMAKE_OUTPUT_DIR%
)

echo "Generating cmake project"
%CMAKE_EXE% %ROOT_DIR% --preset=%PRESET% %cmake_args%
if "%ERRORLEVEL%" NEQ "0" (
	echo "Failed to generate cmake project"
	pause
    popd
    exit
)

popd

echo ===========================
echo Configuring Visual Studio solution
echo ===========================

"%MSBUILD_PATH%" /nologo -m -t:restore %CMAKE_OUTPUT_DIR%/GameEngine.sln

pause
