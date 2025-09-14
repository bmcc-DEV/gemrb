@echo off
setlocal EnableDelayedExpansion

REM ================================================================================
REM GemRB Xbox (.xbe) Build Script - Automated Compilation for Original Xbox
REM ================================================================================
REM
REM This script automates the compilation of GemRB for the original Xbox console
REM using the NXDK (Xbox Development Kit) toolchain. It handles environment setup,
REM build configuration, and compilation to produce a .xbe file.
REM
REM Prerequisites:
REM   - NXDK (Xbox Development Kit) installed and configured
REM   - CMake 3.25 or later
REM   - MinGW or MSYS2 with make
REM   - Git (for repository operations)
REM
REM Usage:
REM   build_xbox.bat [clean|release|debug|quick]
REM
REM Options:
REM   clean   - Clean build directory before building
REM   release - Build optimized release version (default)
REM   debug   - Build debug version with symbols
REM   quick   - Skip environment checks and build immediately
REM
REM ================================================================================

echo.
echo ================================================================================
echo  GemRB Xbox (.xbe) Build Script
echo ================================================================================
echo.

REM Parse command line arguments
set BUILD_TYPE=Release
set CLEAN_BUILD=false
set QUICK_BUILD=false

:parse_args
if "%~1"=="" goto check_env
if /I "%~1"=="clean" set CLEAN_BUILD=true
if /I "%~1"=="release" set BUILD_TYPE=Release
if /I "%~1"=="debug" set BUILD_TYPE=Debug
if /I "%~1"=="quick" set QUICK_BUILD=true
shift
goto parse_args

:check_env
REM Environment validation (skip if quick build)
if "%QUICK_BUILD%"=="true" goto setup_vars

echo [INFO] Checking build environment...

REM Check for NXDK
if not defined NXDK_DIR (
    echo.
    echo [ERROR] NXDK_DIR environment variable not set!
    echo [ERROR] 
    echo [ERROR] Xbox builds require NXDK ^(Xbox Development Kit^) to be installed and configured.
    echo [ERROR]
    echo [SOLUTION] To fix this issue:
    echo [SOLUTION] 1. Download NXDK from: https://github.com/XboxDev/nxdk
    echo [SOLUTION] 2. Follow NXDK installation instructions
    echo [SOLUTION] 3. Set NXDK_DIR environment variable:
    echo [SOLUTION]    Example: set NXDK_DIR=C:\nxdk
    echo [SOLUTION] 4. Restart command prompt after setting environment variable
    echo [SOLUTION] 5. Re-run this script
    echo [SOLUTION]
    echo [HELP] For detailed Xbox build instructions, see:
    echo [HELP] - platforms\xbox\README.Xbox.md
    echo [HELP] - Xbox_Build_Scripts_README.md
    echo.
    goto error_exit
)

if not exist "%NXDK_DIR%" (
    echo.
    echo [ERROR] NXDK directory not found: %NXDK_DIR%
    echo [ERROR]
    echo [SOLUTION] Please verify NXDK installation:
    echo [SOLUTION] 1. Check that NXDK_DIR points to correct installation
    echo [SOLUTION] 2. Verify NXDK was installed successfully
    echo [SOLUTION] 3. Try reinstalling NXDK if the directory is missing
    echo [SOLUTION]
    echo [HELP] Get NXDK from: https://github.com/XboxDev/nxdk
    echo.
    goto error_exit
)

echo [OK] NXDK found at: %NXDK_DIR%

REM Check for NXDK toolchain file
if not exist "%NXDK_DIR%\share\toolchain-nxdk.cmake" (
    echo.
    echo [ERROR] NXDK CMake toolchain file not found!
    echo [ERROR] Expected: %NXDK_DIR%\share\toolchain-nxdk.cmake
    echo [ERROR]
    echo [SOLUTION] This indicates an incomplete or corrupt NXDK installation:
    echo [SOLUTION] 1. Verify NXDK was installed completely
    echo [SOLUTION] 2. Check NXDK version compatibility
    echo [SOLUTION] 3. Try reinstalling NXDK
    echo [SOLUTION]
    echo [HELP] Expected NXDK structure should include:
    echo [HELP] - %NXDK_DIR%\share\toolchain-nxdk.cmake
    echo [HELP] - %NXDK_DIR%\bin\
    echo [HELP] - %NXDK_DIR%\lib\
    echo.
    goto error_exit
)

echo [OK] NXDK CMake toolchain found

REM Check for CMake
cmake --version >nul 2>&1
if errorlevel 1 (
    echo.
    echo [ERROR] CMake not found in PATH!
    echo [ERROR]
    echo [SOLUTION] CMake 3.25 or later is required for Xbox builds:
    echo [SOLUTION] 1. Download CMake from: https://cmake.org/download/
    echo [SOLUTION] 2. Install CMake and add to PATH
    echo [SOLUTION] 3. Restart command prompt
    echo [SOLUTION] 4. Verify installation: cmake --version
    echo [SOLUTION]
    echo [HELP] Alternative: Use full path to cmake.exe
    echo.
    goto error_exit
)

