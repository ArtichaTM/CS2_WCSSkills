#include <cstdio>
#include <limits>
#include "ticker.hpp"

using std::size_t;
using std::function;

namespace utilities {
	TickerContainer::TickerContainer(function<void()> func) : tick_function(func) {}

	void TickerContainer::operator()() const { tick_function(); }

	Ticker::~Ticker()
	{
		for (unsigned int i = 0; i < this->size; i++) {
			if (values[i]) {
				delete values[i];
			}
		}
	}

	Ticker::Ticker(unsigned char _tickrate) :
		counter(),
		values(),
		tickrate(_tickrate)
	{}

	unsigned char Ticker::addTask(function<void()> func, float delay)
	{
		if (this->getMaxSeconds() < delay) {
			throw DelayOverflow("T_DO0");
		}

		// Calculates target tick
		unsigned char target_tick_index = static_cast<unsigned char>(counter + (delay * tickrate));
		unsigned char offset = 0;
		while (values[target_tick_index]) {
			target_tick_index++;
			offset++;
		}
		values[target_tick_index] = new TickerContainer(func);
		return offset;
	}


	void Ticker::tick()
	{
		counter++;
		if (values[counter]) {
			(*values[counter])();
			delete values[counter];
			values[counter] = nullptr;
		}
	}

	size_t Ticker::getMaxTicks() {
#ifdef USHRT_MAX
		return USHRT_MAX + 1;
#else
		return std::numeric_limits<unsigned short int>::max() + 1;
#endif

	}

	float Ticker::getMaxSeconds() { return static_cast<float>(this->getMaxTicks()) / tickrate; }
	float Ticker::getMaxMinutes() { return this->getMaxSeconds() / 60; }

}
