#ifndef WCSSKILLS_LEFTOVERS_HPP
#define WCSSKILLS_LEFTOVERS_HPP

namespace stateff {
    class Leftover;
}

#include "../player/wcsplayer.hpp"
#include "../events/event.hpp"
#include "../managers/infomanager.hpp"

namespace stateff {

    class Leftover {
    public:
        Leftover(WCSPlayer*, short, managers::SEInfo*);
        
        const WCSPlayer* owner;
        short rounds_left;
        managers::SEInfo* status_effect_info;
        events::eventReceiver eventReceiver;
        
        events::ReturnEvent roundStart(std::shared_ptr<events::Event>&);
    };

} // stateff

template<> struct std::hash<stateff::Leftover> {
    std::size_t operator()(stateff::Leftover const& s) const noexcept;
};

#endif //WCSSKILLS_LEFTOVERS_HPP
