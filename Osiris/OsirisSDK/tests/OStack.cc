#include <OsirisSDK/OStack.hpp>

#include "OsirisTests.h"

OTEST_START(OStack, InsertPop) {
	OStack<int> stack;
	int values[] = { 1,2,3,4,5 };
	for (auto val : values) {
		stack.push(val);
	}
	EXPECT_EQ(stack.top(), 5);
	stack.pop();
	EXPECT_EQ(stack.top(), 4);
	stack.pop();
	EXPECT_EQ(stack.top(), 3);
	stack.push(99);
	EXPECT_EQ(stack.top(), 99);
	stack.pop();
	for (int i = 2; i >= 0; i--) {
		EXPECT_EQ(stack.top(), values[i]);
		stack.pop();
	}
	EXPECT_EQ(stack.empty(), true);
	
	bool exception = false;
	try {
		stack.pop();
	} catch (OException& e) {
		exception = true;
	}
	EXPECT_EQ(exception, true);
}
OTEST_END