cmake_minimum_required(VERSION 3.5)

project(Co-Work LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        kanban.cpp
        kanban.h
        kanban.ui
        login.cpp
        login.h
        login.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Co-Work
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(Co-Work SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Co-Work
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Co-Work PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
