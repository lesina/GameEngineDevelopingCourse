# GameEngineDevelopingCourse

## Requirements
1. Visual studio 2022
2. MSVC toolset v134 (or you can just import the default.vsconfig that lies at the root of the project)
3. Windows 10 SDK (10.0.19041.0)

## Installation steps (for Windows)
1. .\get_submodules.bat
2. Got to the Samples folder and choose any available sample
3. .\generate_vs2022.bat
4. .\cmake_output\build_win64\SampleName.sln

## How to launch the project
1. Install the Smart Command Line Arguments extension for visual studio. It will help you, believe me
2. Use the command line arguments in order to properly launch the project

### List of useful command line Arguments
1. -project_root=/path/to/sample/root