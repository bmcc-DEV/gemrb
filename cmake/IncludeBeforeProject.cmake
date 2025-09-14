# VCPKG manifest file: platforms/windows/vcpkg.json
# must be declared before PROJECT()
IF(WIN32)
	SET(VCPKG_MANIFEST_DIR "${CMAKE_SOURCE_DIR}/platforms/windows" )
	message(STATUS "VCPKG_MANIFEST_DIR: ${VCPKG_MANIFEST_DIR}")
	
	# Fix for long path issues on Windows (especially for Xbox builds)
	# Increase the maximum object file path length to handle deep directory structures
	SET(CMAKE_OBJECT_PATH_MAX 300 CACHE STRING "Maximum object file path length")
	message(STATUS "CMAKE_OBJECT_PATH_MAX set to: ${CMAKE_OBJECT_PATH_MAX}")
ENDIF(WIN32)