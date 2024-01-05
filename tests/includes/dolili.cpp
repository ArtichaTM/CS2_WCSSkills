#include <gtest/gtest.h>
#include <unordered_set>

#include "../../src/includes/doublelinkedlist.hpp"

#include "../leaks.hpp"

#ifndef DOLILI_UNSIGNED_CHAR_NUMBER
#define DOLILI_UNSIGNED_CHAR_NUMBER 51341234
#endif

using namespace std;

namespace dataStorage {
	TEST(DoubleLinkedList, unordered_set_int) {
		MemoryLeakDetector _;
		auto dolili = dataStorage::DoubleLinkedList<unordered_set<int>>();

		unordered_set<int> new_s{ 12, 50 };
		dolili.insertAfter(nullptr, new_s);
		dolili.insertAfter(dolili.tail, { 123, 51903, 2 });
		dolili.insertAfter(dolili.tail, { 560123, 6324021, 51903 });
		dolili.insertAfter(dolili.tail, { 60123, -2312, 2321, 50 });

		auto* node = dolili.findNode([&](auto set)->bool { return set.contains(51903); });
		auto* node2 = node;
		node2 = nullptr;
		ASSERT_TRUE(node) << "nDid not found node1!";
		dolili.insertAfter(node, { 213, 4123 });

		node = dolili.findNodeReverse([&](auto set)->bool { return set.contains(51903); });
		node2 = dolili.findNodeReverse([&](auto set)->bool { return set.contains(12); });
		ASSERT_TRUE(node) << "nDid not found node1!";
		ASSERT_TRUE(node2) << "nDid not found node2!";
		dolili.erase(node);
		dolili.erase(node2);

		while (dolili.size()) dolili.erase(dolili.head);
	}

	TEST(DoubleLinkedList, unsigned_char) {
		MemoryLeakDetector _;
		auto dolili = dataStorage::DoubleLinkedList<unsigned char>();

		/* Base checks */
		ASSERT_EQ(dolili.size(), 0);
		ASSERT_EQ(dolili.head, nullptr);
		ASSERT_EQ(dolili.tail, nullptr);
		dolili.insertAfter(nullptr, 50);
		auto* node = dolili.head;
		ASSERT_EQ(dolili.head, node);
		ASSERT_EQ(dolili.tail, node);
		ASSERT_EQ(dolili.size(), 1);
		dolili.erase(dolili.head);
		ASSERT_EQ(dolili.size(), 0);
		ASSERT_EQ(dolili.head, nullptr);

		unsigned char overflow_char = static_cast<unsigned char>(DOLILI_UNSIGNED_CHAR_NUMBER);
		unsigned char inserted_values[] = { 150, 1, 0, 0, static_cast<unsigned char>(DOLILI_UNSIGNED_CHAR_NUMBER), 10};
		dolili.insertAfter(nullptr, inserted_values[0]);
		for (unsigned char i = 1; i < sizeof(inserted_values); i++) {
			dolili.insertAfter(dolili.tail, inserted_values[i]);
		}

		/* Size/head/tail checks */
		ASSERT_EQ(dolili.size(), sizeof(inserted_values) / sizeof(unsigned char));
		ASSERT_EQ(dolili.head->data, 150);
		ASSERT_EQ(dolili.tail->data, 10);

		/* Check for overflow corrections */
		node = dolili.findNodeReverse([=](const unsigned char value) { return value == overflow_char; });
		ASSERT_NE(node, nullptr);

		/* Check for the same values as from start */
		for (unsigned char i = 0; i < sizeof(inserted_values); i++) {
			node = dolili.findNode([&](const unsigned char value) { return value == inserted_values[i];  });
			ASSERT_NE(node, nullptr);
		}

		/* Check for different values from start and end */
		ASSERT_NE(
			dolili.findNode([&](const unsigned char value) { return value == 0;  }),
			dolili.findNodeReverse([&](const unsigned char value) { return value == 0;  })
		);
	}
}
