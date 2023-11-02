#ifndef WCSSKILLS_MANAGERS_INFOMANAGER
#define WCSSKILLS_MANAGERS_INFOMANAGER

#include "../includes/exceptions.hpp"

namespace managers {
    class SEInfo;
    class SkillSE;
    class SkillInfo;
    class InfoManager;
    
    class NoSuchSkill : public CustomException { using CustomException::CustomException; };
}

#include <vector>
#include <unordered_map>
#include "../includes/json.hpp"
#include "../traits/traits.hpp"
#include "../player/wcsplayer.hpp"
#include "../functions/all.hpp"

using nlohmann::json;

namespace managers {    
    class InfoManagerRecreating : public std::exception {};
    
    class SEInfo {
        static traits::tr_set traits_init(json&);
        static functions::function function_init(json&);
    public:
        explicit SEInfo(json&);
        ~SEInfo();
        const unsigned int id;
        const traits::tr_set traits;
        const traits::tr_set activation_traits;
        const functions::function function;
    };
    
    typedef std::unordered_map<int, std::shared_ptr<SEInfo>> se_map;
    
    class SkillSE {
    public:
        explicit SkillSE(se_map&, json&);
        const std::shared_ptr<SEInfo> seInfo;
        const dataStorage::DataStorage arguments;

        template<bool force>
        events::ReturnEvent applyStatusEffect(WCSPlayer*);
    };
    
    class SkillInfo {
    public:
        explicit SkillInfo(se_map&, json&);
        const std::string name;
        const std::string description;
        const std::string menu_description;
        traits::tr_set traits;
        std::vector<std::shared_ptr<SkillSE>> effects;

        template<bool force>
        events::ReturnEvent applySkill(WCSPlayer*);
    };
    
    
    class InfoManager {
    protected:
        explicit InfoManager();
        ~InfoManager();
        static InfoManager* instance;
    public:
        static void init();
        static void close();
        
        std::unordered_map<std::string, std::shared_ptr<SkillInfo>> skills;
        se_map se;
        std::unordered_map<int, std::string*> traits;
        
        static InfoManager* getManager();
    };
} // skills

#endif //WCSSKILLS_MANAGERS_INFOMANAGER
