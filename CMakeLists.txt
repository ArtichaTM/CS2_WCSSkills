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

set(db
	${src}/db/sqlite3/sqlite3.c
	${src}/db/sqlite3/shell.c
	${src}/db/db.cpp
)

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
	${src}/includes/ticker.cpp
)

set(status_effects
	${src}/status_effects/skill.cpp
	${src}/status_effects/se.cpp
	${src}/status_effects/leftovers.cpp
)

set(managers
	${src}/managers/infomanager.cpp
)

set (paths
	${src}/paths/paths.cpp
)

set(player
	${src}/player/wcsplayer.cpp
)

set(WCSSkills_compile
	${db}
	${functions}
	${includes}
	${traits}
	${events}
	${managers}
	${paths}
	${status_effects}
	${skills}
	${player}
)

add_executable(WCSSkills
	${WCSSkills_compile}
	${src}/main.cpp
)


set(tests tests)

set(t_includes
	${tests}/includes/datastorage.cpp
	${tests}/includes/dolili.cpp
	${tests}/includes/ticker.cpp
)


set(t_data
	${tests}/data/skills_se_traits.cpp
)

set(TestWCSSkills_compile
	${WCSSkills_compile}
	${t_includes}
	${t_data}
	${tests}/main.cpp
)

add_executable(TestWCSSkills
	${TestWCSSkills_compile}
)

target_link_libraries(TestWCSSkills GTest::gtest_main)
target_compile_definitions(WCSSkills PRIVATE
	CMAKE_DATA_DIR="${CMAKE_CURRENT_LIST_DIR}"
	CMAKE=1
	META_CONPRINTF=printf
)
target_compile_definitions(TestWCSSkills PRIVATE
	CMAKE_DATA_DIR="${CMAKE_CURRENT_LIST_DIR}"
	CMAKE=1
	META_CONPRINTF=printf
)

include(GoogleTest)
gtest_discover_tests(TestWCSSkills)
