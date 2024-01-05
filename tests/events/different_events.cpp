#include <gtest/gtest.h>
#include "../../src/events/event.hpp"
#include "../leaks.hpp"

using namespace std;
using namespace dataStorage;

namespace events {
	namespace {
		static unsigned char called1 = 0;
		static unsigned char called2 = 0;
		static unsigned char called3 = 0;

		void dif_ev_helper1(shared_ptr<Event> e) { called1++; }
		void dif_ev_helper2(shared_ptr<Event> e) { called2++; }
		void dif_ev_helper3(shared_ptr<Event> e) { called3++; }
	}

	TEST(events, traits_double_call) {
		MemoryLeakDetector _;
		called1 = 0;
		called2 = 0;
		called3 = 0;
		EventManager* manager = EventManager::getManager();
		Function* func1 = manager->registerForEvent({ 201 }, &dif_ev_helper1);
		Function* func2 = manager->registerForEvent({ 202 }, &dif_ev_helper1);
		Function* func3 = manager->registerForEvent({ 201, 202 }, &dif_ev_helper2);

		// Testing
		shared_ptr<Event> event = shared_ptr<Event>(new Event({ 201, 202 }));
		ASSERT_EQ(called1, 0);
		ASSERT_EQ(called2, 0);
		ASSERT_EQ(called3, 0);
		manager->fireEvent(event);
		ASSERT_EQ(called1, 2);
		ASSERT_EQ(called2, 1);
		ASSERT_EQ(called3, 0);
		event->changeActivationTraits({ 201 });
		manager->fireEvent(event);
		ASSERT_EQ(called1, 3);
		ASSERT_EQ(called2, 1);
		ASSERT_EQ(called3, 0);
		event->changeActivationTraits({ 201, 202 });
		manager->fireEvent(event);
		ASSERT_EQ(called1, 5);
		ASSERT_EQ(called2, 2);
		ASSERT_EQ(called3, 0);

		// Closing
		manager->unregisterForEvent({ 201 }, func1);
		manager->unregisterForEvent({ 202 }, func2);
		manager->unregisterForEvent({ 201, 202 }, func3);
	}
}
