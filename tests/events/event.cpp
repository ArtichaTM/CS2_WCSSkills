/*
Tests valid only with DEBUG defined!
*/

#include <gtest/gtest.h>
#include <memory>
#include "../../src/events/event.hpp"
#include "../../src/includes/doublelinkedlist.hpp"
#include "../leaks.hpp"

using namespace std;
using namespace dataStorage;

namespace events {
	void increase_projectile_damage(shared_ptr<Event> e) {
		function<void(float&)> func = [&](float& value) { value += 1; };
		e->changeData("multiplier", func);
	}

	TEST(events, simple_value_change) {
		MemoryLeakDetector _;
		EventManager* manager = EventManager::getManager();
		Function* func = manager->registerForEvent({ 3001 }, &increase_projectile_damage);
		shared_ptr<Event> event = shared_ptr<Event>(new Event({ 3001 }));
		float multiplier = 1.0f;
		event->setData<false>("multiplier", &multiplier);
		ASSERT_FLOAT_EQ(multiplier, 1);
		manager->fireEvent(event);
		ASSERT_FLOAT_EQ(multiplier, 2);
		manager->fireEvent(event);
		ASSERT_FLOAT_EQ(multiplier, 3);
		manager->unregisterForEvent({ 3001 }, func);
	}
	static unsigned char called1 = 0;
	static unsigned char called2 = 0;
	static unsigned char called3 = 0;

	void helper2(shared_ptr<Event> e) {
		called1++;
		EventManager* manager = EventManager::getManager();
		shared_ptr<Event> event = shared_ptr<Event>(new Event({ 2 }));
		float multiplier = 1.0f;
		event->setData<false>("multiplier", &multiplier);
		manager->fireEvent(event);
	}

	void helper3(shared_ptr<Event> e) {
		called2++;
	}

	void helper4(shared_ptr<Event> e) {
		called3++;
	}


	TEST(events, double_event) {
		MemoryLeakDetector _;
		called1 = 0;
		called2 = 0;
		EventManager* manager = EventManager::getManager();
		Function* func1 = manager->registerForEvent({ 1 }, &helper2);
		Function* func2 = manager->registerForEvent({ 2 }, &helper3);
		shared_ptr<Event> event = shared_ptr<Event>(new Event({ 1 }));
		float multiplier = 1.0f;
		event->setData<false>("multiplier", &multiplier);
		ASSERT_EQ(called1, 0);
		ASSERT_EQ(called2, 0);
		manager->fireEvent(event);
		ASSERT_EQ(called1, 1);
		ASSERT_EQ(called2, 1);
		event->changeActivationTraits({ 2 });
		manager->fireEvent(event);
		ASSERT_EQ(called1, 1);
		ASSERT_EQ(called2, 2);
		manager->unregisterForEvent({ 1 }, func1);
		manager->unregisterForEvent({ 2 }, func2);
	}
	

	TEST(events, change_activation_traits) {
		MemoryLeakDetector _;
		called2 = 0;
		called3 = 0;
		EventManager* manager = EventManager::getManager();
		Function* func1 = manager->registerForEvent({ 1 }, &helper3);
		Function* func2 = manager->registerForEvent({ 2 }, &helper4);
		shared_ptr<Event> event = shared_ptr<Event>(new Event({ 1 }));
		float multiplier = 1.0f;
		event->setData<false>("multiplier", &multiplier);
		ASSERT_EQ(called2, 0);
		manager->fireEvent(event);
		ASSERT_EQ(called2, 1);
		event->changeActivationTraits({ 2 });
		ASSERT_EQ(called3, 0);
		manager->fireEvent(event);
		ASSERT_EQ(called3, 1);
		manager->unregisterForEvent({ 1 }, func1);
		manager->unregisterForEvent({ 2 }, func2);
	}

	void add_new_values1(shared_ptr<Event> e) {
		DoubleLinkedList<unsigned char>* dolili = e->getData<DoubleLinkedList<unsigned char>>("values");
		dolili->insertAfter(dolili->tail, 5);
	}

	void add_new_values2(shared_ptr<Event> e) {
		std::function<void(DoubleLinkedList<unsigned char>&)> func = [&](DoubleLinkedList<unsigned char>& dolili)->void {
			dolili.insertAfter(dolili.tail, 5);
		};
		e->changeData("values", func);
	}

	TEST(events, dolili_changing) {
		MemoryLeakDetector _;
		EventManager* manager = EventManager::getManager();
		Function* func1 = manager->registerForEvent({ 1 }, &add_new_values1);
		Function* func2 = manager->registerForEvent({ 2 }, &add_new_values2);
		shared_ptr<Event> event = shared_ptr<Event>(new Event({ 1 }));
		auto* dolili = new DoubleLinkedList<unsigned char>();
		event->setData<false>("values", dolili);
		ASSERT_EQ(event->getData<DoubleLinkedList<unsigned char>>("values")->size(), 0);
		manager->fireEvent(event);
		ASSERT_EQ(event->getData<DoubleLinkedList<unsigned char>>("values")->size(), 1);
		event->changeActivationTraits({ 2 });
		manager->fireEvent(event);
		ASSERT_EQ(event->getData<DoubleLinkedList<unsigned char>>("values")->size(), 2);
		ASSERT_EQ(event->getData<DoubleLinkedList<unsigned char>>("values")->head->data, 5);
		ASSERT_EQ(event->getData<DoubleLinkedList<unsigned char>>("values")->head->getNext()->data, 5);
		
		manager->unregisterForEvent({ 1 }, func1);
		manager->unregisterForEvent({ 2 }, func2);
		delete dolili;
	}
}
