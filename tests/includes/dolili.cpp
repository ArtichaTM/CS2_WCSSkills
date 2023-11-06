#include <gtest/gtest.h>
#include <unordered_set>

#include "../../src/includes/doublelinkedlist.hpp"

using namespace std;

template <typename T>
void test_dolili();

template<> void test_dolili<unordered_set<int>>() {
	auto dolili = dataStorage::DoubleLinkedList<unordered_set<int>>();

	unordered_set<int> new_s{ 12, 50 };
	dolili.insertAfter(nullptr, new_s);
	dolili.insertAfter(dolili.tail, { 123, 51903, 2 });
	dolili.insertAfter(dolili.tail, { 560123, 6324021, 51903 });
	dolili.insertAfter(dolili.tail, { 60123, -2312, 2321, 50 });

	//printLinkedList(dolili, "After inserting data directly");

	auto* node = dolili.findNode([&](auto set)->bool { return set.contains(51903); });
	auto* node2 = node;
	node2 = nullptr;
	if (!node) {
		std::cout << "\nDid not found element!\n";
	}
	else {
		dolili.insertAfter(node, { 213, 4123 });
	}
	//printLinkedList(dolili, "After inserting new node after search");

	node = dolili.findNodeReverse([&](auto set)->bool { return set.contains(51903); });
	node2 = dolili.findNodeReverse([&](auto set)->bool { return set.contains(12); });
	if (!node) {
		//std::cout << "\nDid not found node1!\n";
	}
	else if (!node2) {
		//std::cout << "\nDid not found node2!\n";
	}
	else {
		dolili.erase(node, node2);
	}
	//printLinkedList(dolili, "After deleting node 2 nodes");

	while (dolili.size()) dolili.erase(dolili.head);
	//printLinkedList(dolili, "After deleting everything");
}

TEST(DoubleLinkedList, general) {
	test_dolili<unordered_set<int>>();
}
