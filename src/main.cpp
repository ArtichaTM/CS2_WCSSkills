#include "managers/infomanager.hpp"
#include "events/event.hpp"
#include "paths/paths.hpp"
#include "events/event.hpp"

using namespace managers;
using namespace traits;

int main(int argc, char** argv)
{
	try {
		Paths::init();
	}
	catch (Paths::WrongDataLocation& e) {
		return 1;
	}
	events::EventManager::init();
	functions::Functions::init();
	try {
		managers::InfoManager::init();
	}
	catch (managers::InfoManagerRecreating& e) {
		return 2;
	}
	//
	//// Test code
	//
	events::EventManager::close();
	functions::Functions::close();
	try {
		managers::InfoManager::close();
	}
	catch (managers::InfoManagerReclosing& e) {
		return 3;
	}
	Paths::close();
	return 0;
}
