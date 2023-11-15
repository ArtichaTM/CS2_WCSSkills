#ifndef WCSSKILLS_PATHS
#define WCSSKILLS_PATHS

#include <string>
#include <filesystem>


namespace paths {
    using namespace std::filesystem;
#ifdef CMAKE
    const static path MAIN_DIR = path(CMAKE_DATA_DIR);
#else
    const static path MAIN_DIR = path();
#endif
    const static path DATA_DIR = MAIN_DIR / path("data");
    const static path SKILLS = DATA_DIR / path("skills.json");
    const static path SE = DATA_DIR / path("se.json");
    const static path TRAITS = DATA_DIR / path("traits.json");

    /*const static std::basic_string MAIN_DIR = DATA_DIR;
    const static std::basic_string DATA = MAIN_DIR + R"(\data)";
    const static std::basic_string SKILLS = DATA + R"(\skills.json)";
    const static std::basic_string SE = DATA + R"(\se.json)";
    const static std::basic_string TRAITS = DATA + R"(\traits.json)";*/
}

#endif //WCSSKILLS_PATHS
