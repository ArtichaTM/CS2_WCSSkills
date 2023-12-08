#include <iostream>
#include "managers/infomanager.hpp"
#include "events/event.hpp"
#include "paths/paths.hpp"

using namespace events;
using namespace managers;
using namespace traits;

int main(int argc, char** argv)
{
	try {
		Paths::init();
	}
	catch (Paths::WrongDataLocation& e) {
		std::cout << e.message;
		return 1;
	}
	events::EventManager::init();
	functions::Functions::init();
	try {
		managers::InfoManager::init();
	}
	catch (managers::InfoManagerRecreating& e) {
		std::cout << "InfoManager re-creation!\n";
		return 2;
	}

	// Test code

	events::EventManager::close();
	functions::Functions::close();
	try {
		managers::InfoManager::close();
	}
	catch (managers::InfoManagerReclosing& e) {
		std::cout << "InfoManager re-close-ing!\n";
		return 3;
	}
	return 0;
}