#include <gtest/gtest.h>
#include "../src/managers/infomanager.hpp"
#include "../src/events/event.hpp"

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	events::EventManager::init();
	functions::Functions::init();
	managers::InfoManager::init();
	auto value = RUN_ALL_TESTS();
	events::EventManager::close();
	functions::Functions::close();
	managers::InfoManager::close();
	return value;
}