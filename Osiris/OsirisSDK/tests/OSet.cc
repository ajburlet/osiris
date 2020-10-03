#include <OsirisSDK/OSet.hpp>

#include "OsirisTests.h"


OTEST_START(OSet, Insert) {
	OSet<int> set;
	int testContent[] = { 1,2,3,4,5 };
	for (auto item : testContent) set.insert(item);
	ASSERT_EQ(set.size(), 5);

	int idx = 0;
	for (const auto& item : set) EXPECT_EQ(item, testContent[idx++]);
}
OTEST_END


OTEST_START(OSet, Remove) {
	OSet<int> set;
	int testContent[] = { 1,2,3,4,5 };
	int expected[] = { 1,2,4,5 };
	for (auto item : testContent) set.insert(item);
	set.remove(3);
	ASSERT_EQ(set.size(), 4);

	int idx = 0;
	for (const auto& item : set) EXPECT_EQ(item, expected[idx++]);
}
OTEST_END
