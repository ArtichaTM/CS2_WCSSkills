#include <gtest/gtest.h>
#include <vector>

#include "../../src/managers/infomanager.hpp"
#include "../../src/traits/traits.hpp"
#include "../leaks.hpp"

using std::bitset;
using std::vector;
using std::pair;
using traits::Trait;
using traits::tr_set;
using managers::TraitInfo;


namespace managers {
	TEST(infomanager, bitset_all_traits) {
		MemoryLeakDetector _;
		InfoManager* manager = InfoManager::getManager();
		bitset<TRAIT_INDEX_MAX> values;
		TRAIT_INDEX_TYPE counter = 0;
		for (auto& trait : manager->traits) {
			ASSERT_TRUE(manager->traits.contains(trait.first));
			ASSERT_EQ(values.count(), counter++);
			values[manager->bit_index(trait.first)] = 1;
		}
		ASSERT_EQ(values.count(), manager->traits.size());
	}

	TEST(infomanager, trset_to_bitset_specific) {
		MemoryLeakDetector _;
		InfoManager* manager = InfoManager::getManager();
		vector<pair<const Trait, TraitInfo*>> all_traits;
		all_traits.reserve(manager->traits.size());
		for (auto& trait : manager->traits) {
			all_traits.push_back(trait);
		}
		for (auto& t_left : all_traits) {
			for (auto& t_right : all_traits) {
				if (t_left.first == t_right.first) continue;
				tr_set two_traits({ t_left.first, t_right.first });
				auto test_bitset(manager->trset_to_bitset<TRAIT_INDEX_MAX>(two_traits));
				ASSERT_EQ(test_bitset.size(), TRAIT_INDEX_MAX);
				ASSERT_EQ(test_bitset.count(), 2);
				ASSERT_EQ(test_bitset[t_left.second->bitwise_index], 1);
				ASSERT_EQ(test_bitset[t_right.second->bitwise_index], 1);
			}
		}
	}
}
