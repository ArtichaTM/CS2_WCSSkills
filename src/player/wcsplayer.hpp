#ifndef WCSSKILLS_WCSPLAYER
#define WCSSKILLS_WCSPLAYER

#include <unordered_map>
#include <unordered_set>

class WCSPlayer;

#include "../includes/doublelinkedlist.hpp"
#include "../traits/traits.hpp"
#include "../status_effects/skill.hpp"
#include "../status_effects/se.hpp"
#include "../status_effects/leftovers.hpp"
#include "../events/event.hpp"


class WCSPlayer {
public:
    traits::tr_set traits;
    float level;
    std::unordered_multimap<unsigned int, stateff::StatusEffect*> status_effects;
    std::unordered_map<std::string, stateff::Skill*> skills;
    std::unordered_map<unsigned short, managers::SkillInfo*> skillsSelected;
    dataStorage::DoubleLinkedList<stateff::Leftover> leftovers;
    
    [[nodiscard]] events::ReturnEvent activateSkills(std::shared_ptr<events::Event> const&);
    [[nodiscard]] events::ReturnEvent deactivateSkills(std::shared_ptr<events::Event> const&);
    [[nodiscard]] events::ReturnEvent spawn(std::shared_ptr<events::Event> const&);
};


#endif //WCSSKILLS_WCSPLAYER
