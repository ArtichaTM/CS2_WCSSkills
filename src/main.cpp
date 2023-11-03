#include <iostream>
#include "includes/paths.hpp"
#include "managers/infomanager.hpp"
#include "events/event.hpp"
#include "includes/doublelinkedlist.hpp"

using namespace std;
using namespace traits;
using namespace events;

class IntValue {
public:
    float value;
    int value2;
    IntValue(float _value, int _value2) : value(_value), value2(_value2) {};
};

ReturnEvent subTestEvents(shared_ptr<Event>& e) {
    auto* multiplier = e->getData<float>("mult");
    *multiplier = *multiplier * 1.1f;
    return events::ReturnEvent::PASS;
}

void testEvents() {
    auto* manager = EventManager::getManager();
    manager->registerForEvent({2, 3}, subTestEvents);
    
    auto* multiplier = new float(1);
    auto e = shared_ptr<Event>(new Event({2, 3}));
    e->setData<false>("mult", multiplier);
    std::cout << "\n\nBefore event: " << std::to_string(*multiplier) << std::endl;
    manager->fireEvent(e);
    std::cout << "After event: " << std::to_string(*multiplier) << std::endl;
    //delete e;
}

void testManager() {
    using namespace managers;
    InfoManager* manager = InfoManager::getManager();
}

template<typename T>
void print_map(unordered_set<T> const& m) {
    std::cout << '[';
    auto it = m.begin();
    if (it==m.end()) {
        std::cout << ']';
        return;
    }
    T value = *it;
    std::cout << value;
    it++;
    while (it != m.end()) {
        value = *it;
        std::cout << ", " << value;
        it++;
    }
    std::cout << ']';
}

template<typename T>
void printLinkedList(dataStorage::DoubleLinkedList<T>& ll, basic_string<char> const& text) {
    std::cout << "\n> " + text + "\nList:";
    ll.forEach([&](T set) { std::cout << " "; print_map(set); });
//    std::cout << "\nReversed list:";
//    ll.forEachReverse([&](T set) { std::cout << " "; print_map(set); });
    std::cout << "\nSize: " + to_string(ll.size());
    
}

template<typename T>
void printLinkedList(dataStorage::DoubleLinkedList<T>& ll) {
    printLinkedList(ll, "");
}

void testLinkedList() {
    auto dolili = dataStorage::DoubleLinkedList<unordered_set<int>>();
    
    unordered_set<int> new_s{12, 50};
    dolili.insertAfter(nullptr, new_s);
    dolili.insertAfter(dolili.tail, {123, 51903, 2});
    dolili.insertAfter(dolili.tail, {560123, 6324021, 51903});
    dolili.insertAfter(dolili.tail, {60123, -2312, 2321, 50});
    printLinkedList(dolili, "After inserting data directly");
    
    auto* node = dolili.findNode([&](auto set)->bool { return set.contains(51903); });
    auto* node2 = node;
    node2 = nullptr;
    if (!node) {
        std::cout << "\nDid not found element!\n";
    } else {
        dolili.insertAfter(node, {213, 4123});
    }
    printLinkedList(dolili, "After inserting new node after search");
    
    node = dolili.findNodeReverse([&](auto set)->bool { return set.contains(51903); });
    node2 = dolili.findNodeReverse([&](auto set)->bool { return set.contains(12); });
    if (!node) {
        std::cout << "\nDid not found node1!\n";
    } else if (!node2) {
        std::cout << "\nDid not found node2!\n";
    } else {
        dolili.erase(node, node2);
    }
    printLinkedList(dolili, "After deleting node 2 nodes");
    
    while (dolili.size()) dolili.erase(dolili.head);
    printLinkedList(dolili, "After deleting everything");
}

void testFuncManager() {
    auto* funcManager = functions::Functions::get();
    std::cout << "\n\nSize: " << (*funcManager)->size() << "\n";
    assert(!(*funcManager)->empty());
    for (auto& funcs : **funcManager) {
        std::cout << "Name " + funcs.first << "\n";
    }
}


int main() {/*
    events::EventManager::init();
    functions::Functions::init();
    managers::InfoManager::init();
    testLinkedList();
    testFuncManager();
    testEvents();
    events::EventManager::close();
    functions::Functions::close();
    managers::InfoManager::close();*/
    return 0;
}
