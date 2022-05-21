/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   file.h
* \author Alex Arefyev
* \date   14.06.2018
*
*
*/

#ifndef __COLORTONEMODEL_H__INCLUDED__
#define __COLORTONEMODEL_H__INCLUDED__

#include <QAbstractListModel>
#include <QStyledItemDelegate>
#include "types.h"

class ColorToneModel : public QAbstractListModel
{
public:
    explicit ColorToneModel(QObject *parent = 0);

    void setSource(const ColorTones *);
    QColor getColor(int);

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex & parent) const;

private:
    const ColorTones * mTones = nullptr;
};


class ColorToneModelDelegate : public QStyledItemDelegate
{
public:
    explicit ColorToneModelDelegate(QWidget *, const QColor &c, QObject * parent = 0, int h = 20);

    void setHeight(int);
    int height() const;

    void paint(QPainter *, const QStyleOptionViewItem &, const QModelIndex &) const;
    QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const;

private:
    int mHeight = 20;
    QWidget * mWidget = nullptr;
    QColor mBgColor;
};

#endif // __COLORTONEMODEL_H__INCLUDED__
