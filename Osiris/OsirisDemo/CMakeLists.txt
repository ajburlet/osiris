
include_directories(
	${PROJECT_SOURCE_DIR}/deps/include 
	${PROJECT_SOURCE_DIR}/OsirisSDK/include
	)
link_directories(
	${PROJECT_SOURCE_DIR}/deps/lib/x64/${CMAKE_BUILD_TYPE} 
	${PROJECT_SOURCE_DIR}/OsirisSDK/lib/x64/${CMAKE_BUILD_TYPE}
)

file (GLOB SOURCES *.cpp)
file (GLOB HEADERS *.h *.hpp)

source_group("Sources" FILES ${SOURCES})
source_group("Headers" FILES ${HEADERS})

add_executable(OsirisDemo ${SOURCES} ${HEADERS})

target_link_libraries(OsirisDemo freeglut freetype OsirisSDK)

