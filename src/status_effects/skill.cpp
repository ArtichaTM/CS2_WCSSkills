#include "skill.hpp"


namespace stateff {
	stateff::Skill::Skill(managers::SkillInfo* _info, WCSPlayer* _owner, unsigned short _slot) :
		info(_info), owner(_owner), slot(_slot) {}
}