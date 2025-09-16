# Xbox Platform Support for GemRB

This directory contains Xbox-specific implementations for the GemRB Infinity Engine.

## Features Implemented

### 1. Controller Integration
- **Full controller mapping**: All buttons, analog sticks, triggers, and D-pad
- **Xbox-specific rumble support**: Haptic feedback for game events
- **Deadzone handling**: Configurable analog stick deadzones
- **Connection detection**: Hot-plug controller support
- **Enhanced input processing**: Optimized for Xbox controller layout

### 2. Audio Format Support
- **XMA format support**: Native Xbox audio compression
- **ADPCM decoding**: Microsoft ADPCM format support
- **Audio memory management**: Optimized for 64MB memory constraint
- **Custom soundtrack**: Integration with Xbox music features
- **Audio quality settings**: Adjustable for performance

### 3. Memory Management
- **64MB constraint handling**: Smart memory allocation and cleanup
- **Automatic resource cleanup**: LRU-based texture and sound disposal
- **Memory monitoring**: Real-time usage tracking
- **Cache optimization**: Configurable cache sizes for different resource types
- **Memory defragmentation**: Periodic cleanup to prevent fragmentation

### 4. Performance Optimization
- **64 FPS target**: Consistent frame rate optimization
- **Visual effects scaling**: Reduced particles and effects for performance
- **Preloading system**: Critical resource preloading
- **Streaming support**: Large file streaming to reduce memory usage

### 5. Display Optimization for CRT TVs
- **Safe zone support**: Overscan compensation for CRT displays
- **Font size optimization**: Increased font sizes for TV viewing
- **UI scaling**: Enhanced UI elements for living room gaming
- **Controller cursor**: Analog stick-driven cursor system

### 6. Xbox System Integration
- **Dashboard integration**: Safe return to Xbox dashboard
- **Screensaver control**: Disable during gameplay
- **Partition access**: Support for C:, E:, F:, and D: drives
- **LED and rumble control**: Controller feedback features

### 7. User Interface Enhancements
- **Loading screens**: Progress indicators and feedback
- **Error handling**: Clear error messages and recovery
- **Splash screens**: Xbox-optimized startup sequence
- **Button prompts**: Xbox controller button indicators

## Files Overview

- `Xbox.h/cpp`: Main Xbox platform abstraction layer
- `XboxController.h/cpp`: Enhanced controller support with Xbox features
- `XboxAudio.h/cpp`: Xbox audio format support and optimization
- `XboxMemory.h/cpp`: Memory management for 64MB constraint
- `XboxUI.h/cpp`: UI optimizations for CRT TVs and controller input
- `XboxAudioReader.h`: Audio format plugins for XMA and ADPCM
- `xbox.cfg`: Configuration file with Xbox-specific settings
- `CMakeLists.txt`: Build configuration for Xbox platform

## Building for Xbox

When building for Xbox, define `_XBOX` to enable Xbox-specific features:

```cmake
SET(CMAKE_SYSTEM_NAME "Xbox")
ADD_DEFINITIONS(-D_XBOX)
```

## Configuration

Copy `xbox.cfg` to your GemRB configuration directory and adjust settings:

- Memory thresholds for cleanup
- Performance optimization levels
- Controller sensitivity and deadzone
- Audio quality settings
- Display optimizations for CRT

## Memory Usage Guidelines

The Xbox has only 64MB of RAM, so resource management is critical:

- Textures: 16MB cache (default)
- Sounds: 8MB cache (default)
- Animations: 4MB cache (default)
- Scripts: 2MB cache (default)

These can be adjusted in the configuration file based on your game's requirements.

## Performance Targets

- **Frame Rate**: 64 FPS stable
- **Memory Usage**: <60MB (leaving 4MB for system)
- **Load Times**: <30 seconds for large areas
- **Controller Response**: <16ms input latency

## Xbox-Specific Features

### Controller Rumble
Rumble is triggered for various game events:
- Damage taken (left motor)
- Successful hits (right motor)
- Spell casting (both motors)
- UI feedback (light rumble)

### Audio Enhancements
- XMA format provides better compression than OGG on Xbox
- ADPCM support for legacy audio files
- Custom soundtrack integration with Xbox music library

### Memory Optimization
- Automatic texture compression for Xbox hardware
- Aggressive cleanup when memory is low
- Smart caching based on usage patterns

## Known Limitations

- XMA decoding requires Xbox XAudio2 libraries
- Some advanced OpenGL features may not be available
- Memory constraints may limit simultaneous sound effects
- Save file size limitations due to storage constraints

## Future Enhancements

Planned improvements include:
- Enhanced Xbox Live integration
- Advanced controller features (if available)
- Improved audio streaming
- Better memory defragmentation
- Additional visual optimizations

For more information, see the main GemRB documentation.