#include <functional>

#include "wcsplayer.hpp"

using std::shared_ptr;
using std::make_shared;
using std::bind;
using traits::tr_set;
using events::EventManager;
using events::Event;
using events::ReturnEvent;
using managers::SkillInfo;
using managers::SkillSE;
using stateff::Leftover;
using stateff::StatusEffect;
using stateff::Skill;


WCSPlayer::WCSPlayer(unsigned short level, unsigned short xp, std::vector<managers::SkillInfo*> selected_skills) :
	traits(),
	level(level),
	xp(xp),
	status_effects(),
	leftovers(),
	skills_activated()
{
	unsigned short i = 0;
	for (; i < selected_skills.size(); i++) {
		skills_selected[i] = selected_skills[i];
	}
	for (; i < SKILLS_MAX; i++) {
		skills_selected[i] = nullptr;
	}

	EventManager* manager = EventManager::getManager();
	spawn_event = manager->registerForEvent({ 246 }, bind(&WCSPlayer::spawn, this, std::placeholders::_1));
}

WCSPlayer::~WCSPlayer() {
	if (skills_activated[0] != nullptr) {
		events::ReturnEvent output = deactivateSkills(shared_ptr<Event>(new Event{ 249 }));
	}

	EventManager* manager = EventManager::getManager();
	if (spawn_event) {
		manager->unregisterForEvent({ 246 }, spawn_event);
	}

}

ReturnEvent WCSPlayer::deactivateSkills(shared_ptr<Event> const& e) {
	traits.clear();
	for (unsigned char i = 0; i < SKILLS_MAX; i++) {
		delete skills_activated[i];
		skills_activated[i] = nullptr;
	}
	return ReturnEvent::PASS;
}

template<bool force>
ReturnEvent WCSPlayer::activateSkills(shared_ptr<Event> const& e) {
	for (unsigned char i = 0; i < SKILLS_MAX; i++) {
		SkillInfo* skill_info = skills_selected[i];
		if (skill_info) {
			ReturnEvent answer = this->activateSkill<force>(i);
		}
	}
	return ReturnEvent::PASS;
}


template events::ReturnEvent WCSPlayer::activateSkills<true >(std::shared_ptr<events::Event> const&);
template events::ReturnEvent WCSPlayer::activateSkills<false>(std::shared_ptr<events::Event> const&);


ReturnEvent WCSPlayer::spawn(const shared_ptr<events::Event>& e) {
	return activateSkills<true>(e);
}


ReturnEvent WCSPlayer::despawn(const shared_ptr<events::Event>& e) {
	return this->deactivateSkills(e);
}

template<bool force>
events::ReturnEvent WCSPlayer::applyStatusEffect(managers::SkillSE* skill_se) {
	auto* eventManager = EventManager::getManager();

	// Effect apply event
	auto event = std::make_shared<Event>(tr_set {248});
	event->setConstData("target", this);
	event->setConstData("seInfo", skill_se->seInfo);
	event->setConstData("arguments", skill_se->arguments);
	event->setData<true>("multiplier", new float(1.));
	if constexpr (force) {
		event->setData<true>("force", new bool(force));
	}
	eventManager->fireEvent(event);
	if constexpr (!force) {
		if (event->result != ReturnEvent::PASS) {
			return event->result;
		}
	}

	this->status_effects.insertAfter(
		this->status_effects.tail,
		std::make_shared<StatusEffect>(
			this,
			skill_se->seInfo,
			skill_se->arguments,
			event->getData<float>("multiplier")
		)
	);

	return event->result;
}
template events::ReturnEvent WCSPlayer::applyStatusEffect<true >(managers::SkillSE* skill_se);
template events::ReturnEvent WCSPlayer::applyStatusEffect<false>(managers::SkillSE* skill_se);


template<bool force>
events::ReturnEvent WCSPlayer::activateSkill(unsigned char& index) {
	auto skillActivateE = make_shared<Event>(traits::tr_set{ 247 });

	skillActivateE->setConstData("target", this);
	skillActivateE->setConstData("skill_info", this->skills_selected[index]);
	skillActivateE->setData<true>("slot", new unsigned char(index));

	if constexpr (force) {
		skillActivateE->setData<true>("force", new bool(true));
	}

	EventManager::getManager()->fireEvent(skillActivateE);
	if constexpr (!force) {
		if (skillActivateE->result != ReturnEvent::PASS) {
			return skillActivateE->result;
		}
	}

	WCSPlayer* target = skillActivateE->getData<WCSPlayer>("target");
	unsigned char* slot = skillActivateE->getData<unsigned char>("slot");
	SkillInfo* skill_info = skillActivateE->getData<SkillInfo>("skill_info");

	target->skills_activated[*slot] = new Skill(skill_info, target, *slot);

	/*for (shared_ptr<SkillSE> const& sse : this->skills_selected[index]->effects) {
		ReturnEvent output = this->applyStatusEffect<force>(sse.get());
	}*/

	return skillActivateE->result;
}
template events::ReturnEvent WCSPlayer::activateSkill<true >(unsigned char& index);
template events::ReturnEvent WCSPlayer::activateSkill<false>(unsigned char& index);


EDATA_SIMPLE_DEFINE_CONST(WCSPlayer, WCSP)
