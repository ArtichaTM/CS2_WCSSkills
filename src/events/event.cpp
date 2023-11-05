#include "event.hpp"

#include <utility>

using std::string;
using std::function;
using std::unordered_map;
using std::queue;
using traits::tr_set;
using dataStorage::DataStorage;
using dataStorage::make;

namespace events {
	namespace {
		template<typename T, typename... U>
		size_t getAddress(function<T(U...)> f) {
			typedef T(fnType)(U...);
			auto** fnPointer = f.template target<fnType*>();
			return (size_t) *fnPointer;
		}
	}

	class NoSuchReceiver : public std::exception {
	public:
		[[nodiscard]] const char * what() const noexcept override {
			return "There's no such receiver registered to unregister it";
		}
	};
	
	EventManager* EventManager::instance = nullptr;
	
	void EventManager::registerForEvent(const tr_set& activation_traits, eventReceiver& eventF) {
		if (!this->registered_events->contains(activation_traits)) {
			this->registered_events->insert({activation_traits, vectorofEventReceivers()});
		}
		this->registered_events->at(activation_traits).push_back(eventF);
	}

	void EventManager::unregisterForEvent(const tr_set& activation_traits, eventReceiver& eventF) {
		if (!this->registered_events->contains(activation_traits)) {
			throw NoSuchReceiver();
		}
		vectorofEventReceivers eventRegisteredReceivers = this->registered_events->at(activation_traits);
		for (auto iter = eventRegisteredReceivers.begin(); iter != eventRegisteredReceivers.end(); ++iter) {
			if ((*iter).target_type() != eventF.target_type()) continue;
			if (getAddress(*iter) != getAddress(eventF)) continue;
			eventRegisteredReceivers.erase(iter);
			return;
		}
		throw NoSuchReceiver();
	}
	
	void EventManager::registerForEvent(const tr_set& activation_traits, eventReceiver&& eventF) {
		registerForEvent(activation_traits, eventF);
	}

	void EventManager::unregisterForEvent(const tr_set& activation_traits, eventReceiver&& eventF) {
		unregisterForEvent(activation_traits, eventF);
	}

	void EventManager::iterateOverEvents() {
		lateRunEvents->front()->initialEvent = true;
		iterating = true;
		while (!this->lateRunEvents->empty()) {
			std::shared_ptr<Event> event = this->lateRunEvents->front();
			this->lateRunEvents->pop();
			if (!this->registered_events->contains(event->activation_traits)) {
				continue;
			}
			vectorofEventReceivers functions = this->registered_events->at(event->activation_traits);
			for (const eventReceiver& eventF : functions) {
				eventF.operator()(event);
				if (event->result != events::ReturnEvent::PASS) {
					continue;
				}
			}
		}
		iterating = false;
	}
	
	void EventManager::fireEvent(std::shared_ptr<Event>& event) {
		this->lateRunEvents->push(event);
		if (!iterating) this->iterateOverEvents();
	}
	
	void EventManager::fireEvent(std::shared_ptr<Event>&& event) {
		fireEvent(event);
	}
	
	bool EventManager::receiversExist(const tr_set& activation_traits) {
		return this->registered_events->contains(activation_traits);
	}

	EventManager::EventManager() {
		this->registered_events = new unordered_map<tr_set, vectorofEventReceivers>;
		this->lateRunEvents = new queue<std::shared_ptr<Event>>;
	}

	EventManager::~EventManager() {
		delete this->registered_events;
		delete this->lateRunEvents;
	}

	void EventManager::init() {
		assert(!instance);
		instance = new EventManager();
	}

	void EventManager::close() {
		assert(instance);
		delete instance;
		instance = nullptr;
	}
	
	EventManager* EventManager::getManager() {
		if (!EventManager::instance) {
			EventManager::instance = new EventManager();
		}
		return EventManager::instance;
	}
	
	Event::Event(tr_set traits) : activation_traits(std::move(traits)), data{ new DataStorage() } {}
	Event::Event(tr_set traits, DataStorage* eData) : activation_traits(std::move(traits)), data{ eData } {}
	Event::~Event() {
		delete data;
	}
	Event::Event(std::initializer_list<traits::Trait> ilist) : activation_traits(ilist), data{ new DataStorage() } {}

	void Event::changeActivationTraits(traits::tr_set actTraits) {
		activation_traits = std::move(actTraits);
	}

} // events
