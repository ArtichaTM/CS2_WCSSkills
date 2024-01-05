#include "leftovers.hpp"

using std::shared_ptr;
using managers::SEInfo;
using events::Event;
using events::EventManager;

namespace stateff {
    Leftover::Leftover(
            WCSPlayer* _owner,
            short _rounds_left,
            SEInfo* _info
    ) : owner(_owner), rounds_left(_rounds_left), info(_info), 
    eventReceiver(EventManager::getManager()->registerForEvent(info->activation_traits, bind(&Leftover::roundStart, this, std::placeholders::_1))) {}

    void Leftover::roundStart(shared_ptr<Event> e) {
//        owner->applyStatusEffect(owner); TODO
        rounds_left--;
        if (!rounds_left) {
            owner->leftovers;
        }
    }
} // stateff