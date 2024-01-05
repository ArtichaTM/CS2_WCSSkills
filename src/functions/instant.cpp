#include "instant.hpp"

using std::shared_ptr;
using dataStorage::DataStorage;
using events::ReturnEvent;
using events::Event;

namespace functions::instant {
    ReturnEvent forceInstantDeath(shared_ptr<Event> e, DataStorage& arguments) {
        return events::ReturnEvent::PASS;
    }
    ReturnEvent donotAct(shared_ptr<Event> e, DataStorage& arguments) {
        return events::ReturnEvent::PASS;
    }
    void fillMap(functions_map* m) {
        m->emplace("forceInstantDeath", forceInstantDeath);
        m->emplace("donotAct", donotAct);
    }

} // functions::instant
