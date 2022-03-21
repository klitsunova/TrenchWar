cmake_minimum_required(VERSION 3.21)
project(TrenchWar)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "~/Qt/6.2.3/gcc_64/lib/cmake")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

set(SOURCES
        main.cpp
        Views/view.cpp
        Views/view.h
        Controllers/controller.cpp
        Controllers/controller.h
        Models/map.cpp
        Models/map.h
        Models/GameObjects/game_object.cpp
        Models/GameObjects/game_object.h
        Models/GameObjects/soldier.cpp
        Models/GameObjects/soldier.h
        Models/GameObjects/terrain_object.cpp
        Models/GameObjects/terrain_object.h
)

add_executable(TrenchWar ${SOURCES})

target_link_libraries(TrenchWar
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )

