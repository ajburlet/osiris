cmake_minimum_required(VERSION 3.0)

#
# Check if in-source build is taking place.
#
if ("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
	message(FATAL_ERROR "In-source builds should be avoided. \n"
			    "Create a build directory and run cmake from there.")
endif ()

#
# needed to force MSVC runtime library setting in SDK tests
#
cmake_policy(SET CMP0091 NEW)

#
# Project definitions
#
project(Osiris)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

#
# freeglut
#
set(FREEGLUT_BUILD_DEMOS OFF CACHE BOOL "")
set(FREEGLUT_BUILD_SHARED_LIBS OFF CACHE BOOL "")
set(FREEGLUT_BUILD_STATIC_LIBS ON CACHE BOOL "")
add_subdirectory(dependencies/FreeGLUT/freeglut/freeglut)

#
# other dependencies
#
add_subdirectory(dependencies/freetype2)
add_subdirectory(dependencies/glload)
add_subdirectory(dependencies/googletest)

#
# SDK & Demo
#
add_subdirectory(OsirisSDK)
add_subdirectory(OsirisDemo)
