@echo off
REM ================================================================================
REM Xbox Environment Validation Script for GemRB
REM ================================================================================
REM
REM This script validates that your environment is properly configured for
REM Xbox development with NXDK before attempting to build GemRB.
REM
REM Usage: validate_xbox_env.bat
REM
REM ================================================================================

echo.
echo ================================================================================
echo  Xbox Development Environment Validation
echo ================================================================================
echo.

set ERROR_COUNT=0

echo [INFO] Checking Xbox development environment...
echo.

REM Check NXDK_DIR environment variable
echo [CHECK] NXDK_DIR environment variable...
if not defined NXDK_DIR (
    echo [FAIL] NXDK_DIR is not set
    echo        Please set NXDK_DIR to your NXDK installation path
    echo        Example: set NXDK_DIR=C:\nxdk
    set /a ERROR_COUNT+=1
) else (
    echo [PASS] NXDK_DIR = %NXDK_DIR%
)

REM Check NXDK directory exists
echo [CHECK] NXDK installation directory...
if not exist "%NXDK_DIR%" (
    echo [FAIL] NXDK directory not found: %NXDK_DIR%
    echo        Please verify NXDK installation path
    set /a ERROR_COUNT+=1
) else (
    echo [PASS] NXDK directory exists
)

REM Check NXDK toolchain file
echo [CHECK] NXDK CMake toolchain file...
if not exist "%NXDK_DIR%\share\toolchain-nxdk.cmake" (
    echo [FAIL] NXDK CMake toolchain file not found
    echo        Expected: %NXDK_DIR%\share\toolchain-nxdk.cmake
    echo        This indicates incomplete NXDK installation
    set /a ERROR_COUNT+=1
) else (
    echo [PASS] NXDK CMake toolchain file found
)

REM Check CMake
echo [CHECK] CMake availability and version...
cmake --version >nul 2>&1
if errorlevel 1 (
    echo [FAIL] CMake not found in PATH
    echo        Please install CMake 3.25 or later from cmake.org
    set /a ERROR_COUNT+=1
) else (
    echo [PASS] CMake is available
    for /f "tokens=3" %%i in ('cmake --version ^| findstr "cmake version"') do (
        echo        Version: %%i
        REM Basic version check - we'll just report it since version parsing in batch is complex
        echo        Requirement: 3.25 or later - please verify if build fails
    )
)

REM Check Make
echo [CHECK] Make utility availability...
make --version >nul 2>&1
if errorlevel 1 (
    echo [FAIL] Make not found in PATH
    echo        Please install MinGW/MSYS2 or ensure make.exe is in PATH
    set /a ERROR_COUNT+=1
) else (
    echo [PASS] Make is available
    make --version | head -1
)

REM Check Git (optional but recommended)
echo [CHECK] Git availability (optional)...
git --version >nul 2>&1
if errorlevel 1 (
    echo [WARN] Git not found in PATH
    echo        Git is recommended for development but not required
) else (
    echo [PASS] Git is available
    git --version
)

REM Check for compiler (when NXDK is available)
echo [CHECK] Cross-compiler availability (via NXDK)...
if defined NXDK_DIR (
    if exist "%NXDK_DIR%\bin" (
        echo [PASS] NXDK compiler tools directory found
        echo        Location: %NXDK_DIR%\bin
    ) else (
        echo [WARN] NXDK compiler tools directory not found
        echo        Expected: %NXDK_DIR%\bin
        echo        This may indicate incomplete NXDK installation
    )
) else (
    echo [SKIP] Skipping compiler check (NXDK_DIR not set)
)

REM Check GemRB repository
echo [CHECK] GemRB repository structure...
if not exist "CMakeLists.txt" (
    echo [FAIL] CMakeLists.txt not found in current directory
    echo        Please run this script from the GemRB repository root
    set /a ERROR_COUNT+=1
) else (
    echo [PASS] GemRB repository root detected
)

echo [CHECK] Xbox platform files...
if not exist "platforms\xbox\xbox.cmake" (
    echo [FAIL] Xbox platform files not found
    echo        Please ensure you're using GemRB version with Xbox support
    set /a ERROR_COUNT+=1
) else (
    echo [PASS] Xbox platform files found
)

echo.
echo ================================================================================
echo  Validation Summary
echo ================================================================================
echo.

if %ERROR_COUNT%==0 (
    echo [SUCCESS] All checks passed! Your environment is ready for Xbox development.
    echo.
    echo Next steps:
    echo 1. Run: build_xbox.bat              (comprehensive build with full validation)
    echo 2. OR:   quick_xbox_build.bat       (quick build for experienced users)
    echo 3. OR:   .\Build-Xbox.ps1           (PowerShell version)
    echo.
    echo For detailed build instructions, see:
    echo - Xbox_Build_Scripts_README.md
    echo - platforms\xbox\README.Xbox.md
    echo.
) else (
    echo [FAILED] %ERROR_COUNT% issue(s) found that must be resolved before building.
    echo.
    echo Solutions:
    echo.
    echo 1. Install NXDK:
    echo    - Download from: https://github.com/XboxDev/nxdk
    echo    - Follow installation instructions
    echo    - Set NXDK_DIR environment variable
    echo.
    echo 2. Install required tools:
    echo    - CMake 3.25+ from: https://cmake.org/download/
    echo    - MinGW or MSYS2 for make utility
    echo.
    echo 3. Restart command prompt after installing/configuring tools
    echo.
    echo 4. Re-run this validation script to verify fixes
    echo.
    echo For detailed setup instructions, see:
    echo - Xbox_Build_Scripts_README.md
    echo - platforms\xbox\README.Xbox.md
    echo.
)

echo Press any key to exit...
pause >nul
exit /b %ERROR_COUNT%