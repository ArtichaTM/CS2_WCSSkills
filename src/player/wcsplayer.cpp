#include "wcsplayer.hpp"

using std::shared_ptr;
using events::Event;
using events::ReturnEvent;
using stateff::Skill;
using stateff::Leftover;


WCSPlayer::WCSPlayer(float level, std::vector<managers::SkillInfo> selected_skills) :
	traits(),
	level(level),
	status_effects(),
	skills(),
	skillsSelected(),
	leftovers()
{

}

WCSPlayer::~WCSPlayer() {
	for (auto& [_, value] : skills) {
		delete value;
	}
}

ReturnEvent WCSPlayer::deactivateSkills(shared_ptr<Event> const& e) {
	for (auto& [_, value] : skills) {
		delete value;
	}
	skills.clear();
	traits.clear();
	return ReturnEvent::PASS;
}

ReturnEvent WCSPlayer::activateSkills(shared_ptr<Event> const& e) {
	for (auto& [_, skillInfo] : skillsSelected) {
		skills[skillInfo->name] = new Skill(this, skillInfo);
	}
	return ReturnEvent::PASS;
}

ReturnEvent WCSPlayer::spawn(const shared_ptr<events::Event>& e) {
	for (auto const& [id, skill_info] : skillsSelected) {
		ReturnEvent answer = skill_info->applySkill<false>(this);
	}
	return ReturnEvent::PASS;
}


EDATA_SIMPLE_DEFINE_CONST(WCSPlayer, WCSP)
