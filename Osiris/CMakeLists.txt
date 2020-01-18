cmake_minimum_required(VERSION 2.6)

project(Osiris)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)


set(OSIRIS_BUILD_DIR ${PROJECT_SOURCE_DIR}/build)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OSIRIS_BUILD_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OSIRIS_BUILD_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OSIRIS_BUILD_DIR}/lib)

add_subdirectory(glload)
add_subdirectory(OsirisSDK)
add_subdirectory(OsirisDemo)


