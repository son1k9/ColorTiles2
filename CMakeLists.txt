cmake_minimum_required (VERSION 3.8)
project ("ColorTiles")

# Dependencies
include(FetchContent)

set(RAYLIB_VERSION 5.0)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    FIND_PACKAGE_ARGS ${RAYLIB_VERSION} EXACT
)
set(BUILD_EXAMPLES OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(raylib)

add_executable (ColorTiles  "src/main.cpp")
target_sources(ColorTiles PUBLIC 
    src/main.cpp
    src/main_menu_scene.cpp
    src/scores_scene.cpp
    src/settings.cpp
    src/settings_scene.cpp
    src/skin.cpp
    src/gameplay_scene.cpp
    src/raygui_impl.cpp
)
target_include_directories(ColorTiles PUBLIC "src" "include")
target_link_libraries(${PROJECT_NAME} raylib)

set_property(TARGET ColorTiles PROPERTY CXX_STANDARD 20)
