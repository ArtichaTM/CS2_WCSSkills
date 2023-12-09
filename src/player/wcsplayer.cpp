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
		events::ReturnEvent output = removeSkills(shared_ptr<Event>(new Event{ 249 }));
	}

	EventManager* manager = EventManager::getManager();
	if (spawn_event) {
		manager->unregisterForEvent({ 246 }, spawn_event);
	}

}

ReturnEvent WCSPlayer::removeSkills(shared_ptr<Event> const& e) {
	traits.clear();
	for (unsigned char i = 0; i < SKILLS_MAX; i++) {
		delete skills_activated[i];
		skills_activated[i] = nullptr;
	}
	return ReturnEvent::PASS;
}

ReturnEvent WCSPlayer::applySkills(shared_ptr<Event> const& e) {
	for (unsigned char i = 0; i < SKILLS_MAX; i++) {
		SkillInfo* skill_info = skills_selected[i];
		if (skill_info) {
			ReturnEvent answer = this->applySkill(i);
		}
	}
	return ReturnEvent::PASS;
}


ReturnEvent WCSPlayer::spawn(const shared_ptr<events::Event>& e) {
	return applySkills(e);
}


ReturnEvent WCSPlayer::despawn(const shared_ptr<events::Event>& e) {
	return this->removeSkills(e);
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


events::ReturnEvent WCSPlayer::applySkill(unsigned char& index) {
	auto skillApplyE = make_shared<Event>(traits::tr_set{ 247 });

	skillApplyE->setConstData("target", this);
	skillApplyE->setConstData("skill_info", this->skills_selected[index]);
	skillApplyE->setData<true>("slot", new unsigned char(index));

	EventManager::getManager()->fireEvent(skillApplyE);
	if (skillApplyE->result != ReturnEvent::PASS) {
		return skillApplyE->result;
	}

	WCSPlayer* target = skillApplyE->getData<WCSPlayer>("target");
	unsigned char* slot = skillApplyE->getData<unsigned char>("slot");
	SkillInfo* skill_info = skillApplyE->getData<SkillInfo>("skill_info");

	target->skills_activated[*slot] = new Skill(skill_info, target, *slot);

	/*for (shared_ptr<SkillSE> const& sse : this->skills_selected[index]->effects) {
		ReturnEvent output = this->applyStatusEffect<force>(sse.get());
	}*/

	return skillApplyE->result;
}


EDATA_SIMPLE_DEFINE_CONST(WCSPlayer, WCSP)
