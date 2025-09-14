INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(CppScript_SOURCES
	CppScript.cpp
	CppScriptAPI.cpp
	ClockScript.cpp
)

SET(CppScript_HEADERS
	CppScript.h
	CppScriptAPI.h
	ClockScript.h
)

ADD_LIBRARY(CppScript MODULE ${CppScript_SOURCES} ${CppScript_HEADERS})
TARGET_LINK_LIBRARIES(CppScript gemrb_core)
INSTALL(TARGETS CppScript DESTINATION ${LIB_DIR})