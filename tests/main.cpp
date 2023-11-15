#include <gtest/gtest.h>
#include <iostream>
#include "../src/managers/infomanager.hpp"
#include "../src/events/event.hpp"
#include "../src/paths/paths.hpp"

int main(int argc, char** argv)
{
	try {
		Paths::init();
	}
	catch (Paths::WrongDataLocation& e) {
		std::cout << e.message;
		return 1;
	}
	::testing::InitGoogleTest(&argc, argv);
	events::EventManager::init();
	functions::Functions::init();
	try {
		managers::InfoManager::init();
	}
	catch (managers::InfoManagerRecreating& e) {
		std::cout << "InfoManager re-creation!\n";
		return 2;
	}
	auto value = RUN_ALL_TESTS();
	events::EventManager::close();
	functions::Functions::close();
	try {
		managers::InfoManager::close();
	}
	catch (managers::InfoManagerReclosing& e) {
		std::cout << "InfoManager re-close-ing!\n";
		return 3;
	}
	return value;
}