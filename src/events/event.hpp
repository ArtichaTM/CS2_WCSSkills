#ifndef WCSSKILLS_EVENT_HPP
#define WCSSKILLS_EVENT_HPP

#include <string>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <memory>
#include <bitset>
#include "../includes/exceptions.hpp"
#include "../includes/datastorage.hpp"
#include "../includes/doublelinkedlist.hpp"
#include "../defines.h"

namespace events {
	enum ReturnEvent {
		PASS, GUARD, NO_EFFECT, MISSED
	};
	class NoSuchReceiver;
	class SettingsModify : public CustomException { using CustomException::CustomException; };
	class SetDataModify : public CustomException { using CustomException::CustomException; };
	class Event;
	class EventManager;
	class Function;

	typedef std::function<void(std::shared_ptr<Event>)> EventReceiver;
	typedef dataStorage::DoubleLinkedList<Function*> vectorofFunctions;
}

namespace functions {
	typedef std::function<events::ReturnEvent(
		std::shared_ptr<events::Event>&,
		dataStorage::DataStorage&
	)> function;
	typedef std::unordered_map<std::basic_string<char>, function> functions_map;
}

#include "../traits/traits.hpp"
#include "../player/wcsplayer.hpp"


namespace events {
	
	class Event {
	public:
		traits::tr_set activation_traits;
		dataStorage::DataStorage* data;
		bool initialEvent = false;
		events::ReturnEvent result = PASS;
		
		Event(Event const&) = delete;
		Event(traits::tr_set);
		Event(std::initializer_list<traits::Trait>);
		Event(traits::tr_set, dataStorage::DataStorage*);
		~Event();
		
		void changeActivationTraits(traits::tr_set);

		template<bool autoDelete, typename T>
		void setData(std::string const& key, T* value) {
#ifdef DEBUG
			if (this->data->contains(key)) {
				char message[] = "Trying to \"setData\" already existing key."
					"Use \"changeData\" instead";
				throw SetDataModify(message);
			}
#endif
			auto* eData = new dataStorage::EData(autoDelete);
			eData->setData(value);
			this->data->insert(std::pair(key, eData));
		}

		template<typename T>
		void setConstData(std::string const& key, T const* value) {
#ifdef DEBUG
			if (this->data->contains(key)) {
				char message[] = "Trying to \"setData\" already existing key."
					"Use \"changeData\" instead";
				throw SetDataModify(message);
			}
#endif
			auto* eData = new dataStorage::EData(false);
			eData->setConstData(value);
			this->data->insert(std::pair(key, eData));
		}
		
		template<typename T>
		T* getData(std::string const& key) {
			return this->data->at(key)->getData<T>();
		}

		template<typename T> void changeData(std::string const& key, std::function<void(T&)>& func) {
			this->data->at(key)->changeData(func);
		}

		template<typename T> void changeData(std::string const& key, std::function<void(T&&)>& func) {
			return changeData(func);
		}
	};

	class Function {
		EventReceiver inner_function;
	public:
		Function() = delete;
		Function(const Function&&) = delete;
		Function(Function&&) = delete;
		Function(EventReceiver);

		void operator()(std::shared_ptr<Event>);
	};

	namespace {
		typedef std::unordered_map<std::bitset<TRAIT_INDEX_MAX>, vectorofFunctions*> receivers_map;
	}

	class EventManager {
		//std::unordered_map<traits::tr_set, vectorofFunctions*>* registered_events = new std::unordered_map<traits::tr_set, vectorofFunctions*>();
		receivers_map registered_events[MAX_TRAITS_IN_TRSET];
		dataStorage::DoubleLinkedList<std::shared_ptr<Event>>* lateRunEvents;
		void iterateOverEvents();
		void fire_during_iteration(vectorofFunctions*, std::shared_ptr<Event>);
		static EventManager* instance;
		volatile bool iterating = false;
		EventManager();
		~EventManager();
	public:
		class NotInitialized : public CustomException { using CustomException::CustomException; };


		static void init();
		static void close();
		static bool initialized();
		static EventManager* getManager();
		Function* registerForEvent(traits::tr_set const& activation_traits, EventReceiver);
		void unregisterForEvent(traits::tr_set const& activation_traits, Function*);

		void fireEvent(std::shared_ptr<Event>);
		
		bool receiversExist(traits::tr_set const& activation_traits);
	};
	
} // events

#endif //WCSSKILLS_EVENT_HPP
