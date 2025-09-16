# Xbox Platform Support for GemRB

IF(WIN32 OR CMAKE_SYSTEM_NAME STREQUAL "Xbox")
    # Xbox platform files
    SET(XBOX_PLATFORM_SOURCES
        Xbox.cpp
        XboxController.cpp
        XboxAudio.cpp
        XboxMemory.cpp
        XboxUI.cpp
    )

    SET(XBOX_PLATFORM_HEADERS
        Xbox.h
        XboxController.h
        XboxAudio.h
        XboxMemory.h
        XboxUI.h
        XboxAudioReader.h
    )

    # Check if we're building for Xbox
    IF(CMAKE_SYSTEM_NAME STREQUAL "Xbox")
        ADD_DEFINITIONS(-D_XBOX)
        MESSAGE(STATUS "Building for Xbox platform")
        
        # Xbox-specific compiler flags
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_XBOX /DXBOX")
        
        # Xbox libraries
        SET(XBOX_LIBRARIES
            xapilib
            d3d8
            d3dx8
            dsound
            xgraphics
        )
        
        # Enable Xbox optimizations
        SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /Ob2 /Oi /Ot /GL")
        SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG")
        
    ELSE()
        MESSAGE(STATUS "Xbox platform support available for cross-compilation testing")
    ENDIF()

    # Create Xbox platform library
    ADD_LIBRARY(xbox_platform STATIC
        ${XBOX_PLATFORM_SOURCES}
        ${XBOX_PLATFORM_HEADERS}
    )

    TARGET_INCLUDE_DIRECTORIES(xbox_platform PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/gemrb/includes
        ${CMAKE_SOURCE_DIR}/gemrb/core
        ${CMAKE_SOURCE_DIR}/gemrb/core/GUI
        ${CMAKE_SOURCE_DIR}/gemrb/core/Audio
    )

    # Link with existing GemRB libraries
    TARGET_LINK_LIBRARIES(xbox_platform gemrb_core)

    IF(CMAKE_SYSTEM_NAME STREQUAL "Xbox")
        TARGET_LINK_LIBRARIES(xbox_platform ${XBOX_LIBRARIES})
    ENDIF()

    # Install headers for development
    INSTALL(FILES ${XBOX_PLATFORM_HEADERS}
        DESTINATION include/gemrb/platforms/xbox
        COMPONENT Devel
    )

ELSE()
    MESSAGE(STATUS "Xbox platform support only available on Windows")
ENDIF()