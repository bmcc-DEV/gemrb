# GemRB port for Xbox (Original Xbox) using NXDK

## Overview

This port brings GemRB to the original Xbox console using the NXDK (Xbox Development Kit) homebrew toolchain. It allows you to play Baldur's Gate, Icewind Dale, and Planescape: Torment on your modded Xbox.

## Prerequisites

### Hardware Requirements
- Modded original Xbox console with homebrew capability
- Xbox hard drive with available space (at least 2GB recommended)
- Original Xbox controller
- Game data from supported titles

### Development Requirements
- NXDK (Xbox Development Kit) installed and configured
- Cross-compilation toolchain (i686-w64-mingw32)
- SDL 1.2 development libraries for NXDK
- Python 3.x development libraries (compiled for Xbox)

## Installation

### Setting up NXDK
1. Download and install NXDK from https://github.com/XboxDev/nxdk
2. Follow NXDK installation instructions for your platform
3. Ensure the NXDK environment variables are set correctly

### Installing game data
1. Copy your original game installation to `E:\GemRB\`
2. The structure should look like:
   ```
   E:\GemRB\
   ├── GemRB.cfg
   ├── GUIScripts\
   ├── override\
   ├── unhardcoded\
   └── [GameFolder]\  (e.g., BG1, BG2, IWD, etc.)
   ```

## Building

### Automated Build Scripts (Recommended)
**NEW: Automated Windows build scripts are now available to streamline the build process!**

For Windows users, you can use the provided automated build scripts located in the repository root:

- **`build_xbox.bat`** - Comprehensive batch script with full error checking and validation
- **`quick_xbox_build.bat`** - Simple quick build for experienced users  
- **`Build-Xbox.ps1`** - PowerShell version with enhanced features

**Quick Start:**
```cmd
# From the GemRB repository root:
build_xbox.bat              # Standard release build
build_xbox.bat clean        # Clean build
quick_xbox_build.bat        # Minimal output build
```

See `Xbox_Build_Scripts_README.md` in the repository root for detailed usage instructions.

### Manual Build Configuration
If you prefer to build manually or are using a non-Windows system:

```bash
mkdir build && cd build
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=$NXDK_DIR/share/toolchain-nxdk.cmake \
  -DSDL_BACKEND=SDL \
  -DSTATIC_LINK=ON \
  -DDISABLE_WERROR=ON \
  -DXBOX=ON \
  -DUSE_OPENAL=OFF \
  -DUSE_FREETYPE=OFF \
  -DUSE_LIBVLC=OFF \
  -DCMAKE_BUILD_TYPE=Release \
  -DXBOX_AUDIO_OPTIMIZATIONS=ON \
  -DXBOX_MEMORY_OPTIMIZATIONS=ON
make
```

### Enhanced Xbox Optimizations (NEW in 2024)
When building with `-DXBOX=ON`, the following advanced optimizations are automatically enabled:

**Performance Optimizations:**
- **CPU:** -O3 optimization, function inlining, loop unrolling, Pentium III-specific tuning
- **Memory:** Disabled exceptions and RTTI, dead code elimination, intelligent memory pooling
- **Audio:** Xbox DSP integration, surround sound support, optimized buffer management
- **Graphics:** Xbox GPU acceleration, texture compression, progressive scan support (480p)
- **Linking:** Static linking with advanced size and performance optimizations

**Xbox Hardware Integration:**
- **Controller Features:** Enhanced rumble feedback for game events, LED status indicators, precise analog control
- **Audio Hardware:** Xbox DSP effects, Dolby Digital 5.1 surround sound, Xbox soundtrack integration
- **Display Features:** 480p progressive scan, Xbox-optimized rendering pipeline, hardware acceleration
- **Storage Optimization:** Smart caching, Xbox hard drive optimization, efficient save game compression
- **System Integration:** Power management, Xbox dashboard integration, system LED control

### Installing to Xbox
1. Copy the built executable to your Xbox: `E:\GemRB\GemRB.xbe`
2. Copy required data files:
   - `GUIScripts\` folder
   - `override\` folder  
   - `unhardcoded\` folder
3. Create or modify `GemRB.cfg` with Xbox-specific settings

## Configuration

### Enhanced GemRB.cfg Settings for Xbox
```ini
# Enhanced video settings optimized for Xbox hardware
Width=640
Height=480
Bpp=16
Fullscreen=1
ProgressiveScan=1          # NEW: Enable 480p progressive scan
XboxGPUAcceleration=1      # NEW: Enable Xbox GPU acceleration

# Enhanced audio settings with Xbox hardware features
AudioDriver=sdlaudio
VolumeMusic=75
VolumeAmbients=75
XboxDSPEffects=1           # NEW: Enable Xbox DSP audio effects
XboxSurroundSound=1        # NEW: Enable 5.1 surround if available
XboxAudioOptimization=1    # Enhanced Xbox audio optimizations

