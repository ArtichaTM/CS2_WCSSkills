#ifndef WCSSKILLS_WCSPLAYER
#define WCSSKILLS_WCSPLAYER

#include <unordered_map>
#include <unordered_set>
#include <vector>

class WCSPlayer;

#ifndef SKILLS_MAX
#define SKILLS_MAX 6
#endif

#include "../includes/doublelinkedlist.hpp"
#include "../traits/traits.hpp"
#include "../status_effects/se.hpp"
#include "../status_effects/leftovers.hpp"
#include "../status_effects/skill.hpp"
#include "../events/event.hpp"


class WCSPlayer {
public:
	WCSPlayer(WCSPlayer const&) = delete;
	WCSPlayer(unsigned short level, unsigned short xp, std::vector<managers::SkillInfo*> selected_skills);
	~WCSPlayer();
	traits::tr_set traits;
	unsigned short level;
	unsigned int xp;
	dataStorage::DoubleLinkedList<std::shared_ptr<stateff::StatusEffect>> status_effects;
	managers::SkillInfo* skills_selected[SKILLS_MAX];
	stateff::Skill* skills_activated[SKILLS_MAX];
	dataStorage::DoubleLinkedList<stateff::Leftover> leftovers;
	
	[[nodiscard]] events::ReturnEvent activateSkills(std::shared_ptr<events::Event> const&);
	[[nodiscard]] events::ReturnEvent deactivateSkills(std::shared_ptr<events::Event> const&);
	[[nodiscard]] events::ReturnEvent spawn(std::shared_ptr<events::Event> const&);
};


#endif //WCSSKILLS_WCSPLAYER