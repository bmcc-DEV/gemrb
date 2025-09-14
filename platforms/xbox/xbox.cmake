# Xbox platform configuration for NXDK toolchain
# Based on vita.cmake implementation

ADD_DEFINITIONS("-DXBOX")

# Xbox-specific compiler flags for NXDK with maximum performance optimizations
SET(CMAKE_DL_LIBS "")

# Core Xbox performance flags for Pentium III/Celeron CPU optimization
SET(XBOX_FLAGS "-march=pentium3 -msse -mfpmath=sse -O3 -fomit-frame-pointer -DNDEBUG")

# Advanced optimization flags to maximize Xbox capabilities
SET(XBOX_FLAGS "${XBOX_FLAGS} -funroll-loops -finline-functions -ffast-math")
SET(XBOX_FLAGS "${XBOX_FLAGS} -fno-exceptions -fno-rtti") # Reduce binary size and memory usage
SET(XBOX_FLAGS "${XBOX_FLAGS} -ffunction-sections -fdata-sections") # Enable dead code elimination

# Xbox-specific CPU optimizations
SET(XBOX_FLAGS "${XBOX_FLAGS} -mpreferred-stack-boundary=4") # Optimize for Xbox memory alignment
SET(XBOX_FLAGS "${XBOX_FLAGS} -fno-stack-protector") # Disable stack protection for performance
SET(XBOX_FLAGS "${XBOX_FLAGS} -mtune=pentium3") # Tune specifically for Xbox CPU

# Memory and cache optimization flags for 64MB limit
SET(XBOX_FLAGS "${XBOX_FLAGS} -fmerge-all-constants") # Merge identical constants
SET(XBOX_FLAGS "${XBOX_FLAGS} -fno-common") # Don't use common symbols
SET(XBOX_FLAGS "${XBOX_FLAGS} -falign-functions=16") # Align functions for better cache performance

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${XBOX_FLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${XBOX_FLAGS}")

# Enhanced Xbox linker optimizations for maximum performance
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections -Wl,--strip-all")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--sort-section=alignment") # Sort sections for better layout
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--relax") # Enable linker relaxation
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-O2") # Enable linker optimizations

# Xbox-specific library linking for NXDK
# These libraries are typically provided by NXDK
# Note: Actual library names may vary based on NXDK version
SET(XBOX_LIBRARIES_SDL_COMMON "-lSDL -lxboxkrnl -lhal -lusb")
SET(XBOX_LIBRARIES_SDL "${XBOX_LIBRARIES_SDL_COMMON}")

# Xbox filesystem paths - using standard Xbox drive mappings
SET(DEFAULT_CACHE_DIR "E:\\GemRB\\Cache2\\")
SET(DATA_DIR "E:\\GemRB\\")
SET(SYSCONF_DIR "E:\\GemRB")

include_directories(AFTER SYSTEM "${CMAKE_CURRENT_LIST_DIR}")

# Xbox uses SDL 1.2 with NXDK
IF (SDL_BACKEND STREQUAL "SDL")
	SET(CMAKE_CXX_STANDARD_LIBRARIES "${XBOX_LIBRARIES_SDL}")
ENDIF ()

# Force static linking for Xbox
SET(STATIC_LINK ON CACHE BOOL "Static linking required for Xbox" FORCE)

# Disable features that may not work well on Xbox's limited hardware
SET(USE_OPENAL OFF CACHE BOOL "OpenAL not available on Xbox" FORCE)
SET(USE_LIBVLC OFF CACHE BOOL "LibVLC not available on Xbox" FORCE)
SET(OPENGL_BACKEND "None" CACHE STRING "OpenGL not available on Xbox" FORCE)

# Enhanced Xbox memory optimizations for 64MB RAM limit
ADD_DEFINITIONS("-DXBOX_MEMORY_OPTIMIZED")
ADD_DEFINITIONS("-DXBOX_AUDIO_OPTIMIZED") # Enable Xbox-specific audio optimizations
ADD_DEFINITIONS("-DXBOX_REDUCE_MEMORY_USAGE") # Enable memory reduction features
ADD_DEFINITIONS("-DXBOX_GPU_OPTIMIZED") # Enable Xbox GPU optimizations
ADD_DEFINITIONS("-DXBOX_CONTROLLER_OPTIMIZED") # Enable enhanced controller features

# Advanced Xbox hardware feature support
ADD_DEFINITIONS("-DXBOX_ENHANCED_AUDIO") # Enable surround sound and DSP features
ADD_DEFINITIONS("-DXBOX_PROGRESSIVE_SCAN") # Enable 480p progressive scan support
ADD_DEFINITIONS("-DXBOX_HARDWARE_ACCELERATION") # Enable Xbox GPU acceleration
ADD_DEFINITIONS("-DXBOX_SMART_CACHING") # Enable intelligent file caching

# Xbox audio buffer optimizations - enhanced values for better performance
ADD_DEFINITIONS("-DXBOX_AUDIO_BUFFER_SIZE=8192") # Optimized buffer size
ADD_DEFINITIONS("-DXBOX_MUSIC_BUFFER_SIZE=4096") # Smaller music buffers
ADD_DEFINITIONS("-DXBOX_MAX_AUDIO_CHANNELS=16") # Limit simultaneous audio channels
ADD_DEFINITIONS("-DXBOX_DSP_BUFFER_SIZE=2048") # DSP processing buffer size

# Xbox display and graphics optimizations
ADD_DEFINITIONS("-DXBOX_DISPLAY_WIDTH=640") # Standard Xbox resolution
ADD_DEFINITIONS("-DXBOX_DISPLAY_HEIGHT=480") # Standard Xbox resolution
ADD_DEFINITIONS("-DXBOX_COLOR_DEPTH=16") # 16-bit color for better performance
ADD_DEFINITIONS("-DXBOX_TEXTURE_COMPRESSION") # Enable DXT texture compression

# When using NXDK toolchain, SDL detection should be handled by the toolchain
# This message helps users understand they need the proper NXDK environment
IF(NOT DEFINED ENV{NXDK_DIR} AND XBOX)
	MESSAGE(WARNING "Building for Xbox without NXDK_DIR environment variable set.")
	MESSAGE(WARNING "Make sure you're using the NXDK toolchain file:")
	MESSAGE(WARNING "  -DCMAKE_TOOLCHAIN_FILE=$NXDK_DIR/share/toolchain-nxdk.cmake")
ENDIF()

# Xbox-specific fix for Windows long path issues
# This ensures compatibility with deep directory structures often seen in Xbox development
IF(WIN32)
	SET(CMAKE_OBJECT_PATH_MAX 300 CACHE STRING "Maximum object file path length for Xbox builds")
	MESSAGE(STATUS "Xbox build: CMAKE_OBJECT_PATH_MAX set to ${CMAKE_OBJECT_PATH_MAX}")
ENDIF()