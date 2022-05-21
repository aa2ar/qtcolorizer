# Copyright © 2018 Alexander Arefyev
# License: MIT
# Contacts: alex.arefyev@gmail.com

cmake_minimum_required(VERSION 2.8)

project(colorizer-example)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# qt
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})

add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Widgets_DEFINITIONS})

# moc
QT5_WRAP_CPP(HEADERS_MOC
    ui/mainwindow.h
)

QT5_WRAP_UI(FORMS_HEADERS
    ui/mainwindow.ui
)

set(SOURCES
    main.cpp
    ui/mainwindow.cpp
    ../qtcolorizer/qtcolorizer.cpp
)

# resources
FILE(GLOB_RECURSE RC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc)
QT5_ADD_RESOURCES(RESOURCES ${RC_FILES})

# Executables fail to build with Qt 5 in the default configuration
# without -fPIE. We add that here.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS} -std=c++11")

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS_MOC}
    ${FORMS_HEADERS}
    ${RESOURCES}
)

target_link_libraries(${PROJECT_NAME} stdc++)

qt5_use_modules(${PROJECT_NAME} Core Widgets)

