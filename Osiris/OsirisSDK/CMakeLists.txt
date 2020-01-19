
file (GLOB SOURCES 
	src/*.cpp 
	src/*.cc
)
file (GLOB HEADERS 
	include/resource.h 
	include/OsirisSDK/*.h
	include/OsirisSDK/*.hpp
)

source_group("Sources" FILES ${SOURCES})
source_group("Headers" FILES ${HEADERS})

include_directories(./include 
					${PROJECT_SOURCE_DIR}/dependencies/glload/include
					${PROJECT_SOURCE_DIR}/dependencies/FreeGLUT/freeglut/freeglut/include
					${PROJECT_SOURCE_DIR}/dependencies/freetype2/include
					${PROJECT_SOURCE_DIR}/dependencies/glm)

add_definitions(-DOSIRISSDK_EXPORTS -DOSIRIS_DLL_NAME="OsirisSDK.dll")

add_library(OsirisSDK SHARED ${SOURCES} ${HEADERS})

# freetype
if(CMAKE_BUILD_TYPE EQUAL "DEBUG")
	target_link_libraries(OsirisSDK freeglut_staticd freetyped glload)
else()
	target_link_libraries(OsirisSDK freeglut_static freetype glload)
endif()
