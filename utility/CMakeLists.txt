project(qtcolorizer-util)

# moc
qt5_wrap_cpp(HEADERS_MOC
    ui/mainwindow.h
    ui/testee.h
    ui/colorchooser.h
    ui/aboutdialog.h
    ui/addwidgetsetdialog.h
    core/colorizermodel.h
    core/widgetsetmodeldelegate.h
)

qt5_wrap_ui(UI_HEADERS
    ui/mainwindow.ui
    ui/testee.ui
    ui/colorchooser.ui
    ui/aboutdialog.ui
    ui/addwidgetsetdialog.ui
)

set(SOURCES
    main.cpp
    ui/mainwindow.cpp
    ui/testee.cpp
    ui/colorchooser.cpp
    ui/aboutdialog.cpp
    ui/addwidgetsetdialog.cpp
    core/colordb.cpp
    core/customcolormodel.cpp
    core/appsettings.cpp
    core/colorgroupmodel.cpp
    core/colortonemodel.cpp
    core/widgetsetmodeldelegate.cpp
    core/palettemodel.cpp
    core/colorizermodel.cpp
    ../qtcolorizer/qtcolorizer.cpp
)

# resources
file(GLOB_RECURSE RC_FILES ${CMAKE_SOURCE_DIR}/utility/*.qrc)
qt5_add_resources(RESOURCES ${RC_FILES})

add_executable(${PROJECT_NAME}
        ${SOURCES}
        ${HEADERS_MOC}
        ${UI_HEADERS}
        ${RESOURCES}
)

target_link_libraries(${PROJECT_NAME} stdc++)

qt5_use_modules(${PROJECT_NAME} Core Widgets)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${CMAKE_PROJECT_NAME})

install(PROGRAMS ./../bin/qtcolorizer DESTINATION /usr/local/bin)
install(FILES ./CONTROL/qtcolorizer.desktop DESTINATION /usr/share/applications)
install(FILES ./CONTROL/qtcolorizer.png DESTINATION /usr/share/pixmaps)
