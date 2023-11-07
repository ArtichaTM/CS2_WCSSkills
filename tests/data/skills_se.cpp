#include <gtest/gtest.h>
#include <unordered_set>

#include "../../src/managers/infomanager.hpp"
#include "../leaks.hpp"

using std::shared_ptr;
using traits::Trait;
using managers::InfoManager;
using dataStorage::DataStorage;

namespace data_jsons {
	TEST(skills, general) {
		MemoryLeakDetector _;
		InfoManager* manager = InfoManager::getManager();
		for (auto const& [key, skill_info] : manager->skills) {

			ASSERT_GE(skill_info->effects.size(), 1)
				<< "Skill "
				<< key
				<< " doesn't contain at least one effect, which required";
			
			// Checking arguments
			for (shared_ptr<managers::SkillSE> info : skill_info->effects) {
				DataStorage const* arguments = info->arguments;

				ASSERT_TRUE(arguments->contains("Multiplier"))
					<< "Skill "
					<< key
					<< " doesn't contain \"Multiplier\" argument, which required";

				ASSERT_TRUE(arguments->contains("Id"))
					<< "Skill "
					<< key
					<< " doesn't contain \"Id\" argument, which required";

				ASSERT_TRUE(arguments->contains("Times") || arguments->contains("Duration"))
					<< "Skill "
					<< key
					<< " doesn't contain \"Times\" or \"Duration\" argument, which required ";

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
	
	TEST(se_traits, general) {
		MemoryLeakDetector _;
		InfoManager* manager = InfoManager::getManager();
		for (auto const& [key, seinfo] : manager->se) {
			for (Trait const& trait : seinfo->activation_traits) {
				ASSERT_TRUE(manager->traits.contains(trait)) <<
					"Trait #" << trait.id <<
					" contains in se.json as activation trait for se #" << key <<
					" but doesn't described in file traits.json";
			}
			for (Trait const& trait : seinfo->traits) {
				ASSERT_TRUE(manager->traits.contains(trait)) <<
					"Trait #" << trait.id <<
					" contains in se.json as se trait for se #" << key <<
					" but doesn't described in file traits.json";
			}
		}
	}
}
