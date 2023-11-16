#ifndef SRC_INCLUDES_TICKER
#define SRC_INCLUDES_TICKER

namespace utilities {
	class TickerContainer;
	class Ticker;
}

#include <cstdio>
#include <functional>
#include "exceptions.hpp"

namespace utilities {
	/**
	* One container used in 
	*/
	class TickerContainer {
		std::function<void()> tick_function;
	public:
		TickerContainer() = delete;
		TickerContainer(std::function<void()>);
		// Exceptions
		class FunctionOverwrite : public CustomException { using CustomException::CustomException; };
		
		// Operators
		void operator()() const;
	};

	/**
	 * Ticker class, used to register events onTick.
	 * Uses memory up to 1 megabyte due to big rely on memory for big performance
	 * Ticker using number overflow method to loop infinitely across static array
	 * Unrderlying counter uses unsigned short int type, so there's limits on delay:
	 * • Max ticks delay are sizeof(unsigned short int)
	 * • Max seconds delay are sizeof(unsigned short int)/tickrate (1024 tickrate=64 and 512 tickrate=128)
	 * • Max minutes delay are sizeof(unsigned short int)/tickrate/60 (17.0(6) tickrate=64 and 8.5(3) tickrate=128)
	 * For more precise values, use methods getMaxTicks(), getMaxSeconds, getMaxMinutes();
	 * 
	 * 
	 * Other notes:
	 * • Trying to assign several tasks on same tick leads to delay each function by tick
	 */
	class Ticker {
		unsigned short int counter = 0;
	public:
		~Ticker();
#ifdef USHRT_MAX
		const unsigned int size = USHRT_MAX + 1;
		TickerContainer* values[USHRT_MAX+1];
#else
		const unsigned int size = 0xffff;
		TickerContainer* values[0xffff];
#endif
		class DelayOverflow : public CustomException { using CustomException::CustomException; };

		const unsigned char tickrate;
		Ticker(const unsigned char);
		Ticker() = delete;
		Ticker(const Ticker&) = delete;

		unsigned char addTask(std::function<void()>, float);

		/**
		 * Added task with defined delay. Delay in seconds. If delay is over limit, DelayOverflow is thrown
		 */
		template <typename T>
		unsigned char addTask(T* object, void(T::* method)(), float delay) {
			std::function<void()> function = std::bind(method, object);
			return addTask(function, delay);
		}
		
		// Loop ticker once. Must be called on EVERY tick for precise delays
		void tick();

		// Returns maximum delay in ticks
		std::size_t getMaxTicks();
		
		// Returns maximum delay in seconds
		float getMaxSeconds();
		
		// Returns maximum delay in minutes
		float getMaxMinutes();
	};
}

#endif // SRC_INCLUDES_TICKER