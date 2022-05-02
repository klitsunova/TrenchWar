cmake_minimum_required(VERSION 3.20)
project(MapGenerator)

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


set(SOURCES
        main.cpp
        map_generator.cpp
        map_generator.h)

add_executable(MapGenerator ${SOURCES})

target_link_libraries(MapGenerator
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )
