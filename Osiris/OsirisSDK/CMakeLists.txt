
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

include_directories(./include ${PROJECT_SOURCE_DIR}/deps/include)
link_directories(${PROJECT_SOURCE_DIR}/deps/lib/${CMAKE_BUILD_TYPE})

add_library(OsirisSDK SHARED ${SOURCES} ${HEADERS})

target_link_libraries(OsirisSDK freeglut freetype)

