cmake_minimum_required(VERSION 3.20)
project(TrenchWar)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#set(CMAKE_PREFIX_PATH "~/Qt/6.2.3/gcc_64/lib/cmake")
set(CMAKE_PREFIX_PATH "~/Qt/6.3.0/mingw_64/lib/cmake")

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
        Models/GameObjects/terrain_object.cpp
        Models/Tools/pixmap_loader.cpp
        Models/Tools/settings.cpp
        Models/weapon.cpp
        )

set(VIEWS
        Views/Game/map_view.cpp
        Views/Game/game_view.cpp
        Views/Game/store_view.cpp
        Views/Menu/menu_view.cpp
        Views/Menu/settings_menu_view.cpp
        Views/Menu/pause_menu_view.cpp
        Views/Menu/exit_window.cpp
        )

set(CONTROLLERS
        Controllers/game_controller.cpp
        Controllers/main_controller.cpp
        Controllers/menu_controller.cpp
        Controllers/events_controller.cpp
        )

set(SOURCES
        main.cpp
        ${MODELS}
        ${VIEWS}
        ${CONTROLLERS} Controllers/trench_update.cpp Controllers/trench_update.h)

qt6_add_big_resources(RESOURCES resources.qrc)

add_executable(TrenchWar ${SOURCES} ${RESOURCES})

target_link_libraries(TrenchWar
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )
