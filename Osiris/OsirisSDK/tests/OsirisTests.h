#pragma once

#include <OsirisSDK/OException.h>
#include <gtest/gtest.h>

#define OTEST_START(aTestSuite, aTestName) \
TEST(aTestSuite, aTestName) { \
	OMemoryManagerSingleton memoryMgr; \
	auto memUsage = memoryMgr->usageAt(OMemoryManagerScope::Default); \
	try

#define OTEST_END \
	catch (OException& e) {	\
		ASSERT_TRUE(false) << "Exception caught: " << e.what(); \
	} \
	EXPECT_EQ(memUsage, memoryMgr->usageAt(OMemoryManagerScope::Default)) << "Possible leak."; \
}

#define OTEST_F_START(aTestSuite, aTestName) \
TEST_F(aTestSuite, aTestName) { \
	try

#define OTEST_F_END \
	catch (OException& e) {	\
		ASSERT_TRUE(false) << "Exception caught: " << e.what(); \
	} \
}
