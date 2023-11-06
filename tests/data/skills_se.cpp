#include <gtest/gtest.h>
#include <unordered_set>

#include "../../src/managers/infomanager.hpp"

using std::shared_ptr;
using managers::InfoManager;
using dataStorage::DataStorage;

namespace data_jsons {
	TEST(effect_apply, general) {
		InfoManager* manager = InfoManager::getManager();
		for (auto& [key, skill_info] : manager->skills) {

			// skill should contain at least 1 effect
			ASSERT_GE(skill_info->effects.size(), 1);
			
			// Checking arguments
			for (shared_ptr<managers::SkillSE> info : skill_info->effects) {
				DataStorage const* arguments = info->arguments;

				// Every effect on skill must contain multiplier
				ASSERT_TRUE(arguments->contains("Multiplier"));

				// Every effect on skill must contain Id
				ASSERT_TRUE(arguments->contains("Id"));

				// Every effect on skill must contain Times or Duration
				ASSERT_TRUE(arguments->contains("Times") || arguments->contains("Duration"));

				// Effect ID should be described in se.json
				unsigned int* effect_id = arguments->at("Id")->getData<unsigned int>();
				ASSERT_TRUE(manager->se.contains(*effect_id))
					<< "Skill "
					<< key
					<< " have effect with id "
					<< *effect_id
					<< ", but this effect doesn't described in se.json";
			}
		}
	}
}
