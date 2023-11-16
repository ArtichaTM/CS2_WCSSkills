#include <cstdio>
#include <limits>
#include "ticker.hpp"

using namespace utilities;

utilities::TickerContainer::TickerContainer(std::function<void()> func) : tick_function(func) {}

void TickerContainer::operator()() const { tick_function(); }

utilities::Ticker::~Ticker()
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

unsigned char utilities::Ticker::addTask(std::function<void()> func, float delay)
{
	if (this->getMaxSeconds() < delay) {
		throw DelayOverflow();
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


void utilities::Ticker::tick()
{
	counter++;
	if (values[counter]) {
		(*values[counter])();
		delete values[counter];
		values[counter] = nullptr;
	}
}

std::size_t	utilities::Ticker::getMaxTicks() {
#ifdef USHRT_MAX
	return USHRT_MAX + 1;
#else
	return std::numeric_limits<unsigned short int>::max() + 1;
#endif

}

float utilities::Ticker::getMaxSeconds() { return static_cast<float>(this->getMaxTicks()) / tickrate;}
float utilities::Ticker::getMaxMinutes() { return this->getMaxSeconds()/60;							 }
