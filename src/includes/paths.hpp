#ifndef WCSSKILLS_PATHS
#define WCSSKILLS_PATHS

#include <string>


namespace paths {
    const static std::basic_string MAIN_DIR = R"(C:\Users\Articha\Desktop\EveryThing\Editors\Cpp\WCSSkills)";
    const static std::basic_string DATA = MAIN_DIR + R"(\data)";
    const static std::basic_string SKILLS = DATA + R"(\skills.json)";
    const static std::basic_string SE = DATA + R"(\se.json)";
    const static std::basic_string TRAITS = DATA + R"(\traits.json)";
}

#endif //WCSSKILLS_PATHS
