#ifndef WCSSKILLS_PATHS_PATHS
#define WCSSKILLS_PATHS_PATHS

#include <string>
#include <filesystem>

#include "../includes/exceptions.hpp"

class Paths {
protected:
    explicit Paths(std::filesystem::path _main_dir);
    ~Paths();
    static Paths* _instance;
    Paths(Paths const&) = delete;
public:
    class PathsIsNotInitialized : public CustomException { using CustomException::CustomException; };
    class WrongDataLocation : public CustomException { using CustomException::CustomException; };
    class PathsRecreating : public CustomException { using CustomException::CustomException; };
    class PathsReclosing : public CustomException { using CustomException::CustomException; };

    const std::filesystem::path main_dir;
    const std::filesystem::path data_dir;
    const std::filesystem::path skills;
    const std::filesystem::path se;
    const std::filesystem::path traits;
    const std::filesystem::path database;

    static void init();
    static void close();
    static Paths* getInstance();
};

#endif //WCSSKILLS_PATHS_PATHS
