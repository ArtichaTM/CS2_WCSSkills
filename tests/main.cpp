#include <gtest/gtest.h>
#include "../src/managers/infomanager.hpp"
#include "../src/events/event.hpp"

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	events::EventManager::init();
	functions::Functions::init();
	managers::InfoManager::init();
	try {
		return RUN_ALL_TESTS();
	} catch (CustomException& exception) {}
	
	events::EventManager::close();
	functions::Functions::close();
	managers::InfoManager::close();
}