#include "event.hpp"
#include "../managers/infomanager.hpp"

#include <utility>

using std::shared_ptr;
using std::string;
using std::function;
using std::unordered_map;
using std::queue;
using std::bitset;
using std::pair;
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


	Function::Function(EventReceiver receiver, bitset<TRAIT_INDEX_MAX> _activation_bitset)
		: inner_function(receiver), activation_bitset(_activation_bitset) {}
	
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
		Function* func = new Function(eventF, traits_bitset);
		dolili->insertAfter(dolili->tail, func);
		return func;
	}

	void EventManager::unregisterForEvent(Function* eventF) {
		InfoManager* manager(InfoManager::getManager());
		TRAIT_INDEX_TYPE traits_amount = eventF->activation_bitset.count();
		if (!this->registered_events[traits_amount].contains(eventF->activation_bitset)) {
			throw NoSuchReceiver();
		}
		vectorofFunctions* functions = this->registered_events[traits_amount].at(eventF->activation_bitset);
		dataStorage::DoubleLinkedListNode<Function*>* eventRecNode = functions->findNode([&](Function* receiver) {
			return &(*receiver) == &(*eventF);
		});
		if (!eventRecNode) {
			throw NoSuchReceiver();
		}
		functions->erase(eventRecNode);
		if (!functions->size()) {
			delete functions;
			this->registered_events[traits_amount].erase(eventF->activation_bitset);
		}
		delete eventF;
	}


	void EventManager::fire_during_iteration(vectorofFunctions* functions, std::shared_ptr<Event> event) {
		dataStorage::DoubleLinkedListNode<Function*>* eventF = functions->head;
		while (eventF) {
			(*(eventF->data))(event);
			if (event->result != PASS) {
				break;
			}
			eventF = eventF->getNext();
		}
	}

	void EventManager::iterateOverEvents() {
		lateRunEvents->head->data->initialEvent = true;
		iterating = true;
		InfoManager* manager(InfoManager::getManager());
		do {
			// Getting activation bitset
			std::shared_ptr<Event> event = this->lateRunEvents->head->data;
			unsigned char event_traits_amount = event->activation_traits.size();
			this->lateRunEvents->erase(this->lateRunEvents->head);
			bitset<TRAIT_INDEX_MAX> traits_bitset(
				manager->trset_to_bitset<TRAIT_INDEX_MAX>(event->activation_traits)
			);

			// Checking membership of such traits
			if (!this->registered_events[event_traits_amount].contains(traits_bitset)) {
				continue;
			}

			// Actual event firing
			fire_during_iteration(this->registered_events[event_traits_amount].at(traits_bitset), event);

			// Running through registered events of size below current
			for (; event_traits_amount > 0; event_traits_amount--) {
				for (auto& [i_bitset, i_receivers] : registered_events[event_traits_amount-1]) {
					if (
						i_bitset.count()
						!=
						(traits_bitset & i_bitset).count()
						) continue;
					assert(true);
					fire_during_iteration(i_receivers, event);
				}
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
