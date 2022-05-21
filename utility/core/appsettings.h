/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   appsettings.h
* \author Alex Arefyev
* \date   14.06.2018
*
*
*/

#ifndef __APPSETTINGS_H__INCLUDED__
#define __APPSETTINGS_H__INCLUDED__

#include <QString>

class AppSettings
{
public:
    static bool load(const QString &);
    static bool save(const QString &);
    static bool save();
public:
    static QString lastTargetFile();
    static void    setLastTargetFile(QString);

private:
    AppSettings() {}
    AppSettings(const AppSettings &) = delete;
    AppSettings & operator =(const AppSettings &) = delete;
    static AppSettings & instance() { static AppSettings inst; return inst; }

    struct Data {
        QString lastTargetFile;
    } mData;
    QString mFilePath;
};

#endif // __APPSETTINGS_H__INCLUDED__
