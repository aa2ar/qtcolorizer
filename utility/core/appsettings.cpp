/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   appsettings.cpp
* \author Alexander Arefyev
* \date   14.06.2018
*
*
*/

#include "appsettings.h"
#include <QSettings>


bool AppSettings::load(const QString & filePath)
{
    instance().mFilePath = filePath;

    QSettings ss(filePath, QSettings::IniFormat);
    if (ss.status() != QSettings::NoError)
        return false;

    Data & data = instance().mData;

    ss.beginGroup(QStringLiteral("app"));
    data.lastTargetFile = ss.value(QStringLiteral("lastfile")).toString();
    ss.endGroup();

    return true;
}

bool AppSettings::save(const QString & filePath)
{
    instance().mFilePath = filePath;
    return save();
}

bool AppSettings::save()
{
    QSettings ss(instance().mFilePath, QSettings::IniFormat);

    Data & data = instance().mData;

    ss.beginGroup(QStringLiteral("app"));
    ss.setValue(QStringLiteral("lastfile"), data.lastTargetFile);
    ss.endGroup();

    return true;
}

QString AppSettings::lastTargetFile()
{
    return instance().mData.lastTargetFile;
}

void AppSettings::setLastTargetFile(QString value)
{
    instance().mData.lastTargetFile = value;
}
