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
        events::Function* eventReceiver;
    public:
        Leftover(WCSPlayer*, short, managers::SEInfo*);
        
        const WCSPlayer* owner;
        short rounds_left;
        managers::SEInfo* info;
        
        void roundStart(std::shared_ptr<events::Event>);
    };

} // stateff

#endif //WCSSKILLS_LEFTOVERS_HPP
