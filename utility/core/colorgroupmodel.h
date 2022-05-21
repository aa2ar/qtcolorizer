/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   colorgroupmodel.h
* \author Alex Arefyev
* \date   14.06.2018
*
*
*/

#ifndef __COLORGROUPMODEL_H__INCLUDED__
#define __COLORGROUPMODEL_H__INCLUDED__

#include <QAbstractListModel>
#include <QStyledItemDelegate>
#include "types.h"

class ColorGroupModel : public QAbstractListModel
{
public:
    explicit ColorGroupModel(ColorSet * set, QObject *parent = 0);

    const ColorTones * getTones(int);

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex & parent) const override;

private:
    ColorSet * mSet = nullptr;
};

class ColorGroupModelDelegate : public QStyledItemDelegate
{
public:
    ColorGroupModelDelegate(int h = 20);

    void setHeight(int);

    void paint(QPainter *, const QStyleOptionViewItem &, const QModelIndex &) const;
    QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const;

private:
    int mHeight = 20;
};

#endif // __COLORGROUPMODEL_H__INCLUDED__
