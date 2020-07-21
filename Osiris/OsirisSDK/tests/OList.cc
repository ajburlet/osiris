#include <OsirisSDK/OList.hpp>

#include "OsirisTests.h"

OTEST_START(OList, PushBack) {
	OList<int> list;
	int testContent[] = { 1,2,3,4,5 };
	for (auto item : testContent) list.pushBack(item);
	ASSERT_EQ(list.size(), 5);

	int idx = 0;
	for (const auto& item : list) EXPECT_EQ(item, testContent[idx++]);
}
OTEST_END

OTEST_START(OList, PushFront) {
	OList<int> list;
	int testContent[] = { 1,2,3,4,5 };
	for (int i = 4; i >= 0; i--) list.pushFront(testContent[i]);
	ASSERT_EQ(list.size(), 5);

	int idx = 0;
	for (const auto& item : list) EXPECT_EQ(item, testContent[idx++]);
}
OTEST_END

OTEST_START(OList, PopBack) {
	OList<int> list;
	int testContent[] = { 1,2,3,4,5 };
	for (auto item : testContent) list.pushBack(item);

	list.popBack();
	list.popBack();
	ASSERT_EQ(list.size(), 3);
	
	int idx = 0;
	for (const auto& item : list) EXPECT_EQ(item, testContent[idx++]);
}
OTEST_END

OTEST_START(OList, PopFront) {
	OList<int> list;
	int testContent[] = { 1,2,3,4,5 };
	for (auto item : testContent) list.pushBack(item);

	list.popFront();
	list.popFront();
	ASSERT_EQ(list.size(), 3);
	
	int idx = 0;
	for (const auto& item : list) EXPECT_EQ(item, testContent[idx++] + 2);
}
OTEST_END

OTEST_START(OList, RemoveByValue) {
	OList<int> list;
	int testContent[] = { 1,2,3,4,5,6,7,8,9,10 };
	for (auto item : testContent) list.pushBack(item);

	EXPECT_EQ(list.remove(1), true);
	EXPECT_EQ(list.remove(10), true);
	EXPECT_EQ(list.remove(3), true);
	EXPECT_EQ(list.remove(6), true);
	EXPECT_EQ(list.remove(11), false);

	ASSERT_EQ(list.size(), 6);
	int expectedValues[] = { 2,4,5,7,8,9 };
	int idx = 0;
	for (const auto& item : list) EXPECT_EQ(item, expectedValues[idx++]);
}
OTEST_END

OTEST_START(OList, Insertions) {
	OList<int> list;
	list.insertAfter(555, list.begin());
	ASSERT_EQ(list.size(), 1);
	EXPECT_EQ(list.front(), 555);
	EXPECT_EQ(list.tail(), 555);

	list.clear();
	ASSERT_EQ(list.size(), 0);

	list.insertBefore(444, list.begin());
	ASSERT_EQ(list.size(), 1);
	EXPECT_EQ(list.front(), 444);
	EXPECT_EQ(list.tail(), 444);

	list.clear();
	ASSERT_EQ(list.size(), 0);

	list.insertAfter(333, list.end());
	ASSERT_EQ(list.size(), 1);
	EXPECT_EQ(list.front(), 333);
	EXPECT_EQ(list.tail(), 333);
	
	list.clear();
	ASSERT_EQ(list.size(), 0);

	list.insertBefore(222, list.end());
	ASSERT_EQ(list.size(), 1);
	EXPECT_EQ(list.front(), 222);
	EXPECT_EQ(list.tail(), 222);

	list.clear();
	ASSERT_EQ(list.size(), 0);

	for (int i = 1; i <= 10; i++) list.pushBack(i);
	list.insertAfter(111, list.begin());
	list.insertBefore(0, list.begin());

	OList<int>::Iterator it;
	for (it = list.begin(); it != list.end(); it++) {
		if (*it == 5) break;
	}
	list.insertAfter(666, it);
	list.insertBefore(444, it);

	ASSERT_EQ(list.size(), 14);
	int expectedValues[] = { 0,1,111,2,3,4,444,5,666,6,7,8,9,10 };
	int idx = 0;
	for (const auto& item : list) EXPECT_EQ(item, expectedValues[idx++]);
}
OTEST_END
