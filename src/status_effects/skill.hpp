#ifndef SRC_STATUSEFFECTS_SKILL
#define SRC_STATUSEFFECTS_SKILL

namespace stateff {
	class Skill;
}

#include "../managers/infomanager.hpp"
#include "../player/wcsplayer.hpp"

namespace stateff {
	class Skill {
		managers::SkillInfo* info;
		WCSPlayer* owner;
	};
}

#endif // SRC_PLAYER_SKILL