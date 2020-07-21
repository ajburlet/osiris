#include <OsirisSDK/OMap.hpp>

#include "OsirisTests.h"

namespace OTest {
	class OMap : public testing::Test {
	protected:
		virtual void SetUp() override
		{
			for (int i = 0; i < 4; i++) _map[i] = _expectedValues[i];
		}


		static constexpr float _expectedValues[] = { 5.0f, 10.0f, 20.0f, 30.0f };
		::OMap<int, float> _map;
	};

	OTEST_F_START(OMap, Insertions) {
		ASSERT_EQ(_map.size(), 4);
		int key = 0;
		for (::OMap<int, float>::Iterator it = _map.begin(); it != _map.end(); ++it) {
			EXPECT_EQ(_map[key], _expectedValues[key]);
			key++;
		}
	}
	OTEST_F_END

	OTEST_F_START(OMap, Find) {
		auto it = _map.find(2);
		EXPECT_NE(it, _map.end());
		EXPECT_EQ(it.key(), 2);
		EXPECT_EQ(it.value(), 20.0f);
	}
	OTEST_F_END

	OTEST_F_START(OMap, Erase) {
		auto it = _map.find(2);
		_map.remove(it);
		EXPECT_EQ(_map.size(), 3);
		_map.remove(1);
		EXPECT_EQ(_map.size(), 2);
		EXPECT_EQ(_map[0], 5.0f);
		EXPECT_EQ(_map[3], 30.0f);
	}
	OTEST_F_END
}

