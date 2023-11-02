#include "buffs.hpp"

using namespace std;
using namespace events;
using namespace dataStorage;

namespace functions::buffs {
    ReturnEvent upAtk(shared_ptr<Event> e, DataStorage& arguments) {
        float damageIncrease = *arguments.at("multiplier").getData<float>();
        
        std::function<void(float&)> func = 
                [&](float& value) mutable -> void
                { value *= damageIncrease; };
        
        e->changeData("multiplier", func);
//        e->setData<false>("multiplier", e->getData<float>("multiplier"));
        
        //        try {
//            e->getData<WCSPlayer*>("initiator");
//        } catch (events::WrongType&) {
//            throw events::WrongType{(char*) "Tried to get initiator of attack "
//                                            "during atkUp function, but it's type is wrong"};
//        }
        return ReturnEvent::PASS;
    }
    
    ReturnEvent upNpdamage(shared_ptr<Event>& e, DataStorage& arguments) {
        return ReturnEvent::PASS;
    }
    
    void fillMap(functions_map* m) {
        m->emplace("upAtk", upAtk);
        m->emplace("upNpdamage", upNpdamage);
    }
} // functions
