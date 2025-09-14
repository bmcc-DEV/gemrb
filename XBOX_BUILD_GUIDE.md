# GemRB Xbox Build Quick Start Guide

This guide provides a streamlined path to building GemRB for Xbox with enhanced error handling and validation.

## 🚀 Quick Start (New Users)

1. **Validate Environment** (Recommended first step):
   ```cmd
   validate_xbox_env.bat
   ```
   This checks your setup and provides specific solutions for any issues.

2. **Build GemRB**:
   ```cmd
   build_xbox.bat
   ```
   Comprehensive build with full validation and error handling.

## 📋 Prerequisites

### Required Software
- **NXDK (Xbox Development Kit)**: https://github.com/XboxDev/nxdk
- **CMake 3.25+**: https://cmake.org/download/
- **MinGW or MSYS2**: For make utility
- **Git**: Recommended for development

### Hardware
- Modded original Xbox console
- Xbox controller
- Game data from supported titles

## 🛠️ Build Scripts Overview

| Script | Purpose | Best For |
|--------|---------|----------|
| `validate_xbox_env.bat` | Environment validation | New users, troubleshooting |
| `build_xbox.bat` | Full build with validation | Most users |
| `quick_xbox_build.bat` | Minimal quick build | Experienced developers |
| `Build-Xbox.ps1` | PowerShell version | PowerShell users |

## 🔧 Build Options

### Standard Build
```cmd
build_xbox.bat
```

### Clean Build
```cmd
build_xbox.bat clean
```

### Debug Build
```cmd
build_xbox.bat debug
```

### PowerShell Build
```powershell
.\Build-Xbox.ps1
.\Build-Xbox.ps1 -Clean -Debug
```

## ⚠️ Common Issues & Solutions

### Environment Issues
**Problem**: "NXDK_DIR not set"
**Solution**: 
1. Install NXDK from https://github.com/XboxDev/nxdk
2. Set environment variable: `set NXDK_DIR=C:\path\to\nxdk`
3. Restart command prompt
4. Run `validate_xbox_env.bat` to verify

### Tool Issues
**Problem**: "CMake not found" or "Make not found"
**Solution**:
1. Install missing tools:
   - CMake: https://cmake.org/download/
   - MinGW/MSYS2: https://www.mingw-w64.org/ or https://www.msys2.org/
2. Add to system PATH
3. Restart command prompt

### Path Issues
**Problem**: "Path too long" errors
**Solution**:
1. Build from shorter directory (e.g., `C:\xbox\gemrb`)
2. Avoid spaces in directory names
3. System automatically handles paths up to 300 characters

## 📁 Directory Structure After Build

```
build-xbox/
├── gemrb/
│   ├── gemrb.exe          # Main executable
│   ├── gemrb.xbe          # Xbox executable (if NXDK configured)
│   └── plugins/           # GemRB plugins
└── ...
```

## 🎮 Xbox Deployment

1. **Copy built files to Xbox**:
   ```
   Copy build-xbox/gemrb/* to E:\GemRB\ on your Xbox
   ```

2. **Copy required data files**:
   ```
   Copy gemrb/GUIScripts/ to E:\GemRB\GUIScripts/
   Copy gemrb/override/ to E:\GemRB\override/
   Copy gemrb/unhardcoded/ to E:\GemRB\unhardcoded/
   ```

3. **Copy game data**:
   ```
   Copy your original game files to E:\GemRB/[GameFolder]/
   ```

4. **Configure GemRB.cfg**:
   See `platforms/xbox/README.Xbox.md` for detailed configuration examples.

## 🆘 Getting Help

### Built-in Validation
- Run `validate_xbox_env.bat` for comprehensive environment checking
- Build scripts provide detailed error messages with specific solutions

### Documentation
- **Comprehensive Xbox guide**: `platforms/xbox/README.Xbox.md`
- **Build scripts details**: `Xbox_Build_Scripts_README.md`
- **General installation**: `INSTALL` file

### Troubleshooting Steps
1. Run `validate_xbox_env.bat` to identify issues
2. Check error messages from build scripts (they provide specific solutions)
3. Verify NXDK installation and environment variables
4. Try building from a shorter directory path
5. Check NXDK documentation for compatibility issues

## ✨ New Features (2024)

- **Enhanced Environment Validation**: Comprehensive checking with specific solutions
- **Improved Error Messages**: Detailed troubleshooting guidance
- **Path Length Handling**: Automatic handling of Windows long path issues
- **Dependency Auto-Configuration**: Automatic Xbox-specific dependency setup
- **Multiple Build Scripts**: Options for different user preferences
- **NXDK Integration**: Better integration with NXDK toolchain

## 🎯 Success Indicators

When everything works correctly, you should see:
```
[SUCCESS] GemRB Xbox build completed successfully!

Happy gaming on your Xbox! 🎮
```

The build system provides clear feedback at each step, making it easy to identify and resolve any issues that arise.