echo [OK] CMake is available

REM Check for Make
make --version >nul 2>&1
if errorlevel 1 (
    echo.
    echo [ERROR] Make not found in PATH!
    echo [ERROR]
    echo [SOLUTION] Make is required to compile the project:
    echo [SOLUTION] 1. Install MinGW from: https://www.mingw-w64.org/
    echo [SOLUTION] 2. OR install MSYS2 from: https://www.msys2.org/
    echo [SOLUTION] 3. Add make.exe to your system PATH
    echo [SOLUTION] 4. Restart command prompt
    echo [SOLUTION] 5. Verify installation: make --version
    echo [SOLUTION]
    echo [HELP] Common make.exe locations:
    echo [HELP] - MinGW: C:\MinGW\bin\make.exe
    echo [HELP] - MSYS2: C:\msys64\usr\bin\make.exe
    echo.
    goto error_exit
)

echo [OK] Make is available

REM Check if we're in the GemRB repository
if not exist "CMakeLists.txt" (
    echo [ERROR] CMakeLists.txt not found in current directory!
    echo [ERROR] Please run this script from the GemRB repository root.
    goto error_exit
)

if not exist "platforms\xbox\xbox.cmake" (
    echo [ERROR] Xbox platform files not found!
    echo [ERROR] Please ensure you're using a GemRB version with Xbox support.
    goto error_exit
)

echo [OK] GemRB repository and Xbox platform files found
echo.

:setup_vars
REM Setup build variables
set BUILD_DIR=build-xbox
set CMAKE_TOOLCHAIN_FILE=%NXDK_DIR%\share\toolchain-nxdk.cmake

echo [INFO] Build configuration:
echo         Build Type: %BUILD_TYPE%
echo         Build Directory: %BUILD_DIR%
echo         Clean Build: %CLEAN_BUILD%
echo         NXDK Directory: %NXDK_DIR%
echo.

:setup_build_dir
REM Setup build directory
if "%CLEAN_BUILD%"=="true" (
    echo [INFO] Cleaning build directory...
    if exist "%BUILD_DIR%" (
        rmdir /s /q "%BUILD_DIR%"
        if errorlevel 1 (
            echo [ERROR] Failed to clean build directory!
            goto error_exit
        )
    )
    echo [OK] Build directory cleaned
)

if not exist "%BUILD_DIR%" (
    echo [INFO] Creating build directory...
    mkdir "%BUILD_DIR%"
    if errorlevel 1 (
        echo [ERROR] Failed to create build directory!
        goto error_exit
    )
)

:configure
echo [INFO] Configuring CMake for Xbox build...
cd "%BUILD_DIR%"

REM Xbox-specific CMake configuration with comprehensive optimizations
cmake .. ^
    -DCMAKE_TOOLCHAIN_FILE="%CMAKE_TOOLCHAIN_FILE%" ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DXBOX=ON ^
    -DSTATIC_LINK=ON ^
    -DSDL_BACKEND=SDL ^
    -DDISABLE_WERROR=ON ^
    -DUSE_OPENAL=OFF ^
    -DUSE_FREETYPE=OFF ^
    -DUSE_LIBVLC=OFF ^
    -DUSE_SDLMIXER=ON ^
    -DXBOX_AUDIO_OPTIMIZATIONS=ON ^
    -DXBOX_MEMORY_OPTIMIZATIONS=ON ^
    -DBUILD_TESTING=OFF ^
    -DSKIP_DEMO_DATA=OFF ^
    -G "MinGW Makefiles"

if errorlevel 1 (
    echo.
    echo [ERROR] CMake configuration failed!
    echo [ERROR]
    echo [TROUBLESHOOTING] Common causes and solutions:
    echo [TROUBLESHOOTING] 1. NXDK toolchain issues:
    echo [TROUBLESHOOTING]    - Verify NXDK_DIR points to correct installation
    echo [TROUBLESHOOTING]    - Check NXDK version compatibility
    echo [TROUBLESHOOTING]    - Try reinstalling NXDK
    echo [TROUBLESHOOTING]
    echo [TROUBLESHOOTING] 2. Missing dependencies:
    echo [TROUBLESHOOTING]    - Ensure all NXDK dependencies are installed
    echo [TROUBLESHOOTING]    - Check NXDK installation documentation
    echo [TROUBLESHOOTING]
    echo [TROUBLESHOOTING] 3. Path issues:
    echo [TROUBLESHOOTING]    - Try building from shorter directory path
    echo [TROUBLESHOOTING]    - Avoid spaces in directory names
    echo [TROUBLESHOOTING]
    echo [HELP] For detailed troubleshooting, see:
    echo [HELP] - platforms\xbox\README.Xbox.md
    echo [HELP] - NXDK documentation
    echo.
    cd ..
    goto error_exit
)

