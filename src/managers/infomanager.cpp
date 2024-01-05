#include <fstream>
#include <vector>
#include "infomanager.hpp"
#include "../paths/paths.hpp"


using std::make_shared;
using std::shared_ptr;
using std::unordered_set;
using std::ifstream;
using nlohmann::json;
using nlohmann::detail::value_t;
using traits::Trait;
using traits::make;
using dataStorage::DataStorage;
using dataStorage::make;
using stateff::StatusEffect;
using functions::Functions;


namespace managers {
	InfoManager* InfoManager::instance = nullptr;
	TRAIT_INDEX_TYPE TraitInfo::traits_amount = 0;

	void InfoManager::init() {
		if (instance) {
			throw InfoManagerRecreating("IMR1");
			return;
		}
		instance = new InfoManager();
	}

	void InfoManager::close() {
		if (!instance) {
			throw InfoManagerReclosing("IMR2");
			return;
		}
		delete instance;
		instance = nullptr;
	}
	
	InfoManager* InfoManager::getManager() {
		return instance;
	}

	InfoManager::InfoManager() : se(), skills(), traits() {
		// Status effects
		ifstream f = ifstream(Paths::getInstance()->se);
		json basicInfo = json::parse(f);
		se.reserve(basicInfo.size());
		assert(basicInfo.is_array());
		for (auto& el : basicInfo) {
			assert(el.is_array());
			se.emplace(el.at(0), new SEInfo(el));
		}
		f.close();

		// Skills
		f = ifstream(Paths::getInstance()->skills);
		basicInfo = json::parse(f);
		skills.reserve(basicInfo.size());
		assert(basicInfo.is_object());
		for (auto& [key, value] : basicInfo.items()) {
			skills.emplace(key, new SkillInfo(se, value));
		}
		f.close();

		// Traits
		f = ifstream(Paths::getInstance()->traits);
		basicInfo = json::parse(f);
		traits.reserve(basicInfo.size());
		if (basicInfo.size() > TRAIT_INDEX_MAX) {
			META_CONPRINTF(
				"There's more traits in file that available for type, contained in TRAIT_INDEX_TYPE (%llo > %llo)",
				basicInfo.size(),
				TRAIT_INDEX_MAX
			);
		}
		for (auto& [key2, value] : basicInfo.items()) {
			Trait key = Trait(std::stoi(key2));
			traits[key] = new TraitInfo(key, value);
		}
		f.close();
	}

	InfoManager::~InfoManager() {
		for (auto& [key, value] : skills) {
			delete value;
		}
		for (auto& [key, value] : se) {
			delete value;
		}
		for (auto& [key, value] : traits) {
			delete value;
		}
	}

	TRAIT_INDEX_TYPE managers::InfoManager::bit_index(Trait trait)
	{
		return InfoManager::getManager()->traits.at(trait)->bitwise_index;
	}

	SkillInfo::SkillInfo(se_map& se, json& info) :
			name(info["Name"]),
			description(info["Description"]),
			menu_description(info["Menu Description"]),
			effects(),
			traits()
	{
		effects.reserve(info["Effects"].size());
		SkillSE* skillSe;
		for (auto& el : info["Effects"]) {
			skillSe = new SkillSE(se, el);
			traits.insert(skillSe->seInfo->traits.begin(), skillSe->seInfo->traits.end());
			effects.emplace_back(skillSe);
		}
	}

	SkillSE::SkillSE(se_map& se, json& info)
		: seInfo(se.at(info.at("Id"))),
		arguments(make<DataStorage>(true, info)) {}

	SkillSE::~SkillSE() {
		for (auto& [key, value] : *(this->arguments)) {
			delete value;
		}
		delete arguments;
	}

	SEInfo::SEInfo(json& info) 
		: id(info[0].get<unsigned int>()),
		traits(traits_init(info[1])),
		activation_traits(traits_init(info[2])),
		function(function_init(info[3])) {}

	SEInfo::~SEInfo() {
	}

	traits::tr_set SEInfo::traits_init(json& info) {
		auto set = traits::tr_set();
		for (auto& value : info) {
			assert(value.is_number_unsigned());
			set.emplace(value.get<unsigned int>());
		}
		return set;
	}

	functions::function SEInfo::function_init(json& info) {
		auto* funcManager = Functions::get();
		const std::string& func_name = info.get<std::string>();
#ifdef DEBUG
		if (!info.is_string()) {
			throw CustomException("Wrong info type");
		}
		if (!(*funcManager)->contains(func_name)) {
			throw CustomException("FUNCS2");
		}
#endif
		return (*funcManager)->at(func_name);
	}

	TraitInfo::TraitInfo(traits::Trait _id, json& info) :
		id(_id)
		, name(info["name"])
		, mutual_exclusive_category(make(info.value("mutual_exclusive_category", unordered_set<traits::inner_type>())))
		, enemy_traits(make(info.value("enemy_traits", unordered_set<traits::inner_type>())))
		, bitwise_index(traits_amount)
	{
		traits_amount++;
	}

} // skills


EDATA_SIMPLE_DEFINE_CONST(managers::SEInfo, SEInfo)
EDATA_SIMPLE_DEFINE_CONST(managers::SkillSE, SkillSE)
EDATA_SIMPLE_DEFINE_CONST(managers::SkillInfo, SkillInfo)
