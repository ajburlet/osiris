
include_directories(
	${PROJECT_SOURCE_DIR}/OsirisSDK/include
	${PROJECT_SOURCE_DIR}/dependencies/glload/include
	${PROJECT_SOURCE_DIR}/dependencies/FreeGLUT/freeglut/freeglut/include
	${PROJECT_SOURCE_DIR}/dependencies/glm
	)
link_directories(
	${PROJECT_SOURCE_DIR}/OsirisSDK/lib/x64/${CMAKE_BUILD_TYPE}
)

file (GLOB SOURCES *.cpp)
file (GLOB HEADERS *.h *.hpp)

source_group("Sources" FILES ${SOURCES})
source_group("Headers" FILES ${HEADERS})

add_definitions(-DOSIRIS_DEMO_BASEDIR="${PROJECT_SOURCE_DIR}/OsirisDemo")

add_executable(OsirisDemo ${SOURCES} ${HEADERS})
set_property(TARGET OsirisDemo PROPERTY CXX_STANDARD 17)

target_link_libraries(OsirisDemo OsirisSDK)

