# Copyright © 2018 Alexander Arefyev
# License: MIT
# Contacts: alex.arefyev@gmail.com

cmake_minimum_required(VERSION 3.1)

project(qtcolorizer)

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE RELEASE)
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Widgets SerialPort REQUIRED)

include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})

add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(-fPIC)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS} -fPIE -std=c++11")

add_subdirectory(qtcolorizer)
add_subdirectory(utility)
add_subdirectory(example)

# ***********************************************************************
# install qtcolorizer
install(FILES ${LIBRARY_OUTPUT_PATH}/lib${CMAKE_PROJECT_NAME}.a DESTINATION /usr/lib)
install(FILES ${LIBRARY_OUTPUT_PATH}/lib${CMAKE_PROJECT_NAME}.so DESTINATION /usr/lib)
install(FILES ${CMAKE_SOURCE_DIR}/qtcolorizer/qtcolorizer.h DESTINATION /usr/local/include/${CMAKE_PROJECT_NAME})

# install utility
install(PROGRAMS ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_PROJECT_NAME} DESTINATION /usr/local/bin)


# ***********************************************************************
# generate DEB package

if("${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")

    string(TIMESTAMP DATETIME_NOW %Y%m%d%H%M UTC)

    if(NOT DEFINED ARCH)

        execute_process(
            COMMAND "/usr/bin/dpkg" "--print-architecture"
            RESULT_VARIABLE RESULT
            OUTPUT_VARIABLE ARCH
        )

        string(STRIP "${ARCH}" ARCH)

    endif()

    if("${ARCH}" STREQUAL "")
        set(ARCH "amd64")
    endif()

    set(VERSION_MAJOR "1")
    set(VERSION_MINOR "0")
    set(VERSION_PATCH "${DATETIME_NOW}")

    set(CPACK_GENERATOR "DEB")
    SET(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
    SET(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
    SET(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${ARCH})
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5core5a,libqt5widgets5")
    SET(CPACK_PACKAGE_VENDOR "Qt Colorizer")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Colorize your Qt applications")
    set(CPACK_PACKAGE_DESCRIPTION "Colorize your Qt applications")
    set(CPACK_PACKAGE_CONTACT "Alexander Arefyev, alex.arefyev@gmail.com")

    set(PROJECT_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}-${VERSION_PATCH}")

    set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${PROJECT_VERSION}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")

    include(CPack)

endif()
