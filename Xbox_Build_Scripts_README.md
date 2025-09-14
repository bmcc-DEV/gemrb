# Xbox Build Scripts for GemRB

This directory contains automated build scripts to streamline the compilation of GemRB for the original Xbox console (.xbe format).

## Scripts Overview

### 1. `build_xbox.bat` - Comprehensive Windows Batch Script
**Recommended for most users**

A full-featured batch script with extensive error checking, environment validation, and detailed output.

**Features:**
- Complete environment validation (NXDK, CMake, Make)
- Multiple build modes (Release, Debug)
- Clean build option
- Parallel compilation support
- Detailed post-build deployment instructions
- Comprehensive error handling

**Usage:**
```cmd
build_xbox.bat [clean|release|debug|quick]
```

**Examples:**
```cmd
build_xbox.bat                 # Standard release build
build_xbox.bat clean           # Clean build directory first
build_xbox.bat debug           # Build debug version
build_xbox.bat quick           # Skip environment checks
```

### 2. `quick_xbox_build.bat` - Simple Quick Build
**For experienced users who want minimal output**

A streamlined script that assumes your environment is properly configured.

**Usage:**
```cmd
quick_xbox_build.bat
```

### 3. `Build-Xbox.ps1` - PowerShell Script
**For users who prefer PowerShell**

A PowerShell version with enhanced error handling and colored output.

**Usage:**
```powershell
.\Build-Xbox.ps1 [-Clean] [-Debug] [-Quick] [-Jobs <number>]
```

**Examples:**
```powershell
.\Build-Xbox.ps1                    # Standard release build
.\Build-Xbox.ps1 -Clean             # Clean build first
.\Build-Xbox.ps1 -Debug             # Debug build
.\Build-Xbox.ps1 -Jobs 4            # Use 4 parallel jobs
.\Build-Xbox.ps1 -Clean -Debug      # Clean debug build
```

## Prerequisites

Before using any of these scripts, ensure you have:

1. **NXDK (Xbox Development Kit)**
   - Download from: https://github.com/XboxDev/nxdk
   - Set `NXDK_DIR` environment variable
   - Example: `set NXDK_DIR=C:\nxdk`

2. **CMake 3.25 or later**
   - Download from: https://cmake.org/download/

3. **MinGW or MSYS2**
   - For the `make` command
   - Ensure `make` is in your PATH

4. **Git** (optional)
   - For repository operations

## Build Output

After a successful build, you'll find:
- **Build directory**: `build-xbox/`
- **Executable**: `build-xbox/gemrb/gemrb.exe`
- **Xbox executable**: `build-xbox/gemrb/gemrb.xbe` (if NXDK is properly configured)

## Deployment to Xbox

After building, follow these steps:

1. **Copy built files to Xbox:**
   ```
   Copy build-xbox/gemrb/* to E:\GemRB\ on your Xbox
   ```

2. **Copy required data files:**
   ```
   Copy gemrb/GUIScripts/ to E:\GemRB\GUIScripts/
   Copy gemrb/override/ to E:\GemRB\override/
   Copy gemrb/unhardcoded/ to E:\GemRB\unhardcoded/
   ```

3. **Copy game data:**
   ```
   Copy your original game files to E:\GemRB/[GameFolder]/
   Example: E:\GemRB\BG1\, E:\GemRB\BG2\, etc.
   ```

4. **Configure GemRB.cfg:**
   - See `platforms/xbox/README.Xbox.md` for detailed configuration

## Troubleshooting

### Common Issues

1. **"NXDK_DIR not set" error**
   - Install NXDK and set the environment variable
   - Restart your command prompt after setting

2. **"CMake not found" error**
   - Install CMake and add it to your PATH
   - Or specify full path to cmake.exe

3. **"Make not found" error**
   - Install MinGW/MSYS2 or add existing installation to PATH

4. **Build fails with linking errors**
   - Ensure NXDK is properly installed and configured
   - Check that you're using the correct NXDK version

### Getting Help

For Xbox-specific issues:
- Check `platforms/xbox/README.Xbox.md`
- Review the comprehensive Xbox documentation
- Search existing GemRB issues on GitHub

For general build issues:
- Check the main `INSTALL` file
- Review CMake configuration in the root `CMakeLists.txt`

## Build Optimization Settings

These scripts automatically apply Xbox-specific optimizations:

- **Performance**: `-O3` optimization, function inlining, loop unrolling
- **Memory**: Disabled exceptions and RTTI, dead code elimination  
- **Audio**: Optimized buffer sizes and channel limits
- **Linking**: Static linking with size optimization
- **Xbox-specific**: Memory optimizations for 64MB RAM limit

## Script Customization

You can modify the scripts to suit your needs:

- **Change build type**: Modify `BUILD_TYPE` variable
- **Add custom CMake options**: Add to the cmake command line
- **Change parallel jobs**: Modify the `-j` parameter for make
- **Custom build directory**: Change `BUILD_DIR` variable

## Success Indicator

When the build completes successfully, you'll see:

```
[SUCCESS] GemRB Xbox build completed successfully!

Happy gaming on your Xbox! 🎮
```

The scripts will also provide detailed deployment instructions and next steps.

---

**Note**: These scripts are designed to work with GemRB versions that include Xbox support via NXDK. Ensure you're using a compatible version of GemRB that includes the Xbox platform files.