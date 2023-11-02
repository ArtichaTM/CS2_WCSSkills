#include "instant.hpp"

using namespace dataStorage;
using namespace events;
using namespace std;

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
