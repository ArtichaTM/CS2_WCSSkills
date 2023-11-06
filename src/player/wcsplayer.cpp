#include "wcsplayer.hpp"

using std::shared_ptr;
using events::Event;
using events::ReturnEvent;
using managers::SkillInfo;
using stateff::Leftover;


WCSPlayer::WCSPlayer(float level, std::vector<managers::SkillInfo*> selected_skills) :
	traits(),
	level(level),
	status_effects(),
	leftovers()
{
	unsigned short i = 0;
	for (; i < selected_skills.size(); i++) {
		skills_selected[i] = selected_skills[i];
	}
	for (; i < SKILLS_MAX; i++) {
		skills_selected[i] = nullptr;
	}
}

WCSPlayer::~WCSPlayer() {

}

ReturnEvent WCSPlayer::deactivateSkills(shared_ptr<Event> const& e) {
	traits.clear();
	return ReturnEvent::PASS;
}

ReturnEvent WCSPlayer::activateSkills(shared_ptr<Event> const& e) {
	for (unsigned short i = 0; i < SKILLS_MAX; i++) {
		SkillInfo* skill_info = skills_selected[i];
		if (skill_info) {
			ReturnEvent answer = skill_info->applySkill(this, i);
		}
	}
	return ReturnEvent::PASS;
}

ReturnEvent WCSPlayer::spawn(const shared_ptr<events::Event>& e) {
	for (unsigned short i = 0; i < SKILLS_MAX; i++) {
		SkillInfo* skill_info = this->skills_selected[i];
		if (!skill_info) {
			break;
		}
		ReturnEvent _ = skill_info->applySkill(this, i);
	}
	return ReturnEvent::PASS;
}


EDATA_SIMPLE_DEFINE_CONST(WCSPlayer, WCSP)