# Xbox filesystem paths
CachePath=E:\GemRB\Cache2\
GemRB=E:\GemRB\
GamePath=E:\GemRB\[YourGameFolder]\

# Enhanced Xbox soundtrack integration
XboxSoundtrackPath=C:\TDATA\FFFE0000\music\
XboxUserSoundtrackPath=E:\UDATA\soundtrack\
EnableXboxSoundtracks=1
XboxSoundtrackPlaylist=1   # NEW: Enable playlist functionality

# Enhanced performance optimizations for Xbox
GamepadPointerSpeed=5
MaxPartySize=6
TooltipDelay=500
MemoryOptimizations=1      # Enhanced memory management
XboxSmartCaching=1         # NEW: Enable intelligent caching
XboxTextureCompression=1   # NEW: Enable DXT texture compression

# Enhanced Xbox controller features
XboxRumbleEnabled=1        # NEW: Enable haptic feedback
XboxControllerLEDs=1       # NEW: Enable controller LED feedback
XboxPrecisionMode=1        # NEW: Enable enhanced analog precision
XboxAnalogDeadzone=0.15    # NEW: Configure analog stick deadzone

# Xbox system integration features
XboxSystemIntegration=1    # NEW: Enable dashboard integration
XboxPowerManagement=1      # NEW: Enable smart power management
XboxProgressiveScan=1      # NEW: Enable 480p when available
XboxMemoryMonitoring=1     # NEW: Enable memory usage monitoring
```

## Controls

### Xbox Controller Enhancements (NEW)
GemRB now fully utilizes Xbox controller capabilities for an immersive gaming experience:

**Enhanced Rumble Feedback:**
- **Combat Events:** Rumble feedback for damage taken, critical hits, spell casting
- **Game Events:** Level up celebrations, inventory notifications, dialogue choices
- **Multi-Controller Support:** Support for up to 4 controllers with individual feedback
- **Intensity Scaling:** Different rumble patterns and intensities for various game events

**Advanced Input Features:**
- **Precision Control:** Enhanced analog stick precision for fine cursor movement
- **Trigger Pressure:** Full analog trigger support for variable input
- **Controller LEDs:** Visual feedback through controller LED patterns
- **Deadzone Control:** Customizable analog stick deadzones for optimal control

**Xbox Controller Mapping (Enhanced):**
- **Left analog stick:** Precision mouse cursor movement with enhanced sensitivity
- **A button:** Left mouse click (select/confirm) with haptic feedback
- **B button:** Right mouse click (context menu/cancel) with haptic feedback
- **X button:** Open inventory with notification rumble
- **Y button:** Open map with confirmation feedback
- **D-Pad:** Navigate interface with precise 8-directional movement
- **Right analog stick:** Alternative map scrolling with momentum
- **Left trigger:** Highlight objects with pressure-sensitive intensity
- **Right trigger:** Pause game with confirmation rumble
- **Back button:** Open main menu with system integration
- **Start button:** Escape/close current window with haptic confirmation

## Performance Considerations

### Enhanced Memory Management (NEW)
The original Xbox has only 64MB of RAM, so comprehensive memory optimizations are implemented:

**Intelligent Memory Management:**
- **Dynamic Memory Pools:** Efficient allocation and deallocation with automatic compaction
- **Smart Caching:** Intelligent caching of frequently used assets with LRU eviction
- **Memory Monitoring:** Real-time memory usage tracking with automatic optimization
- **Cache Optimization:** Separate optimized caches for textures, audio, and game data

**Xbox GPU Optimizations:**
- **Texture Compression:** Hardware DXT compression support for reduced memory usage
- **Progressive Scan:** 480p progressive scan support for enhanced visual quality
- **Hardware Acceleration:** Utilize Xbox GPU for 2D operations, sprite blitting, and alpha blending
- **Memory Layout:** Optimized memory layout for Xbox's 64-bit memory bus

**Storage and Caching:**
- **Smart File Caching:** Predictive loading of game assets based on usage patterns
- **Xbox Hard Drive Optimization:** Optimized for Xbox's IDE hard drive characteristics
- **Save Game Compression:** Efficient save game compression to minimize storage usage
- **Background Loading:** Asynchronous loading to reduce gameplay interruptions

**Performance Features:**
- Static linking to reduce memory overhead and improve load times
- Disabled Python site packages and user directories to save memory
- Optimized script memory allocation with garbage collection
- Reduced concurrent resource limits optimized for 64MB constraint

### Enhanced Audio Optimizations (NEW)
GemRB on Xbox now includes comprehensive audio optimizations that maximize Xbox's audio capabilities:

**Hardware Integration:**
- **Xbox DSP Integration:** Utilizes Xbox's Digital Signal Processor for enhanced audio effects
- **Dolby Digital 5.1 Support:** Full surround sound when connected to compatible audio systems
- **Xbox Audio Hardware:** Direct integration with Xbox's audio processing capabilities
- **Enhanced Soundtrack System:** Seamless integration with Xbox system soundtracks and user music

**Performance Optimizations:**
- Buffer size optimized from 16KB to 8KB for regular audio, 4KB for music
- Maximum concurrent sounds limited to 16 (optimized for Xbox's 64MB RAM)
- Audio frequency optimized to 22050Hz for better memory usage while maintaining quality
- Xbox-specific audio effects: echo, reverb, and spatial audio processing

**Features:**
- Automatic detection of Xbox soundtrack directories
- Support for both system soundtracks (`C:\TDATA\FFFE0000\music\`) and user-ripped music
- Dynamic audio quality adjustment based on available memory
- Hardware-accelerated audio mixing and effects processing

### Xbox System Integration Features (NEW)
GemRB now deeply integrates with Xbox system features for a console-like experience:

**Dashboard Integration:**
- **System Title Display:** GemRB appears properly in Xbox dashboard with custom title
- **Status Updates:** Real-time game status displayed in system interface
- **Power Management:** Automatic sleep prevention during gameplay, sleep allowed during idle
- **System LED Control:** Xbox front panel LED provides visual feedback for game states

**Enhanced Save System:**
- **Memory Card Support:** Full Xbox memory card detection and usage
- **Save Thumbnails:** Xbox dashboard-compatible save game thumbnails
- **Metadata Integration:** Rich save game information displayed in Xbox dashboard
- **Compression:** Optimized save compression for memory card storage efficiency

**Audio/Video System Integration:**
- **System Settings:** Automatically applies Xbox system audio and video preferences
- **Display Mode Detection:** Automatic detection and configuration of optimal display modes
- **Audio Output Detection:** Automatic configuration for stereo, surround, or digital output
- **Progressive Scan:** Automatic 480p progressive scan when supported by display

**Xbox Live Integration (for modded consoles):**
- **Network Awareness:** Detection of network connectivity for future online features
- **Clock Synchronization:** Integration with Xbox system clock for accurate timestamps
- **System Events:** Integration with Xbox system events and notifications

### Recommended Game Settings
- Use 16-bit color depth for better performance
- Keep party size reasonable (max 6 characters)
- Avoid heavy scripting mods
- Use lower resolution textures when available

### Storage Notes
- Game data should be stored on the Xbox hard drive (E:\ drive)
- Cache directory will be created automatically
- Save games are stored in the game folder

## Limitations

### Current Limitations
- No network play support
- Limited Python scripting capabilities due to memory constraints
- No movie playback (LibVLC not supported)
- No OpenAL audio (uses SDL audio only)
- No TrueType font support (FreeFree disabled)

### Compatibility Notes
- Tested primarily with Baldur's Gate series
- Large mods may not work due to memory constraints
- Some advanced scripting features may be disabled

## Troubleshooting

### Common Issues

#### Runtime Issues
1. **Game won't start**: Check that GemRB.cfg paths are correct for Xbox filesystem
2. **Memory errors**: Reduce party size and disable unnecessary features
3. **Audio problems**: Ensure AudioDriver is set to "sdlaudio"
4. **Controller not working**: Verify controller is connected before starting

#### Build Issues
1. **CMake error "object file directory has XXX characters"**: This occurs when build paths exceed Windows' 250-character limit
   - **Solution**: The repository now includes automatic path length handling
   - The `CMAKE_OBJECT_PATH_MAX` is automatically set to 300 for Xbox builds
   - Use shorter directory names when possible (e.g., `C:\xbox\gemrb` instead of deep folder structures)
   - If still encountering issues, try building from a shorter root path

2. **"NXDK_DIR not set" during CMake configuration**: Ensure NXDK environment is properly configured
   - Set `NXDK_DIR` environment variable to your NXDK installation path
   - Restart command prompt after setting environment variables
   - Use the provided build scripts which include environment validation

### Debug Information
Debug output is written to:
- Xbox debug console (visible with debugging tools)
- Log file at `E:\GemRB\gemrb.log`

## Building from Source

### Prerequisites
```bash
# Install NXDK
git clone https://github.com/XboxDev/nxdk.git
cd nxdk
make

# Set environment variables
export NXDK_DIR=/path/to/nxdk
export PATH=$NXDK_DIR/bin:$PATH
```

### Compilation
```bash
# Clone GemRB with Xbox support
git clone https://github.com/gemrb/gemrb.git
cd gemrb

# Configure for Xbox
mkdir build-xbox && cd build-xbox
cmake .. -DCMAKE_TOOLCHAIN_FILE=$NXDK_DIR/share/toolchain-nxdk.cmake \
         -DXBOX=ON -DSTATIC_LINK=ON -DSDL_BACKEND=SDL

# Build
make -j$(nproc)
```

## Credits

- GemRB development team for the original engine
- Xbox homebrew community for NXDK
- SDL developers for Xbox SDL support

## Support

For Xbox-specific issues, please:
1. Check this documentation first
2. Search existing GemRB issues
3. Create a new issue with "[Xbox]" prefix if needed

Remember that this is homebrew software - ensure your Xbox console is properly modified and you have legal backups of your games.