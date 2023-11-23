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
	class NoSuchReceiver : public std::exception {
	public:
		[[nodiscard]] const char * what() const noexcept override {
			return "There's no such receiver registered to unregister it";
		}
	};


	Function::Function(EventReceiver receiver) : inner_function(receiver) {}
	void Function::operator()(std::shared_ptr<Event> e) { return inner_function(e); }
	

	EventManager* EventManager::instance = nullptr;
	
	Function* EventManager::registerForEvent(const tr_set& activation_traits, EventReceiver eventF) {
		if (!this->registered_events->contains(activation_traits)) {
			this->registered_events->insert({activation_traits, new vectorofFunctions()});
		}
		auto dolili = this->registered_events->at(activation_traits);
		Function* func = new Function(eventF);
		dolili->insertAfter(dolili->tail, func);
		return func;
	}

	void EventManager::unregisterForEvent(const tr_set& activation_traits, Function* eventF) {
		if (!this->registered_events->contains(activation_traits)) {
			throw NoSuchReceiver();
		}
		vectorofFunctions* functions = this->registered_events->at(activation_traits);
		dataStorage::DoubleLinkedListNode<Function*>* eventRecNode = functions->findNode([&](Function* receiver) {
			return &(*receiver) == &(*eventF);
		});
		if (!eventRecNode) {
			throw NoSuchReceiver();
		}
		functions->erase(eventRecNode);
		if (!functions->size()) {
			delete functions;
			this->registered_events->erase(activation_traits);
		}
		delete eventF;
	}

	void EventManager::iterateOverEvents() {
		lateRunEvents->head->data->initialEvent = true;
		iterating = true;
		do {
			std::shared_ptr<Event> event = this->lateRunEvents->head->data;
			if (!this->registered_events->contains(event->activation_traits)) {
				continue;
			}
			vectorofFunctions* functions = this->registered_events->at(event->activation_traits);
			dataStorage::DoubleLinkedListNode<Function*>* eventF = functions->head;
			while (eventF) {
				(*(eventF->data))(event);
				if (event->result != PASS) {
					break;
				}
				eventF = eventF->getNext();
			}
			this->lateRunEvents->erase(this->lateRunEvents->head);
		} while (this->lateRunEvents->head);
		iterating = false;
	}
	
	void EventManager::fireEvent(std::shared_ptr<Event> event) {
		lateRunEvents->insertAfter(lateRunEvents->tail, event);
		if (!iterating) this->iterateOverEvents();
	}
	
	bool EventManager::receiversExist(const tr_set& activation_traits) {
		return this->registered_events->contains(activation_traits);
	}

	EventManager::EventManager() {}

	EventManager::~EventManager() {
		for (auto iter = registered_events->begin(); iter != registered_events->end(); iter++) {
			delete (*iter).second;
		}
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

	bool EventManager::initialized() { return instance != nullptr; }
	
	EventManager* EventManager::getManager() {
		if (!EventManager::instance) {
			throw EventManager::NotInitialized();
		}
		return EventManager::instance;
	}
	
	Event::Event(tr_set traits) : activation_traits(std::move(traits)), data{ new DataStorage() } {}
	Event::Event(tr_set traits, DataStorage* eData) : activation_traits(std::move(traits)), data{ eData } {}
	Event::~Event() {
		for (auto& [key, value] : *data) {
			delete value;
		}
		delete data;
	}
	Event::Event(std::initializer_list<traits::Trait> ilist) : activation_traits(ilist), data{ new DataStorage() } {}

	void Event::changeActivationTraits(traits::tr_set actTraits) {
		activation_traits = std::move(actTraits);
	}

} // events
