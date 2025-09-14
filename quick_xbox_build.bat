@echo off
REM ================================================================================
REM GemRB Xbox Quick Build - Simple .xbe Compilation Script
REM ================================================================================
REM
REM Simple script for experienced users who just want to build quickly.
REM Assumes NXDK is properly configured and all dependencies are installed.
REM
REM Usage: quick_xbox_build.bat
REM
REM ================================================================================

echo Building GemRB for Xbox (.xbe)...

REM Check basic requirements
if not defined NXDK_DIR (
    echo.
    echo ERROR: NXDK_DIR not set! 
    echo.
    echo This script requires NXDK to be properly configured.
    echo Please install NXDK and set NXDK_DIR environment variable.
    echo.
    echo For comprehensive setup, use: build_xbox.bat
    echo For detailed instructions, see: Xbox_Build_Scripts_README.md
    echo.
    pause
    exit /b 1
)

if not exist "CMakeLists.txt" (
    echo.
    echo ERROR: Run from GemRB repository root!
    echo.
    echo This script must be run from the directory containing CMakeLists.txt
    echo Navigate to the GemRB repository root directory and try again.
    echo.
    pause
    exit /b 1
)

REM Clean and create build directory
if exist "build-xbox" rmdir /s /q "build-xbox"
mkdir "build-xbox"
cd "build-xbox"

REM Configure and build
echo Configuring...
cmake .. -DCMAKE_TOOLCHAIN_FILE="%NXDK_DIR%\share\toolchain-nxdk.cmake" -DXBOX=ON -DSTATIC_LINK=ON -DSDL_BACKEND=SDL -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles"

if errorlevel 1 (
    echo.
    echo Configuration failed!
    echo.
    echo For detailed error analysis and troubleshooting, use: build_xbox.bat
    echo For help, see: Xbox_Build_Scripts_README.md
    echo.
    cd ..
    pause
    exit /b 1
)

echo Building...
make -j%NUMBER_OF_PROCESSORS%

if errorlevel 1 (
    echo.
    echo Build failed!
    echo.
    echo For detailed error analysis and troubleshooting, use: build_xbox.bat
    echo For help, see: Xbox_Build_Scripts_README.md  
    echo.
    cd ..
    pause
    exit /b 1
)

cd ..
echo.
echo SUCCESS! GemRB built for Xbox.
echo Check build-xbox\gemrb\ for output files.
echo.
pause