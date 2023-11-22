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
//#ifdef CMAKE
		if (instance) {
			throw InfoManagerRecreating("Trying to init already initialized InfoManager");
			return;
		}
		instance = new InfoManager();
//#endif // CMAKE
	}

	void InfoManager::close() {
//#ifdef CMAKE
		if (!instance) {
			throw InfoManagerReclosing("Trying to close already closed InfoManager");
			return;
		}
		delete instance;
		instance = nullptr;
//#endif // CMAKE
	}
	
	InfoManager* InfoManager::getManager() {
		return instance;
	}

	InfoManager::InfoManager() : se(), skills(), traits() {
#ifndef CMAKE
		// Status effects
		std::ifstream f = std::ifstream(Paths::getInstance()->se);
		/*json basicInfo = json::parse(f);
		se.reserve(basicInfo.size());
		assert(basicInfo.is_array());
		for (auto& el : basicInfo) {
			assert(el.is_array());
			se.emplace(el.at(0), new SEInfo(el));
		}*/
		f.close();
#else // CMAKE
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
#endif // CMAKE
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

	template<bool force>
	ReturnEvent SkillInfo::applySkill(WCSPlayer* target, unsigned char& index) {
		auto skillActivateE = make_shared<Event>(traits::tr_set{247});
		
		skillActivateE->setConstData("target", target);
		skillActivateE->setConstData("skill",  this   );
		skillActivateE->setData<true>("slot", new unsigned short(index));

		if constexpr (force) {
			skillActivateE->setData<true>("force", new bool(true));
		}
		
		EventManager::getManager()->fireEvent(skillActivateE);
		if constexpr (!force) {
			if (skillActivateE->result != ReturnEvent::PASS) {
				return skillActivateE->result;
			}
		}
		
		for (shared_ptr<SkillSE> const& sse : effects) {
			ReturnEvent output = sse->applyStatusEffect<force>(target);
		}
		
		return skillActivateE->result;
	}

	template ReturnEvent SkillInfo::applySkill<false>(WCSPlayer*, unsigned char&);
	template ReturnEvent SkillInfo::applySkill<true >(WCSPlayer*, unsigned char&);

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
			throw CustomException("Can't find specific function in functions list");
		}
#endif
		return (*funcManager)->at(func_name);
	}

	template<bool force>
	events::ReturnEvent SkillSE::applyStatusEffect(WCSPlayer* wcsp) {
		auto* eventManager = EventManager::getManager();
		
		// Effect apply event
		auto event = std::make_shared<Event>(tr_set{248});
		event->setConstData("target", wcsp);
		event->setConstData("seinfo", this->seInfo);
		event->setData<true>("multiplier", new float(1.));
		if constexpr (force) {
			event->setData<true>("force", new bool(true));
		}
		eventManager->fireEvent(event);
		if constexpr (!force) {
			if (event->result != ReturnEvent::PASS) {
				return event->result;
			}
		}
		
		wcsp->status_effects.insertAfter(
			wcsp->status_effects.tail,
			std::make_shared<StatusEffect>(
				wcsp,
				this->seInfo,
				this->arguments,
				event->getData<float>("multiplier")
			)
		);
		
		return event->result;
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
