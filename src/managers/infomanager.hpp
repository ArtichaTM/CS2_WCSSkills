#ifndef WCSSKILLS_MANAGERS_INFOMANAGER
#define WCSSKILLS_MANAGERS_INFOMANAGER

#include "../includes/exceptions.hpp"

namespace managers {
	class SEInfo;
	class SkillSE;
	class SkillInfo;
	class InfoManager;
	
	class NoSuchSkill : public CustomException { using CustomException::CustomException; };
	class InfoManagerRecreating : public CustomException { using CustomException::CustomException; };
	class InfoManagerReclosing : public CustomException { using CustomException::CustomException; };
}

#include <vector>
#include <unordered_map>
#include <bitset>
#include "../defines.h"
#include "../includes/json.hpp"
#include "../traits/traits.hpp"
#include "../player/wcsplayer.hpp"
#include "../functions/all.hpp"

using nlohmann::json;

namespace managers {

	/**
	 * Contains information about specific trait. Usually it's:
	 * � Mutual exclusive traits: this traits cames from one category and can't be included in the same set
	 * � Enemy traits: this traits can't be in the same set because they directly opposite
	 * 
	 * There's no validators for this outside debug mode, so they carries only information matter
	 */
	class TraitInfo {
		friend InfoManager;
		explicit TraitInfo(traits::Trait, json&);
	public:
		static TRAIT_INDEX_TYPE traits_amount;
		TraitInfo(TraitInfo const&) = delete;
		const traits::Trait id;
		const TRAIT_INDEX_TYPE bitwise_index;
		const std::string name;
		const traits::tr_set mutual_exclusive_category;
		const traits::tr_set enemy_traits;
	};
	
	/**
	 * Contains information about specific buff/debuff/instant effects.
	 * Members:
	 * � id: id of effect. Each map uses this id as key
	 * � traits: traits, which will be applied to user on effect apply
	 * � activation_traits: on which event traits effect function will be called
	 * � function: function that will be called on event, that contains ALL activation_traits or more
	 */
	class SEInfo {
		friend InfoManager;
		static traits::tr_set traits_init(json&);
		static functions::function function_init(json&);
		explicit SEInfo(json&);
	public:
		~SEInfo();
		const unsigned int id;
		const traits::tr_set traits;
		const traits::tr_set activation_traits;
		const functions::function function;
	};
	
	typedef std::unordered_map<unsigned int, SEInfo*> se_map;

	/**
	 * Contains information about buff/debuff applied by Skill on activation
	 * Members:
	 * � seInfo: information about target effect
	 * � arguments: arguments which will be passed to target function (seInfo->function)
	 *
	 * Notes:
	 * � 2 different SkillSE classes can point to one SEInfo with different arguments
	 * � SkillSE can't be initialized, if target seInfo can't be found in InfoManager.se_map.
	 * This usually happenes when ID of special effect (SE for short) defined in skills.json skill effect, but
	 * not described in se.json
	 */
	class SkillSE {
		friend SkillInfo;
		explicit SkillSE(se_map&, json&);
	public:
		~SkillSE();
		const SEInfo* seInfo;
		const dataStorage::DataStorage* arguments;
	};

	/**
	 * Contains information about skill.
	 * Members:
	 * � name: Name of the skill, displayed in skills menu
	 * � description: describes short information about skill for user
	 * � menu_description: describes detailed information about skill for user
	 * � traits: Which traits applied to user on skill equip (they can conflict!)
	 * � effects: Information about buff applied on user (can be on party, on enemy, on map, ...)
	 */
	class SkillInfo {
		friend InfoManager;
		explicit SkillInfo(se_map&, json&);
	public:
		~SkillInfo() = default;
		const std::string name;
		const std::string description;
		const std::string menu_description;
		traits::tr_set traits;
		std::vector<std::shared_ptr<SkillSE>> effects;
	};
	

	/**
	 * Contains all information about skills from files:
	 * 1. data/se.json
	 * 2. data/skills.json
	 * 3. data/traits.json
	 * 
	 * Storage of classes SkillInfo, SEInfo.
	 */
	class InfoManager {
	protected:
		explicit InfoManager();
		~InfoManager();
		static InfoManager* instance;
	public:
		static void init();
		static void close();
		
		std::unordered_map<std::string, SkillInfo*> skills;
		se_map se;
		std::unordered_map<traits::Trait, TraitInfo*> traits;

		static InfoManager* getManager();

		TRAIT_INDEX_TYPE bit_index(traits::Trait);

		template<TRAIT_INDEX_TYPE N>
		std::bitset<N> trset_to_bitset(traits::tr_set const& traits) {
			std::bitset<N> bitset;
			InfoManager* manager = managers::InfoManager::getManager();
			for (auto& trait : traits) {
				bitset[bit_index(trait)] = 1;
			}
			return bitset;
		}
	};
} // skills

#endif //WCSSKILLS_MANAGERS_INFOMANAGER
