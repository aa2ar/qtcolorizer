/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   main.cpp
* \author Alexander Arefyev
* \date   14.06.2018
*
*
*/

#include "ui/mainwindow.h"
#include "core/appsettings.h"
#include "../qtcolorizer/qtcolorizer.h"
#include <QApplication>
#include <QStandardPaths>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("qtcolorizer");
    a.setApplicationVersion("1.0");
    a.setApplicationDisplayName("Qt Colorizer");

    //Colorizer::apply(":/rc/colorizer.json", a, "system-palette");

    QStringList paths = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation);
    QString appConfigDir = QStringList({paths[0], a.applicationName()}).join(QDir::separator());
    QDir().mkpath(appConfigDir);
    if (!paths.isEmpty()) {
        a.setProperty("custom-colors-file",
                      QStringList({appConfigDir, QStringLiteral("custom-colors")}).join(QDir::separator()));

        AppSettings::load(QStringList({appConfigDir, QStringLiteral("app-settings")}).join(QDir::separator()));
    }

    MainWindow w;
    w.show();

    int res = a.exec();

    AppSettings::save();

    return res;
}
