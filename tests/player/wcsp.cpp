#include <gtest/gtest.h>
#include <unordered_set>

#include "../../src/player/wcsplayer.hpp"

#include "../leaks.hpp"

using namespace std;
using namespace managers;
using namespace events;
using namespace traits;

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

	TEST(WCSP, spawn_no_skills) {
		MemoryLeakDetector _;
		WCSPlayer* wcsp = new WCSPlayer(5, 1, {});
		auto e = make_shared<Event>(tr_set{ 246 });
		EventManager* manager = EventManager::getManager();
		manager->fireEvent(e);
		delete wcsp;
		e = nullptr;
	}

	TEST(WCSP, spawn_one_skill) {
		MemoryLeakDetector _;
		WCSPlayer* wcsp = new WCSPlayer(5, 1, {});
		SkillInfo * skill_info = (*InfoManager::getManager()->skills.begin()).second;
		wcsp->skills_selected[0] = skill_info;
		auto e = make_shared<Event>(tr_set{ 246 });
		EventManager* manager = EventManager::getManager();
		manager->fireEvent(e);
		delete wcsp;
		e = nullptr;
	}
}
