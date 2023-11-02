cmake_minimum_required(VERSION 3.25)
project(WCSSkills)

set(CMAKE_CXX_STANDARD 20)

set(traits 
    traits/traits.cpp
)

set(events
    events/event.cpp
)

set(functions
    functions/buffs.cpp
    functions/debuffs.cpp
    functions/instant.cpp
    functions/all.cpp
)

set(includes
    includes/exceptions.cpp
    includes/datastorage.cpp
)

set(status_effects
    status_effects/se.cpp
    status_effects/skill.cpp
    status_effects/leftovers.cpp
)

set(managers
    managers/infomanager.cpp
)

set(player
    player/wcsplayer.cpp
)


add_executable(WCSSkills
        ${functions}
        ${includes}
        ${traits}
        ${events}
        ${managers}
        ${status_effects}
        ${skills}
        ${player}
        main.cpp
)

#target_link_libraries(WCSSkills data/sqlite/sqlite3.dll)