echo [OK] CMake configuration completed successfully
echo.

:build
echo [INFO] Building GemRB for Xbox...
echo [INFO] This may take several minutes depending on your system...
echo.

REM Build with parallel jobs for faster compilation
set /a JOBS=%NUMBER_OF_PROCESSORS%
if %JOBS% LSS 1 set JOBS=1
if %JOBS% GTR 8 set JOBS=8

echo [INFO] Building with %JOBS% parallel jobs...

make -j%JOBS%

if errorlevel 1 (
    echo.
    echo [ERROR] Build failed!
    echo [ERROR]
    echo [TROUBLESHOOTING] Common build issues and solutions:
    echo [TROUBLESHOOTING] 1. Compilation errors:
    echo [TROUBLESHOOTING]    - Check for NXDK compatibility issues
    echo [TROUBLESHOOTING]    - Verify source code is compatible with Xbox target
    echo [TROUBLESHOOTING]
    echo [TROUBLESHOOTING] 2. Linking errors:
    echo [TROUBLESHOOTING]    - Ensure NXDK libraries are accessible
    echo [TROUBLESHOOTING]    - Check for missing Xbox-specific dependencies
    echo [TROUBLESHOOTING]
    echo [TROUBLESHOOTING] 3. Resource issues:
    echo [TROUBLESHOOTING]    - Try building with fewer parallel jobs: make -j2
    echo [TROUBLESHOOTING]    - Ensure sufficient disk space
    echo [TROUBLESHOOTING]
    echo [HELP] For detailed troubleshooting, see:
    echo [HELP] - platforms\xbox\README.Xbox.md
    echo [HELP] - Build logs above for specific error details
    echo.
    cd ..
    goto error_exit
)

cd ..
echo.
echo [OK] Build completed successfully!

:post_build
echo [INFO] Post-build operations...

REM Check if the executable was created
if exist "%BUILD_DIR%\gemrb\gemrb.exe" (
    echo [OK] GemRB executable created: %BUILD_DIR%\gemrb\gemrb.exe
) else (
    echo [WARNING] GemRB executable not found at expected location
)

REM Look for .xbe file (NXDK should create this)
for /r "%BUILD_DIR%" %%f in (*.xbe) do (
    echo [OK] Xbox executable found: %%f
    set XBE_FOUND=true
)

if not defined XBE_FOUND (
    echo [INFO] .xbe file not found - this may be normal depending on NXDK configuration
    echo [INFO] Check NXDK documentation for .xbe generation
)

REM Create deployment instructions
echo.
echo ================================================================================
echo  Build Summary
echo ================================================================================
echo.
echo Build Type: %BUILD_TYPE%
echo Build Directory: %BUILD_DIR%
echo.
echo Next Steps for Xbox Deployment:
echo.
echo 1. Copy the built files to your Xbox:
echo    - Copy %BUILD_DIR%\gemrb\* to E:\GemRB\ on your Xbox
echo.
echo 2. Copy required data files:
echo    - Copy gemrb\GUIScripts\ to E:\GemRB\GUIScripts\
echo    - Copy gemrb\override\ to E:\GemRB\override\
echo    - Copy gemrb\unhardcoded\ to E:\GemRB\unhardcoded\
echo.
echo 3. Copy your game data:
echo    - Copy your original game files to E:\GemRB\[GameFolder]\
echo    - Example: E:\GemRB\BG1\, E:\GemRB\BG2\, etc.
echo.
echo 4. Create or modify GemRB.cfg with Xbox-specific settings:
echo    - See platforms\xbox\README.Xbox.md for configuration examples
echo.
echo 5. Launch GemRB.xbe from your Xbox dashboard or file manager
echo.
echo For detailed Xbox deployment instructions, see:
echo platforms\xbox\README.Xbox.md
echo.
echo ================================================================================
goto success_exit

:error_exit
echo.
echo ================================================================================
echo  BUILD FAILED
echo ================================================================================
echo.
echo The build process encountered an error. Please check the output above for
echo details and ensure all prerequisites are properly installed:
echo.
echo - NXDK (Xbox Development Kit) with toolchain
echo - CMake 3.25 or later
echo - MinGW/MSYS2 with make
echo - All required dependencies
echo.
echo For troubleshooting help, see:
echo - platforms\xbox\README.Xbox.md
echo - INSTALL file in the repository root
echo.
exit /b 1

:success_exit
echo [SUCCESS] GemRB Xbox build completed successfully!
echo.
echo Happy gaming on your Xbox! 🎮
echo.
exit /b 0