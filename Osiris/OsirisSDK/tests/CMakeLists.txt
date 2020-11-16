enable_testing()

include_directories(${PROJECT_SOURCE_DIR}/dependencies/googletest/googletest/include)

#
# array tests
#
set (TEST_FILES
	OsirisTests.h
	OArray.cc
	ODynArray.cc
	OList.cc
	OStack.cc
	OMap.cc
	OSet.cc
	OWavefrontObjectFile.cc
)

add_definitions(-DOSIRISTEST_BASEDIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(OsirisTests ${TEST_FILES})
target_link_libraries(OsirisTests OsirisSDK gtest_main)
add_test(OsirisTests OsirisTests)
