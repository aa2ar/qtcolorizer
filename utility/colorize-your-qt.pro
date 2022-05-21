#-------------------------------------------------
#
# Project created by QtCreator 2018-03-02T15:48:59
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = colorize-your-qt
TEMPLATE = app


SOURCES += \
    main.cpp\
    ui/mainwindow.cpp \
    ui/testee.cpp \
    ui/colorchooser.cpp \
    core/colordb.cpp \
    core/customcolormodel.cpp \
    core/appsettings.cpp \
    core/colorgroupmodel.cpp \
    core/colortonemodel.cpp \
    ui/aboutdialog.cpp \
    core/widgetsetmodeldelegate.cpp \
    ui/addwidgetsetdialog.cpp \
    core/palettemodel.cpp \
    core/colorizermodel.cpp \
    ../qtcolorizer/qtcolorizer.cpp

HEADERS  += \
    ui/mainwindow.h \
    ui/testee.h \
    ui/colorchooser.h \
    core/colordb.h \
    core/types.h \
    core/customcolormodel.h \
    core/appsettings.h \
    core/colorgroupmodel.h \
    core/colortonemodel.h \
    ui/aboutdialog.h \
    core/widgetsetmodeldelegate.h \
    ui/addwidgetsetdialog.h \
    core/paletteprovider.h \
    core/palettemodel.h \
    core/colorizermodel.h \
    ../qtcolorizer/qtcolorizer.h

FORMS    += \
    ui/mainwindow.ui \
    ui/testee.ui \
    ui/colorchooser.ui \
    ui/aboutdialog.ui \
    ui/addwidgetsetdialog.ui

DISTFILES += \
    rc/colorizer.json \
    TODO

RESOURCES += \
    resources.qrc
