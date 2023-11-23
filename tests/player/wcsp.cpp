#include <gtest/gtest.h>
#include <unordered_set>

#include "../../src/player/wcsplayer.hpp"

#include "../leaks.hpp"

using namespace std;
using namespace managers;

namespace wcsplayer {
	TEST(WCSP, creation) {
		MemoryLeakDetector _;
		WCSPlayer* wcsp = new WCSPlayer(5, 1, {});
		ASSERT_EQ(wcsp->level, 5);
		ASSERT_EQ(wcsp->xp, 1);
		ASSERT_EQ(wcsp->status_effects.size(), 0);
		ASSERT_EQ(wcsp->leftovers.size(), 0);
		delete wcsp;
	}
}
