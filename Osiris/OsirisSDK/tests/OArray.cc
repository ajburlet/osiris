#include <OsirisSDK/OArray.hpp>
	
#include "OsirisTests.h"

OTEST_START(OArray, Init) {
	OArray<int> arr(10, 1);
	ASSERT_EQ(arr.size(), 0) << "Wrong size.";
	ASSERT_EQ(arr.capacity(), 10) << "Wrong capacity.";

	OArray<int> arr1(10, 1, true);
	ASSERT_EQ(arr1.size(), 10) << "Wrong size.";
	ASSERT_EQ(arr1.capacity(), 10) << "Wrong capacity.";
	for (auto& item : arr1) EXPECT_EQ(item, 1) << "All array items were supposed to be initialized.";

	OArray<int> arr2;
	arr2.changeCapacity(10);
	ASSERT_EQ(arr2.size(), 0) << "Size should be zero.";
	ASSERT_EQ(arr2.capacity(), 10) << "Wrong capacity.";

	OArray<int> arr3;
	arr3.resizeInit(10, 1);
	ASSERT_EQ(arr3.size(), 10) << "Wrong size (for move construction).";
	ASSERT_EQ(arr3.capacity(), 10) << "Wrong capacity (for move construction).";
	for (auto& item : arr3) EXPECT_EQ(item, 1) << "All array items were supposed to be initialized.";
}
OTEST_END

OTEST_START(OArray, Insertions) {
	OArray<int> arr;
	arr.changeCapacity(10);
	arr.append(1);
	ASSERT_EQ(arr.size(), 1) << "Wrong size.";
	ASSERT_EQ(arr[0], 1) << "Wrong added value.";

	arr[2] = 3;
	arr[1] = 2;
	ASSERT_EQ(arr.size(), 3) << "Wrong size after implicit increase.";
	int expectedValues[] = { 1,2,3 };
	int idx = 0;
	for (const auto& item : arr) {
		EXPECT_EQ(item, expectedValues[idx++]) << "Unexpected array value at index " << idx - 1 << ".";
	}

	bool exceptionCaught = false;
	try {
		arr[11] = 10;
	} catch (OException&) {
		exceptionCaught = true;
	}
	EXPECT_TRUE(exceptionCaught) << "An exception should be thrown when overflowing the array.";
	ASSERT_EQ(arr.size(), 3);
	ASSERT_EQ(arr.capacity(), 10);
}
OTEST_END

OTEST_START(OArray, Resize) {
	OArray<int> arr(10, 999, true);
	int val = 0;
	for (auto& item : arr) {
		EXPECT_EQ(item, 999) << "Wrong initial value.";
		item = val++;
	}

	arr.resize(5);
	ASSERT_EQ(arr.capacity(), 10) << "Invalid capacity.";
	ASSERT_EQ(arr.size(), 5) << "Invalid size.";
	int expectedValues[] = { 0,1,2,3,4,5,6,7,8,9 };
	int idx = 0;
	for (const auto& item : arr) {
		EXPECT_EQ(item, expectedValues[idx++]) << "Unexpected array value at index " << idx - 1 << ".";
	}
	arr.resize(10);
	idx = 0;
	for (const auto& item : arr) {
		EXPECT_EQ(item, expectedValues[idx++]) << "Unexpected array value at index " << idx - 1 << ".";
	}

	arr.changeCapacity(5);
	ASSERT_EQ(arr.capacity(), 5) << "Invalid capacity.";
	ASSERT_EQ(arr.size(), 5) << "Invalid size.";

	arr.changeCapacity(10);
	ASSERT_EQ(arr.capacity(), 10) << "Invalid capacity.";
	ASSERT_EQ(arr.size(), 5) << "Invalid size.";
	arr.append(5);
	arr.append(6);
	ASSERT_EQ(arr.capacity(), 10) << "Invalid capacity.";
	ASSERT_EQ(arr.size(), 7) << "Invalid size.";
	idx = 0;
	for (const auto& item : arr) {
		EXPECT_EQ(item, expectedValues[idx++]) << "Unexpected array value at index " << idx - 1 << ".";
	}

	arr.resizeInit(10, 999);
	idx = 0;
	for (const auto& item : arr) {
		if (idx < 7) {
			EXPECT_EQ(item, expectedValues[idx]) << "Unexpected array value at index " << idx - 1 << ".";
		} else {
			EXPECT_EQ(item, 999);
		}
		idx++;
	}
}
OTEST_END

OTEST_START(OArray, Remove) {
	OArray<int> arr(10);
	for (uint32_t i = 0; i < 10; i++) arr[i] = i;
	arr.remove(9);
	arr.remove(0);
	arr.remove(4);
	arr.remove(5);
	EXPECT_EQ(arr.capacity(), 10) << "Capacity was supposed to remain unchanged.";
	EXPECT_EQ(arr.size(), 6) << "Wrong size.";

	int idx = 0;
	int expectedValues[] = { 1,2,3,4,6,8 };
	for (const auto& item : arr) {
		EXPECT_EQ(item, expectedValues[idx++]) << "Unexpected array value at index " << idx - 1 << ".";
	}
}
OTEST_END

OTEST_START(OArray, Clone) {
	OArray<int> a(10);
	for (uint32_t i = 0; i < 10; i++) a[i] = i;
	OArray<int> b;
	a.cloneTo(b);
	EXPECT_EQ(b.size(), a.size());
	for (uint32_t i = 0; i < 10; i++) EXPECT_EQ(a[i], b[i]);
}
OTEST_END

OTEST_START(OArray, Move) {
	OArray<int> a(10);
	for (uint32_t i = 0; i < 10; i++) a[i] = i;
	OArray<int> b(std::move(a));
	EXPECT_EQ(a.size(), 0);
	EXPECT_EQ(a.capacity(), 0);
	EXPECT_EQ(b.size(), 10);
	EXPECT_EQ(b.capacity(), 10);
	for (uint32_t i = 0; i < 10; i++) EXPECT_EQ(b[i], i);
	OArray<int> c = std::move(b);
	EXPECT_EQ(b.size(), 0);
	EXPECT_EQ(b.capacity(), 0);
	EXPECT_EQ(c.size(), 10);
	EXPECT_EQ(c.capacity(), 10);
	for (uint32_t i = 0; i < 10; i++) EXPECT_EQ(c[i], i);
}
OTEST_END
