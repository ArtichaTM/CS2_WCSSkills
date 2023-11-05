#include <iostream>
#include <fstream>
#include "includes/paths.hpp"
#include "managers/infomanager.hpp"
#include "events/event.hpp"
#include "includes/doublelinkedlist.hpp"

using namespace std;
using namespace traits;
using namespace events;

class IntValue {
public:
	float value;
	int value2;
	IntValue(float _value, int _value2) : value(_value), value2(_value2) {};
};

ReturnEvent subTestEvents(shared_ptr<Event>& e) {
	auto* multiplier = e->getData<float>("mult");
	*multiplier = *multiplier * 1.1f;
	return events::ReturnEvent::PASS;
}

void testManager() {
	using namespace managers;
	InfoManager* manager = InfoManager::getManager();
}

template<typename T>
void print_map(unordered_set<T> const& m) {
	std::cout << '[';
	auto it = m.begin();
	if (it==m.end()) {
		std::cout << ']';
		return;
	}
	T value = *it;
	std::cout << value;
	it++;
	while (it != m.end()) {
		value = *it;
		std::cout << ", " << value;
		it++;
	}
	std::cout << ']';
}

void testFuncManager() {
	auto* funcManager = functions::Functions::get();
	std::cout << "\n\nSize: " << (*funcManager)->size() << "\n";
	assert(!(*funcManager)->empty());
	for (auto& funcs : **funcManager) {
		std::cout << "Name " + funcs.first << "\n";
	}
}



int main() {
	events::EventManager::init();
	functions::Functions::init();
	managers::InfoManager::init();
	events::EventManager::close();
	functions::Functions::close();
	managers::InfoManager::close();
	return 0;
}
