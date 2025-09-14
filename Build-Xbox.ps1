# ================================================================================
# GemRB Xbox (.xbe) Build Script - PowerShell Version
# ================================================================================
#
# PowerShell script to automate GemRB compilation for original Xbox using NXDK.
# Offers more robust error handling and better user experience than batch files.
#
# Prerequisites:
#   - NXDK (Xbox Development Kit) installed and configured
#   - CMake 3.25 or later
#   - MinGW or MSYS2 with make
#
# Usage:
#   .\Build-Xbox.ps1 [-Clean] [-Debug] [-Quick] [-Jobs <number>]
#
# Parameters:
#   -Clean  : Clean build directory before building
#   -Debug  : Build debug version instead of release
#   -Quick  : Skip environment validation
#   -Jobs   : Number of parallel build jobs (default: CPU count)
#
# ================================================================================

param(
    [switch]$Clean,
    [switch]$Debug,
    [switch]$Quick,
    [int]$Jobs = $env:NUMBER_OF_PROCESSORS
)

# Configuration
$BuildType = if ($Debug) { "Debug" } else { "Release" }
$BuildDir = "build-xbox"
$MaxJobs = 8

# Limit parallel jobs to reasonable number
if ($Jobs -gt $MaxJobs) { $Jobs = $MaxJobs }
if ($Jobs -lt 1) { $Jobs = 1 }

Write-Host ""
Write-Host "================================================================================" -ForegroundColor Cyan
Write-Host " GemRB Xbox (.xbe) Build Script - PowerShell" -ForegroundColor Cyan
Write-Host "================================================================================" -ForegroundColor Cyan
Write-Host ""

Write-Host "[INFO] Build Configuration:" -ForegroundColor Green
Write-Host "       Build Type: $BuildType"
Write-Host "       Build Directory: $BuildDir"
Write-Host "       Clean Build: $Clean"
Write-Host "       Parallel Jobs: $Jobs"
Write-Host ""

# Environment validation (skip if quick build)
if (-not $Quick) {
    Write-Host "[INFO] Validating build environment..." -ForegroundColor Yellow
    
    # Check NXDK
    if (-not $env:NXDK_DIR) {
        Write-Host "[ERROR] NXDK_DIR environment variable not set!" -ForegroundColor Red
        Write-Host "[ERROR] Please install NXDK and set NXDK_DIR environment variable." -ForegroundColor Red
        exit 1
    }
    
    if (-not (Test-Path $env:NXDK_DIR)) {
        Write-Host "[ERROR] NXDK directory not found: $env:NXDK_DIR" -ForegroundColor Red
        exit 1
    }
    
    Write-Host "[OK] NXDK found at: $env:NXDK_DIR" -ForegroundColor Green
    
    # Check NXDK toolchain
    $ToolchainPath = Join-Path $env:NXDK_DIR "share\toolchain-nxdk.cmake"
    if (-not (Test-Path $ToolchainPath)) {
        Write-Host "[ERROR] NXDK CMake toolchain file not found!" -ForegroundColor Red
        Write-Host "[ERROR] Expected: $ToolchainPath" -ForegroundColor Red
        exit 1
    }
    
    Write-Host "[OK] NXDK CMake toolchain found" -ForegroundColor Green
    
    # Check CMake
    try {
        $null = Get-Command cmake -ErrorAction Stop
        Write-Host "[OK] CMake is available" -ForegroundColor Green
    } catch {
        Write-Host "[ERROR] CMake not found in PATH!" -ForegroundColor Red
        Write-Host "[ERROR] Please install CMake 3.25 or later." -ForegroundColor Red
        exit 1
    }
    
    # Check Make
    try {
        $null = Get-Command make -ErrorAction Stop
        Write-Host "[OK] Make is available" -ForegroundColor Green
    } catch {
        Write-Host "[ERROR] Make not found in PATH!" -ForegroundColor Red
        Write-Host "[ERROR] Please install MinGW, MSYS2, or ensure make is in PATH." -ForegroundColor Red
        exit 1
    }
    
    # Check repository
    if (-not (Test-Path "CMakeLists.txt")) {
        Write-Host "[ERROR] CMakeLists.txt not found in current directory!" -ForegroundColor Red
        Write-Host "[ERROR] Please run this script from the GemRB repository root." -ForegroundColor Red
        exit 1
    }
    
    if (-not (Test-Path "platforms\xbox\xbox.cmake")) {
        Write-Host "[ERROR] Xbox platform files not found!" -ForegroundColor Red
        Write-Host "[ERROR] Please ensure you're using a GemRB version with Xbox support." -ForegroundColor Red
        exit 1
    }
    
    Write-Host "[OK] GemRB repository and Xbox platform files found" -ForegroundColor Green
    Write-Host ""
}

# Setup build directory
if ($Clean -and (Test-Path $BuildDir)) {
    Write-Host "[INFO] Cleaning build directory..." -ForegroundColor Yellow
    Remove-Item $BuildDir -Recurse -Force
    Write-Host "[OK] Build directory cleaned" -ForegroundColor Green
}

