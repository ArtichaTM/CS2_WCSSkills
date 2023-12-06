#include <fstream>
#include <vector>
#include "infomanager.hpp"
#include "../paths/paths.hpp"

using namespace events;
using namespace traits;
using std::make_shared;
using std::shared_ptr;
using nlohmann::json;
using nlohmann::detail::value_t;
using dataStorage::DataStorage;
using dataStorage::make;
using stateff::StatusEffect;
using functions::Functions;


namespace managers {
	InfoManager* InfoManager::instance = nullptr;

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
		std::ifstream f = std::ifstream(Paths::getInstance()->se);
		json basicInfo = json::parse(f);
		se.reserve(basicInfo.size());
		assert(basicInfo.is_array());
		for (auto& el : basicInfo) {
			assert(el.is_array());
			se.emplace(el.at(0), new SEInfo(el));
		}
		f.close();

		// Skills
		f = std::ifstream(Paths::getInstance()->skills);
		basicInfo = json::parse(f);
		skills.reserve(basicInfo.size());
		assert(basicInfo.is_object());
		for (auto& [key, value] : basicInfo.items()) {
			skills.emplace(key, new SkillInfo(se, value));
		}
		f.close();

		// Traits
		f = std::ifstream(Paths::getInstance()->traits);
		basicInfo = json::parse(f);
		traits.reserve(basicInfo.size());
		for (auto& [key2, value] : basicInfo.items()) {
			traits::Trait key = traits::Trait(std::stoi(key2));
			traits[key] = new TraitInfo(key, value);
		}
		f.close();
	}

	InfoManager::~InfoManager() {
		for (auto& [key, value] : traits) {
			delete value;
		}
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
		arguments(make<DataStorage>(false, info)) {}

	SkillSE::~SkillSE() {
		for (auto& [key, value] : *(this->arguments)) {
			delete value;
		}
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
		, mutual_exclusive_category(traits::make(info.value("mutual_exclusive_category", std::unordered_set<traits::inner_type>())))
		, enemy_traits(traits::make(info.value("enemy_traits", std::unordered_set<traits::inner_type>())))
	{}

} // skills


EDATA_SIMPLE_DEFINE_CONST(managers::SEInfo, SEInfo)
EDATA_SIMPLE_DEFINE_CONST(managers::SkillSE, SkillSE)
EDATA_SIMPLE_DEFINE_CONST(managers::SkillInfo, SkillInfo)
