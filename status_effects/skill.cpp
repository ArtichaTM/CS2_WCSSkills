#include "skill.hpp"
#include "se.hpp"

namespace stateff {
    Skill::Skill(WCSPlayer* _owner, managers::SkillInfo* _info) : info(_info), owner(_owner) {}

    Skill::~Skill() {}

    traits::tr_set Skill::getTraits() const noexcept {
        return info->traits;
    }

    void Skill::activate() {
        for (auto& effect: info->effects) {
            
        }
    }
} // skills
