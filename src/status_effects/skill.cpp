#include "skill.hpp"

using managers::SkillInfo;

namespace stateff {
	Skill::Skill(SkillInfo* _info, WCSPlayer* _owner, unsigned short _slot) :
		info(_info), owner(_owner), slot(_slot) {}
}