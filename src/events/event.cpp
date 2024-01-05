#include "event.hpp"
#include "../managers/infomanager.hpp"

#include <utility>

using std::shared_ptr;
using std::string;
using std::function;
using std::unordered_map;
using std::queue;
using std::bitset;
using traits::tr_set;
using dataStorage::DataStorage;
using dataStorage::DoubleLinkedList;
using dataStorage::make;
using managers::InfoManager;

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
		InfoManager* manager(InfoManager::getManager());
		bitset<TRAIT_INDEX_MAX> traits_bitset(
			manager->trset_to_bitset<TRAIT_INDEX_MAX>(activation_traits)
		);
		if (!this->registered_events[activation_traits.size()].contains(traits_bitset)) {
			this->registered_events[activation_traits.size()].insert({ traits_bitset, new vectorofFunctions()});
		}
		auto dolili = this->registered_events[activation_traits.size()].at(traits_bitset);
		Function* func = new Function(eventF);
		dolili->insertAfter(dolili->tail, func);
		return func;
	}

	void EventManager::unregisterForEvent(const tr_set& activation_traits, Function* eventF) {
		InfoManager* manager(InfoManager::getManager());
		bitset<TRAIT_INDEX_MAX> traits_bitset(
			manager->trset_to_bitset<TRAIT_INDEX_MAX>(activation_traits)
		);
		if (!this->registered_events[activation_traits.size()].contains(traits_bitset)) {
			throw NoSuchReceiver();
		}
		vectorofFunctions* functions = this->registered_events[activation_traits.size()].at(traits_bitset);
		dataStorage::DoubleLinkedListNode<Function*>* eventRecNode = functions->findNode([&](Function* receiver) {
			return &(*receiver) == &(*eventF);
		});
		if (!eventRecNode) {
			throw NoSuchReceiver();
		}
		functions->erase(eventRecNode);
		if (!functions->size()) {
			delete functions;
			this->registered_events[activation_traits.size()].erase(traits_bitset);
		}
		delete eventF;
	}

	void EventManager::iterateOverEvents() {
		lateRunEvents->head->data->initialEvent = true;
		iterating = true;
		InfoManager* manager(InfoManager::getManager());
		do {
			std::shared_ptr<Event> event = this->lateRunEvents->head->data;
			this->lateRunEvents->erase(this->lateRunEvents->head);
			bitset<TRAIT_INDEX_MAX> traits_bitset(
				manager->trset_to_bitset<TRAIT_INDEX_MAX>(event->activation_traits)
			);
			if (!this->registered_events[event->activation_traits.size()].contains(traits_bitset)) {
				continue;
			}
			vectorofFunctions* functions = this->registered_events[
				event->activation_traits.size()
			].at(traits_bitset);
			dataStorage::DoubleLinkedListNode<Function*>* eventF = functions->head;
			while (eventF) {
				(*(eventF->data))(event);
				if (event->result != PASS) {
					break;
				}
				eventF = eventF->getNext();
			}
		} while (this->lateRunEvents->head);
		iterating = false;
	}
	
	void EventManager::fireEvent(std::shared_ptr<Event> event) {
		lateRunEvents->insertAfter(lateRunEvents->tail, event);
		if (!iterating) this->iterateOverEvents();
	}
	
	bool EventManager::receiversExist(const tr_set& activation_traits) {
		return this->registered_events[activation_traits.size()].contains(
			InfoManager::getManager()->trset_to_bitset<TRAIT_INDEX_MAX>(activation_traits)
		);
	}

	EventManager::EventManager() : registered_events(), lateRunEvents(new DoubleLinkedList<shared_ptr<Event>>()){
		for (unsigned char i = 0; i < MAX_TRAITS_IN_TRSET; i++) {
			registered_events[i] = receivers_map();
		}
	}

	EventManager::~EventManager() {
		for (unsigned char i = 0; i < MAX_TRAITS_IN_TRSET; i++) {
			for (auto iter = registered_events[i].begin(); iter != registered_events[i].end(); iter++) {
				delete (*iter).second;
			}
		}
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
