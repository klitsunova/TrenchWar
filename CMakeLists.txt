cmake_minimum_required(VERSION 3.21)
project(TrenchWar)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "~/Qt/6.2.3/gcc_64/lib/cmake")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

set(MODELS
        Models/world.cpp
        Models/GameObjects/game_object.cpp
        Models/GameObjects/soldier.cpp
        Models/GameObjects/terrain_object.cpp)

set(VIEWS
        Views/view.cpp)

set(CONTROLLERS
        Controllers/controller.cpp)

set(SOURCES
        main.cpp
        ${MODELS}
        ${VIEWS}
        ${CONTROLLERS})

qt6_add_big_resources(RESOURCES resources.qrc)

add_executable(TrenchWar ${SOURCES} ${RESOURCES})

target_link_libraries(TrenchWar
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )

