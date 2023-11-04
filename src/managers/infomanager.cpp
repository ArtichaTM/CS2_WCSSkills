#include <fstream>
#include <vector>
#include "infomanager.hpp"
#include "../includes/paths.hpp"

using namespace events;
using namespace traits;
using std::make_shared;
using std::shared_ptr;
using nlohmann::json;
using nlohmann::detail::value_t;
using dataStorage::DataStorage;
using dataStorage::make;

namespace managers {
	InfoManager* InfoManager::instance = nullptr;

	void InfoManager::init() {
		assert(!instance);
		instance = new InfoManager();
	}

	void InfoManager::close() {
		assert(instance);
		delete instance;
		instance = nullptr;
	}
	
	InfoManager* InfoManager::getManager() {
		return instance;
	}

	InfoManager::InfoManager() : se(), skills(), traits() {
		// Status effects
		std::ifstream f = std::ifstream(paths::SE);
		json basicInfo = json::parse(f);
		se.reserve(basicInfo.size());
		assert(basicInfo.is_array());
		for (auto& el : basicInfo) {
			assert(el.is_array());
			se.emplace(el.at(0), new SEInfo(el));
		}
		f.close();
		
		// Skills
		f = std::ifstream(paths::SKILLS);
		basicInfo = json::parse(f);
		skills.reserve(basicInfo.size());
		assert(basicInfo.is_object());
		for (auto& [key, value] : basicInfo.items()) {
			skills.emplace(key, new SkillInfo(se, value));
		}
		f.close();
		
		// Traits
		f = std::ifstream(paths::TRAITS);
		basicInfo = json::parse(f);
		traits.reserve(basicInfo.size());
		for (auto& [key, value] : basicInfo.items()) {
			std::string* char_ptr = new std::string(value);
			traits[std::stoi(key)] = char_ptr;
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
			menu_description(info["Menu_Description"]),
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

	SkillInfo::~SkillInfo(){
	}

	template<bool force>
	events::ReturnEvent SkillInfo::applySkill(WCSPlayer* target) {
		auto* evManager = events::EventManager::getManager();
		auto skillActivateE = std::make_shared<Event>(traits::tr_set{247});
		
		skillActivateE->setData<false>("target", target         );
		skillActivateE->setData<false>("skill",  this           );
		skillActivateE->setData<true >("force",  new bool(force));
		
		evManager->fireEvent(skillActivateE);
		if (skillActivateE->result == BLOCK and !force) {
			return BLOCK;
		}
		
		for (SkillSE* const& sse : effects) {
			sse->applyStatusEffect<force>(target);
		}
		
		return PASS;
	}

	SkillSE::SkillSE(se_map& se, json& info)
		: seInfo(se.at(info.at("Id"))),
		arguments(make<DataStorage>(false, info)) {}

	SkillSE::~SkillSE() {}

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
		auto* funcManager = functions::Functions::get();
		const std::string func_name = info.get<std::string>();
#ifdef DEBUG
		if (!info.is_string()) {
			throw CustomException("Wrong info type");
		}
		if (!(*funcManager)->contains(func_name)) {
			throw CustomException("Can't find specific function in functions list");
		}
#endif
		return (*funcManager)->at(func_name);
	}

	template<bool force>
	events::ReturnEvent SkillSE::applyStatusEffect(WCSPlayer* wcsp) {
		auto* eventManager = EventManager::getManager();
		
		auto event = std::make_shared<Event>(traits::tr_set{248});
		event->setData<false>("target", wcsp);
		event->setData<true>("force", new bool(force));
		eventManager->fireEvent(event);
		if (event->result == BLOCK and !force) {
			return BLOCK;
		}
		
		wcsp->status_effects.insert({
			this->seInfo->id,
			std::make_shared<stateff::StatusEffect>(
					wcsp,
					this->seInfo,
					&this->arguments)
		});
		
		return PASS;
	}

} // skills


EDATA_SIMPLE_DEFINE(managers::SEInfo, SEInfo)
EDATA_SIMPLE_DEFINE_CONST(managers::SEInfo, SEInfo)
EDATA_SIMPLE_DEFINE(managers::SkillSE, SkillSE)
EDATA_SIMPLE_DEFINE_CONST(managers::SkillSE, SkillSE)
EDATA_SIMPLE_DEFINE(managers::SkillInfo, SkillInfo)
EDATA_SIMPLE_DEFINE_CONST(managers::SkillInfo, SkillInfo)
