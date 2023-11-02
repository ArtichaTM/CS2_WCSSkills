#ifndef WCSSKILLS_EVENT_HPP
#define WCSSKILLS_EVENT_HPP

#include <string>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <memory>
#include "../includes/exceptions.hpp"
#include "../includes/datastorage.hpp"

namespace events {
    enum ReturnEvent {
        BLOCK, PASS, GUARD, NO_EFFECT
    };
    class NoSuchReceiver;
    class SettingsModify : public CustomException { using CustomException::CustomException; };
    class SetDataModify : public CustomException { using CustomException::CustomException; };
    class Event;
    typedef std::function<ReturnEvent(std::shared_ptr<Event>&)> eventReceiver;
    typedef std::vector<eventReceiver> vectorofEventReceivers;
    class EventManager;
}

namespace functions {
    typedef std::function<events::ReturnEvent(
        std::shared_ptr<events::Event>&,
        dataStorage::DataStorage&
    )> function;
    typedef std::unordered_map<std::basic_string<char>, function> functions_map;
}

#include "../traits/traits.hpp"
#include "../player/wcsplayer.hpp"


namespace events {
    
    class Event {
    public:
        traits::tr_set activation_traits;
        dataStorage::DataStorage* data;
        bool initialEvent = false;
        events::ReturnEvent result = PASS;
        
        Event(traits::tr_set);
        Event(std::initializer_list<traits::Trait>);
        Event(traits::tr_set, dataStorage::DataStorage*);
        
        void changeActivationTraits(traits::tr_set);

        template<bool autoDelete, typename T>
        void setData(std::string const& key, T* value) {
            if (this->data->contains(key)) {
                char message[] = "Trying to \"setData\" already existing key."
                                    "Use \"changeData\" instead";
                throw SetDataModify(message);
            } else {
                auto eValue = dataStorage::EData(autoDelete);
                eValue.setData(value);
                this->data->insert({key, eValue});
            }
        }
        
        template<typename T>
        T* getData(std::string const& key) {
            return this->data->at(key).getData<T>();
        }

        template<typename T> void changeData(std::string const& key, std::function<void(T&)>& func) {
            this->data->at(key).changeData(func);
        }

        template<typename T> void changeData(std::string const& key, std::function<void(T&&)>& func) {
            return changeData(func);
        }
    };
    
    class EventManager {
        std::unordered_map<traits::tr_set, vectorofEventReceivers>* registered_events;
        std::queue<std::shared_ptr<Event>>* lateRunEvents;
        void iterateOverEvents();
        static EventManager* instance;
        volatile bool iterating = false;
        EventManager();
        ~EventManager();
    public:
        static void init();
        static void close();
        static EventManager* getManager();
        void registerForEvent(traits::tr_set const& activation_traits, eventReceiver&);
        void unregisterForEvent(traits::tr_set const& activation_traits, eventReceiver&);
        void registerForEvent(traits::tr_set const& activation_traits, eventReceiver&&);
        void unregisterForEvent(traits::tr_set const& activation_traits, eventReceiver&&);

        void fireEvent(std::shared_ptr<Event>&);
        void fireEvent(std::shared_ptr<Event>&&);
        
        bool receiversExist(traits::tr_set const& activation_traits);
    };
    
} // events

#endif //WCSSKILLS_EVENT_HPP
