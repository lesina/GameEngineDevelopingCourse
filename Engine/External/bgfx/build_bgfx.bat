cd bgfx\
..\bx\tools\bin\windows\genie vs2022
cd ..

setlocal enabledelayedexpansion

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

SET CL=/std:c++20 /MDd
SET _CL_=/std:c++20 /MDd

"%MSBUILD_PATH%" bgfx\.build\projects\vs2022\bgfx.sln /p:Configuration=Debug /p:Platform=x64 /p:PlatformToolset=v143

SET CL=/std:c++20 /MD
SET _CL_=/std:c++20 /MD

"%MSBUILD_PATH%" bgfx\.build\projects\vs2022\bgfx.sln /p:Configuration=Release /p:Platform=x64 /p:PlatformToolset=v143

xcopy bgfx\.build\win64_vs2022\bin lib\x64 /Y