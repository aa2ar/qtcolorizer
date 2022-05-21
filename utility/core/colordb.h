/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   colordb.h
* \author Alex Arefyev
* \date   14.06.2018
*
*
*/

#ifndef __COLORDB_H__INCLUDED__
#define __COLORDB_H__INCLUDED__

#include "colorgroupmodel.h"
#include "colortonemodel.h"

class QAbstractListModel;
class QAbstractItemModel;
class ColorDB
{
public:
    ColorDB();

    QAbstractListModel * colorModel() const;
    QAbstractItemModel * tonesModel() const;

    void setCurrentTone(int);
    QColor getColor(int);

private:
    ColorGroupModel * mColorModel = nullptr;
    ColorToneModel  * mToneModel  = nullptr;
};

#endif // __COLORDB_H__INCLUDED__
