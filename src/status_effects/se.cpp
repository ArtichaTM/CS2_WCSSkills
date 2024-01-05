#include <memory>
#include <functional>

#include "se.hpp"
#include "../includes/ticker.hpp"

using std::shared_ptr;
using traits::tr_set;
using events::Event;
using events::EventManager;
using events::ReturnEvent;
using dataStorage::DataStorage;

using std::shared_ptr;
using events::EventManager;
using utilities::Ticker;

namespace stateff {
	StatusEffect::StatusEffect
	(
		WCSPlayer* _owner,
		const managers::SEInfo* _info,
		const DataStorage* _arguments,
		const float* multiplier
	)
	: owner(_owner), info(_info), arguments(new DataStorage(*_arguments)),
		eventReceiver(EventManager::getManager()->registerForEvent(info->activation_traits,
			bind(&StatusEffect::execute, this, std::placeholders::_1)))
	{
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

		if (arguments->contains("Duration")) {
			Ticker* ticker = Ticker::getMainTicker();
			ticker->addTask(
				this, &StatusEffect::expire,
				*(arguments->at("Duration")->getData<float>())
			);
		}
	}

	StatusEffect::~StatusEffect() {
		auto* eventManager = EventManager::getManager();
		eventManager->unregisterForEvent(eventReceiver);
		delete arguments;
	}

	void StatusEffect::expire() {
		auto* node = this->owner->status_effects.findNode(
			[&](shared_ptr<StatusEffect> eff) { return eff.get() == this; }
		);
		this->owner->status_effects.erase(node);
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