if (-not (Test-Path $BuildDir)) {
    Write-Host "[INFO] Creating build directory..." -ForegroundColor Yellow
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

# Configure CMake
Write-Host "[INFO] Configuring CMake for Xbox build..." -ForegroundColor Yellow

$ToolchainFile = Join-Path $env:NXDK_DIR "share\toolchain-nxdk.cmake"

Push-Location $BuildDir

$CMakeArgs = @(
    "..",
    "-DCMAKE_TOOLCHAIN_FILE=$ToolchainFile",
    "-DCMAKE_BUILD_TYPE=$BuildType",
    "-DXBOX=ON",
    "-DSTATIC_LINK=ON",
    "-DSDL_BACKEND=SDL",
    "-DDISABLE_WERROR=ON",
    "-DUSE_OPENAL=OFF",
    "-DUSE_FREETYPE=OFF",
    "-DUSE_LIBVLC=OFF",
    "-DUSE_SDLMIXER=ON",
    "-DXBOX_AUDIO_OPTIMIZATIONS=ON",
    "-DXBOX_MEMORY_OPTIMIZATIONS=ON",
    "-DBUILD_TESTING=OFF",
    "-DSKIP_DEMO_DATA=OFF",
    "-G `"MinGW Makefiles`""
)

try {
    & cmake $CMakeArgs
    if ($LASTEXITCODE -ne 0) {
        throw "CMake configuration failed with exit code $LASTEXITCODE"
    }
    Write-Host "[OK] CMake configuration completed successfully" -ForegroundColor Green
} catch {
    Write-Host "[ERROR] CMake configuration failed: $_" -ForegroundColor Red
    Pop-Location
    exit 1
}

# Build
Write-Host ""
Write-Host "[INFO] Building GemRB for Xbox..." -ForegroundColor Yellow
Write-Host "[INFO] Using $Jobs parallel jobs..." -ForegroundColor Yellow
Write-Host "[INFO] This may take several minutes..." -ForegroundColor Yellow
Write-Host ""

try {
    & make "-j$Jobs"
    if ($LASTEXITCODE -ne 0) {
        throw "Build failed with exit code $LASTEXITCODE"
    }
    Write-Host ""
    Write-Host "[OK] Build completed successfully!" -ForegroundColor Green
} catch {
    Write-Host "[ERROR] Build failed: $_" -ForegroundColor Red
    Pop-Location
    exit 1
}

Pop-Location

# Post-build operations
Write-Host "[INFO] Post-build operations..." -ForegroundColor Yellow

# Check for executable
$ExePath = Join-Path $BuildDir "gemrb\gemrb.exe"
if (Test-Path $ExePath) {
    Write-Host "[OK] GemRB executable created: $ExePath" -ForegroundColor Green
} else {
    Write-Host "[WARNING] GemRB executable not found at expected location" -ForegroundColor Yellow
}

# Look for .xbe file
$XbeFiles = Get-ChildItem -Path $BuildDir -Filter "*.xbe" -Recurse
if ($XbeFiles) {
    foreach ($xbe in $XbeFiles) {
        Write-Host "[OK] Xbox executable found: $($xbe.FullName)" -ForegroundColor Green
    }
} else {
    Write-Host "[INFO] .xbe file not found - this may be normal depending on NXDK configuration" -ForegroundColor Yellow
}

# Display success message and deployment instructions
Write-Host ""
Write-Host "================================================================================" -ForegroundColor Cyan
Write-Host " Build Summary" -ForegroundColor Cyan
Write-Host "================================================================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Build Type: $BuildType" -ForegroundColor White
Write-Host "Build Directory: $BuildDir" -ForegroundColor White
Write-Host ""
Write-Host "Next Steps for Xbox Deployment:" -ForegroundColor Yellow
Write-Host ""
Write-Host "1. Copy the built files to your Xbox:" -ForegroundColor White
Write-Host "   - Copy $BuildDir\gemrb\* to E:\GemRB\ on your Xbox" -ForegroundColor Gray
Write-Host ""
Write-Host "2. Copy required data files:" -ForegroundColor White
Write-Host "   - Copy gemrb\GUIScripts\ to E:\GemRB\GUIScripts\" -ForegroundColor Gray
Write-Host "   - Copy gemrb\override\ to E:\GemRB\override\" -ForegroundColor Gray
Write-Host "   - Copy gemrb\unhardcoded\ to E:\GemRB\unhardcoded\" -ForegroundColor Gray
Write-Host ""
Write-Host "3. Copy your game data:" -ForegroundColor White
Write-Host "   - Copy your original game files to E:\GemRB\[GameFolder]\" -ForegroundColor Gray
Write-Host "   - Example: E:\GemRB\BG1\, E:\GemRB\BG2\, etc." -ForegroundColor Gray
Write-Host ""
Write-Host "4. Create or modify GemRB.cfg with Xbox-specific settings:" -ForegroundColor White
Write-Host "   - See platforms\xbox\README.Xbox.md for configuration examples" -ForegroundColor Gray
Write-Host ""
Write-Host "For detailed Xbox deployment instructions, see:" -ForegroundColor White
Write-Host "platforms\xbox\README.Xbox.md" -ForegroundColor Gray
Write-Host ""
Write-Host "================================================================================" -ForegroundColor Cyan
Write-Host "[SUCCESS] GemRB Xbox build completed successfully! 🎮" -ForegroundColor Green
Write-Host "================================================================================" -ForegroundColor Cyan
Write-Host ""