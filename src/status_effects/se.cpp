#include "se.hpp"

#include <memory>
#include <functional>

using std::shared_ptr;
using traits::tr_set;
using events::Event;
using events::EventManager;
using events::ReturnEvent;
using dataStorage::DataStorage;

using namespace std;

namespace stateff {
	StatusEffect::StatusEffect
	(
		const WCSPlayer* _owner,
		const managers::SEInfo* _info,
		const DataStorage* _arguments,
		const float* multiplier
	)
	: owner(_owner), info(_info), arguments(new DataStorage(*_arguments)),
		eventReceiver(EventManager::getManager()->registerForEvent(info->activation_traits, bind(&StatusEffect::execute, this, placeholders::_1)))
	{

		auto* eventManager = EventManager::getManager();
#ifdef DEBUG
		if (!arguments->contains("Multiplier")) {
			throw CustomException("There's no multplier in arguments. Wrong effect info?");
		}
#endif
		arguments->at("Multiplier")->changeData(
			(std::function<void(float&)>)
			[&multiplier](float& value) mutable -> void
			{ value *= *multiplier; }
		);
	}

	StatusEffect::~StatusEffect() {
		auto* eventManager = events::EventManager::getManager();
		eventManager->unregisterForEvent(info->activation_traits, eventReceiver);
		delete arguments;
	}
	
	ReturnEvent StatusEffect::execute(shared_ptr<Event> e) {
		auto* evmanager = EventManager::getManager();
		auto effect_execute_event = std::make_shared<Event>(info->traits);
		
		effect_execute_event->setConstData("target", owner);
		effect_execute_event->setConstData("seinfo", info);
		effect_execute_event->setData<false>("arguments", arguments);
		
		evmanager->fireEvent(effect_execute_event);
		if (effect_execute_event->result == events::PASS) {
			return info->function(e, *arguments);
		}
		return effect_execute_event->result;
	}

	tr_set StatusEffect::getTraits() const noexcept {
		return info->traits;
	}
	
	bool StatusEffect::is_unremovable() const noexcept {
		try {
			dataStorage::EData* parameter = arguments->at("unremovable");
			return parameter->getData<bool>();
		} catch (std::out_of_range) {
			return false;
		}
	}
} // stateff

EDATA_SIMPLE_DEFINE(stateff::SE_target, SE_target)