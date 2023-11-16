#include <gtest/gtest.h>

using namespace std;

#include "../../src/includes/ticker.hpp"

namespace utilities {
	struct MockTickReceiver {
		MockTickReceiver() {

		}
		MockTickReceiver(MockTickReceiver const&) {

		}
		unsigned int called = 0;
		void onTick() {
			called++;
		}
	};

	TEST(numbers, number_char_overflow_samples) {
		char value = 0;
		ASSERT_EQ(value, 0);
		value += 255;
		ASSERT_EQ(value, -1);
		value++;
		ASSERT_EQ(value, 0);
		value++;
		ASSERT_EQ(value, 1);
		value++;
		ASSERT_EQ(value, 2);
		value += 255;
		ASSERT_EQ(value, 1);
		value += 256;
		ASSERT_EQ(value, 1);
		value += 256;
		ASSERT_EQ(value, 1);
		value += 256;
		ASSERT_EQ(value, 1);
		value += 256;
		ASSERT_EQ(value, 1);
		value += 256;
		ASSERT_EQ(value, 1);
	}

	TEST(numbers, number_char_overflow_iteration) {
		unsigned char v_char = 0;
		int v_int = 0;
		for (; v_int < 5000000; v_int++, v_char++) {
			ASSERT_EQ(v_char, v_int%256);
		}
	}

	TEST(ticker_calculations, ticks) {
		Ticker* t64 = new Ticker(64);
		Ticker* t128 = new Ticker(128);

		delete t64;
		delete t128;
	}

	TEST(ticker_calculations, seconds) {
		Ticker* t64 = new Ticker(64);
		Ticker* t128 = new Ticker(128);

		ASSERT_FLOAT_EQ(t64->getMaxSeconds(), t64->getMaxTicks() / 64);
		ASSERT_FLOAT_EQ(t128->getMaxSeconds(), t128->getMaxTicks() / 128);

		delete t64;
		delete t128;
	}

	TEST(ticker_calculations, minutes) {
		Ticker* t64 = new Ticker(64);
		Ticker* t128 = new Ticker(128);

		ASSERT_FLOAT_EQ(t64->getMaxMinutes(), t64->getMaxSeconds() / 60);
		ASSERT_FLOAT_EQ(t128->getMaxMinutes(), t128->getMaxSeconds() / 60);

		delete t64;
		delete t128;
	}

	TEST(ticker, minimum_limits) {
		Ticker* t64 = new Ticker(64);
		Ticker* t128 = new Ticker(128);

		ASSERT_GT(t64->getMaxTicks(), 38400);  // Allow delays up to 10 minutes
		ASSERT_GT(t128->getMaxTicks(), 38400);  // Allow delays up to 10 minutes

		delete t64;
		delete t128;
	}

	TEST(std_preparations, std_function) {
		MockTickReceiver receiver;
		std::function<void()> func = std::bind(&(MockTickReceiver::onTick), &receiver);
		ASSERT_EQ(receiver.called, 0);
		func();
		ASSERT_EQ(receiver.called, 1);
	}

	void test_call(unsigned char tickrate) {
		Ticker* ticker = new Ticker(64);
		MockTickReceiver receiver;

		ticker->addTask(&receiver, &(MockTickReceiver::onTick), 1.0f);
		unsigned short pre_one_second = ticker->tickrate - 1;
		for (unsigned short i = 0; i < pre_one_second; i++) {
			ticker->tick();
		}
		ASSERT_EQ(receiver.called, 0);
		ticker->tick();
		ASSERT_EQ(receiver.called, 1);
	}

	TEST(ticker_delayed_call, 32 ) { test_call(32 ); }
	TEST(ticker_delayed_call, 64 ) { test_call(64 ); }
	TEST(ticker_delayed_call, 128) { test_call(128); }
}
