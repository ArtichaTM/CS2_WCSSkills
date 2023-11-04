cmake_minimum_required(VERSION 3.25)
project(WCSSkills)

add_compile_definitions(DEBUG)
set(CMAKE_CXX_STANDARD 20)

enable_testing()
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(src src)

set(traits 
    ${src}/traits/traits.cpp
)

set(events
    ${src}/events/event.cpp
)

set(functions
    ${src}/functions/buffs.cpp
    ${src}/functions/debuffs.cpp
    ${src}/functions/instant.cpp
    ${src}/functions/all.cpp
)

set(includes
    ${src}/includes/exceptions.cpp
    ${src}/includes/datastorage.cpp
)

set(status_effects
    ${src}/status_effects/se.cpp
    ${src}/status_effects/skill.cpp
    ${src}/status_effects/leftovers.cpp
)

set(managers
    ${src}/managers/infomanager.cpp
)

set(player
    ${src}/player/wcsplayer.cpp
)

set(WCSSkills_compile
    ${functions}
    ${includes}
    ${traits}
    ${events}
    ${managers}
    ${status_effects}
    ${skills}
    ${player}
)

add_executable(WCSSkills
    ${WCSSkills_compile}
    ${src}/main.cpp
)


set(tests tests)

set(includes
    ${tests}/includes/Testdatastorage.cpp
)

set(TestWCSSkills_compile
    ${WCSSkills_compile}
    ${includes}
)

add_executable(TestWCSSkills
    ${TestWCSSkills_compile}
)

target_link_libraries(
  TestWCSSkills
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(TestWCSSkills)
