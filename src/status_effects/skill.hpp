#ifndef SRC_STATUSEFFECTS_SKILL
#define SRC_STATUSEFFECTS_SKILL

namespace stateff {
	class Skill;
}

#include "../managers/infomanager.hpp"
#include "../player/wcsplayer.hpp"

namespace stateff {
	class Skill {
	public:
		Skill(Skill const&) = delete;
		Skill() = delete;
		Skill(managers::SkillInfo*, WCSPlayer*, unsigned short);

		managers::SkillInfo* info;
		WCSPlayer* owner;
		unsigned short slot;
	};
}

#endif // SRC_PLAYER_SKILL