@echo off
setlocal enabledelayedexpansion

echo Updating git submodules

where /q git
if errorlevel 1 (
    echo "ERROR: Couldn't find git, probably not a git repo"
	pause
) ELSE (
    git submodule sync --recursive
    git submodule update --init --recursive
)

exit /B 0