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
    echo [ERROR] NXDK_DIR environment variable not set!
    echo [ERROR] Please install NXDK and set NXDK_DIR to the installation path.
    echo [ERROR] Example: set NXDK_DIR=C:\nxdk
    goto error_exit
)

if not exist "%NXDK_DIR%" (
    echo [ERROR] NXDK directory not found: %NXDK_DIR%
    echo [ERROR] Please verify NXDK installation path.
    goto error_exit
)

echo [OK] NXDK found at: %NXDK_DIR%

REM Check for NXDK toolchain file
if not exist "%NXDK_DIR%\share\toolchain-nxdk.cmake" (
    echo [ERROR] NXDK CMake toolchain file not found!
    echo [ERROR] Expected: %NXDK_DIR%\share\toolchain-nxdk.cmake
    goto error_exit
)

echo [OK] NXDK CMake toolchain found

REM Check for CMake
cmake --version >nul 2>&1
if errorlevel 1 (
    echo [ERROR] CMake not found in PATH!
    echo [ERROR] Please install CMake 3.25 or later.
    goto error_exit
)

echo [OK] CMake is available

REM Check for Make
make --version >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Make not found in PATH!
    echo [ERROR] Please install MinGW, MSYS2, or ensure make is in PATH.
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
    echo [ERROR] CMake configuration failed!
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
    echo [ERROR] Build failed!
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