#include "leftovers.hpp"

using std::shared_ptr;
using managers::SEInfo;
using events::Event;
using events::EventManager;

namespace stateff {
    Leftover::Leftover(
            WCSPlayer* _owner,
            short _rounds_left,
            SEInfo* info
    ) : owner(_owner), rounds_left(_rounds_left), status_effect_info(info) {
        eventReceiver = [&](shared_ptr<Event>& e) { return roundStart(e); };
        auto manager = EventManager::getManager();
        manager->registerForEvent(info->activation_traits, eventReceiver);
    }

    events::ReturnEvent Leftover::roundStart(shared_ptr<Event>& e) {
//        owner->applyStatusEffect(owner); TODO
        rounds_left--;
        if (!rounds_left) {
            owner->leftovers;
        }
        return events::ReturnEvent::PASS;
    }
} // stateff

std::size_t std::hash<stateff::Leftover>::operator()(const stateff::Leftover &s) const noexcept {
    const auto t = s.eventReceiver.target<bool(events::Event*)>();
    return t ?
        (size_t)(uintptr_t)(void*)*t :
        s.eventReceiver.target_type().hash_code();
}
