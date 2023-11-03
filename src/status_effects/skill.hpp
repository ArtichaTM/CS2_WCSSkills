#ifndef WCSSKILLS_SKILL_HPP
#define WCSSKILLS_SKILL_HPP

namespace stateff {
    class Skill;
}

#include "se.hpp"
#include "../managers/infomanager.hpp"


namespace stateff {
    class Skill {
    public:
        Skill(WCSPlayer*, managers::SkillInfo*);
        ~Skill();
        
        WCSPlayer* owner;
        managers::SkillInfo* info;
        
        [[nodiscard]] traits::tr_set getTraits() const noexcept;
        void activate();
    };
} // skills

#endif //WCSSKILLS_SKILL_HPP
