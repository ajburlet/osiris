#include <OsirisSDK/OArray.hpp>

#include "OsirisTests.h"

template <class T> using ODynArrayTest = ODynArray <T, OSystemMemoryAllocator<OMemoryManager::Scope::Default>, 8>;

OTEST_START(ODynArray, Init) {
	ODynArrayTest<int> arr(10, 1);
	ASSERT_EQ(arr.size(), 10) << "Wrong size.";
	ASSERT_EQ(arr.capacity(), 16) << "Wrong capacity.";
	EXPECT_EQ(memoryMgr->usageAt(OMemoryManager::Scope::Default), 16*4) << "Incoherent heap usage.";
	for (auto& item : arr) EXPECT_EQ(item, 1) << "All array items were supposed to be initialized.";

	ODynArrayTest<int> arr2;
	arr2.resize(10);
	ASSERT_EQ(arr2.size(), 0) << "Size should be zero.";
	ASSERT_EQ(arr2.capacity(), 16) << "Wrong capacity.";

	ODynArrayTest<int> arr3(std::move(arr));
	ASSERT_EQ(arr3.size(), 10) << "Wrong size (for move construction).";
	ASSERT_EQ(arr3.capacity(), 16) << "Wrong capacity (for move construction).";
	EXPECT_EQ(memoryMgr->usageAt(OMemoryManager::Scope::Default), 32*4) << "Incoherent heap usage.";
	for (auto& item : arr3) EXPECT_EQ(item, 1) << "All array items were supposed to be initialized.";
}
OTEST_END

OTEST_START(ODynArray, Insertions) {
	ODynArrayTest<int> arr;
	arr.resize(10);
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

	arr[11] = 10;
	ASSERT_EQ(arr.size(), 12);
	ASSERT_EQ(arr.capacity(), 16);
}
OTEST_END

OTEST_START(ODynArray, Resize) {
	ODynArrayTest<int> arr(10, 999);
	int val = 0;
	for (auto& item : arr) item = val++;

	arr.resize(5);
	ASSERT_EQ(arr.capacity(), 8) << "Invalid capacity.";
	ASSERT_EQ(arr.size(), 5) << "Invalid size.";
	EXPECT_EQ(memoryMgr->usageAt(OMemoryManager::Scope::Default), 8*4) << "Incoherent heap usage.";
	int expectedValues[] = { 0,1,2,3,4,5,6,7,8,9,10,11 };
	int idx = 0;
	for (const auto& item : arr) {
		EXPECT_EQ(item, expectedValues[idx++]) << "Unexpected array value at index " << idx - 1 << ".";
	}

	arr.resize(10);
	ASSERT_EQ(arr.capacity(), 16) << "Invalid capacity.";
	ASSERT_EQ(arr.size(), 5) << "Invalid size.";
	EXPECT_EQ(memoryMgr->usageAt(OMemoryManager::Scope::Default), 16*4) << "Incoherent heap usage.";
	for (auto i = 5; i < 12; i++) arr.append(i);
	ASSERT_EQ(arr.capacity(), 16) << "Invalid capacity.";
	ASSERT_EQ(arr.size(), 12) << "Invalid size.";
	idx = 0;
	for (const auto& item : arr) {
		EXPECT_EQ(item, expectedValues[idx++]) << "Unexpected array value at index " << idx - 1 << ".";
	}
}
OTEST_END

OTEST_START(ODynArray, Remove) {
	ODynArrayTest<int> arr(10);
	for (uint32_t i = 0; i < 10; i++) arr[i] = i;
	arr.remove(9);
	arr.remove(0);
	arr.remove(4);
	arr.remove(5);
	EXPECT_EQ(arr.capacity(), 16) << "Capacity was supposed to remain unchanged.";
	EXPECT_EQ(arr.size(), 6) << "Wrong size.";

	int idx = 0;
	int expectedValues[] = { 1,2,3,4,6,8 };
	for (const auto& item : arr) {
		EXPECT_EQ(item, expectedValues[idx++]) << "Unexpected array value at index " << idx - 1 << ".";
	}
}
OTEST_END