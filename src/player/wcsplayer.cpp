#include "wcsplayer.hpp"

events::ReturnEvent WCSPlayer::deactivateSkills(std::shared_ptr<events::Event> const& e) {
    for (auto& n : skills) {
        delete n.second;
    }
    skills.clear();
    traits.clear();
    return events::ReturnEvent::PASS;
}

events::ReturnEvent WCSPlayer::activateSkills(std::shared_ptr<events::Event> const& e) {
    for (auto [_, skillInfo] : skillsSelected) {
        skills[skillInfo->name] = new stateff::Skill(this, skillInfo);
    }
    return events::ReturnEvent::PASS;
}

events::ReturnEvent WCSPlayer::spawn(const std::shared_ptr<events::Event>& e) {
    if (e->activation_traits.contains(246)) {
        // Spawn on round start
        e->setData<true>("roundStart", new bool(true));
        activateSkills(e);
    } else {
        // Spawn during round
        e->setData<true>("roundStart", new bool(false));
        activateSkills(e);
    }
    return events::ReturnEvent::PASS;
}


EDATA_SIMPLE_DEFINE_CONST(WCSPlayer, WCSP)
