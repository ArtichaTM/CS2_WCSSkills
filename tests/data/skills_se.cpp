#include <gtest/gtest.h>
#include <unordered_set>

#include "../../src/managers/infomanager.hpp"

using std::shared_ptr;
using managers::InfoManager;
using dataStorage::DataStorage;

namespace data_jsons {
	TEST(effect_apply, general) {
		InfoManager* manager = InfoManager::getManager();
		for (auto& [_, skill_info] : manager->skills) {
			for (shared_ptr<managers::SkillSE> info : skill_info->effects) {
				DataStorage arguments = info->arguments;

				// Every effect on skill must contain multiplier
				ASSERT_TRUE(arguments.contains("Multiplier"));

				// Every effect on skill must contain Id
				ASSERT_TRUE(arguments.contains("Id"));

				// Every effect on skill must contain Times or Duration
				ASSERT_TRUE(arguments.contains("Times") || arguments.contains("Duration"));

				// Effect ID should be described in se.json
				auto value = manager->se.contains(*(arguments.at("Id")->getData<unsigned int>()));
				//ASSERT_TRUE();
			}
		}
	}
}
