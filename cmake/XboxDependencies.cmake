# Xbox-specific dependency configuration for GemRB
# This module handles Xbox platform-specific dependencies and configurations

# Ensure this is only included for Xbox builds
IF(NOT XBOX)
    RETURN()
ENDIF()

MESSAGE(STATUS "Configuring Xbox-specific dependencies...")

# Validate NXDK environment before proceeding
IF(NOT DEFINED ENV{NXDK_DIR})
    MESSAGE(FATAL_ERROR 
        "Xbox build requires NXDK environment to be configured!\n"
        "Missing NXDK_DIR environment variable.\n"
        "\n"
        "Quick fix:\n"
        "1. Install NXDK from: https://github.com/XboxDev/nxdk\n"
        "2. Set environment variable: set NXDK_DIR=C:\\path\\to\\nxdk\n"
        "3. Use NXDK toolchain: -DCMAKE_TOOLCHAIN_FILE=$NXDK_DIR/share/toolchain-nxdk.cmake\n"
        "\n"
        "For automated setup, use the provided build scripts:\n"
        "- build_xbox.bat (comprehensive)\n"
        "- validate_xbox_env.bat (environment check)"
    )
ENDIF()

# Xbox builds have specific dependency requirements
MESSAGE(STATUS "Xbox build: Configuring platform-specific dependencies")

# Validate that this is actually an Xbox build with proper toolchain
IF(NOT CMAKE_SYSTEM_NAME STREQUAL "Xbox" AND NOT CMAKE_TOOLCHAIN_FILE MATCHES "nxdk")
    MESSAGE(WARNING 
        "Xbox build flag (-DXBOX=ON) is set but system configuration suggests this is not an Xbox build.\n"
        "System: ${CMAKE_SYSTEM_NAME}\n"
        "Toolchain: ${CMAKE_TOOLCHAIN_FILE}\n"
        "\n"
        "For proper Xbox builds, ensure you're using:\n"
        "-DCMAKE_TOOLCHAIN_FILE=$ENV{NXDK_DIR}/share/toolchain-nxdk.cmake\n"
        "\n"
        "Use the provided build scripts for automatic setup."
    )
ENDIF()

# Force Xbox-appropriate settings for dependencies that don't work on Xbox
IF(USE_OPENAL)
    MESSAGE(STATUS "Xbox build: Disabling OpenAL (not supported on Xbox)")
    SET(USE_OPENAL OFF CACHE BOOL "OpenAL not supported on Xbox" FORCE)
ENDIF()

IF(USE_LIBVLC) 
    MESSAGE(STATUS "Xbox build: Disabling LibVLC (not supported on Xbox)")
    SET(USE_LIBVLC OFF CACHE BOOL "LibVLC not supported on Xbox" FORCE)
ENDIF()

IF(USE_FREETYPE)
    MESSAGE(STATUS "Xbox build: Disabling FreeType (not supported on Xbox)")
    SET(USE_FREETYPE OFF CACHE BOOL "FreeType not supported on Xbox" FORCE)
ENDIF()

# Ensure static linking for Xbox
IF(NOT STATIC_LINK)
    MESSAGE(STATUS "Xbox build: Enabling static linking (required for Xbox)")
    SET(STATIC_LINK ON CACHE BOOL "Static linking required for Xbox" FORCE)
ENDIF()

# Set appropriate SDL backend for Xbox
IF(NOT SDL_BACKEND STREQUAL "SDL")
    MESSAGE(STATUS "Xbox build: Setting SDL backend to SDL (required for Xbox/NXDK)")
    SET(SDL_BACKEND "SDL" CACHE STRING "SDL backend for Xbox" FORCE)
ENDIF()

# Xbox-specific optimizations
ADD_DEFINITIONS("-DXBOX_BUILD=1")
ADD_DEFINITIONS("-DXBOX_OPTIMIZED_BUILD=1")

# Xbox filesystem and path configurations
SET(XBOX_CACHE_PATH "E:\\GemRB\\Cache2\\")
SET(XBOX_DATA_PATH "E:\\GemRB\\")
SET(XBOX_CONFIG_PATH "E:\\GemRB\\")

# Override default paths for Xbox
SET(DEFAULT_CACHE_DIR "${XBOX_CACHE_PATH}" CACHE STRING "Xbox cache directory" FORCE)
SET(DATA_DIR "${XBOX_DATA_PATH}" CACHE STRING "Xbox data directory" FORCE)
SET(SYSCONF_DIR "${XBOX_CONFIG_PATH}" CACHE STRING "Xbox config directory" FORCE)

# Xbox memory optimization flags (64MB RAM limit)
ADD_DEFINITIONS("-DXBOX_MEMORY_LIMIT=67108864") # 64MB in bytes
ADD_DEFINITIONS("-DXBOX_REDUCED_MEMORY_USAGE=1")

# Xbox audio optimizations
ADD_DEFINITIONS("-DXBOX_AUDIO_BUFFER_SIZE=8192")
ADD_DEFINITIONS("-DXBOX_MUSIC_BUFFER_SIZE=4096") 
ADD_DEFINITIONS("-DXBOX_MAX_CONCURRENT_SOUNDS=16")

# Xbox video optimizations
ADD_DEFINITIONS("-DXBOX_DEFAULT_WIDTH=640")
ADD_DEFINITIONS("-DXBOX_DEFAULT_HEIGHT=480")
ADD_DEFINITIONS("-DXBOX_DEFAULT_BPP=16")

MESSAGE(STATUS "Xbox build: Configuration completed successfully")
MESSAGE(STATUS "Xbox build: Cache path = ${XBOX_CACHE_PATH}")
MESSAGE(STATUS "Xbox build: Data path = ${XBOX_DATA_PATH}")
MESSAGE(STATUS "Xbox build: Static linking = ${STATIC_LINK}")
MESSAGE(STATUS "Xbox build: SDL backend = ${SDL_BACKEND}")